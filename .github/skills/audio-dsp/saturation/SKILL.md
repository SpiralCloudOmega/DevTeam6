---
name: saturation
description: Saturation and waveshaping — tube/tape/transistor modeling, oversampling, aliasing reduction
---

# Saturation & Waveshaping

Saturation adds harmonics by applying nonlinear transfer functions to audio. It is the foundation of analog warmth — tubes, tape, and transistors all saturate differently, producing distinct harmonic spectra. The key challenge is that nonlinear processing creates new frequency content that can alias when it exceeds Nyquist, so oversampling is essential.

## Soft vs Hard Clipping

- **Hard clipping**: `output = clamp(input, -1, 1)`. Abruptly flattens peaks — harsh, odd harmonics (3rd, 5th, 7th…).
- **Soft clipping**: smooth curve that gradually limits amplitude. More musical — lower-order harmonics with gentler rolloff.

```cpp
float hardClip(float x) { return std::clamp(x, -1.0f, 1.0f); }

float softClipCubic(float x) {
    if (x > 1.0f) return 2.0f / 3.0f;
    if (x < -1.0f) return -2.0f / 3.0f;
    return x - (x * x * x) / 3.0f;
}
```

## Mathematical Waveshapers

- **tanh(x)** — smooth, symmetric, warm. Most popular general-purpose saturator.
- **atan(x)** — softer knee than tanh, slightly different harmonic weighting.
- **Algebraic sigmoid: x / (1 + |x|)** — cheap, no transcendentals, good tanh approximation.
- **Exponential soft clip** — can produce asymmetric character depending on formulation.

```cpp
float tanhSat(float x, float drive) { return std::tanh(x * drive); }
float atanSat(float x, float drive) { return (2.0f / juce::MathConstants<float>::pi) * std::atan(x * drive); }
float fastSigmoid(float x) { return x / (1.0f + std::abs(x)); }
```

## Tube Modeling

Vacuum tube (valve) circuits saturate asymmetrically:

- **Triode**: clips harder on the negative half-cycle, producing even + odd harmonics. Even harmonics (2nd, 4th) add warmth.
- **Pentode**: more symmetric, harder clipping, more aggressive with stronger odd harmonics.
- Grid conduction and plate saturation create the soft-knee compression that makes tubes feel dynamic.

```cpp
float triodeApprox(float x, float drive) {
    x *= drive;
    if (x >= 0.0f)
        return std::tanh(x);
    else
        return std::tanh(x * 1.2f) / 1.2f;  // asymmetric negative clip
}
```

## Tape Saturation

- **Hysteresis**: output depends on input history — the magnetization curve forms a loop.
- **Frequency-dependent saturation**: HF saturates first, producing natural rolloff under drive.
- **Transient softening**: tape compresses peaks gently, adding glue and cohesion.
- The **Jiles-Atherton** model (used in CHOWTapeModel) is the standard for realistic tape emulation.

## Transistor Clipping

- **Hard-knee** (e.g., Boss DS-1): silicon diodes in feedback loop, aggressive clipping.
- **Soft-knee** (e.g., Tube Screamer): diodes to ground, retains dynamics and midrange.
- **Germanium vs silicon**: germanium has lower forward voltage (~0.3V vs ~0.7V), softer knee, warmer tone.

```cpp
float diodeClip(float x, float threshold = 0.7f) {
    if (x > threshold)
        return threshold + (1.0f - std::exp(-(x - threshold)));
    if (x < -threshold)
        return -threshold - (1.0f - std::exp(-(-x - threshold)));
    return x;
}
```

## Oversampling for Aliasing Reduction

The most critical consideration for nonlinear DSP. Nonlinear functions generate new harmonics — any above Nyquist (sampleRate / 2) fold back as inharmonic aliasing artifacts. Solution: upsample → saturate → downsample with anti-aliasing filter. Typical factors are 2×–8×.

```cpp
// JUCE 8 oversampling — 4x with polyphase IIR
juce::dsp::Oversampling<float> oversampling(
    2, 2,  // numChannels, order (2^2 = 4x)
    juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR);

void prepareToPlay(double sampleRate, int samplesPerBlock) {
    oversampling.initProcessing(static_cast<size_t>(samplesPerBlock));
}

void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    juce::dsp::AudioBlock<float> block(buffer);
    auto oversampledBlock = oversampling.processSamplesUp(block);
    for (size_t ch = 0; ch < oversampledBlock.getNumChannels(); ++ch) {
        auto* samples = oversampledBlock.getChannelPointer(ch);
        for (size_t i = 0; i < oversampledBlock.getNumSamples(); ++i)
            samples[i] = std::tanh(samples[i]);
    }
    oversampling.processSamplesDown(block);
}
```

## Asymmetric Clipping & Even Harmonics

Asymmetric waveshaping applies different curves to positive and negative half-cycles, generating even harmonics (2nd, 4th, 6th…). Tubes and single-ended circuits are inherently asymmetric. Always add a DC blocker after — asymmetric clipping introduces DC offset.

## Mix / Blend Control

A dry/wet mix is essential — it allows dialing from subtle warmth to heavy destruction.

```cpp
float processSample(float x, float drive, float mix) {
    float wet = std::tanh(x * drive);
    return x * (1.0f - mix) + wet * mix;
}
```

## Anti-Patterns

- ❌ Nonlinear processing without oversampling — aliasing is guaranteed above moderate drive
- ❌ Hard clipping as "saturation" — sounds digital and harsh unless explicitly intended
- ❌ No input gain staging — clamp or normalize input range before the saturator
- ❌ Forgetting DC offset removal — asymmetric clipping needs a high-pass at ~5 Hz after
- ❌ No dry/wet mix — saturation without blend is unusable at high drive settings

## Checklist

- [ ] Oversampling active (minimum 2×, prefer 4× for heavy saturation)
- [ ] DC blocker after saturation stage (especially asymmetric waveshapers)
- [ ] Input/output gain staging correct
- [ ] Dry/wet mix control implemented
- [ ] Anti-aliasing filter quality appropriate for latency budget
- [ ] Tested with sine wave to verify harmonic content
- [ ] CPU profiled — oversampling and transcendentals are the hot path
