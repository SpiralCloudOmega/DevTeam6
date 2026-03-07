---
name: biquad-filters
description: Biquad filter design — coefficient calculation, Transposed Direct Form II, real-time smoothing, JUCE IIR patterns
---

# Biquad Filter Design

The biquad filter is the fundamental building block of parametric EQ, crossovers, and tone shaping in audio DSP. It implements a second-order IIR transfer function capable of realizing any standard filter type. The general form is:

**H(z) = (b0 + b1·z⁻¹ + b2·z⁻²) / (1 + a1·z⁻¹ + a2·z⁻²)**

Five coefficients (b0, b1, b2, a1, a2) fully define the filter. Every standard filter type shares this structure — only coefficient values differ.

## Transfer Function & Z-Domain

The numerator zeros determine where the frequency response dips toward zero; the denominator poles determine where it peaks. A low-pass places poles near the cutoff and zeros at z = -1 (Nyquist). A peaking EQ places both poles and zeros near the center frequency, with pole-zero distance controlling gain and Q. For stability, all poles must lie inside the unit circle: |pole| < 1.

## Coefficient Calculation (Audio EQ Cookbook)

Reference: Robert Bristow-Johnson's *Audio EQ Cookbook*. Intermediate variables:

- **ω₀ = 2π·f₀/Fs** — normalized angular frequency
- **α = sin(ω₀)/(2·Q)** — bandwidth parameter
- **A = 10^(dBgain/40)** — amplitude for peaking/shelving types

All coefficients must be normalized by dividing by a0.

### Low-Pass Filter

`b0 = (1-cos(ω₀))/2` · `b1 = 1-cos(ω₀)` · `b2 = (1-cos(ω₀))/2` · `a0 = 1+α` · `a1 = -2·cos(ω₀)` · `a2 = 1-α`

### High-Pass Filter

`b0 = (1+cos(ω₀))/2` · `b1 = -(1+cos(ω₀))` · `b2 = (1+cos(ω₀))/2` · `a0 = 1+α` · `a1 = -2·cos(ω₀)` · `a2 = 1-α`

### Band-Pass (constant skirt gain)

`b0 = α` · `b1 = 0` · `b2 = -α` · `a0 = 1+α` · `a1 = -2·cos(ω₀)` · `a2 = 1-α`

### Notch / Allpass

Notch: `b0 = 1` · `b1 = -2·cos(ω₀)` · `b2 = 1` — Allpass: `b0 = 1-α` · `b1 = -2·cos(ω₀)` · `b2 = 1+α` — Both share: `a0 = 1+α` · `a1 = -2·cos(ω₀)` · `a2 = 1-α`

### Peaking EQ

`b0 = 1+α·A` · `b1 = -2·cos(ω₀)` · `b2 = 1-α·A` · `a0 = 1+α/A` · `a1 = -2·cos(ω₀)` · `a2 = 1-α/A`

### Low Shelf / High Shelf

With `β = 2·√A·α`: Low shelf numerator uses `(A+1) - (A-1)·cos(ω₀) ± β` terms scaled by A; high shelf swaps the sign on the `(A-1)` cosine terms. Both share the same denominator structure. See the Audio EQ Cookbook for the full expansion.

## Bilinear Transform & Frequency Warping

The bilinear transform maps s-domain to z-domain via **s = 2·Fs·(z-1)/(z+1)**, compressing the entire analog axis into 0–Fs/2. This introduces nonlinear frequency warping significant above Fs/4. Pre-warping compensates: **ω_analog = 2·Fs·tan(π·f/Fs)**. The Audio EQ Cookbook formulas already incorporate this correction implicitly — no additional pre-warp step is needed.

## Transposed Direct Form II Implementation

TDF2 is the preferred structure for floating-point audio: two state variables, minimal round-off accumulation, no large intermediate values. State equations: `y = b0·x + s1`, `s1 = b1·x - a1·y + s2`, `s2 = b2·x - a2·y`.

