---
name: ai-mastering
description: AI-powered automated mastering chain with reference matching, loudness normalization, and spectral analysis
---

# AI-Powered Mastering Chain

Automated mastering pipeline for REVITHION STUDIO that replicates an iZotope Ozone-style chain: Reference Analysis → EQ → Multiband Dynamics → Stereo Imaging → Limiter → Loudness Normalization. Every stage is driven by spectral data and target loudness metrics.

## Reference Track Analysis

Extract the spectral envelope and integrated loudness from a reference track, then use the delta to drive downstream EQ and dynamics decisions.

```python
import numpy as np
import soundfile as sf
import pyloudnorm as pyln
from scipy.signal import welch

def analyze_reference(ref_path: str) -> dict:
    """Return spectral envelope and loudness stats for a reference track."""
    data, rate = sf.read(ref_path)
    if data.ndim > 1:
        mono = np.mean(data, axis=1)
    else:
        mono = data
    freqs, psd = welch(mono, fs=rate, nperseg=4096)
    envelope_db = 10 * np.log10(psd + 1e-12)
    meter = pyln.Meter(rate)
    integrated_lufs = meter.integrated_loudness(data)
    return {"freqs": freqs, "envelope_db": envelope_db, "lufs": integrated_lufs, "rate": rate}
```

## Matchering 2.0 Integration

Single-call mastering that matches a target's spectral profile and loudness. Use batch mode for album workflows.

```python
import matchering as mg

# Single track
mg.process(target="mix.wav", reference="ref.wav", results=[mg.pcm24("master.wav")])

# Batch processing for albums
import glob, os

def batch_master(input_dir: str, ref_path: str, output_dir: str):
    os.makedirs(output_dir, exist_ok=True)
    for track in sorted(glob.glob(os.path.join(input_dir, "*.wav"))):
        stem = os.path.splitext(os.path.basename(track))[0]
        out = os.path.join(output_dir, f"{stem}_master.wav")
        mg.process(target=track, reference=ref_path, results=[mg.pcm24(out)])
```

## Automated EQ Decisions

Compute the spectral difference between the mix and reference, then generate parametric EQ bands to close the gap.

```python
from scipy.interpolate import interp1d
from scipy.signal import argrelextrema

def generate_eq_curve(mix_env: dict, ref_env: dict, max_bands: int = 8) -> list:
    """Return a list of {freq_hz, gain_db, q} parametric EQ bands."""
    common = np.intersect1d(mix_env["freqs"], ref_env["freqs"], return_indices=True)
    delta = ref_env["envelope_db"][common[2]] - mix_env["envelope_db"][common[1]]
    freqs = mix_env["freqs"][common[1]]
    smoothed = np.convolve(delta, np.ones(5) / 5, mode="same")
    peaks = argrelextrema(np.abs(smoothed), np.greater, order=10)[0]
    bands = sorted(peaks, key=lambda i: abs(smoothed[i]), reverse=True)[:max_bands]
    return [{"freq_hz": float(freqs[i]), "gain_db": float(np.clip(smoothed[i], -6, 6)),
             "q": 1.5} for i in bands]
```

## Multiband Dynamics Processing

Split the signal at standard crossover points and apply per-band compression. The C++ skeleton below shows the JUCE-based approach.

| Band | Range | Ratio | Attack (ms) | Release (ms) |
|------|-------|-------|-------------|---------------|
| Low | 20–200 Hz | 3:1 | 30 | 200 |
| Low-Mid | 200–2 kHz | 2.5:1 | 15 | 150 |
| High-Mid | 2–8 kHz | 2:1 | 10 | 100 |
| High | 8–20 kHz | 1.5:1 | 5 | 80 |

```cpp
// JUCE multiband compressor skeleton — four-band Linkwitz-Riley crossover
struct BandCompressor {
    juce::dsp::LinkwitzRileyFilter<float> lowpass, highpass;
    juce::dsp::Compressor<float> compressor;
    float crossoverHz;

    void prepare(const juce::dsp::ProcessSpec& spec, float xover, float ratio,
                 float attackMs, float releaseMs) {
        crossoverHz = xover;
        lowpass.setCutoffFrequency(xover);
        highpass.setCutoffFrequency(xover);
        lowpass.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
        highpass.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
        compressor.setRatio(ratio);
        compressor.setAttack(attackMs);
        compressor.setRelease(releaseMs);
        compressor.setThreshold(-18.0f);
        lowpass.prepare(spec);
        highpass.prepare(spec);
        compressor.prepare(spec);
    }
};
```

