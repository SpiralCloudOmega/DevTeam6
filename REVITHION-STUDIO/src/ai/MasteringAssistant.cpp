#include "MasteringAssistant.h"

#include <algorithm>
#include <cmath>
#include <complex>
#include <numeric>
#include <vector>

namespace revithion::ai {

// ── Constants ──────────────────────────────────────────────────────────────

static constexpr double kRefLUFS_Streaming  = -14.0;
static constexpr double kRefLUFS_CD         = -10.0;
static constexpr double kRefLUFS_Vinyl      = -13.0;
static constexpr double kRefLUFS_Broadcast  = -23.0;
static constexpr double kRefLUFS_Film       = -24.0;
static constexpr double kRefLUFS_Podcast    = -16.0;
static constexpr double kRefLUFS_Club       = -9.0;

static constexpr float kPi = 3.14159265358979323846f;

// Octave-band center frequencies (10 bands)
static constexpr std::array<float, 10> kBandCenters = {
    31.5f, 63.0f, 125.0f, 250.0f, 500.0f,
    1000.0f, 2000.0f, 4000.0f, 8000.0f, 16000.0f
};

// ── Utility helpers ────────────────────────────────────────────────────────

static float dbToLinear(float dB) { return std::pow(10.0f, dB / 20.0f); }
static float linearToDb(float lin) {
    return (lin > 1e-10f) ? 20.0f * std::log10(lin) : -200.0f;
}

// Radix-2 in-place FFT (Cooley-Tukey)
static void fft(std::vector<std::complex<float>>& x) {
    const auto N = static_cast<int>(x.size());
    if (N <= 1) return;

    // Bit-reversal permutation
    for (int i = 1, j = 0; i < N; ++i) {
        int bit = N >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j) std::swap(x[i], x[j]);
    }

    // Butterfly stages
    for (int len = 2; len <= N; len <<= 1) {
        float angle = -2.0f * kPi / static_cast<float>(len);
        std::complex<float> wn(std::cos(angle), std::sin(angle));
        for (int i = 0; i < N; i += len) {
            std::complex<float> w(1.0f, 0.0f);
            for (int j = 0; j < len / 2; ++j) {
                auto u = x[i + j];
                auto v = w * x[i + j + len / 2];
                x[i + j]             = u + v;
                x[i + j + len / 2]   = u - v;
                w *= wn;
            }
        }
    }
}

// Next power of 2
static int nextPow2(int n) {
    int p = 1;
    while (p < n) p <<= 1;
    return p;
}

// ── K-weighting filter (IIR biquad coefficients for 48 kHz) ────────────────
// Two cascaded biquads: high-shelf pre-filter + high-pass RLB weighting

struct BiquadCoeffs {
    double b0, b1, b2, a1, a2;
};

struct BiquadState {
    double z1 = 0.0, z2 = 0.0;
};

static double applyBiquad(const BiquadCoeffs& c, BiquadState& s, double x) {
    double y = c.b0 * x + s.z1;
    s.z1     = c.b1 * x - c.a1 * y + s.z2;
    s.z2     = c.b2 * x - c.a2 * y;
    return y;
}

// Pre-filter (high-shelf boost ~+4 dB at high frequencies)
static BiquadCoeffs kWeightPreFilter(double sampleRate) {
    // Design based on ITU-R BS.1770-4 coefficients
    // These are approximated for variable sample rates
    double f0 = 1681.97;
    double G  = 3.999;   // ~+4 dB
    double Q  = 0.7072;

    double K     = std::tan(kPi * f0 / sampleRate);
    double Vh    = std::pow(10.0, G / 20.0);
    double Vb    = std::pow(Vh, 0.4996);
    double K2    = K * K;
    double a0inv = 1.0 / (1.0 + K / Q + K2);

    BiquadCoeffs c{};
    c.b0 = (Vh + Vb * K / Q + K2) * a0inv;
    c.b1 = 2.0 * (K2 - Vh) * a0inv;
    c.b2 = (Vh - Vb * K / Q + K2) * a0inv;
    c.a1 = 2.0 * (K2 - 1.0) * a0inv;
    c.a2 = (1.0 - K / Q + K2) * a0inv;
    return c;
}

