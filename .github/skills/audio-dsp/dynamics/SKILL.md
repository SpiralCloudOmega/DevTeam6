---
name: dynamics
description: Compressor/limiter/gate design — envelope followers, gain computer, sidechain, multiband, limiting
---

# Dynamics Processing

Dynamics processors are essential mixing and mastering tools that control the amplitude envelope of audio signals. Compressors reduce dynamic range by attenuating signals above a threshold, limiters act as brick-wall ceilings to prevent clipping, and gates silence signals below a threshold to remove noise. All three share a canonical signal flow: level detection → gain computation → gain application.

## Signal Flow

1. **Input** → optional sidechain filter (HPF to ignore bass, or external key input)
2. **Level detection** — measure signal amplitude (peak or RMS)
3. **Gain computer** — map detected level to gain reduction using threshold, ratio, and knee
4. **Envelope follower** — smooth gain with attack/release ballistics to avoid zipper noise
5. **Gain application** — multiply the original audio by the computed linear gain
6. **Makeup gain** — compensate for level reduction introduced by compression

The sidechain and audio paths are independent — the detector analyzes the sidechain while gain is applied to the audio path.

## Level Detection

### Peak Detection

Peak detection takes the absolute value of each sample: `level = std::abs(sample)`. It responds instantaneously to transients — correct for brick-wall limiting but overly reactive for musical compression because it tracks individual waveform cycles rather than perceived loudness.

### RMS Detection

RMS measures signal power and correlates more closely with perceived loudness. A one-pole smoother provides a cheap running estimate:

```cpp
// Running RMS estimator — one-pole IIR smoother
// coeff controls the integration window: coeff = 1 - exp(-1 / (windowMs * 0.001f * sampleRate))
rmsSquared = (1.0f - coeff) * rmsSquared + coeff * (sample * sample);
float rmsLevel = std::sqrt(rmsSquared);
```

True RMS uses a fixed-length sliding window; the one-pole approximation is cheaper and sufficient for most musical applications. Convert linear level to dB: `float levelDb = 20.0f * std::log10(level + 1e-6f);` — the epsilon prevents log(0).

## Gain Computer

The gain computer is a pure static curve in the dB domain. Given a detected input level in dB, it returns gain reduction in dB. Soft-knee compression uses a quadratic blend around the threshold:

```cpp
float computeGainDb(float inputDb, float threshold, float ratio, float kneeWidth)
{
    const float halfKnee = kneeWidth * 0.5f;

    if (inputDb < threshold - halfKnee)
    {
        return 0.0f; // Below knee — no compression applied
    }
    else if (inputDb > threshold + halfKnee)
    {
        // Above knee — full ratio compression
        return (threshold + (inputDb - threshold) / ratio) - inputDb;
    }
    else
    {
        // Inside knee region — smooth quadratic transition
        float x = inputDb - threshold + halfKnee;
        return ((1.0f / ratio - 1.0f) * x * x) / (2.0f * kneeWidth);
    }
}
```

Key ratio values: **1:1** = unity (no compression), **4:1** = moderate compression, **10:1** = heavy limiting, **∞:1** = brick-wall limiting, **< 1:1** = upward expansion (used in gates and expanders).

## Envelope Follower (Ballistics)

The envelope follower smooths the gain reduction signal over time. The branching one-pole filter uses different coefficients for attack (gain decreasing) and release (gain recovering):

```cpp
const float attackCoeff  = std::exp(-1.0f / (attackMs  * 0.001f * sampleRate));
const float releaseCoeff = std::exp(-1.0f / (releaseMs * 0.001f * sampleRate));

// Per-sample envelope follower — called once per sample
float targetGainDb = computeGainDb(inputDb, threshold, ratio, knee);
float coeff = (targetGainDb < envelopeDb) ? attackCoeff : releaseCoeff;
envelopeDb = coeff * envelopeDb + (1.0f - coeff) * targetGainDb;
```

**Critical note:** Gain reduction values are negative dB. Attack means the envelope is *decreasing* (compressing harder), release means it is *increasing* (returning toward 0 dB). This is inverted from what newcomers expect.

## Sidechain Filtering

The sidechain is the signal fed to the level detector:

- **Internal HPF** — High-pass filter (60–300 Hz) prevents low-frequency content like kick drums from triggering excessive gain reduction ("pumping").
- **Band-pass (de-esser)** — Bandpass around 4–8 kHz makes the compressor respond only to sibilance.
- **External sidechain** — A separate signal (e.g., voiceover) drives the detector to duck the music underneath.

## Lookahead

Lookahead delays the audio path by 1–5 ms while feeding the *un-delayed* signal to the detector. Gain reduction begins *before* the transient arrives, eliminating overshoot. Essential for brick-wall limiting. Must be reported to the host via `setLatencySamples()` for delay compensation.

## Multiband Dynamics

Multiband compression splits the input into frequency bands, applies independent dynamics processing to each band, then sums the results:

