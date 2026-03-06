#pragma once

// AudioModule.h — Base class for all modules in the REVITHION STUDIO rack.
//
// Each AudioModule is a self-contained audio processor with typed I/O ports,
// automatable parameters, and a pluggable editor UI.  Modules are registered
// with ModuleFactory and wired together inside a ConnectionGraph, similar
// to Guitar Rig / VCV Rack / Reason's modular routing paradigm.
//
// Copyright (c) REVITHION STUDIO — All rights reserved.

#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace revithion::rack
{

// ── Module taxonomy ─────────────────────────────────────────────────────────

enum class ModuleCategory : uint8_t
{
    Amp,
    Cab,
    Drive,
    Delay,
    Reverb,
    Modulation,
    EQ,
    Dynamics,
    Pitch,
    Synth,
    AI,
    Utility,
    Input,
    Output
};

// ── Port descriptor ─────────────────────────────────────────────────────────

struct ModulePort
{
    enum class Direction : uint8_t { Input, Output };

    // Signal type determines cable colour in the rack UI
    enum class SignalType : uint8_t { Audio, Sidechain, CV, MIDI };

    uint32_t    id           = 0;
    juce::String name;
    Direction   direction    = Direction::Input;
    SignalType  signalType   = SignalType::Audio;
    bool        connected    = false;
    int         channelCount = 2;       // 1 = mono, 2 = stereo
};

// ── Parameter descriptor ────────────────────────────────────────────────────

struct ModuleParameter
{
    juce::String id;
    juce::String name;
    float value        = 0.0f;
    float defaultValue = 0.0f;
    float minValue     = 0.0f;
    float maxValue     = 1.0f;
    float skew         = 1.0f;         // non-linear mapping (1.0 = linear)
    juce::String unit;                 // "dB", "ms", "Hz", …

    // Optional value-to-string formatter for UI display
    std::function<juce::String(float)> formatter = nullptr;
};

// ── Module metadata (used by ModuleFactory) ─────────────────────────────────

struct ModuleInfo
{
    juce::String      uniqueId;              // e.g. "revithion.amp.neural"
    juce::String      displayName;           // e.g. "Neural Amp"
    juce::String      vendor       = "REVITHION";
    juce::String      version      = "1.0.0";
    ModuleCategory    category     = ModuleCategory::Utility;
    juce::StringArray tags;                  // preset browser / search tokens
    int               defaultInputPorts  = 1;
    int               defaultOutputPorts = 1;
};

// ── AudioModule — abstract base class ───────────────────────────────────────

class AudioModule
{
public:
    // ── Listener interface ──────────────────────────────────────────────

    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void moduleParameterChanged(AudioModule* /*module*/,
                                            const juce::String& /*paramId*/) {}
        virtual void moduleBypassChanged(AudioModule* /*module*/)  {}
        virtual void moduleStateChanged(AudioModule* /*module*/)   {}
    };

    // ── Construction / destruction ──────────────────────────────────────

    explicit AudioModule(const ModuleInfo& info)
        : info_(info)
    {
    }

    virtual ~AudioModule() = default;

    // ── Pure-virtual audio lifecycle ────────────────────────────────────

    virtual void prepareToPlay(double sampleRate, int blockSize) = 0;
    virtual void processBlock(juce::AudioBuffer<float>& buffer,
                              juce::MidiBuffer& midi) = 0;
    virtual void releaseResources() = 0;

    // ── Editor ──────────────────────────────────────────────────────────

    /// Return the module's rack-panel UI component (ownership transferred).
    virtual std::unique_ptr<juce::Component> createEditor() = 0;

    // ── Latency / tail ──────────────────────────────────────────────────

    virtual int getLatencySamples() const   { return latencySamples_; }
    virtual int getTailLengthSamples() const { return 0; }

    // ── State persistence ───────────────────────────────────────────────

    virtual juce::ValueTree saveState() const
    {
        juce::ValueTree state("ModuleState");
        state.setProperty("uniqueId",  info_.uniqueId, nullptr);
        state.setProperty("uuid",      uuid_.toString(), nullptr);
        state.setProperty("bypassed",  bypassed_.load(), nullptr);

        juce::ValueTree paramsNode("Parameters");

        for (const auto& p : parameters_)
            paramsNode.setProperty(p.id, p.value, nullptr);

        state.addChild(paramsNode, -1, nullptr);
        return state;
    }

    virtual void restoreState(const juce::ValueTree& state)
    {
        if (! state.isValid())
            return;

        bypassed_.store(state.getProperty("bypassed", false));

        auto paramsNode = state.getChildWithName("Parameters");

        if (paramsNode.isValid())
        {
            const juce::ScopedLock lock(paramLock_);

            for (auto& p : parameters_)
            {
                if (paramsNode.hasProperty(p.id))
                    p.value = paramsNode.getProperty(p.id);
            }
        }

        listeners_.call([this](Listener& l) { l.moduleStateChanged(this); });
    }

    // ── Port accessors ──────────────────────────────────────────────────

    const std::vector<ModulePort>& getInputPorts() const  { return inputPorts_; }
    const std::vector<ModulePort>& getOutputPorts() const { return outputPorts_; }

    ModulePort* findPort(uint32_t portId)
    {
        for (auto& p : inputPorts_)
            if (p.id == portId) return &p;

        for (auto& p : outputPorts_)
            if (p.id == portId) return &p;

        return nullptr;
    }

    // ── Parameter accessors ─────────────────────────────────────────────

    void setParameter(const juce::String& id, float value)
    {
        const juce::ScopedLock lock(paramLock_);

        for (auto& p : parameters_)
        {
            if (p.id == id)
            {
                p.value = juce::jlimit(p.minValue, p.maxValue, value);
                listeners_.call([this, &id](Listener& l)
                {
                    l.moduleParameterChanged(this, id);
                });
                return;
            }
        }
    }

    float getParameter(const juce::String& id) const
    {
        const juce::ScopedLock lock(paramLock_);

        for (const auto& p : parameters_)
            if (p.id == id) return p.value;

        return 0.0f;
    }

    const std::vector<ModuleParameter>& getParameters() const { return parameters_; }

    // ── Identity / info ─────────────────────────────────────────────────

    const ModuleInfo&  getInfo() const { return info_; }
    const juce::Uuid&  getUuid() const { return uuid_; }

    // ── Bypass ──────────────────────────────────────────────────────────

    bool isBypassed() const            { return bypassed_.load(std::memory_order_acquire); }

    void setBypassed(bool bypass)
    {
        bypassed_.store(bypass, std::memory_order_release);
        listeners_.call([this](Listener& l) { l.moduleBypassChanged(this); });
    }

    // ── CPU usage ───────────────────────────────────────────────────────

    float getCpuUsage() const { return cpuUsage_.load(std::memory_order_relaxed); }

    // ── Active state ────────────────────────────────────────────────────

    bool isActive() const { return active_; }

    // ── Listener management ─────────────────────────────────────────────

    void addListener(Listener* listener)    { listeners_.add(listener); }
    void removeListener(Listener* listener) { listeners_.remove(listener); }

protected:
    // ── Port helpers (call from subclass constructors) ───────────────────

    void addInputPort(const juce::String& name,
                      ModulePort::SignalType type = ModulePort::SignalType::Audio,
                      int channels = 2)
    {
        inputPorts_.push_back({ nextPortId_++, name,
                                ModulePort::Direction::Input, type,
                                false, channels });
    }

    void addOutputPort(const juce::String& name,
                       ModulePort::SignalType type = ModulePort::SignalType::Audio,
                       int channels = 2)
    {
        outputPorts_.push_back({ nextPortId_++, name,
                                 ModulePort::Direction::Output, type,
                                 false, channels });
    }

    // ── Parameter helpers ───────────────────────────────────────────────

    void addParameter(const ModuleParameter& param)
    {
        const juce::ScopedLock lock(paramLock_);
        parameters_.push_back(param);
    }

    void setLatencySamples(int samples) { latencySamples_ = samples; }

    // ── Protected state ─────────────────────────────────────────────────

    uint32_t nextPortId_ = 1;

    std::vector<ModulePort>      inputPorts_;
    std::vector<ModulePort>      outputPorts_;
    std::vector<ModuleParameter> parameters_;
    mutable juce::CriticalSection paramLock_;

    double sampleRate_    = 44100.0;
    int    blockSize_     = 512;
    int    latencySamples_ = 0;

    std::atomic<float> cpuUsage_ { 0.0f };

private:
    ModuleInfo            info_;
    juce::Uuid            uuid_;
    std::atomic<bool>     bypassed_ { false };
    bool                  active_   = false;

    juce::ListenerList<Listener> listeners_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioModule)
};

} // namespace revithion::rack