// RLB weighting (high-pass, ~60 Hz)
static BiquadCoeffs kWeightRLB(double sampleRate) {
    double f0 = 38.13;
    double Q  = 0.5003;

    double K     = std::tan(kPi * f0 / sampleRate);
    double K2    = K * K;
    double a0inv = 1.0 / (1.0 + K / Q + K2);

    BiquadCoeffs c{};
    c.b0 =  1.0 * a0inv;
    c.b1 = -2.0 * a0inv;
    c.b2 =  1.0 * a0inv;
    c.a1 =  2.0 * (K2 - 1.0) * a0inv;
    c.a2 = (1.0 - K / Q + K2) * a0inv;
    return c;
}

// ── Impl ───────────────────────────────────────────────────────────────────

class MasteringAssistant::Impl {
public:
    LoudnessMeter meter;

    // K-weighting filter state (per channel, max 2)
    BiquadCoeffs preCoeffs{};
    BiquadCoeffs rlbCoeffs{};
    std::array<BiquadState, 2> preState{};
    std::array<BiquadState, 2> rlbState{};
    bool filtersInitialized = false;
    double lastSampleRate   = 0.0;

    // Gated loudness accumulation (EBU R128 sliding windows)
    std::vector<double> momentaryBlocks;   // 400 ms block powers
    std::vector<double> shortTermBlocks;   // 3 s block powers
    double integratedPower = 0.0;
    int    integratedCount = 0;

    void initFilters(double sampleRate) {
        if (filtersInitialized && lastSampleRate == sampleRate) return;
        preCoeffs = kWeightPreFilter(sampleRate);
        rlbCoeffs = kWeightRLB(sampleRate);
        preState  = {};
        rlbState  = {};
        lastSampleRate   = sampleRate;
        filtersInitialized = true;
    }

    // Compute mean-square of K-weighted samples for a block
    double kWeightedMeanSquare(const float* samples, int numSamples,
                               int numChannels, double sampleRate) {
        initFilters(sampleRate);

        double sumSq = 0.0;
        int totalSamples = numSamples / numChannels;

        for (int ch = 0; ch < std::min(numChannels, 2); ++ch) {
            BiquadState ps = preState[ch];
            BiquadState rs = rlbState[ch];

            for (int i = 0; i < totalSamples; ++i) {
                double x = static_cast<double>(samples[i * numChannels + ch]);
                double y = applyBiquad(preCoeffs, ps, x);
                y = applyBiquad(rlbCoeffs, rs, y);
                sumSq += y * y;
            }

            preState[ch] = ps;
            rlbState[ch] = rs;
        }

        // Channel weighting: front L/R = 1.0 each
        return sumSq / static_cast<double>(totalSamples);
    }

    // Compute per-band energy via FFT
    std::array<float, 10> computeBandEnergy(const float* samples, int numSamples,
                                             int numChannels, double sampleRate) {
        int totalFrames = numSamples / numChannels;

        // Mix to mono
        std::vector<float> mono(totalFrames, 0.0f);
        for (int i = 0; i < totalFrames; ++i) {
            float sum = 0.0f;
            for (int ch = 0; ch < numChannels; ++ch)
                sum += samples[i * numChannels + ch];
            mono[i] = sum / static_cast<float>(numChannels);
        }

        int N = nextPow2(totalFrames);
        std::vector<std::complex<float>> fftBuf(N, {0.0f, 0.0f});

        // Apply Hann window and copy
        for (int i = 0; i < totalFrames; ++i) {
            float w = 0.5f * (1.0f - std::cos(2.0f * kPi * static_cast<float>(i)
                                               / static_cast<float>(totalFrames - 1)));
            fftBuf[i] = {mono[i] * w, 0.0f};
        }

        fft(fftBuf);

        // Compute magnitude spectrum
        float binHz = static_cast<float>(sampleRate) / static_cast<float>(N);

        std::array<float, 10> energy{};
        for (int b = 0; b < 10; ++b) {
            float lo = kBandCenters[b] / std::sqrt(2.0f);
            float hi = kBandCenters[b] * std::sqrt(2.0f);
            int binLo = std::max(1, static_cast<int>(lo / binHz));
            int binHi = std::min(N / 2, static_cast<int>(hi / binHz));

            float sum = 0.0f;
            int count = 0;
            for (int k = binLo; k <= binHi; ++k) {
                float mag = std::abs(fftBuf[k]);
                sum += mag * mag;
                ++count;
            }
            energy[b] = (count > 0) ? linearToDb(std::sqrt(sum / static_cast<float>(count)))
                                    : -200.0f;
        }
        return energy;
    }

