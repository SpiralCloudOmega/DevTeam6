#pragma once
/*  ═══════════════════════════════════════════════════════════════════════════
    REVITHION STUDIO — AudioSettings
    ═══════════════════════════════════════════════════════════════════════════
    Central audio-engine configuration: sample rate, bit depth, buffer size.

    WHY 96 kHz IS THE SWEET SPOT
    ─────────────────────────────
    •  Nyquist at 48 kHz — captures the full audible range (20 Hz–20 kHz)
       with generous headroom above 20 kHz for anti-alias filter roll-off.
    •  Latency is half that of 48 kHz for the same buffer size, giving a
       128-sample buffer only ~1.33 ms one-way latency — perceptually
       instant for real-time instrument playing and spatial 3D audio.
    •  Oversampling benefits DSP (saturation, compression, modulation)
       without needing internal 2× upsample stages, saving CPU.
    •  File sizes are only 2× CD-rate — manageable on modern storage,
       unlike 192 kHz (4×) or 384 kHz (8×).
    •  Widely supported by every professional audio interface on the market.
    •  At 96 kHz the signal path "feels real" — no perceptible lag between
       a guitar pick attack and the monitored sound, critical for performers.
    ═══════════════════════════════════════════════════════════════════════════ */

#include <JuceHeader.h>

#include <array>
#include <atomic>
#include <cstdint>
#include <string>
#include <string_view>

namespace revithion::settings {

// ── Sample Rate ─────────────────────────────────────────────────────────────
enum class SampleRate : int {
    Rate_44100  = 44100,    // CD quality — streaming export, Spotify/Apple Music
    Rate_48000  = 48000,    // Video standard — film / TV / broadcast
    Rate_88200  = 88200,    // 2× CD — vinyl mastering, DDP pre-master
    Rate_96000  = 96000,    // ★ SWEET SPOT — spatial 3D audio, real-time instruments
    Rate_176400 = 176400,   // 4× CD — high-end mastering
    Rate_192000 = 192000,   // Ultra HD Audio — archival, DXD source
    Rate_352800 = 352800,   // DSD-level — extreme mastering, academic
    Rate_384000 = 384000    // Maximum — scientific measurement, preservation
};

// ── Bit Depth ───────────────────────────────────────────────────────────────
enum class BitDepth : int {
    Bits_16 = 16,   // CD standard, final distribution
    Bits_24 = 24,   // ★ Professional standard — 144 dB dynamic range
    Bits_32 = 32,   // 32-bit float — internal processing headroom
    Bits_64 = 64    // 64-bit double — internal only, scientific
};

// ── Buffer Size ─────────────────────────────────────────────────────────────
enum class BufferSize : int {
    Samples_32   = 32,     // Ultra-low latency   (~0.33 ms @ 96 kHz)
    Samples_64   = 64,     // Very low latency    (~0.67 ms @ 96 kHz)
    Samples_128  = 128,    // ★ Low latency       (~1.33 ms @ 96 kHz) — default for instruments
    Samples_256  = 256,    // Standard            (~2.67 ms @ 96 kHz)
    Samples_512  = 512,    // Comfortable         (~5.33 ms @ 96 kHz)
    Samples_1024 = 1024,   // Mixing / mastering  (~10.67 ms @ 96 kHz)
    Samples_2048 = 2048,   // High stability      (~21.33 ms @ 96 kHz)
    Samples_4096 = 4096    // Maximum stability   (~42.67 ms @ 96 kHz)
};

// ── Helpers — enum arrays for iteration ─────────────────────────────────────
inline constexpr std::array kAllSampleRates = {
    SampleRate::Rate_44100, SampleRate::Rate_48000, SampleRate::Rate_88200,
    SampleRate::Rate_96000, SampleRate::Rate_176400, SampleRate::Rate_192000,
    SampleRate::Rate_352800, SampleRate::Rate_384000
};

inline constexpr std::array kAllBitDepths = {
    BitDepth::Bits_16, BitDepth::Bits_24, BitDepth::Bits_32, BitDepth::Bits_64
};

inline constexpr std::array kAllBufferSizes = {
    BufferSize::Samples_32, BufferSize::Samples_64, BufferSize::Samples_128,
    BufferSize::Samples_256, BufferSize::Samples_512, BufferSize::Samples_1024,
    BufferSize::Samples_2048, BufferSize::Samples_4096
};

// ── Defaults ────────────────────────────────────────────────────────────────
inline constexpr SampleRate kDefaultSampleRate = SampleRate::Rate_96000;
inline constexpr BitDepth   kDefaultBitDepth   = BitDepth::Bits_24;
inline constexpr BufferSize kDefaultBufferSize = BufferSize::Samples_128;

// ── Free-function utilities (constexpr, zero-overhead) ──────────────────────

/// Integer value of a SampleRate enum.
constexpr int toInt(SampleRate r) noexcept { return static_cast<int>(r); }
constexpr int toInt(BitDepth b)   noexcept { return static_cast<int>(b); }
constexpr int toInt(BufferSize s) noexcept { return static_cast<int>(s); }

/// One-way latency in milliseconds for a given buffer/rate pair.
constexpr float latencyMs(BufferSize buf, SampleRate rate) noexcept {
    return static_cast<float>(toInt(buf)) / static_cast<float>(toInt(rate)) * 1000.0f;
}

/// Round-trip (input → process → output) latency.
constexpr float roundTripLatencyMs(BufferSize buf, SampleRate rate) noexcept {
    return 2.0f * latencyMs(buf, rate);
}

/// Nyquist frequency for a given sample rate.
constexpr float nyquistHz(SampleRate rate) noexcept {
    return static_cast<float>(toInt(rate)) / 2.0f;
}

/// True when the sample rate is 96 kHz — the sweet spot for spatial audio
/// and real-time instrument monitoring.
constexpr bool isOptimalForSpatialAudio(SampleRate r) noexcept {
    return r == SampleRate::Rate_96000;
}

/// Alias — 96 kHz is equally optimal for live instrument performance.
constexpr bool isOptimalForInstruments(SampleRate r) noexcept {
    return r == SampleRate::Rate_96000;
}

/// Human-readable label, e.g. "96.0 kHz".
std::string sampleRateLabel(SampleRate r);

/// Detailed description of a sample rate's intended use case.
std::string sampleRateDescription(SampleRate r);

/// Human-readable label, e.g. "24-bit".
std::string bitDepthLabel(BitDepth b);

/// Human-readable label, e.g. "128 samples".
std::string bufferSizeLabel(BufferSize s);

/// Look up a SampleRate enum from an integer value. Returns nullopt on miss.
std::optional<SampleRate> sampleRateFromInt(int value) noexcept;
std::optional<BitDepth>   bitDepthFromInt(int value)   noexcept;
std::optional<BufferSize> bufferSizeFromInt(int value)  noexcept;

// ═════════════════════════════════════════════════════════════════════════════
// AudioSettings — thread-safe, real-time-safe configuration store
// ═════════════════════════════════════════════════════════════════════════════
class AudioSettings {
public:
    /// Listener interface for change notifications (called on the message thread).
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void audioSettingsChanged(const AudioSettings& settings) = 0;
    };

