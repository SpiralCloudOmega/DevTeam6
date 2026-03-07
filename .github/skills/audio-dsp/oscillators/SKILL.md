---
name: oscillators
description: Digital oscillator design — band-limited methods, wavetable, FM/PM, additive, anti-aliasing
---

# Digital Oscillator Design

Oscillators are the primary sound source in any synthesizer. The core challenge: generating waveforms that are **band-limited** (no aliasing) while remaining computationally efficient. Naive waveforms contain harmonics above the Nyquist frequency (sampleRate / 2) that fold back as inharmonic distortion.

## Phase Accumulator

Every digital oscillator is built on a phase accumulator — a normalized `[0, 1)` ramp that advances by `frequency / sampleRate` each sample.

```cpp
class PhaseAccumulator {
    float phase = 0.0f;    // [0, 1) normalized phase
    float phaseInc = 0.0f; // frequency / sampleRate

public:
    void setFrequency(float freq, float sampleRate) {
        phaseInc = freq / sampleRate;
    }

    float tick() {
        float p = phase;
        phase += phaseInc;
        phase -= std::floor(phase); // wrap to [0, 1)
        return p;
    }
};
```

## Naive Waveforms & Aliasing

Naive waveforms map phase directly to output. The sawtooth has a discontinuous jump at wrap; the square has two step discontinuities per cycle. These produce infinite harmonics — anything above Nyquist reflects back as audible metallic artifacts, especially above ~500 Hz.

```cpp
// Naive saw — DO NOT USE in production (aliases badly)
float naiveSaw(float phase) { return 2.0f * phase - 1.0f; }

// Naive square — same aliasing problem at the step edges
float naiveSquare(float phase) { return phase < 0.5f ? 1.0f : -1.0f; }

// Naive triangle — less aliasing (continuous) but corners still produce odd-harmonic foldover
float naiveTriangle(float phase) {
    return 4.0f * std::abs(phase - 0.5f) - 1.0f;
}
```

## PolyBLEP (Polynomial Band-Limited Step)

PolyBLEP is the most practical real-time anti-aliasing method. It applies a polynomial correction near each discontinuity, smoothing the step in a band-limited way. The correction depends on phase proximity to the discontinuity relative to `phaseInc`. Cost is negligible.

```cpp
float polyBlep(float t, float dt) {
    if (t < dt) {
        t /= dt;
        return t + t - t * t - 1.0f;
    } else if (t > 1.0f - dt) {
        t = (t - 1.0f) / dt;
        return t * t + t + t + 1.0f;
    }
    return 0.0f;
}

float polyBlepSaw(float phase, float phaseInc) {
    float saw = 2.0f * phase - 1.0f;
    saw -= polyBlep(phase, phaseInc);
    return saw;
}

float polyBlepSquare(float phase, float phaseInc) {
    float square = phase < 0.5f ? 1.0f : -1.0f;
    square += polyBlep(phase, phaseInc);
    square -= polyBlep(std::fmod(phase + 0.5f, 1.0f), phaseInc);
    return square;
}
```

A polyBLEP triangle is obtained by integrating the polyBLEP square and applying a leaky integrator to remove DC drift.

## Wavetable Synthesis

Wavetable oscillators pre-compute one cycle into a lookup table (typically 2048 samples, power-of-two for bitmasked wrapping). The phase accumulator indexes into the table with interpolation. For anti-aliasing, create **mipmapped tables** — one per octave — each containing only harmonics below Nyquist for that range. Select or crossfade tables based on playback frequency.

```cpp
float readWavetable(const float* table, int tableSize, float phase) {
    float index = phase * static_cast<float>(tableSize);
    int i0 = static_cast<int>(index);
    int i1 = (i0 + 1) & (tableSize - 1); // power-of-2 wrap
    float frac = index - static_cast<float>(i0);
    return table[i0] + frac * (table[i1] - table[i0]); // linear interp
}
```

For higher quality, use cubic Hermite interpolation (four-point) to preserve waveform curvature.

## BLIT and MinBLEP

**BLIT (Band-Limited Impulse Train)** generates band-limited impulses then integrates to produce a sawtooth. Clean spectrally, but the integrator accumulates DC drift requiring a leaky filter.

**MinBLEP (Minimum-phase Band-Limited Step)** pre-computes a minimum-phase step from a windowed sinc and inserts it at discontinuities. Excellent quality with low latency. Used in high-end synths (u-he Diva, NI Massive).

