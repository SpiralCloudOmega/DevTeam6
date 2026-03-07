---
name: dynamics
description: Compressor/limiter/gate design — envelope followers, gain computer, sidechain, multiband, limiting
---

# Dynamics Processing

Dynamics processors control signal amplitude. Compressors reduce dynamic range by attenuating signals above a threshold, limiters act as brick-wall ceilings to prevent clipping, and gates silence signals below a threshold to remove noise. All share a canonical signal flow: level detection → gain computation → gain application.

## Signal Flow

1. **Input** → optional sidechain filter (HPF to ignore bass, or external key input)
2. **Level detection** — measure signal amplitude (peak or RMS)
3. **Gain computer** — map detected level to gain reduction using threshold, ratio, and knee
4. **Envelope follower** — smooth gain with attack/release ballistics to avoid zipper noise
5. **Gain application** — multiply the original audio by the computed linear gain
6. **Makeup gain** — compensate for level reduction introduced by compression

## Level Detection

### Peak Detection

Peak detection takes the absolute value of each sample: `level = std::abs(sample)`. It responds instantaneously to transients — correct for brick-wall limiting but overly reactive for musical compression because it tracks waveform cycles rather than perceived loudness.

### RMS Detection
RMS measures signal power and correlates with perceived loudness. A one-pole smoother provides a cheap running estimate:

```cpp
// Running RMS estimator — coeff = 1 - exp(-1 / (windowMs * 0.001f * sampleRate))
rmsSquared = (1.0f - coeff) * rmsSquared + coeff * (sample * sample);
float rmsLevel = std::sqrt(rmsSquared);
```

True RMS uses a fixed-length sliding window; the one-pole approximation is cheaper and sufficient for most applications. Convert to dB: `float levelDb = 20.0f * std::log10(level + 1e-6f);` — epsilon prevents log(0).

## Gain Computer

The gain computer is a static curve in the dB domain — input level in dB maps to gain reduction in dB. Soft-knee compression blends the transition with a quadratic curve:

```cpp
float computeGainDb(float inputDb, float threshold, float ratio, float kneeWidth)
{
    const float halfKnee = kneeWidth * 0.5f;
    if (inputDb < threshold - halfKnee)
        return 0.0f; // Below knee — no compression
    if (inputDb > threshold + halfKnee)
        return (threshold + (inputDb - threshold) / ratio) - inputDb; // Above knee
    // Inside knee region — smooth quadratic transition
    float x = inputDb - threshold + halfKnee;
    return ((1.0f / ratio - 1.0f) * x * x) / (2.0f * kneeWidth);
}
```

Key ratio values: **1:1** = no compression, **4:1** = moderate, **10:1** = heavy limiting, **∞:1** = brick-wall, **< 1:1** = expansion.

## Envelope Follower (Ballistics)

The envelope follower smooths the gain signal. The branching one-pole filter uses different coefficients for attack (gain decreasing) and release (gain recovering):

```cpp
const float attackCoeff  = std::exp(-1.0f / (attackMs  * 0.001f * sampleRate));
const float releaseCoeff = std::exp(-1.0f / (releaseMs * 0.001f * sampleRate));

float targetGainDb = computeGainDb(inputDb, threshold, ratio, knee);
float coeff = (targetGainDb < envelopeDb) ? attackCoeff : releaseCoeff;
envelopeDb = coeff * envelopeDb + (1.0f - coeff) * targetGainDb;
```

**Critical:** Gain reduction values are negative dB. Attack means the envelope is *decreasing* (compressing harder), release means *increasing* (returning toward 0 dB). This is inverted from what newcomers expect.

## Sidechain Filtering

- **Internal HPF** — High-pass (60–300 Hz) prevents kick drums from triggering excessive gain reduction ("pumping").
- **Band-pass (de-esser)** — Bandpass around 4–8 kHz targets sibilance only.
- **External sidechain** — A separate signal (e.g., voiceover) drives the detector to duck music underneath.

## Lookahead

Delay the audio path by 1–5 ms while feeding the *un-delayed* signal to the detector. Gain reduction begins *before* the transient arrives, eliminating overshoot. Essential for brick-wall limiting. Report latency to the host via `setLatencySamples()`.

