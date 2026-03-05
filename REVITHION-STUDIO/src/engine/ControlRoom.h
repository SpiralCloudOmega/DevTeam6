#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace revithion::engine {

// Monitor output configuration
struct MonitorOutput {
    std::string name;
    float level = 0.0f;           // dB
    float dim = -20.0f;           // Dim level (dB)
    bool dimmed = false;
    bool mono = false;
    bool mute = false;
    bool phaseInvertL = false;
    bool phaseInvertR = false;

    enum class Config : uint8_t {
        Stereo,
        Surround_5_1,
        Surround_7_1,
        Surround_7_1_4,   // Atmos 7.1.4
        Surround_7_1_6,   // Atmos 7.1.6
    };
    Config config = Config::Stereo;

    // Downmix matrix (for folding surround to stereo)
    float centerLevel = -3.0f;    // dB
    float lfeLevel = -10.0f;      // dB
    float surroundLevel = -3.0f;  // dB
};

// Headphone output (for cue mixes)
struct HeadphoneOutput {
    std::string name;
    float level = 0.0f;
    bool mute = false;
    int sourceIndex = 0;   // Which cue mix to use
};

// Cue mix — independent mix sent to performer headphones
struct CueMix {
    std::string name;
    float level = 0.0f;        // Master level dB
    bool active = true;
    bool talkback = false;

    struct ChannelSend {
        float level = 0.0f;    // dB offset from main mix
        float pan = 0.0f;      // Pan override (-1 to +1)
        bool followMain = true;
        bool mute = false;
    };
    std::unordered_map<std::string, ChannelSend> channelSends;
};

// Talkback configuration
struct Talkback {
    bool active = false;
    float level = 0.0f;       // dB
    bool latchMode = false;    // true = toggle, false = push-to-talk
    bool toAllCues = true;
    std::vector<int> targetCues;

    // Auto-dim: reduce monitors when talkback active
    bool autoDim = true;
    float dimAmount = -20.0f;  // dB
};

// Reference track for A/B comparison
struct ReferenceTrack {
    std::string name;
    std::string filePath;
    float level = 0.0f;       // Level match dB
    bool active = false;
    float startOffset = 0.0;  // Sync offset in seconds
};

// The Control Room — central monitoring hub
class ControlRoom {
public:
    static constexpr int kMaxMonitors = 4;
    static constexpr int kMaxCueMixes = 4;
    static constexpr int kMaxHeadphones = 4;
    static constexpr int kMaxReferences = 8;

    ControlRoom();
    ~ControlRoom();

    // Monitor management
    MonitorOutput& monitor(int index);
    const MonitorOutput& monitor(int index) const;
    int activeMonitor() const;
    void setActiveMonitor(int index);
    void toggleDim();
    void toggleMono();
    void setMonitorLevel(float db);

    // Cue mixes
    CueMix& cueMix(int index);
    const CueMix& cueMix(int index) const;
    void setCueMixLevel(int index, float db);

    // Headphones
    HeadphoneOutput& headphone(int index);
    void setHeadphoneSource(int hpIndex, int cueIndex);

    // Talkback
    Talkback& talkback();
    void talkbackOn();
    void talkbackOff();
    void toggleTalkback();

    // Reference tracks
    void addReference(const ReferenceTrack& ref);
    void removeReference(int index);
    ReferenceTrack& reference(int index);
    int referenceCount() const;
    void setActiveReference(int index);
    int activeReference() const;
    void toggleReference();  // A/B switch between main mix and reference
    bool isReferenceActive() const;

    // Global controls
    float masterLevel() const;
    void setMasterLevel(float db);
    bool isMuted() const;
    void setMute(bool mute);

    // Listen bus (for solo-in-place monitoring)
    float listenLevel() const;
    void setListenLevel(float db);
    bool listenEnabled() const;
    void setListenEnabled(bool e);

private:
    std::array<MonitorOutput, kMaxMonitors> monitors_;
    std::array<CueMix, kMaxCueMixes> cueMixes_;
    std::array<HeadphoneOutput, kMaxHeadphones> headphones_;
    std::vector<ReferenceTrack> references_;
    Talkback talkback_;
    int activeMonitor_ = 0;
    int activeReference_ = -1;
    bool referenceActive_ = false;
    float masterLevel_ = 0.0f;
    bool masterMute_ = false;
    float listenLevel_ = 0.0f;
    bool listenEnabled_ = false;
};

} // namespace revithion::engine