## FM / PM Synthesis

Most "FM" implementations — including the Yamaha DX7 — actually use **Phase Modulation (PM)**. The modulator is added to the carrier's phase (not frequency), avoiding pitch drift.

```
output = sin(2π · fc · t + modulationIndex · sin(2π · fm · t))
```

The **modulation index** controls brightness (number of sidebands). Operators stack in **algorithms** — DX7 has 32 across 6 operators. **Feedback FM** routes output back into an operator's own phase, morphing sine toward sawtooth-like shapes.

```cpp
struct FMOperator {
    PhaseAccumulator osc;
    float ratio = 1.0f;      // frequency ratio to base note
    float modIndex = 0.0f;   // modulation depth in radians
    float feedback = 0.0f;   // self-feedback amount
    float prevOut = 0.0f;    // previous output for feedback

    float process(float baseFreq, float sampleRate, float modInput = 0.0f) {
        osc.setFrequency(baseFreq * ratio, sampleRate);
        float phase = osc.tick();
        float fbMod = feedback * prevOut;
        float out = std::sin(2.0f * juce::MathConstants<float>::pi
                             * phase + modInput + fbMod);
        prevOut = out;
        return out * modIndex;
    }
};
```

## Additive Synthesis

Additive synthesis sums sine waves at harmonic (or inharmonic) frequencies. Band-limiting is inherent — omit partials above Nyquist. Cost scales linearly with partial count; SIMD and inverse-FFT methods reduce this dramatically.

```cpp
float additiveSaw(float phase, float sampleRate, float freq, int maxHarmonics) {
    float out = 0.0f;
    int N = std::min(maxHarmonics, static_cast<int>(sampleRate / (2.0f * freq)));
    for (int k = 1; k <= N; ++k) {
        float sign = (k % 2 == 0) ? -1.0f : 1.0f;
        out += sign * std::sin(2.0f * juce::MathConstants<float>::pi * k * phase)
               / static_cast<float>(k);
    }
    return out * (2.0f / juce::MathConstants<float>::pi);
}
```

## Waveshaping

Waveshaping applies a nonlinear transfer function `f(x)` to a simple waveform (typically sine). **Chebyshev polynomials** are uniquely useful: the nth polynomial applied to `cos(θ)` produces exactly `cos(nθ)`, enabling precise harmonic injection. Soft-clipping curves (tanh, arctan) add odd harmonics progressively.

## Unison / Detune / Spread

Supersaw-style unison stacks N voices detuned symmetrically around center frequency with stereo spread. Typical: 3–7 voices, ±10–50 cents detune. Normalize amplitude (divide by √N) to prevent clipping.

```cpp
// Compute detuned frequencies for N unison voices, ±maxDetuneCents
void calcUnisonFreqs(float baseFreq, int numVoices, float maxDetuneCents,
                     float* outFreqs) {
    for (int i = 0; i < numVoices; ++i) {
        float t = (numVoices == 1) ? 0.0f
                  : (2.0f * i / (numVoices - 1) - 1.0f); // [-1, 1]
        float cents = t * maxDetuneCents;
        outFreqs[i] = baseFreq * std::pow(2.0f, cents / 1200.0f);
    }
}
```

## Anti-Patterns

- ❌ Don't use naive waveforms (raw saw/square) — they alias audibly above ~500 Hz
- ❌ Don't forget to wrap phase to `[0, 1)` — phase will drift and lose float precision
- ❌ Don't use a single wavetable for all frequencies — need mipmapped tables per octave
- ❌ Don't modulate frequency directly for vibrato — modulate phase increment smoothly
- ❌ Don't sum unison voices without amplitude normalization — clips at high voice counts
- ❌ Don't use `std::sin` in a tight inner loop without considering lookup-table or polynomial approximation

## Checklist

- [ ] Phase accumulator wraps correctly to `[0, 1)`
- [ ] Anti-aliasing method applied (polyBLEP, wavetable mipmaps, or additive)
- [ ] Wavetable uses appropriate interpolation (at least linear, prefer cubic Hermite)
- [ ] FM synthesis uses phase modulation (not true FM) for stability
- [ ] Unison voices detuned symmetrically with amplitude normalization
- [ ] Frequency range clamped to reasonable audio range (20 Hz – 20 kHz)
- [ ] All oscillators tested at high frequencies (>5 kHz) for aliasing artifacts
- [ ] SIMD alignment considered for wavetable and additive inner loops