## Multiband Dynamics

Split the input into frequency bands, compress each independently, then sum:
1. **Band splitting** — Linkwitz-Riley crossovers are phase-aligned and sum flat. LR4 = two cascaded Butterworth biquads.
2. **Per-band compression** — Each band has its own threshold, ratio, attack, release, and makeup.
3. **Band summation** — LR crossovers are magnitude-complementary; unprocessed bands reconstruct perfectly.

3-band topology: input → LR4 at f₁ → **low** + remainder → LR4 at f₂ → **mid** + **high**.

## Brick-Wall Limiting

True-peak limiting prevents the output from exceeding 0 dBTP:
- **Lookahead buffer** — 1–5 ms lets the limiter see transients before they reach the output.
- **True-peak detection** — ITU-R BS.1770 at 4× oversampling catches inter-sample peaks. Critical for streaming platforms normalizing to −1 dBTP.
- **Adaptive release** — Program-dependent release avoids pumping while recovering between transients.

## Gate with Hysteresis

A noise gate silences the signal below a threshold (**threshold**, **attack**, **hold**, **release**). Hysteresis uses separate open/close thresholds (e.g., open at −30 dB, close at −34 dB) to prevent chattering:

```cpp
enum class GateState { Closed, Attack, Open, Hold, Release };

if (state == GateState::Closed && inputDb > openThreshold)
    state = GateState::Attack;
else if (state == GateState::Open && inputDb < closeThreshold)
    state = GateState::Hold; // Hold timer starts, then transitions to Release
```

The hold phase keeps the gate open for a fixed duration after the signal drops, preventing chatter on decaying signals.

## JUCE Compressor Pattern

Minimal stereo-linked feed-forward compressor in `processBlock()`:

```cpp
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
{
    const int numSamples  = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();
    for (int i = 0; i < numSamples; ++i)
    {
        float inputLevel = 0.0f;
        for (int ch = 0; ch < numChannels; ++ch)
            inputLevel = std::max(inputLevel, std::abs(buffer.getSample(ch, i)));

        float inputDb = 20.0f * std::log10(inputLevel + 1e-6f);
        float gainDb  = computeGainDb(inputDb, threshold, ratio, knee);

        float coeff = (gainDb < envelopeDb) ? attackCoeff : releaseCoeff;
        envelopeDb  = coeff * envelopeDb + (1.0f - coeff) * gainDb;

        float gainLinear = std::pow(10.0f, (envelopeDb + makeupDb) / 20.0f);
        for (int ch = 0; ch < numChannels; ++ch)
            buffer.setSample(ch, i, buffer.getSample(ch, i) * gainLinear);

        currentGainReductionDb.store(envelopeDb, std::memory_order_relaxed);
    }
}
```

Recalculate `attackCoeff` and `releaseCoeff` in `prepareToPlay()` — not per-sample.

## Anti-Patterns

- ❌ **Mixed domains** — Don't detect in linear and threshold in dB. Work consistently in dB after conversion.
- ❌ **Missing makeup gain** — Compressed output is quieter, breaking gain staging and A/B comparisons.
- ❌ **Peak detection for musical compression** — Tracks waveform cycles. Use RMS; reserve peak for limiting.
- ❌ **Limiting without lookahead** — Cannot prevent overshoot. Use at least 1 ms lookahead.
- ❌ **Non-phase-aligned crossovers** — Butterworth/Chebyshev cause comb filtering. Use Linkwitz-Riley.

## Checklist

- [ ] Level detection mode selectable (peak / RMS) with appropriate smoothing
- [ ] Gain computer handles soft knee correctly across the full input range
- [ ] Attack/release coefficients recalculated on parameter change, not per-sample
- [ ] Sidechain HPF available (60–300 Hz) to prevent bass-induced pumping
- [ ] Makeup gain (auto or manual) compensates for gain reduction
- [ ] Lookahead latency reported via `setLatencySamples()` / `getLatencySamples()`
- [ ] Gain reduction meter output available for UI (atomic float, lock-free)
- [ ] Stereo linking mode to preserve stereo image
- [ ] All dB conversions use epsilon to guard against `log10(0)`