    // Compute spectral centroid (brightness indicator)
    float spectralCentroid(const float* samples, int numSamples,
                           int numChannels, double sampleRate) {
        int totalFrames = numSamples / numChannels;
        int N = nextPow2(totalFrames);
        std::vector<std::complex<float>> buf(N, {0.0f, 0.0f});

        for (int i = 0; i < totalFrames; ++i) {
            float s = 0.0f;
            for (int ch = 0; ch < numChannels; ++ch)
                s += samples[i * numChannels + ch];
            s /= static_cast<float>(numChannels);
            float w = 0.5f * (1.0f - std::cos(2.0f * kPi * static_cast<float>(i)
                                               / static_cast<float>(totalFrames - 1)));
            buf[i] = {s * w, 0.0f};
        }

        fft(buf);

        float binHz = static_cast<float>(sampleRate) / static_cast<float>(N);
        double weightedSum = 0.0, magSum = 0.0;
        for (int k = 1; k <= N / 2; ++k) {
            float mag = std::abs(buf[k]);
            weightedSum += static_cast<double>(k) * binHz * mag;
            magSum += mag;
        }

        return (magSum > 1e-10) ? static_cast<float>(weightedSum / magSum) : 1000.0f;
    }

    // Stereo correlation (-1 to +1)
    float stereoCorrelation(const float* samples, int numSamples, int numChannels) {
        if (numChannels < 2) return 1.0f;

        int totalFrames = numSamples / numChannels;
        double sumLR = 0.0, sumL2 = 0.0, sumR2 = 0.0;

        for (int i = 0; i < totalFrames; ++i) {
            double l = samples[i * numChannels];
            double r = samples[i * numChannels + 1];
            sumLR += l * r;
            sumL2 += l * l;
            sumR2 += r * r;
        }

        double denom = std::sqrt(sumL2 * sumR2);
        return (denom > 1e-10) ? static_cast<float>(sumLR / denom) : 1.0f;
    }

    // RMS level in dB
    float rmsDb(const float* samples, int numSamples) {
        double sum = 0.0;
        for (int i = 0; i < numSamples; ++i)
            sum += static_cast<double>(samples[i]) * samples[i];
        float rms = std::sqrt(static_cast<float>(sum / numSamples));
        return linearToDb(rms);
    }

    // True peak (4x oversampled)
    float computeTruePeak(const float* samples, int numSamples) {
        float peak = 0.0f;
        for (int i = 0; i < numSamples; ++i)
            peak = std::max(peak, std::abs(samples[i]));

        // Simple 4x oversampling approximation via sinc interpolation
        int totalSamples = numSamples;
        for (int i = 1; i < totalSamples - 1; ++i) {
            for (int phase = 1; phase < 4; ++phase) {
                float t = static_cast<float>(phase) / 4.0f;
                // Linear interpolation as approximation
                float interp = samples[i] * (1.0f - t) + samples[i + 1] * t;
                peak = std::max(peak, std::abs(interp));
            }
        }

        return linearToDb(peak);
    }