    AudioSettings();
    ~AudioSettings();

    // ── Getters (lock-free, safe to call from the audio thread) ─────────
    SampleRate getSampleRate() const noexcept;
    BitDepth   getBitDepth()   const noexcept;
    BufferSize getBufferSize() const noexcept;

    // ── Computed properties ─────────────────────────────────────────────
    float getLatencyMs()          const noexcept;
    float getRoundTripLatencyMs() const noexcept;
    float getNyquistHz()          const noexcept;

    bool isOptimalForSpatialAudio() const noexcept;
    bool isOptimalForInstruments()  const noexcept;
    bool isHighLatency()            const noexcept;  // > 10 ms one-way

    // ── Setters (message-thread only — notifies listeners) ──────────────
    void setSampleRate(SampleRate rate);
    void setBitDepth(BitDepth depth);
    void setBufferSize(BufferSize size);
    void applyAll(SampleRate rate, BitDepth depth, BufferSize size);

    void resetToDefaults();

    // ── Persistence (JUCE ValueTree serialisation) ──────────────────────
    juce::ValueTree toValueTree() const;
    void fromValueTree(const juce::ValueTree& tree);

    void saveToFile(const juce::File& file) const;
    bool loadFromFile(const juce::File& file);

    /// Default settings file inside the user's app-data folder.
    static juce::File getDefaultSettingsFile();

    // ── Listener management ─────────────────────────────────────────────
    void addListener(Listener* listener);
    void removeListener(Listener* listener);

private:
    void notifyListeners();

    // Atomics give lock-free reads on the audio thread.
    std::atomic<int> sampleRate_ { toInt(kDefaultSampleRate) };
    std::atomic<int> bitDepth_   { toInt(kDefaultBitDepth) };
    std::atomic<int> bufferSize_ { toInt(kDefaultBufferSize) };

    juce::ListenerList<Listener> listeners_;
};

} // namespace revithion::settings
