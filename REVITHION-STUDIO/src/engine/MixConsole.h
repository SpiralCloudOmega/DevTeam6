#pragma once
#include "BoxGraph.h"
#include <array>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace revithion::engine {

// 16 track types (from Nuendo manual)
enum class TrackType : uint8_t {
    Audio,
    Instrument,
    Sampler,
    Drum,
    MIDI,
    Group,
    FX,
    VCA,
    Ruler,
    Marker,
    Arranger,
    Tempo,
    Signature,
    Transpose,
    Chord,
    Folder
};

const char* trackTypeName(TrackType type);

// Automation modes (from Nuendo: 6 modes)
enum class AutomationMode : uint8_t {
    Off,
    Read,
    Touch,
    AutoLatch,
    CrossOver,
    Write,
    Trim
};

// Channel strip processing order (fixed chain per channel)
// Gate → Comp → EQ → Saturation → Limiter
enum class StripModule : uint8_t {
    Gate,
    Compressor,
    EQ,
    Saturation,
    Limiter
};

// Insert slot
struct InsertSlot {
    bool active = false;
    bool preEQ = false;  // pre-fader position
    std::string pluginId;  // Plugin identifier (VST3/AU/Internal)
    // Plugin instance pointer (void* for now, will be typed later)
    void* instance = nullptr;
};

// Send slot
struct SendSlot {
    bool active = false;
    bool preFader = false;
    float level = 0.0f;   // dB (-inf to +12)
    float pan = 0.0f;     // -1 to +1
    UUID targetBus;        // FX/Group channel UUID
};

// 4-band parametric EQ per channel
struct ChannelEQ {
    struct Band {
        enum class Type : uint8_t { LowCut, LowShelf, Peak, HighShelf, HighCut, Notch };
        Type type = Type::Peak;
        float frequency = 1000.0f;  // Hz
        float gain = 0.0f;          // dB
        float q = 1.0f;             // Q factor
        bool active = true;
    };

    std::array<Band, 4> bands;
    bool active = true;

    ChannelEQ() {
        bands[0] = {Band::Type::LowShelf, 80.0f, 0.0f, 0.71f, true};
        bands[1] = {Band::Type::Peak, 500.0f, 0.0f, 1.0f, true};
        bands[2] = {Band::Type::Peak, 3000.0f, 0.0f, 1.0f, true};
        bands[3] = {Band::Type::HighShelf, 12000.0f, 0.0f, 0.71f, true};
    }
};

// Channel strip (fixed processing chain)
struct ChannelStrip {
    // Gate
    struct {
        float threshold = -40.0f;
        float attack = 1.0f;
        float hold = 50.0f;
        float release = 100.0f;
        bool active = false;
    } gate;
    // Compressor
    struct {
        float threshold = -20.0f;
        float ratio = 4.0f;
        float attack = 10.0f;
        float release = 100.0f;
        float makeupGain = 0.0f;
        bool active = false;
    } compressor;
    // EQ
    ChannelEQ eq;
    // Saturation
    struct {
        float drive = 0.0f;
        enum Type { Tape, Tube, Transistor } type = Tape;
        bool active = false;
    } saturation;
    // Limiter
    struct {
        float ceiling = 0.0f;
        float release = 50.0f;
        bool active = false;
    } limiter;
};

// A single mixer channel
class MixChannel {
public:
    MixChannel(const std::string& name, TrackType type, UUID id = UUID{});

    // Identity
    const UUID& id() const { return id_; }
    const std::string& name() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    TrackType type() const { return type_; }

    // Level controls
    float fader() const { return faderDb_; }         // dB (-inf to +12)
    void setFader(float db) { faderDb_ = db; }
    float pan() const { return pan_; }               // -1 to +1
    void setPan(float p) { pan_ = p; }
    bool mute() const { return muted_; }
    void setMute(bool m) { muted_ = m; }
    bool solo() const { return soloed_; }
    void setSolo(bool s) { soloed_ = s; }
    bool recordEnabled() const { return recordEnabled_; }
    void setRecordEnabled(bool r) { recordEnabled_ = r; }
    bool monitorEnabled() const { return monitorEnabled_; }
    void setMonitorEnabled(bool m) { monitorEnabled_ = m; }