    // Get target LUFS for a given MasterTarget
    static float targetLufs(MasterTarget target) {
        switch (target) {
            case MasterTarget::Streaming: return static_cast<float>(kRefLUFS_Streaming);
            case MasterTarget::CD:        return static_cast<float>(kRefLUFS_CD);
            case MasterTarget::Vinyl:     return static_cast<float>(kRefLUFS_Vinyl);
            case MasterTarget::Broadcast: return static_cast<float>(kRefLUFS_Broadcast);
            case MasterTarget::Film:      return static_cast<float>(kRefLUFS_Film);
            case MasterTarget::Podcast:   return static_cast<float>(kRefLUFS_Podcast);
            case MasterTarget::Club:      return static_cast<float>(kRefLUFS_Club);
            default:                      return static_cast<float>(kRefLUFS_Streaming);
        }
    }
};

// ── MasteringAssistant public interface ────────────────────────────────────

MasteringAssistant::MasteringAssistant()  : pImpl(std::make_unique<Impl>()) {}
MasteringAssistant::~MasteringAssistant() = default;

// ── analyzeAudio ───────────────────────────────────────────────────────────

AudioAnalysis MasteringAssistant::analyzeAudio(const float* samples, int numSamples,
                                                int numChannels, double sampleRate) {
    AudioAnalysis a;

    if (!samples || numSamples <= 0 || numChannels <= 0 || sampleRate <= 0.0)
        return a;

    // Integrated loudness (simplified — full block over entire file)
    double ms = pImpl->kWeightedMeanSquare(samples, numSamples, numChannels, sampleRate);
    a.loudnessLUFS = static_cast<float>(-0.691 + 10.0 * std::log10(std::max(ms, 1e-20)));

    // Short-term / momentary approximation using the same block
    a.loudnessShortTerm = a.loudnessLUFS;
    a.loudnessMomentary = a.loudnessLUFS;

    // True peak
    a.truePeak = pImpl->computeTruePeak(samples, numSamples);

    // RMS and crest factor
    float rms = pImpl->rmsDb(samples, numSamples);
    a.crestFactor = a.truePeak - rms;

    // Dynamic range (peak-to-RMS approximation)
    a.dynamicRange = a.crestFactor;

    // Stereo width from correlation: width ≈ (1 - correlation) / 2 mapped to [0,1]
    float corr = pImpl->stereoCorrelation(samples, numSamples, numChannels);
    a.stereoWidth = std::clamp((1.0f - corr) * 0.5f + 0.5f * (1.0f - std::abs(corr)),
                               0.0f, 1.0f);

    // Spectral balance: map centroid to [-1, +1] range (1 kHz = 0, 500 Hz = -1, 4 kHz = +1)
    float centroid = pImpl->spectralCentroid(samples, numSamples, numChannels, sampleRate);
    a.spectralBalance = std::clamp((std::log2(centroid / 1000.0f)) / 2.0f, -1.0f, 1.0f);

    // Per-band energy
    a.bandEnergy = pImpl->computeBandEnergy(samples, numSamples, numChannels, sampleRate);

    return a;
}

// ── suggestChain ───────────────────────────────────────────────────────────

