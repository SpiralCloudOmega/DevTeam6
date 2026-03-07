---
name: metering
description: Audio metering — peak, RMS, VU, EBU R128 loudness, spectral analyzer, correlation meter
---

# Audio Metering

Accurate metering is critical for mixing, mastering, and broadcast compliance. Different meter types measure different properties — peak meters catch transients, RMS/VU meters approximate perceived loudness, and EBU R128 defines the modern loudness standard. REVITHION STUDIO needs comprehensive metering across all of these standards to ensure professional output at every stage.

## Peak Meter

Sample-peak metering takes the absolute value of each sample, tracking the maximum per buffer. Ballistics use instant attack with a slow release fall rate of 1–2 dB/sec. Display in dBFS (dB relative to full scale): `dBFS = 20 * log10(|sample|)`.

```cpp
float peakLevel = 0.0f;
for (int i = 0; i < numSamples; ++i)
    peakLevel = std::max(peakLevel, std::abs(buffer[i]));
float peakDb = 20.0f * std::log10(peakLevel + 1e-6f);
```

Peak hold retains the maximum value for approximately 2 seconds, then releases at the standard fall rate. Track `peakHoldValue` and `peakHoldTimer`; reset the timer on each new peak, decrement per callback, and begin release when the timer expires.

## RMS Meter

RMS (Root Mean Square) measures average signal power over a window, which is more perceptually relevant than peak. A 300 ms window is typical for general use; per-buffer RMS works for visualization. Convert to dB for display.

```cpp
float sumSquares = 0.0f;
for (int i = 0; i < numSamples; ++i)
    sumSquares += buffer[i] * buffer[i];
float rms = std::sqrt(sumSquares / static_cast<float>(numSamples));
float rmsDb = 20.0f * std::log10(rms + 1e-6f);
```

## VU Meter

The VU (Volume Unit) meter uses approximately 300 ms integration time, reaching 99% of a steady-state value in that window. It roughly corresponds to perceived loudness for sustained signals. Historically 0 VU equals +4 dBu at professional reference level. In digital, calibrate 0 VU to −18 dBFS (EBU convention) or −20 dBFS (SMPTE convention). Implement with a single-pole IIR lowpass on the rectified signal to model the ballistic response.

## EBU R128 / ITU-R BS.1770 Loudness

### K-Weighting Filter

Two cascaded biquad filters applied before loudness measurement. The first is a high shelf boosting +3.9999 dB at high frequencies, modeling the head-related diffuse-field response. The second is an RLB high-pass filter removing energy below ~60 Hz. Coefficients are sample-rate dependent — recalculate when the rate changes.

```cpp
// JUCE IIR biquad setup for K-weighting (48 kHz coefficients shown)
auto highShelf = juce::dsp::IIR::Coefficients<float>::makeHighShelf(48000.0, 1681.97, 0.7071f, 3.9999f);
auto highPass  = juce::dsp::IIR::Coefficients<float>::makeHighPass(48000.0, 38.1355, 0.5003f);
```

### Loudness Measurements

- **Momentary Loudness (M)**: 400 ms sliding window over K-weighted signal. Updated every 100 ms.
- **Short-term Loudness (S)**: 3-second sliding window over K-weighted signal. Updated every 100 ms.
- **Integrated Loudness (I)**: Entire program duration, gated. Absolute gate discards blocks below −70 LUFS. Relative gate computes Γ\_a (mean of blocks passing absolute gate), then discards blocks below Γ\_a − 10 LU.
- **Loudness Range (LRA)**: Statistical distribution of short-term loudness between the 10th and 95th percentiles after gating.
- **True Peak (TP)**: Oversampled peak detection (minimum 4× per ITU-R BS.1770) to catch inter-sample peaks that exceed 0 dBFS after DAC reconstruction.
- Units: LUFS (Loudness Units Full Scale) for absolute, LU for relative measurements.

### Targets

