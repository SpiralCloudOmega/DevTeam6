---
name: loudness-metering
description: EBU R128 / ITU-R BS.1770-4 loudness metering, K-weighting, gating, true peak, LRA, and streaming targets for REVITHION STUDIO.
---

# Loudness Metering & Standards

## EBU R128 & ITU-R BS.1770-4

EBU R128 defines four measurements: **Integrated** (entire programme, LUFS), **Short-term** (3 s window, LUFS), **Momentary** (400 ms window, LUFS), and **True Peak** (4× oversampled, dBTP). The ITU-R BS.1770-4 algorithm flows: K-weighting filter → mean square per channel → channel-weighted sum (`L = -0.691 + 10·log10(Σ Gi·zi)`, surround Ls/Rs weighted +1.5 dB) → two-stage gating.

## K-Weighting Filter Chain (48 kHz)

Two cascaded biquads. Stage 1: high-shelf pre-filter (+4 dB above ~1.5 kHz). Stage 2: RLB high-pass (−3 dB at ~38 Hz). Recompute via bilinear transform for other sample rates.

```cpp
void prepareKWeighting (double sampleRate) {
    // Stage 1 high-shelf — b:{1.53512485958697,-2.69169618940638,1.19839281085285} a:{1,-1.69065929318241,0.73248077421585}
    auto shelf = juce::dsp::IIR::Coefficients<float>::Ptr (new juce::dsp::IIR::Coefficients<float> (
        1.53512485958697f, -2.69169618940638f, 1.19839281085285f, 1.0f, -1.69065929318241f, 0.73248077421585f));
    // Stage 2 RLB high-pass — b:{1,-2,1} a:{1,-1.99004745483398,0.99007225036621}
    auto rlb = juce::dsp::IIR::Coefficients<float>::Ptr (new juce::dsp::IIR::Coefficients<float> (
        1.0f, -2.0f, 1.0f, 1.0f, -1.99004745483398f, 0.99007225036621f));
    kWeightShelf.coefficients = shelf; kWeightHighPass.coefficients = rlb;
}
```

## Two-Stage Gating

400 ms blocks, 75% overlap (100 ms step). Absolute gate at −70 LUFS discards silence, then relative gate at −10 LU below ungated loudness.

```cpp
float computeGatedLoudness (const std::vector<float>& blockLUFS) {
    std::vector<float> gated;
    for (auto l : blockLUFS) if (l > -70.0f) gated.push_back (l);
    if (gated.empty()) return -INFINITY;
    float sum = 0.0f;
    for (auto l : gated) sum += std::pow (10.0f, l / 10.0f);
    float threshold = 10.0f * std::log10 (sum / (float) gated.size()) - 10.0f;
    float finalSum = 0.0f; int n = 0;
    for (auto l : gated) if (l > threshold) { finalSum += std::pow (10.0f, l / 10.0f); ++n; }
    return n > 0 ? -0.691f + 10.0f * std::log10 (finalSum / (float) n) : -INFINITY;
}
```

## Loudness Range (LRA)

Statistical spread of gated short-term (3 s) loudness: **LRA = P95 − P10** in LU (absolute gate −70 LUFS, relative gate −20 LU). Low (< 5 LU) = compressed. Medium (5–15 LU) = broadcast. High (> 15 LU) = classical/film.

## True Peak Metering

Sample peaks miss inter-sample overs — 4× oversampling reconstructs peaks between samples. Ceiling: −1.0 dBTP (EBU R128).

```cpp
class TruePeakMeter {
    juce::dsp::Oversampling<float> oversampler { 1, 2, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };
    std::atomic<float> truePeakDB { -100.0f };
public:
    void prepare (const juce::dsp::ProcessSpec& spec) { oversampler.initProcessing (spec.maximumBlockSize); }
    void process (juce::AudioBuffer<float>& buffer) {
        juce::dsp::AudioBlock<float> block (buffer);
        auto up = oversampler.processSamplesUp (block);
        float peak = 0.0f;
        for (int ch = 0; ch < (int) up.getNumChannels(); ++ch)
            for (int i = 0; i < (int) up.getNumSamples(); ++i)
                peak = std::max (peak, std::abs (up.getChannelPointer (ch)[i]));
        truePeakDB.store (juce::Decibels::gainToDecibels (peak), std::memory_order_relaxed);
    }
    float getTruePeakDB() const { return truePeakDB.load (std::memory_order_relaxed); }
};
```