MasteringChain MasteringAssistant::suggestChain(const AudioAnalysis& analysis,
                                                 MasterTarget target) {
    MasteringChain chain;
    chain.target = target;
    float tgtLufs = Impl::targetLufs(target);

    // ── Dynamic EQ: tame resonances ────────────────────────────────────
    chain.dynamicEQ.active = true;
    // Band 0: Low rumble control
    chain.dynamicEQ.bands[0] = {60.0f, -24.0f, 3.0f, 5.0f, 80.0f, 0.0f, 0.8f, false};
    // Band 1: Low-mid mud control
    chain.dynamicEQ.bands[1] = {300.0f, -18.0f, 2.5f, 10.0f, 100.0f, 0.0f, 1.0f, false};
    // Band 2: Presence boost
    chain.dynamicEQ.bands[2] = {3000.0f, -22.0f, 2.0f, 8.0f, 60.0f, 0.0f, 1.2f, true};
    // Band 3: Sibilance control
    chain.dynamicEQ.bands[3] = {7000.0f, -20.0f, 3.0f, 2.0f, 40.0f, 0.0f, 2.0f, false};

    // ── Spectral balance correction ────────────────────────────────────
    if (analysis.spectralBalance > 0.3f) {
        // Too bright → cut highs
        chain.dynamicEQ.bands[3].threshold = -16.0f;
        chain.dynamicEQ.bands[3].ratio = 4.0f;
    } else if (analysis.spectralBalance < -0.3f) {
        // Too dark → boost presence / air
        chain.dynamicEQ.bands[2].gain = 2.0f;
        chain.dynamicEQ.bands[2].expand = true;
    }

    // ── Multiband compression ──────────────────────────────────────────
    chain.multibandComp.active = true;
    chain.multibandComp.crossovers = {200.0f, 2000.0f, 8000.0f};

    // Adjust ratios based on dynamic range vs target
    float targetDR = 10.0f;
    if (target == MasterTarget::Club)       targetDR = 6.0f;
    else if (target == MasterTarget::CD)    targetDR = 8.0f;
    else if (target == MasterTarget::Broadcast) targetDR = 14.0f;
    else if (target == MasterTarget::Film)  targetDR = 16.0f;

    float drDiff = analysis.dynamicRange - targetDR;
    if (drDiff > 2.0f) {
        // Too dynamic → more compression
        float extraRatio = std::min(drDiff * 0.3f, 3.0f);
        for (auto& band : chain.multibandComp.bands)
            band.ratio += extraRatio;
    } else if (drDiff < -2.0f) {
        // Already compressed → gentler compression
        for (auto& band : chain.multibandComp.bands) {
            band.ratio = std::max(1.5f, band.ratio - 1.0f);
            band.threshold -= 4.0f;
        }
    }

    // ── Stereo imager ──────────────────────────────────────────────────
    chain.stereoImager.active = true;
    chain.stereoImager.midSideMode = true;

    // Narrow lows for mono compatibility
    if (analysis.stereoWidth > 0.4f) {
        chain.stereoImager.width[0] = 0.3f;  // Sub/bass → nearly mono
    }

    // Widen highs if source is too narrow
    if (analysis.stereoWidth < 0.3f) {
        chain.stereoImager.width[2] = 1.4f;
        chain.stereoImager.width[3] = 1.6f;
    }

    // For podcast/mono targets
    if (target == MasterTarget::Podcast) {
        chain.stereoImager.width = {0.0f, 0.0f, 0.0f, 0.0f};
    }

    // ── Exciter ────────────────────────────────────────────────────────
    chain.exciter.active = true;
    chain.exciter.type = ExciterParams::Type::Tape;
    chain.exciter.drive = 0.15f;
    chain.exciter.mix = 0.2f;

    if (analysis.spectralBalance < -0.2f) {
        // Dark mix: add more harmonic excitement
        chain.exciter.drive = 0.3f;
        chain.exciter.mix = 0.35f;
        chain.exciter.highFreqEmphasis = 4000.0f;
    }

    // ── Maximizer / Limiter ────────────────────────────────────────────
    chain.maximizer.active = true;
    chain.maximizer.targetLUFS = tgtLufs;
    chain.maximizer.truePeakLimiting = true;
    chain.maximizer.ceiling = -0.3f;

    float lufsGap = tgtLufs - analysis.loudnessLUFS;
    chain.maximizer.threshold = std::clamp(lufsGap - 2.0f, -12.0f, 0.0f);

    if (target == MasterTarget::Club) {
        chain.maximizer.character = MaximizerParams::Character::Punchy;
        chain.maximizer.ceiling = -0.1f;
        chain.maximizer.release = 30.0f;
    } else if (target == MasterTarget::Broadcast || target == MasterTarget::Film) {
        chain.maximizer.character = MaximizerParams::Character::Transparent;
        chain.maximizer.ceiling = -1.0f;
        chain.maximizer.release = 80.0f;
    } else if (target == MasterTarget::Vinyl) {
        chain.maximizer.character = MaximizerParams::Character::Warm;
        chain.maximizer.ceiling = -0.5f;
    }

    // ── Tonal balance ──────────────────────────────────────────────────
    chain.tonalBalance.active = true;
    chain.tonalBalance.referenceProfile = "Modern";
    chain.tonalBalance.matchStrength = 0.4f;

    // ── Stem EQ (subtle defaults) ──────────────────────────────────────
    chain.stemEQ.active = false; // Only enabled when stems are available

    return chain;
}