| Context | Integrated Loudness | True Peak Max |
|---|---|---|
| Spotify / YouTube | −14 LUFS | −1 dBTP |
| Apple Music | −16 LUFS | −2 dBTP |
| EBU R128 (EU broadcast) | −23 LUFS ± 0.5 LU | −1 dBTP |
| ATSC A/85 (US broadcast) | −24 LUFS ± 2 LU | −2 dBTP |

## K-System Metering (Bob Katz)

RMS-based metering with peak indicators and a shifted 0-point: **K-20** (0 = −20 dBFS) for wide dynamic range material like classical and jazz; **K-14** (0 = −14 dBFS) for pop and rock; **K-12** (0 = −12 dBFS) for broadcast. The system encourages mixing to a calibrated monitoring level rather than maximizing peak level.

## Spectral Analyzer

Apply a windowed FFT (Hann window, 2048 or 4096 samples) to compute the magnitude spectrum. Display on a logarithmic frequency axis (20 Hz–20 kHz) to match octave perception, with a dB scale on the Y axis (typically −90 to 0 dBFS). Smooth magnitudes over time with exponential averaging for a stable display.

```cpp
// Exponential smoothing per FFT bin for display stability
smoothedMag[bin] = smoothCoeff * smoothedMag[bin]
                 + (1.0f - smoothCoeff) * currentMag[bin];
```

## Correlation Meter

Measures the phase relationship between L and R channels. +1.0 means perfectly correlated (fully mono compatible), 0.0 means uncorrelated (wide stereo), and −1.0 means perfectly out of phase (mono cancellation — a serious problem). Implement as a running normalized cross-correlation.

```cpp
sumLR += left * right;
sumLL += left * left;
sumRR += right * right;
float correlation = sumLR / (std::sqrt(sumLL * sumRR) + 1e-6f);
```

## Stereo Scope (Goniometer / Lissajous)

An X-Y display of the stereo signal rotated 45° into the M/S domain. Vertical axis represents mid (mono sum), horizontal axis represents side (difference). Plot rotated coordinates: `x = (R - L) / sqrt(2)`, `y = (L + R) / sqrt(2)`. Useful for visualizing stereo width, balance, and phase issues at a glance.

## Phase Meter

A real-time visual representation of the correlation value, typically rendered as a needle or bar oscillating between −1 and +1. Often combined with the goniometer display. Values persistently below 0 indicate phase problems that will cause mono fold-down issues on broadcast systems.

## Thread Safety for Metering

Meter values are computed on the audio thread but displayed on the GUI thread. Never use a mutex or lock on the audio thread. Use `std::atomic<float>` with relaxed memory ordering — sufficient for single-producer single-consumer scalar metering data.

```cpp
std::atomic<float> peakLevelL{0.0f};
std::atomic<float> peakLevelR{0.0f};

// Audio thread: store with relaxed ordering (no fence needed)
peakLevelL.store(newPeak, std::memory_order_relaxed);

// GUI thread: load with relaxed ordering
float displayPeak = peakLevelL.load(std::memory_order_relaxed);
```

## Anti-Patterns

- ❌ Don't use sample-peak as a loudness meter — peak ≠ loudness
- ❌ Don't skip true-peak metering for mastering — inter-sample peaks cause DAC clipping
- ❌ Don't use mutex/lock to transfer meter values from the audio to GUI thread
- ❌ Don't forget K-weighting for loudness measurement — unweighted RMS is not perceptual loudness
- ❌ Don't display raw FFT bins on a linear frequency axis — use logarithmic for musical relevance

## Checklist

- [ ] Peak meter includes true-peak (oversampled) option
- [ ] RMS window length configurable (50 ms to 1000 ms)
- [ ] EBU R128 integrated loudness uses both absolute and relative gates
- [ ] K-weighting filters recalculated for current sample rate
- [ ] Meter values passed via `std::atomic<float>` — no locks on audio thread
- [ ] Spectral analyzer uses windowed FFT with logarithmic frequency axis
- [ ] Correlation meter active for mono compatibility checking
