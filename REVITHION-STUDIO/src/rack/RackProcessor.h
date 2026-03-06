#pragma once

#include <array>
#include <atomic>
#include <memory>
#include <unordered_map>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include "AudioModule.h"
#include "PatchCable.h"
#include "RoutingGraph.h"
#include "ModuleFactory.h"

namespace revithion::rack
{

// =============================================================================
// RackProcessor — main audio processor for the REVITHION modular rack.
//
// Owns every AudioModule in the rack, manages the cable-based routing graph,
// and drives the real-time audio processing loop.  All structural mutations
// (add / remove modules and cables) are funnelled through a lock-free command
// queue so the GUI thread never blocks the audio thread.
// =============================================================================
class RackProcessor : public juce::AudioProcessor
{
public:
    // ── Listener ────────────────────────────────────────────────────────
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void moduleAdded(RackProcessor*, AudioModule*)          {}
        virtual void moduleRemoved(RackProcessor*, const juce::Uuid&)   {}
        virtual void cableAdded(RackProcessor*, PatchCable*)            {}
        virtual void cableRemoved(RackProcessor*, const juce::Uuid&)    {}
        virtual void rackCleared(RackProcessor*)                        {}
    };

    // ── Construction ────────────────────────────────────────────────────
    RackProcessor()
        : juce::AudioProcessor(BusesProperties()
              .withInput ("Input",  juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    {
    }

    ~RackProcessor() override = default;

    // ── Module management ───────────────────────────────────────────────
    AudioModule* addModule(const juce::String& moduleTypeId);
    AudioModule* addModule(std::unique_ptr<AudioModule> module);
    void         removeModule(const juce::Uuid& moduleId);

    AudioModule* getModule(const juce::Uuid& moduleId)
    {
        auto it = modules_.find(moduleId);
        return it != modules_.end() ? it->second.get() : nullptr;
    }

    const AudioModule* getModule(const juce::Uuid& moduleId) const
    {
        auto it = modules_.find(moduleId);
        return it != modules_.end() ? it->second.get() : nullptr;
    }

    std::vector<AudioModule*> getAllModules() const
    {
        std::vector<AudioModule*> result;
        result.reserve(modules_.size());
        for (const auto& [id, mod] : modules_)
            result.push_back(mod.get());
        return result;
    }

    std::vector<AudioModule*> getModulesByCategory(ModuleCategory category) const;

    int getModuleCount() const { return static_cast<int>(modules_.size()); }

    // ── Cable management ────────────────────────────────────────────────
    PatchCable* addCable(CableEndpoint source, CableEndpoint destination, CableType type);
    void        removeCable(const juce::Uuid& cableId);

    PatchCable* getCable(const juce::Uuid& cableId)
    {
        for (auto& cable : cables_)
            if (cable->getUuid() == cableId)
                return cable.get();
        return nullptr;
    }

    std::vector<PatchCable*> getAllCables() const
    {
        std::vector<PatchCable*> result;
        result.reserve(cables_.size());
        for (const auto& cable : cables_)
            result.push_back(cable.get());
        return result;
    }

    std::vector<PatchCable*> getCablesForModule(const juce::Uuid& moduleId) const
    {
        std::vector<PatchCable*> result;
        for (const auto& cable : cables_)
            if (cable->connects(moduleId))
                result.push_back(cable.get());
        return result;
    }

    bool canConnect(const CableEndpoint& source, const CableEndpoint& destination) const;

    // ── Processing control ──────────────────────────────────────────────
    void suspend()          { suspended_.store(true,  std::memory_order_release); }
    void resume()           { suspended_.store(false, std::memory_order_release); }
    bool isSuspended() const { return suspended_.load(std::memory_order_acquire); }

    float getTotalCpuUsage() const
    {
        return totalCpuUsage_.load(std::memory_order_relaxed);
    }

    float getModuleCpuUsage(const juce::Uuid& moduleId) const
    {
        auto it = moduleCpuUsage_.find(moduleId);
        if (it != moduleCpuUsage_.end() && it->second)
            return it->second->load(std::memory_order_relaxed);
        return 0.0f;
    }

    // ── Routing ─────────────────────────────────────────────────────────
    const RoutingGraph& getRoutingGraph() const { return routingGraph_; }
    std::vector<juce::Uuid> getProcessingOrder() const;

    // ── JUCE AudioProcessor overrides ───────────────────────────────────
    void   prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void   releaseResources() override;
    void   processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    const juce::String getName() const override          { return "REVITHION Rack"; }
    bool   acceptsMidi()  const override                 { return true; }
    bool   producesMidi() const override                 { return true; }
    double getTailLengthSeconds() const override;

    int                getNumPrograms()    override      { return 1; }
    int                getCurrentProgram() override      { return 0; }
    void               setCurrentProgram(int) override   {}
    const juce::String getProgramName(int) override      { return {}; }
    void               changeProgramName(int, const juce::String&) override {}

    juce::AudioProcessorEditor* createEditor() override  { return nullptr; }
    bool hasEditor() const override                      { return true; }

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // ── Rack state (full save / load) ───────────────────────────────────
    juce::ValueTree getRackState() const;
    void            setRackState(const juce::ValueTree& state);
    void            clearRack();

    // ── Listener management ─────────────────────────────────────────────
    void addListener(Listener* listener)    { listeners_.add(listener); }
    void removeListener(Listener* listener) { listeners_.remove(listener); }

private:
    // ── Lock-free command queue (GUI → audio thread) ────────────────────
    struct RackCommand
    {
        enum class Type : uint8_t
        {
            AddModule,
            RemoveModule,
            AddCable,
            RemoveCable,
            SetParameter,
            SetBypass
        };

        Type         type         { Type::AddModule };
        juce::Uuid   targetId;                       // module or cable id
        juce::Uuid   secondaryId;                    // e.g. destination module for cables
        juce::String stringData;                     // module type id, parameter name, etc.
        float        floatData    { 0.0f };
        int          intData      { 0 };
    };

    static constexpr int kCommandQueueSize = 1024;

    // ── Processing internals ────────────────────────────────────────────
    void processCommands();
    void processModuleChain(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi);
    void allocateBuffers(int numChannels, int blockSize);
    void routeAudioBetweenModules(const juce::Uuid& source,
                                  const juce::Uuid& dest,
                                  const Connection& conn);

    // ── Modules & cables ────────────────────────────────────────────────
    std::unordered_map<juce::Uuid,
                       std::unique_ptr<AudioModule>,
                       UuidHash>                        modules_;

    std::vector<std::unique_ptr<PatchCable>>            cables_;
    RoutingGraph                                        routingGraph_;

    std::unordered_map<juce::Uuid,
                       juce::AudioBuffer<float>,
                       UuidHash>                        moduleBuffers_;

    // ── Thread safety ───────────────────────────────────────────────────
    juce::CriticalSection processingLock_;
    std::atomic<bool>     suspended_        { false };
    double                currentSampleRate_ { 0.0 };
    int                   currentBlockSize_  { 0 };

    // ── CPU monitoring ──────────────────────────────────────────────────
    std::atomic<float>  totalCpuUsage_ { 0.0f };

    // std::atomic is non-movable, so we heap-allocate each entry.
    std::unordered_map<juce::Uuid,
                       std::unique_ptr<std::atomic<float>>,
                       UuidHash>                        moduleCpuUsage_;

    // ── Command FIFO ────────────────────────────────────────────────────
    juce::AbstractFifo                         commandFifo_ { kCommandQueueSize };
    std::array<RackCommand, kCommandQueueSize> commandQueue_;

    // ── Listeners ───────────────────────────────────────────────────────
    juce::ListenerList<Listener> listeners_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RackProcessor)
};

} // namespace revithion::rack
