#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>

namespace revithion::ai {

// ── Audio Analysis Results (from AI model like Kimi-Audio) ─────────────────

struct AudioAnalysis {
    float loudnessLUFS      = -14.0f;   // Integrated loudness
    float loudnessShortTerm = -14.0f;
    float loudnessMomentary = -14.0f;
    float truePeak          = 0.0f;     // dBTP
    float dynamicRange      = 10.0f;    // dB
    float stereoWidth       = 0.5f;     // 0=mono, 1=full stereo
    float spectralBalance   = 0.0f;     // -1=dark, 0=balanced, +1=bright
    float crestFactor       = 12.0f;    // Peak to RMS ratio

    // Per-band energy (10 octave bands: 31.5, 63, 125, 250, 500, 1k, 2k, 4k, 8k, 16k Hz)
    std::array<float, 10> bandEnergy{};

    // Detected genre/style (from AI)
    std::string detectedGenre;
    std::string detectedMood;
};

// ── Target Format Presets ──────────────────────────────────────────────────

enum class MasterTarget : uint8_t {
    Streaming,   // -14 LUFS (Spotify, Apple Music, YouTube)
    CD,          // -9 to -12 LUFS
    Vinyl,       // -12 to -14 LUFS, limited bass/treble
    Broadcast,   // EBU R128 (-23 LUFS)
    Film,        // -24 LUFS
    Podcast,     // -16 LUFS mono
    Club,        // -8 to -10 LUFS, heavy limiting
    Reference,   // Match to reference track
    Custom
};

// ── Mastering Chain Module Parameters ──────────────────────────────────────

struct StemEQParams {
    bool active = true;

    struct StemBand {
        float gain      = 0.0f;     // dB
        float frequency = 1000.0f;
        float q         = 1.0f;
    };

    std::array<StemBand, 4> vocalBands;
    std::array<StemBand, 4> drumBands;
    std::array<StemBand, 4> bassBands;
    std::array<StemBand, 4> otherBands;
};

struct DynamicEQParams {
    bool active = true;

    struct Band {
        float frequency = 1000.0f;
        float threshold = -20.0f;
        float ratio     = 2.0f;
        float attack    = 10.0f;   // ms
        float release   = 100.0f;  // ms
        float gain      = 0.0f;
        float q         = 1.0f;
        bool  expand    = false;   // true = expand, false = compress
    };

    std::array<Band, 4> bands;
};

struct MultibandCompParams {
    bool active = true;

    struct Band {
        float threshold  = -20.0f;
        float ratio      = 3.0f;
        float attack     = 10.0f;   // ms
        float release    = 100.0f;  // ms
        float makeupGain = 0.0f;
        float knee       = 3.0f;
    };

    std::array<Band, 4>   bands;
    std::array<float, 3>  crossovers = {200.0f, 2000.0f, 8000.0f};
};

struct StereoImagerParams {
    bool active = true;
    // Per-band stereo width (4 bands): 0=mono, 1=natural, 2=wide
    std::array<float, 4>  width      = {0.5f, 1.0f, 1.2f, 1.5f};
    std::array<float, 3>  crossovers = {200.0f, 2000.0f, 8000.0f};
    bool midSideMode = true;
};

struct ExciterParams {
    bool active = true;

    enum class Type { Tube, Tape, Transistor, Retro };
    Type  type             = Type::Tape;
    float drive            = 0.2f;
    float mix              = 0.3f;
    float highFreqEmphasis = 5000.0f;
};

struct MaximizerParams {
    bool active = true;

    float ceiling          = -0.3f;   // dBTP
    float threshold        = -6.0f;   // dB below ceiling
    float release          = 50.0f;   // ms
    float targetLUFS       = -14.0f;  // Target integrated loudness
    bool  truePeakLimiting = true;

    enum class Character { Transparent, Punchy, Warm, Aggressive };
    Character character = Character::Transparent;
};

struct TonalBalanceParams {
    bool active = true;
    std::string referenceProfile;  // Name of reference curve
    float matchStrength = 0.5f;    // 0 = no matching, 1 = full matching
    // Reference curves: Modern, Vintage, Bass Heavy, Bright, Orchestral
};

// ── EBU R128 Loudness Meter ────────────────────────────────────────────────

struct LoudnessMeter {
    float integrated = -14.0f;  // LUFS
    float shortTerm  = -14.0f;  // LUFS (3s window)
    float momentary  = -14.0f;  // LUFS (400ms window)
    float truePeak   = -1.0f;   // dBTP
    float lra        = 8.0f;    // Loudness Range
    bool  gateOpen   = false;
};

// ── Complete Mastering Chain ───────────────────────────────────────────────

struct MasteringChain {
    StemEQParams        stemEQ;
    DynamicEQParams     dynamicEQ;
    MultibandCompParams multibandComp;
    StereoImagerParams  stereoImager;
    ExciterParams       exciter;
    MaximizerParams     maximizer;
    TonalBalanceParams  tonalBalance;
    MasterTarget        target = MasterTarget::Streaming;
};

// ── AI Mastering Assistant ─────────────────────────────────────────────────
// Analyzes audio and suggests/applies a complete mastering chain.

class MasteringAssistant {
public:
    MasteringAssistant();
    ~MasteringAssistant();

    // Analyze audio and get recommendations
    AudioAnalysis analyzeAudio(const float* samples, int numSamples,
                               int numChannels, double sampleRate);

    // Auto-generate mastering chain from analysis
    MasteringChain suggestChain(const AudioAnalysis& analysis, MasterTarget target);

    // Reference matching
    AudioAnalysis analyzeReference(const float* samples, int numSamples,
                                   int numChannels, double sampleRate);
    MasteringChain matchToReference(const AudioAnalysis& source,
                                    const AudioAnalysis& reference);

    // Apply chain (offline processing)
    bool applyChain(const MasteringChain& chain,
                    const float* input, float* output,
                    int numSamples, int numChannels, double sampleRate);

    // Loudness metering (call per buffer)
    LoudnessMeter updateMeter(const float* samples, int numSamples,
                              int numChannels, double sampleRate);
    void resetMeter();

    // Get current meter values
    const LoudnessMeter& currentMeter() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace revithion::ai
