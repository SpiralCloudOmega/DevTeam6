---
name: delay-reverb
description: Delay line and reverb design — circular buffers, interpolation, comb/allpass, FDN, Freeverb
---

# Delay Lines & Reverb Design

The delay line is the fundamental building block for every time-based audio effect. Echo, chorus, flanger, phaser, and reverb are all constructed from delay lines with varying feedback topologies, modulation, and mixing strategies.

## Circular Buffer Implementation

Use power-of-2 sizing with bitmask wrapping — compiles to a single AND instruction, far faster than modulo.

```cpp
class DelayLine {
    std::vector<float> buffer;
    int writePos = 0, mask;
public:
    explicit DelayLine(int maxDelay)
    {
        auto size = juce::nextPowerOfTwo(maxDelay + 1);
        buffer.resize(static_cast<size_t>(size), 0.0f);
        mask = size - 1;
    }
    void write(float sample) { buffer[static_cast<size_t>(writePos & mask)] = sample; ++writePos; }
    float read(int delay) const { return buffer[static_cast<size_t>((writePos - delay) & mask)]; }
    void clear() { std::fill(buffer.begin(), buffer.end(), 0.0f); }
};
```

## Fractional Delay Interpolation

Sub-sample accuracy is required for chorus modulation, pitch shifting, and non-integer tempo-synced delays:

- **Linear** — two-point, cheap, slight HF rolloff proportional to fractional part
- **Cubic Hermite** — four-point, flatter response, preferred for modulated delays
- **Allpass** — flat magnitude, ideal inside feedback loops where cumulative HF loss matters
- **Sinc** — perfect reconstruction, 8–32 tap windowed-sinc FIR, expensive

```cpp
float readLinear(float delaySamples) const {
    auto n = static_cast<int>(delaySamples);
    float frac = delaySamples - static_cast<float>(n);
    return read(n) + frac * (read(n + 1) - read(n));
}

float readCubic(float delaySamples) const {
    auto n = static_cast<int>(delaySamples);
    float f = delaySamples - static_cast<float>(n);
    float y_1 = read(n - 1), y0 = read(n), y1 = read(n + 1), y2 = read(n + 2);
    float c1 = 0.5f * (y1 - y_1);
    float c2 = y_1 - 2.5f * y0 + 2.0f * y1 - 0.5f * y2;
    float c3 = 0.5f * (y2 - y_1) + 1.5f * (y0 - y1);
    return ((c3 * f + c2) * f + c1) * f + y0;
}
```

## Comb Filters

A **feedback comb filter** recirculates delayed output, producing exponentially decaying echoes. Adding a one-pole lowpass in the feedback path (LBCF) is the core building block of Freeverb and most algorithmic reverbs.

```cpp
class LowpassCombFilter {
    DelayLine delayLine;
    float feedback, damping, lastFiltered = 0.0f;
    int delaySamples;
public:
    LowpassCombFilter(int maxDelay, int delay, float fb, float damp)
        : delayLine(maxDelay), delaySamples(delay), feedback(fb), damping(damp) {}
    float process(float input) {
        float delayed = delayLine.read(delaySamples);
        float filtered = (1.0f - damping) * delayed + damping * lastFiltered;
        lastFiltered = filtered;
        delayLine.write(input + feedback * filtered);
        return delayed;
    }
    void clear() { delayLine.clear(); lastFiltered = 0.0f; }
};
```

## Allpass Filters (Schroeder)

Passes all frequencies at unity magnitude but alters phase — creates temporal diffusion without coloring the spectrum. Ideal for smearing sparse echoes into a dense reverb tail.

```cpp
class SchroederAllpass {
    DelayLine delayLine;
    float gain;
    int delaySamples;
public:
    SchroederAllpass(int maxDelay, int delay, float g)
        : delayLine(maxDelay), delaySamples(delay), gain(g) {}
    float process(float input) {
        float delayed = delayLine.read(delaySamples);
        delayLine.write(input + gain * delayed);
        return -gain * input + delayed;
    }
    void clear() { delayLine.clear(); }
};
```

## Schroeder Reverb

Classic topology (1962): 4 parallel feedback comb filters → sum → 2 series allpass filters. First practical algorithmic reverb, but low echo density produces metallic flutter. Modern designs add more combs, damping, and pre-diffusion stages.

## Freeverb

Jezar's Freeverb (2000): 8 parallel LBCFs → 4 series allpass filters. One-pole lowpass in each comb models air absorption. Stereo via L/R delay offset (23 samples). Parameters: **room size** → comb feedback, **damping** → lowpass coefficient, **wet/dry** → mix, **width** → stereo cross-feed.

## Feedback Delay Networks (FDN)

N delay lines whose outputs pass through an N×N **unitary mixing matrix** and feed back. The modern standard for algorithmic reverb. Use **mutually prime** delay lengths, per-channel damping filters, and Hadamard or Householder matrices.

```cpp
void processFDN(float input, float* output) {
    std::array<float, 4> d;
    for (int i = 0; i < 4; ++i)
        d[i] = dampingFilters[i].process(delayLines[i].read(delayTimes[i]));
    // 4x4 Hadamard (normalized by 0.5)
    std::array<float, 4> m;
    m[0] = 0.5f * ( d[0] + d[1] + d[2] + d[3]);
    m[1] = 0.5f * ( d[0] - d[1] + d[2] - d[3]);
    m[2] = 0.5f * ( d[0] + d[1] - d[2] - d[3]);
    m[3] = 0.5f * ( d[0] - d[1] - d[2] + d[3]);
    for (int i = 0; i < 4; ++i)
        delayLines[i].write(input * inputGains[i] + m[i] * feedbackGain);
    *output = d[0] + d[1] + d[2] + d[3];
}
```

## Early Reflections vs Late Reverb

- **Early reflections** (0–80 ms): Sparse discrete echoes encoding room geometry. Multi-tap delay with per-tap gain/pan.
- **Late reverb** (80 ms+): Dense diffuse tail via FDN or Schroeder network. RT60 is the primary parameter.
- **Pre-delay** (10–50 ms): Gap before first reflection, separates dry signal from reverb for clarity.

## Damping & Absorption

Real rooms absorb HF faster than LF. Model with one-pole lowpass (`y[n] = (1-d)*x[n] + d*y[n-1]`) in every feedback path. `d=0` → bright, `d=1` → fully damped. For physical accuracy, use shelving EQ to independently control LF/HF decay.

## Anti-Patterns

- ❌ Don't use modulo (`%`) for buffer wrapping — use power-of-2 bitmask
- ❌ Don't use linear interpolation in allpass feedback paths — cumulative HF loss degrades the tail
- ❌ Don't choose harmonically related delay lengths — use mutually prime values
- ❌ Don't skip zeroing buffers in `prepareToPlay()` — stale data causes pops
- ❌ Don't omit damping in feedback combs — sounds metallic without it

## Checklist

- [ ] Delay buffer sizes are power of 2 for efficient bitmask wrapping
- [ ] Fractional delay uses appropriate interpolation for the use case
- [ ] Comb filter delay lengths are mutually prime
- [ ] All feedback gains satisfy `|g| < 1` to guarantee stability
- [ ] Damping filters present in every feedback path
- [ ] Buffers cleared on sample rate change and `prepareToPlay()`
- [ ] Wet/dry mix and output level controls exposed to host
- [ ] Stereo spread uses offset delay times, not simple pan