1. **Band splitting** — Use Linkwitz-Riley (LR) crossovers, which are phase-aligned (allpass complementary) and sum flat. LR4 (4th-order) filters are constructed from two cascaded 2nd-order Butterworth biquads.
2. **Per-band compression** — Each band has its own threshold, ratio, attack, release, and makeup gain.
3. **Band summation** — Sum all processed bands. Because LR crossovers are magnitude-complementary, the sum reconstructs perfectly when no processing is applied.

A 3-band topology: input → LR4 crossover at f₁ → **low** band + remaining → LR4 crossover at f₂ → **mid** band + **high** band. Each crossover produces a low-pass and high-pass output that sum to an allpass.

## Brick-Wall Limiting

True-peak limiting prevents the output from ever exceeding 0 dBTP (or a user-set ceiling):

- **Lookahead buffer** — Typically 1–5 ms. The entire transient is visible before it reaches the output, allowing the limiter to pre-shape the gain curve smoothly.
- **Oversampled true-peak detection** — ITU-R BS.1770 specifies 4× oversampling for true-peak measurement. Inter-sample peaks (ISPs) occur when the reconstructed analog waveform between two digital samples exceeds the peak sample value. ISP detection is critical for mastering to streaming platforms (Spotify, Apple Music) that normalize to −1 dBTP.
- **Release shaping** — The release curve must be smooth and slow enough to avoid pumping artifacts but fast enough to recover between transients. Program-dependent release (adaptive) adjusts automatically based on signal density.

## Gate with Hysteresis

A noise gate silences the signal when it falls below a threshold. Parameters: **threshold** (open level), **attack** (fade-in time), **hold** (minimum open duration), **release** (fade-out time). Hysteresis uses separate open and close thresholds to prevent chattering — the gate opens at −30 dB but does not close until the signal drops to −34 dB:

```cpp
enum class GateState { Closed, Attack, Open, Hold, Release };

// State machine transition logic (per-sample)
if (state == GateState::Closed && inputDb > openThreshold)
    state = GateState::Attack;
else if (state == GateState::Open && inputDb < closeThreshold)
    state = GateState::Hold; // Hold timer starts, then transitions to Release
```

The hold phase keeps the gate open for a fixed duration after the signal drops below the close threshold, preventing rapid on/off switching on decaying signals like drum tails.

## JUCE Compressor Pattern

A minimal stereo-linked feed-forward compressor inside a JUCE `processBlock()`:

```cpp
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
{
    const int numSamples  = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    for (int i = 0; i < numSamples; ++i)
    {
        // Stereo-linked peak detection: max absolute value across channels
        float inputLevel = 0.0f;
        for (int ch = 0; ch < numChannels; ++ch)
            inputLevel = std::max(inputLevel, std::abs(buffer.getSample(ch, i)));

        float inputDb  = 20.0f * std::log10(inputLevel + 1e-6f);
        float gainDb   = computeGainDb(inputDb, threshold, ratio, knee);

        // Ballistics envelope follower
        float coeff = (gainDb < envelopeDb) ? attackCoeff : releaseCoeff;
        envelopeDb  = coeff * envelopeDb + (1.0f - coeff) * gainDb;

        // Convert dB gain to linear and apply with makeup
        float gainLinear = std::pow(10.0f, (envelopeDb + makeupDb) / 20.0f);

        for (int ch = 0; ch < numChannels; ++ch)
            buffer.setSample(ch, i, buffer.getSample(ch, i) * gainLinear);

        // Store gain reduction for UI metering
        currentGainReductionDb.store(envelopeDb, std::memory_order_relaxed);
    }
}
```

Recalculate `attackCoeff` and `releaseCoeff` in `prepareToPlay()` or whenever parameters change, not per-sample.

## Anti-Patterns

- ❌ **Mixed domains** — Don't detect levels in the linear domain and apply thresholds in dB. Work consistently in dB after the initial conversion to avoid scaling errors.
- ❌ **Missing makeup gain** — Without makeup gain the compressed output is quieter than the input, misleading A/B comparisons and breaking gain staging.
- ❌ **Peak detection for musical compression** — Peak detection follows individual waveform cycles. Use RMS for musically transparent compression; reserve peak for limiting.
- ❌ **Limiting without lookahead** — A zero-lookahead limiter cannot prevent overshoot on fast transients. Always use at least 1 ms lookahead for brick-wall operation.
- ❌ **Non-phase-aligned multiband crossovers** — Butterworth or Chebyshev crossovers introduce phase shifts that cause comb filtering at band edges. Use Linkwitz-Riley crossovers exclusively.

## Checklist

- [ ] Level detection mode selectable (peak / RMS) with appropriate smoothing coefficients
- [ ] Gain computer handles soft knee correctly across the full input range
- [ ] Attack/release coefficients recalculated on parameter change, not per-sample
- [ ] Sidechain HPF available (60–300 Hz tunable) to prevent bass-induced pumping
- [ ] Makeup gain (auto or manual) compensates for average gain reduction
- [ ] Lookahead latency reported to host via `setLatencySamples()` / `getLatencySamples()`
- [ ] Gain reduction meter output available for UI (atomic float, lock-free)
- [ ] Stereo linking mode (peak of L/R or sum) to preserve stereo image
- [ ] All dB conversions use epsilon to guard against `log10(0)`
