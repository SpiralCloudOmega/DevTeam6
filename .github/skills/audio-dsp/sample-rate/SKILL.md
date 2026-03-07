---
name: sample-rate
description: Sample rate conversion — Nyquist, anti-aliasing, interpolation, oversampling, dithering
---

# Sample Rate Conversion & Digital Audio Fundamentals

Sample rate is the foundation of digital audio fidelity. Understanding Nyquist limits, aliasing, and proper rate conversion is critical for every audio DSP task. Common rates: 44.1 kHz (CD), 48 kHz (video/broadcast), 96 kHz (pro production), 192 kHz (archival).

## Nyquist-Shannon Sampling Theorem
A band-limited signal must be sampled at ≥ 2× the highest frequency present to be perfectly reconstructed. The Nyquist frequency equals Fs/2 — for 44.1 kHz that is 22.05 kHz. Content above Nyquist aliases (folds back) as inharmonic distortion that cannot be removed. This is why anti-aliasing filters are applied before ADC.

## Anti-Aliasing Filters
- **Before ADC**: analog LPF attenuates content above Fs/2 before sampling.
- **Before downsampling**: digital LPF removes content above the new target Fs/2.
- **Transition band width** determines filter order — steeper cutoff requires more taps/sections.
- **Linear-phase FIR**: preserves phase but adds latency. **Minimum-phase IIR**: lower latency but phase distortion.

## Interpolation Methods
### Zero-Order Hold
Repeat the nearest sample. Staircase artifacts, sinc-shaped droop. Only for control-rate signals.

### Linear Interpolation
```cpp
float linearInterp(float y0, float y1, float frac) noexcept {
    return y0 + frac * (y1 - y0);
}
```
Cheap but rolls off highs. Adequate for previews, not final output.

### Cubic (Hermite) Interpolation
```cpp
float cubicInterp(float ym1, float y0, float y1, float y2, float frac) noexcept {
    const float a = -0.5f * ym1 + 1.5f * y0 - 1.5f * y1 + 0.5f * y2;
    const float b =  ym1 - 2.5f * y0 + 2.0f * y1 - 0.5f * y2;
    const float c = -0.5f * ym1 + 0.5f * y1;
    return ((a * frac + b) * frac + c) * frac + y0;
}
```
Standard workhorse for wavetable oscillators. Good quality-to-cost ratio.

### Sinc Interpolation
Theoretically perfect reconstruction via windowed sinc kernel (Lanczos, Kaiser). 32–128 taps for mastering-grade SRC. Expensive but highest fidelity — used in libsamplerate `SRC_SINC_BEST_QUALITY`.

## Polyphase Filters

Efficient multirate filtering via polyphase decomposition. Instead of upsampling then filtering the full-rate signal (wasteful), a polyphase structure interleaves sub-filter computations so only needed output samples are calculated. Core technique behind every high-quality SRC library.

## Oversampling for Nonlinear Processing

The most important SRC concept for plugin developers. Nonlinear processing (saturation, clipping, waveshaping) generates harmonics that can exceed Nyquist and alias. The fix: upsample → process → anti-alias filter → decimate. Typical: 2× mild saturation, 4× heavy distortion, 8× extreme waveshaping.

```cpp
// JUCE 8 oversampling — 4x with polyphase IIR
juce::dsp::Oversampling<float> oversampler {
    2, 2, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR
};

// prepareToPlay():
oversampler.initProcessing(static_cast<size_t>(samplesPerBlock));
setLatencySamples(static_cast<int>(oversampler.getLatencyInSamples()));

// processBlock():
auto osBlock = oversampler.processSamplesUp(block);
// ... nonlinear processing at 4x rate ...
oversampler.processSamplesDown(block);
```

## SRC Libraries

| Library | Quality | License | Best For |
|---|---|---|---|
| **libsamplerate** | Excellent | BSD-2 | Offline mastering, batch conversion |
| **r8brain-free-src** | Excellent | MIT | Efficient real-time and offline SRC |
| **Speex resampler** | Good | BSD-3 | Lightweight real-time applications |
| **JUCE dsp::Oversampling** | Good | GPLv3/Commercial | Plugin oversampling (not general SRC) |
| **SoX resampler (soxr)** | Reference | LGPL | Quality benchmarks, mastering pipelines |

## Internal Processing Rate

Many pro plugins process internally at 96 kHz regardless of host rate — one octave of headroom above 20 kHz simplifies filter design and reduces aliasing from nonlinear processing. 192 kHz offers diminishing returns for most algorithms at doubled CPU cost. Plugin upsamples input, processes at 96 kHz, then downsamples output back to host rate.

## Dithering

When reducing bit depth (32-bit float → 16-bit), quantization error becomes correlated distortion. Dither (small random noise) decorrelates the error into a benign noise floor. Types:
- **RPDF**: uniform random noise — simplest. **TPDF**: triangular (two uniform values summed) — standard choice.
- **Noise-shaped**: pushes noise above 15 kHz where hearing is least sensitive.

Apply dither **exactly once**, at the final bit-depth reduction. Never dither intermediate stages.

```cpp
float tpdfDither(std::mt19937& rng) noexcept {
    static constexpr float kScale = 1.0f / 32768.0f; // 1 LSB at 16-bit
    std::uniform_real_distribution<float> dist(-0.5f, 0.5f);
    return (dist(rng) + dist(rng)) * kScale;
}
```

## Anti-Patterns

- ❌ Downsampling without anti-aliasing filter — irreversible aliasing artifacts
- ❌ Linear interpolation for production SRC — use sinc-based or polyphase filters
- ❌ Dithering multiple times in a signal chain — dither only at final output
- ❌ Hardcoding sample rate — always read `sampleRate` from `prepareToPlay()`
- ❌ Forgetting to report oversampling latency via `setLatencySamples()`
- ❌ Recalculating filter coefficients per-sample instead of in `prepareToPlay()`

## Checklist

- [ ] Anti-aliasing filter applied before every downsampling stage
- [ ] Oversampling enabled for all nonlinear processing (saturation, waveshaping, clipping)
- [ ] Interpolation quality matches context: sinc for SRC, cubic for oscillators
- [ ] Dithering applied exactly once at final bit-depth reduction
- [ ] Sample-rate-dependent coefficients recalculated in `prepareToPlay()`
- [ ] Oversampling latency reported accurately to host DAW
- [ ] Internal processing rate documented and configurable (e.g., 96 kHz default)