```cpp
class BiquadTDF2 {
    float b0, b1, b2, a1, a2;
    float s1 = 0.0f, s2 = 0.0f;
public:
    void setCoefficients(float nb0, float nb1, float nb2, float na1, float na2) noexcept {
        b0 = nb0; b1 = nb1; b2 = nb2; a1 = na1; a2 = na2;
    }
    float processSample(float x) noexcept {
        float y = b0 * x + s1;
        s1 = b1 * x - a1 * y + s2;
        s2 = b2 * x - a2 * y;
        return y;
    }
    void reset() noexcept { s1 = 0.0f; s2 = 0.0f; }
};
```

## Cascading for Higher Orders

Higher-order filters cascade biquad sections in series: 4th-order Butterworth = 2 biquads, 8th-order Linkwitz-Riley = 4 biquads. Each section uses Q values derived from Butterworth polynomial roots. Order sections lowest-Q first for numerical stability.

```cpp
template <int NumStages>
class CascadedBiquad {
    std::array<BiquadTDF2, NumStages> stages;
public:
    float processSample(float x) noexcept {
        for (auto& stage : stages) x = stage.processSample(x);
        return x;
    }
    void reset() noexcept { for (auto& s : stages) s.reset(); }
    BiquadTDF2& getStage(int i) { return stages[static_cast<size_t>(i)]; }
};
```

## Real-Time Coefficient Smoothing

Updating coefficients abruptly causes clicks and zipper noise. Smooth the user-facing parameters (frequency, Q, gain) with exponential one-pole smoothing, then recalculate coefficients per block. Typical smoothing time: 20–50 ms.

```cpp
class SmoothedParam {
    float current = 0.0f, target = 0.0f, coeff = 0.0f;
public:
    void prepare(float smoothMs, float sampleRate) {
        coeff = std::exp(-1.0f / (smoothMs * 0.001f * sampleRate));
    }
    void setTarget(float v) noexcept { target = v; }
    float getNext() noexcept {
        current = current * coeff + target * (1.0f - coeff);
        return current;
    }
    bool isSmoothing() const noexcept { return std::abs(current - target) > 1e-6f; }
};
```

## JUCE IIR Patterns

JUCE 8 `dsp` module provides production-ready biquad implementations with factory methods for all standard types:

```cpp
// Stereo biquad filter declaration
juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                juce::dsp::IIR::Coefficients<float>> filter;

// prepareToPlay()
juce::dsp::ProcessSpec spec { sampleRate, (uint32) samplesPerBlock, (uint32) numChannels };
filter.prepare(spec);
filter.reset();

// Update coefficients — peaking EQ example
auto* coeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
    sampleRate, frequency, Q, juce::Decibels::decibelsToGain(dBgain));
*filter.state = *coeffs;

// processBlock()
juce::dsp::AudioBlock<float> block(buffer);
filter.process(juce::dsp::ProcessContextReplacing<float>(block));
```

Available factory methods: `makeLowPass()`, `makeHighPass()`, `makeBandPass()`, `makeNotch()`, `makeAllPass()`, `makePeakFilter()`, `makeLowShelf()`, `makeHighShelf()`, `makeFirstOrderLowPass()`, `makeFirstOrderHighPass()`. For higher-order designs, use `juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod()`.

## Anti-Patterns

- ❌ Don't update coefficients without smoothing — causes clicks and zipper noise
- ❌ Don't use Direct Form I for floating-point — TDF2 has better numerical properties
- ❌ Don't forget to pre-warp analog prototype frequencies when designing from s-domain
- ❌ Don't cascade unstable sections — always verify poles are inside unit circle (|pole| < 1)
- ❌ Don't process in-place if your feedback structure reads from the same buffer it writes to
- ❌ Don't use `double` coefficients with `float` state variables — pick one precision throughout

## Checklist

- [ ] Coefficients normalized (divide by a0)
- [ ] Filter states reset on sample rate change
- [ ] Coefficient smoothing active for real-time parameter changes
- [ ] Cascaded sections ordered for numerical stability (lowest Q first)
- [ ] Frequency parameter clamped below Nyquist (f < Fs/2)
- [ ] Q parameter clamped to prevent instability (Q > 0)
- [ ] All filter types validated against reference (Audio EQ Cookbook)
- [ ] Denormal protection enabled (flush-to-zero / DAZ flags set)