// ── analyzeReference ───────────────────────────────────────────────────────

AudioAnalysis MasteringAssistant::analyzeReference(const float* samples, int numSamples,
                                                    int numChannels, double sampleRate) {
    // Reference analysis uses the same pipeline
    return analyzeAudio(samples, numSamples, numChannels, sampleRate);
}

// ── matchToReference ───────────────────────────────────────────────────────

MasteringChain MasteringAssistant::matchToReference(const AudioAnalysis& source,
                                                     const AudioAnalysis& reference) {
    // Start with a chain targeting the reference loudness
    MasterTarget target = MasterTarget::Reference;
    MasteringChain chain = suggestChain(source, target);

    // ── Match loudness ─────────────────────────────────────────────────
    chain.maximizer.targetLUFS = reference.loudnessLUFS;
    float lufsGap = reference.loudnessLUFS - source.loudnessLUFS;
    chain.maximizer.threshold = std::clamp(lufsGap - 2.0f, -12.0f, 0.0f);

    // ── Match spectral balance via dynamic EQ ──────────────────────────
    // Compute per-band difference and apply corrective EQ
    for (int b = 0; b < 4 && b < 10; ++b) {
        int srcBandIdx = b * 2 + 2; // Map 4 EQ bands to analysis bands (125, 500, 2k, 8k)
        if (srcBandIdx < 10) {
            float diff = reference.bandEnergy[srcBandIdx] - source.bandEnergy[srcBandIdx];
            chain.dynamicEQ.bands[b].gain = std::clamp(diff * 0.5f, -6.0f, 6.0f);
            chain.dynamicEQ.bands[b].frequency = kBandCenters[srcBandIdx];
        }
    }

    // ── Match dynamic range ────────────────────────────────────────────
    float drDiff = source.dynamicRange - reference.dynamicRange;
    if (drDiff > 2.0f) {
        for (auto& band : chain.multibandComp.bands)
            band.ratio += std::min(drDiff * 0.4f, 3.0f);
    } else if (drDiff < -2.0f) {
        for (auto& band : chain.multibandComp.bands) {
            band.ratio = std::max(1.2f, band.ratio + drDiff * 0.3f);
        }
    }

    // ── Match stereo width ─────────────────────────────────────────────
    float widthDiff = reference.stereoWidth - source.stereoWidth;
    for (int i = 0; i < 4; ++i) {
        chain.stereoImager.width[i] = std::clamp(
            chain.stereoImager.width[i] + widthDiff * 0.5f, 0.0f, 2.0f);
    }
    // Always keep sub frequencies narrow
    chain.stereoImager.width[0] = std::min(chain.stereoImager.width[0], 0.5f);

    // ── Tonal balance: full match ──────────────────────────────────────
    chain.tonalBalance.matchStrength = 0.8f;
    chain.tonalBalance.referenceProfile = "Reference";

    return chain;
}

// ── applyChain (offline processing) ────────────────────────────────────────