    // Inserts (16 slots)
    static constexpr int kMaxInserts = 16;
    InsertSlot& insert(int index) { return inserts_[index]; }
    const InsertSlot& insert(int index) const { return inserts_[index]; }

    // Sends (8 slots)
    static constexpr int kMaxSends = 8;
    SendSlot& send(int index) { return sends_[index]; }
    const SendSlot& send(int index) const { return sends_[index]; }

    // Channel strip
    ChannelStrip& strip() { return strip_; }
    const ChannelStrip& strip() const { return strip_; }

    // Automation
    AutomationMode automationMode() const { return autoMode_; }
    void setAutomationMode(AutomationMode mode) { autoMode_ = mode; }

    // Phase/polarity
    bool phaseInvert() const { return phaseInvert_; }
    void setPhaseInvert(bool invert) { phaseInvert_ = invert; }

    // Input/output routing
    const std::string& inputBus() const { return inputBus_; }
    void setInputBus(const std::string& bus) { inputBus_ = bus; }
    const std::string& outputBus() const { return outputBus_; }
    void setOutputBus(const std::string& bus) { outputBus_ = bus; }

    // Metering (set by audio thread)
    float peakL() const { return peakL_; }
    float peakR() const { return peakR_; }
    float rmsL() const { return rmsL_; }
    float rmsR() const { return rmsR_; }
    void setMetering(float pL, float pR, float rL, float rR) {
        peakL_ = pL; peakR_ = pR; rmsL_ = rL; rmsR_ = rR;
    }

    // VCA group (for VCA fader type)
    UUID vcaGroup() const { return vcaGroup_; }
    void setVCAGroup(const UUID& id) { vcaGroup_ = id; }

    // Color (for UI)
    uint32_t color() const { return color_; }
    void setColor(uint32_t c) { color_ = c; }

private:
    UUID id_;
    std::string name_;
    TrackType type_;
    float faderDb_ = 0.0f;
    float pan_ = 0.0f;
    bool muted_ = false;
    bool soloed_ = false;
    bool recordEnabled_ = false;
    bool monitorEnabled_ = false;
    bool phaseInvert_ = false;
    AutomationMode autoMode_ = AutomationMode::Read;
    std::string inputBus_;
    std::string outputBus_ = "Stereo Out";
    std::array<InsertSlot, kMaxInserts> inserts_{};
    std::array<SendSlot, kMaxSends> sends_{};
    ChannelStrip strip_;
    UUID vcaGroup_;
    uint32_t color_ = 0xFF4488FF;
    // Metering (atomic in real impl, float for now)
    float peakL_ = 0.0f, peakR_ = 0.0f;
    float rmsL_ = 0.0f, rmsR_ = 0.0f;
};

// The MixConsole — manages all mixer channels
class MixConsole {
public:
    MixConsole();
    ~MixConsole();

    // Channel management
    MixChannel* addChannel(const std::string& name, TrackType type);
    void removeChannel(const UUID& id);
    MixChannel* channel(const UUID& id);
    MixChannel* channelByIndex(int index);
    int channelCount() const;

    // Iterate channels
    void forEachChannel(std::function<void(MixChannel*)> fn);
    void forEachChannel(std::function<void(const MixChannel*)> fn) const;

    // Solo management (solo-defeat, solo-exclusive, etc.)
    bool hasSoloedChannels() const;
    void clearAllSolos();

    // Master section
    MixChannel* masterBus() { return masterBus_.get(); }

    // Processing format
    double sampleRate() const { return sampleRate_; }
    void setSampleRate(double sr) { sampleRate_ = sr; }
    int bufferSize() const { return bufferSize_; }
    void setBufferSize(int bs) { bufferSize_ = bs; }

    // 64-bit float internal processing
    static constexpr int kInternalBitDepth = 64;

private:
    std::vector<std::unique_ptr<MixChannel>> channels_;
    std::unique_ptr<MixChannel> masterBus_;
    double sampleRate_ = 48000.0;
    int bufferSize_ = 512;
};

} // namespace revithion::engine