## Stereo Imaging Recommendations

Analyze stereo width per frequency band. Keep low frequencies mono (below 200 Hz), allow moderate width in the mids, and widen the highs conservatively to avoid phase issues.

```python
def stereo_width_analysis(data: np.ndarray) -> dict:
    """Return per-band correlation and width estimate for a stereo signal."""
    left, right = data[:, 0], data[:, 1]
    mid = (left + right) / 2.0
    side = (left - right) / 2.0
    mid_energy = np.sum(mid ** 2)
    side_energy = np.sum(side ** 2)
    width = side_energy / (mid_energy + side_energy + 1e-12)
    correlation = np.corrcoef(left, right)[0, 1]
    return {"width": float(width), "correlation": float(correlation),
            "recommendation": "widen" if width < 0.15 else "narrow" if width > 0.45 else "ok"}
```

## Loudness Normalization — Streaming Platform Targets

| Platform | Integrated LUFS | True Peak (dBTP) | Notes |
|----------|----------------|-------------------|-------|
| Spotify | -14 | -1.0 | Normalization on by default |
| Apple Music | -16 | -1.0 | Sound Check enabled |
| YouTube | -14 | -1.0 | Loudness normalization always active |
| Amazon Music | -14 | -2.0 | Stricter true-peak ceiling |
| Tidal | -14 | -1.0 | MQA passthrough preserves dynamics |

```python
import pyloudnorm as pyln
import soundfile as sf

def normalize_loudness(input_path: str, output_path: str, target_lufs: float = -14.0):
    """Normalize a file to the target integrated LUFS."""
    data, rate = sf.read(input_path)
    meter = pyln.Meter(rate)
    current = meter.integrated_loudness(data)
    normalized = pyln.normalize.loudness(data, current, target_lufs)
    peak = np.max(np.abs(normalized))
    if peak > 10 ** (-1.0 / 20):  # -1 dBTP ceiling
        normalized *= 10 ** (-1.0 / 20) / peak
    sf.write(output_path, normalized, rate, subtype="PCM_24")
    return {"original_lufs": current, "target_lufs": target_lufs,
            "final_lufs": meter.integrated_loudness(normalized)}
```

## A/B Comparison Workflow

Level-match the original mix and the master to within ±0.1 LUFS, then run a blind listening test to confirm the master is an improvement rather than just louder.

```bash
# Measure both files with ffmpeg loudnorm filter for quick verification
ffmpeg -i mix.wav -af loudnorm=print_format=json -f null - 2>&1 | grep input_i
ffmpeg -i master.wav -af loudnorm=print_format=json -f null - 2>&1 | grep input_i
```

## Anti-Patterns

- **Mastering without headroom** — always leave at least -1 dBTP of true-peak margin before the final limiter stage.
- **Ignoring inter-sample peaks** — use a true-peak limiter, not a sample-peak limiter, to avoid codec-induced clipping on lossy formats.
- **Matching loudness without matching spectrum** — a reference track match that only adjusts gain will produce a tonally wrong master.
- **Over-widening the low end** — bass content below 200 Hz must stay mono to avoid phase cancellation on playback systems.
- **Skipping A/B comparison** — always level-match and blind-test; louder ≠ better.
- **Single-reference mastering** — use at least two reference tracks from different releases to avoid inheriting one mix's quirks.

## Checklist

- [ ] Reference track analyzed (spectral envelope + integrated LUFS extracted)
- [ ] Spectral delta computed and EQ curve generated (max ±6 dB per band)
- [ ] Multiband compressor configured with per-band ratios and crossovers
- [ ] Stereo width verified (mono below 200 Hz, no excessive widening above 8 kHz)
- [ ] True-peak limiter engaged with ceiling at -1.0 dBTP (or -2.0 dBTP for Amazon)
- [ ] Integrated loudness hits platform target (Spotify/YouTube/Tidal: -14, Apple: -16)
- [ ] A/B blind test passed against level-matched original mix
- [ ] Batch processing validated (all album tracks share consistent tonal balance)