bool MasteringAssistant::applyChain(const MasteringChain& chain,
                                     const float* input, float* output,
                                     int numSamples, int numChannels,
                                     double sampleRate) {
    if (!input || !output || numSamples <= 0 || numChannels <= 0 || sampleRate <= 0.0)
        return false;

    int totalFrames = numSamples / numChannels;

    // Copy input to output as starting point
    std::copy(input, input + numSamples, output);

    // ── Stage 1: Dynamic EQ (simplified — static gain for offline) ─────
    if (chain.dynamicEQ.active) {
        for (const auto& band : chain.dynamicEQ.bands) {
            if (std::abs(band.gain) < 0.1f) continue;

            float gainLin = dbToLinear(band.gain);
            float bandLo  = band.frequency / std::pow(2.0f, 0.5f / band.q);
            float bandHi  = band.frequency * std::pow(2.0f, 0.5f / band.q);

            // Apply gain to affected frequency range via simple biquad
            // (Simplified: apply gain as overall tilt for offline preview)
            float normFreq = band.frequency / static_cast<float>(sampleRate);
            if (normFreq < 0.5f) {
                // First-order shelf approximation
                float alpha = 2.0f * kPi * normFreq;
                float prevL = 0.0f, prevR = 0.0f;

                for (int i = 0; i < totalFrames; ++i) {
                    for (int ch = 0; ch < numChannels; ++ch) {
                        int idx = i * numChannels + ch;
                        float& prev = (ch == 0) ? prevL : prevR;
                        float x = output[idx];
                        float hp = x - (prev + alpha * (x - prev));
                        prev += alpha * (x - prev);

                        if (band.frequency > 1000.0f) {
                            // High shelf
                            output[idx] = prev + hp * gainLin;
                        } else {
                            // Low shelf
                            output[idx] = prev * gainLin + hp;
                        }
                    }
                }
            }
        }
    }

    // ── Stage 2: Multiband compression (simplified soft-knee) ──────────
    if (chain.multibandComp.active) {
        for (int i = 0; i < totalFrames; ++i) {
            for (int ch = 0; ch < numChannels; ++ch) {
                int idx = i * numChannels + ch;
                float x = output[idx];
                float xDb = linearToDb(std::abs(x));

                // Use mid-band parameters as simplified full-band compression
                const auto& mb = chain.multibandComp.bands[1];
                if (xDb > mb.threshold) {
                    float overDb = xDb - mb.threshold;
                    float compDb = overDb * (1.0f - 1.0f / mb.ratio);
                    float compLin = dbToLinear(-compDb + mb.makeupGain);
                    output[idx] = x * compLin;
                } else {
                    output[idx] = x * dbToLinear(mb.makeupGain);
                }
            }
        }
    }

    // ── Stage 3: Stereo imager (mid-side processing) ───────────────────
    if (chain.stereoImager.active && numChannels >= 2) {
        float avgWidth = 0.0f;
        for (float w : chain.stereoImager.width) avgWidth += w;
        avgWidth /= 4.0f;

        for (int i = 0; i < totalFrames; ++i) {
            int idxL = i * numChannels;
            int idxR = i * numChannels + 1;
            float l = output[idxL];
            float r = output[idxR];

            // M/S encode
            float mid  = (l + r) * 0.5f;
            float side = (l - r) * 0.5f;

            // Apply width
            side *= avgWidth;

            // M/S decode
            output[idxL] = mid + side;
            output[idxR] = mid - side;
        }
    }

    // ── Stage 4: Exciter (harmonic saturation) ─────────────────────────
    if (chain.exciter.active) {
        float drive = chain.exciter.drive;
        float mix   = chain.exciter.mix;

        for (int i = 0; i < numSamples; ++i) {
            float x = output[i];
            // Soft-clipping saturation (tanh waveshaper)
            float saturated = std::tanh(x * (1.0f + drive * 10.0f));
            output[i] = x * (1.0f - mix) + saturated * mix;
        }
    }

    // ── Stage 5: Maximizer / True-peak limiter ─────────────────────────
    if (chain.maximizer.active) {
        float ceilingLin = dbToLinear(chain.maximizer.ceiling);
        float threshLin  = dbToLinear(chain.maximizer.ceiling + chain.maximizer.threshold);

        for (int i = 0; i < numSamples; ++i) {
            float x = output[i];
            float absX = std::abs(x);

            if (absX > threshLin) {
                // Soft-knee limiting
                float overLin = absX / threshLin;
                float overDb  = linearToDb(overLin);
                float gainReduction = overDb * 0.9f; // Near-infinite ratio
                float newGain = dbToLinear(-gainReduction);
                x *= newGain;
            }

            // Hard ceiling
            x = std::clamp(x, -ceilingLin, ceilingLin);
            output[i] = x;
        }
    }

    return true;
}