## Streaming Platform Targets

| Platform | Target | Peak | Platform | Target | Peak |
|---|---|---|---|---|---|
| Spotify | −14 LUFS | −1 dBTP | Amazon | −14 LUFS | −2 dBTP |
| Apple Music | −16 LUFS | −1 dBTP | Tidal | −14 LUFS | −1 dBTP |
| YouTube | −14 LUFS | −1 dBTP | CD/Vinyl | −9 to −6 | 0 dBTP |

```cpp
void applyLoudnessNormalization (juce::AudioBuffer<float>& buf, float measuredLUFS, float targetLUFS) {
    buf.applyGain (juce::Decibels::decibelsToGain (targetLUFS - measuredLUFS));
}
```

Re-measure after gain to confirm true peak stays below ceiling; apply a true-peak limiter as final stage if exceeded.

## JUCE Loudness Meter — Real-Time Implementation

Lock-free `AbstractFifo` bridges audio → UI thread. Run `startTimerHz (30)` for smooth meter animation.

```cpp
class LoudnessMeter : public juce::Timer {
    static constexpr int fifoSize = 512;
    juce::AbstractFifo fifo { fifoSize };
    std::array<float, fifoSize> momentaryBuf {}, shortTermBuf {};
    std::atomic<float> integratedLUFS { -70.0f }, truePeakDBTP { -100.0f };
public:
    void pushMeasurement (float m, float s, float i, float tp) { // audio thread
        const auto scope = fifo.write (1);
        if (scope.blockSize1 > 0) { momentaryBuf[(size_t) scope.startIndex1] = m; shortTermBuf[(size_t) scope.startIndex1] = s; }
        integratedLUFS.store (i, std::memory_order_relaxed);
        truePeakDBTP.store (tp, std::memory_order_relaxed);
    }
    void timerCallback() override { // message thread
        const auto scope = fifo.read (fifo.getNumReady());
        if (scope.blockSize1 > 0)
            updateDisplay (momentaryBuf[(size_t) scope.startIndex1 + scope.blockSize1 - 1],
                           shortTermBuf[(size_t) scope.startIndex1 + scope.blockSize1 - 1],
                           integratedLUFS.load(), truePeakDBTP.load());
    }
    void updateDisplay (float momentary, float shortTerm, float integrated, float truePeak);
};
```

## Anti-Patterns

- **Sample-peak instead of true-peak** — misses inter-sample overs; always 4× oversample.
- **Skipping gating** — ungated loudness is meaningless with pauses; apply absolute + relative gate.
- **Hardcoded coefficients across sample rates** — 48 kHz values wrong at 44.1/96 kHz; bilinear transform.
- **Measuring post-limiter only** — measure pre- and post-limiter to understand impact.
- **One LUFS target for all platforms** — use per-platform targets from the table above.
- **Blocking the audio thread** — use lock-free ring buffers; compute in block callbacks.

## Checklist

- [ ] K-weighting filters match ITU-R BS.1770-4 coefficients for the project sample rate
- [ ] Momentary (400 ms), short-term (3 s), and integrated measurements implemented
- [ ] Two-stage gating (absolute −70 LUFS, relative −10 LU) applied to integrated loudness
- [ ] True peak via 4× oversampling, ceiling enforced by limiter (−1.0 dBTP default)
- [ ] LRA computed from gated short-term distribution (P95 − P10)
- [ ] Lock-free FIFO for audio→UI thread meter communication
- [ ] Platform-specific loudness targets configurable in export presets
- [ ] Loudness displayed in LUFS/LU with ≥ 0.1 resolution, bypassing output gain
