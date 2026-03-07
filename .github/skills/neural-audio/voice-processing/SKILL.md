---
name: voice-processing
description: AI-powered vocal processing including pitch correction, isolation, noise reduction, and harmony generation
---
# Voice Processing

AI voice processing pipeline for music production and post-production in REVITHION STUDIO.

## Pitch Correction
Autotune-style correction: pitch detection (YIN/pYIN), snap-to-scale quantization with retune speed, and pitch shifting via PSOLA or phase vocoder. Low retune speed → hard-tune effect; high → natural correction.

```python
import librosa
import numpy as np

def detect_and_correct(audio: np.ndarray, sr: int, scale_hz: list[float], retune_speed: float = 0.1):
    """Detect pitch with pYIN and snap to nearest scale degree."""
    f0, voiced_flag, _ = librosa.pyin(audio, fmin=80, fmax=800, sr=sr)
    corrected_f0 = np.copy(f0)
    for i, (freq, voiced) in enumerate(zip(f0, voiced_flag)):
        if voiced and freq is not None and freq > 0:
            target = min(scale_hz, key=lambda s: abs(s - freq))
            corrected_f0[i] = freq + (target - freq) * retune_speed
    return corrected_f0
```

## Formant Preservation
Estimate spectral envelope via LPC (order = `2 + sr/1000`), strip it before pitch shifting, then re-apply the original envelope. This decouples pitch from vocal tract character, preventing chipmunk artifacts and enabling creative formant shift for gender conversion.

## Vocal Isolation
Demucs v4 (hybrid transformer) and BSRNN (band-split RNN) are leading source separation architectures.

```python
import torch
from demucs.pretrained import get_model
from demucs.apply import apply_model

def isolate_vocals(audio_path: str, device: str = "cuda") -> np.ndarray:
    model = get_model("htdemucs_ft")
    model.to(device)
    wav = load_audio(audio_path, model.samplerate, model.audio_channels)
    ref = wav.mean(0)
    wav = (wav - ref.mean()) / ref.std()
    sources = apply_model(model, wav[None], device=device)[0]
    return sources[model.sources.index("vocals")].cpu().numpy()
```

## De-essing and De-breath
**De-essing**: Detect sibilance via RMS energy ratio in 4–10 kHz vs broadband. Apply dynamic EQ or split-band compression with fast attack (0.5–2 ms), medium release (20–50 ms). Use adaptive thresholds per segment.
**De-breath**: Classify 30 ms frames using energy, low spectral centroid, and rolloff below 2 kHz. Remove by crossfade to room tone or attenuate 15–25 dB.

## Noise Reduction
**RNNoise**: Compact GRU on 10 ms frames at 48 kHz, outputting per-band gain masks for real-time use.

```python
import ctypes, numpy as np

def rnnoise_process(audio: np.ndarray, sr: int = 48000) -> np.ndarray:
    lib = ctypes.cdll.LoadLibrary("librnnoise.so")
    state = lib.rnnoise_create(None)
    frame_size = 480  # 10ms at 48kHz
    output = np.zeros_like(audio, dtype=np.float32)
    buf = (ctypes.c_float * frame_size)()
    for i in range(0, len(audio) - frame_size, frame_size):
        for j in range(frame_size):
            buf[j] = float(audio[i + j]) * 32768.0
        lib.rnnoise_process_frame(state, buf, buf)
        for j in range(frame_size):
            output[i + j] = buf[j] / 32768.0
    lib.rnnoise_destroy(state)
    return output
```

**Spectral gating**: Profile noise from a silent segment, compute mean magnitude, subtract with flooring per STFT frame. Prefer Wiener soft masking over hard subtraction to avoid musical noise.

## Vocal Doubling and Harmony
**Doubling**: Combine timing offset (10–40 ms), pitch variation (±5–15 cents), and formant shift (±2–5%) on a copy of the lead. Pan off-center; verify mono compatibility.
**Harmony**: Detect lead pitch per frame, map to target interval (+4 semitones for thirds, +7 for fifths), pitch-shift to target. Quantize intervals to the song key for diatonic harmony.

## Whisper and Speech Detection
Classify segments as sung, spoken, or whispered using per-frame (50 ms) features: RMS energy (whisper 15–25 dB below speech), spectral centroid (higher for whisper), and zero-crossing rate. A decision tree on these three features routes audio to the appropriate processing chain.

## Real-Time Pitch Correction (C++)

```cpp
#include <cmath>
#include <vector>
class PitchCorrector {
public:
    PitchCorrector(float sr, float speed) : sr_(sr), speed_(speed), phase_(0.f) {}
    void setScale(const std::vector<float>& hz) { scale_ = hz; }
    float snapToScale(float hz) const {
        float best = scale_[0], bestD = std::abs(hz - best);
        for (size_t i = 1; i < scale_.size(); ++i) {
            float d = std::abs(hz - scale_[i]);
            if (d < bestD) { bestD = d; best = scale_[i]; }
        }
        return best;
    }
    void processBlock(float* buf, int n, float hz) {
        float smoothed = 1.f + (snapToScale(hz) / hz - 1.f) * speed_;
        for (int i = 0; i < n; ++i) {
            phase_ += smoothed;
            int idx = static_cast<int>(phase_) % n;
            buf[i] = buf[idx] * 0.5f + buf[i] * 0.5f;
        }
    }
private:
    float sr_, speed_, phase_;
    std::vector<float> scale_;
};
```

## Anti-Patterns
- **Pitch-shifting without formant preservation** — chipmunk artifacts; always decouple envelopes.
- **Hard spectral subtraction** — musical noise; use Wiener masking or learned gain masks.
- **Fixed-threshold de-essing** — varies per singer; use adaptive threshold relative to broadband RMS.
- **Running Demucs on clips < 5 s** — padding artifacts; pad to ≥ 7 s and trim after.
- **Ignoring latency** — YIN needs 2–3 period windows; budget 20–40 ms lookahead for PDC.
- **Harmony before isolation** — instrument bleed corrupts pitch detection; always isolate first.

## Checklist
- [ ] Pitch detection validated (MIREX tolerance ±50 cents)
- [ ] Formant preserved across ±12 semitone shift range
- [ ] Vocal isolation SDR ≥ 8 dB on MUSDB18 test set
- [ ] De-esser adapts threshold per segment, not globally
- [ ] De-breath preserves adjacent transients
- [ ] Noise reduction free of musical noise on sustained notes
- [ ] Vocal doubling mono-compatible (sum to mono check)
- [ ] Harmony intervals correct for song key (diatonic validation)
- [ ] Whisper detection ≥ 90% accuracy on mixed corpus
- [ ] Real-time latency measured and reported to host for PDC