// ── Loudness metering ──────────────────────────────────────────────────────

LoudnessMeter MasteringAssistant::updateMeter(const float* samples, int numSamples,
                                               int numChannels, double sampleRate) {
    if (!samples || numSamples <= 0 || numChannels <= 0 || sampleRate <= 0.0)
        return pImpl->meter;

    // Compute K-weighted mean-square for this block
    double ms = pImpl->kWeightedMeanSquare(samples, numSamples, numChannels, sampleRate);
    float blockLufs = static_cast<float>(-0.691 + 10.0 * std::log10(std::max(ms, 1e-20)));

    // Momentary (this block)
    pImpl->meter.momentary = blockLufs;

    // Short-term (rolling average of recent blocks)
    pImpl->momentaryBlocks.push_back(ms);

    // Keep ~3 s worth of blocks (approximate)
    int totalFrames = numSamples / numChannels;
    double blockDuration = static_cast<double>(totalFrames) / sampleRate;
    auto maxBlocks = static_cast<size_t>(3.0 / std::max(blockDuration, 0.001));
    while (pImpl->momentaryBlocks.size() > maxBlocks)
        pImpl->momentaryBlocks.erase(pImpl->momentaryBlocks.begin());

    if (!pImpl->momentaryBlocks.empty()) {
        double avgMs = 0.0;
        for (double b : pImpl->momentaryBlocks) avgMs += b;
        avgMs /= static_cast<double>(pImpl->momentaryBlocks.size());
        pImpl->meter.shortTerm = static_cast<float>(
            -0.691 + 10.0 * std::log10(std::max(avgMs, 1e-20)));
    }

    // Integrated (gated — EBU R128 absolute gate at -70 LUFS)
    constexpr double kAbsoluteGate = -70.0; // LUFS
    double blockLufsD = static_cast<double>(blockLufs);
    pImpl->meter.gateOpen = (blockLufsD > kAbsoluteGate);

    if (pImpl->meter.gateOpen) {
        pImpl->integratedPower += ms;
        pImpl->integratedCount++;
        double avgPower = pImpl->integratedPower / pImpl->integratedCount;
        pImpl->meter.integrated = static_cast<float>(
            -0.691 + 10.0 * std::log10(std::max(avgPower, 1e-20)));
    }

    // True peak for this block
    float tp = pImpl->computeTruePeak(samples, numSamples);
    pImpl->meter.truePeak = std::max(pImpl->meter.truePeak, tp);

    // Loudness range (simplified — difference between short-term extremes)
    if (pImpl->momentaryBlocks.size() > 1) {
        auto [minIt, maxIt] = std::minmax_element(
            pImpl->momentaryBlocks.begin(), pImpl->momentaryBlocks.end());
        float minLufs = static_cast<float>(-0.691 + 10.0 * std::log10(std::max(*minIt, 1e-20)));
        float maxLufs = static_cast<float>(-0.691 + 10.0 * std::log10(std::max(*maxIt, 1e-20)));
        pImpl->meter.lra = maxLufs - minLufs;
    }

    return pImpl->meter;
}

void MasteringAssistant::resetMeter() {
    pImpl->meter = LoudnessMeter{};
    pImpl->momentaryBlocks.clear();
    pImpl->shortTermBlocks.clear();
    pImpl->integratedPower = 0.0;
    pImpl->integratedCount = 0;
    pImpl->preState = {};
    pImpl->rlbState = {};
}

const LoudnessMeter& MasteringAssistant::currentMeter() const {
    return pImpl->meter;
}

} // namespace revithion::ai
