---
name: stem-separation
description: AI-powered audio stem separation using Demucs and BSRNN for music production workflows
---

# AI Stem Separation

## Architecture Overview
**Demucs v4 (Hybrid Transformer)** encodes audio through parallel waveform and spectrogram paths, fuses them inside a transformer with cross-attention, then decodes back to isolated stems. The hybrid design captures temporal transients (waveform) and harmonic content (spectrogram), yielding state-of-the-art SDR on musdb18-HQ.

**BSRNN (Band-Split RNN)** splits the spectrogram into learnable frequency sub-bands, processes each with dedicated RNNs, then merges via band fusion. Excels at vocal separation where harmonics occupy narrow frequency regions.

## Output Configurations
| Model | Stems | Targets |
|---|---|---|
| `htdemucs` | 4 | vocals, drums, bass, other |
| `htdemucs_ft` | 4 | vocals, drums, bass, other (fine-tuned, highest SDR) |
| `htdemucs_6s` | 6 | vocals, drums, bass, guitar, piano, other |
| `mdx_extra` | 4 | vocals, drums, bass, other (MDX-Net architecture) |

Use `htdemucs_ft` for max quality 4-stem; use `htdemucs_6s` when you need guitar/piano isolation.

## CLI Usage
```bash
# Basic 4-stem separation
demucs --two-stems=vocals -n htdemucs_ft input.wav

# 6-stem separation on GPU with 10-second overlap for gapless output
demucs -n htdemucs_6s --device cuda --overlap 0.25 --clip-mode rescale input.wav

# Batch process a folder, output as float32 WAV
demucs -n htdemucs_ft --device cuda --float32 --out ./stems/ tracks/*.wav
```

## Python — Single-Track Separation
```python
import torch, torchaudio
from demucs.pretrained import get_model
from demucs.apply import apply_model

model = get_model("htdemucs_ft")
model.to("cuda" if torch.cuda.is_available() else "cpu")
wav, sr = torchaudio.load("mix.wav")
with torch.no_grad():
    sources = apply_model(model, wav.unsqueeze(0).to(model.device), overlap=0.25)
for i, name in enumerate(model.sources):  # ['drums', 'bass', 'other', 'vocals']
    torchaudio.save(f"{name}.wav", sources[0, i].cpu(), sr)
```

## Python — GPU Batch Processing
```python
import torch, glob, torchaudio
from demucs.pretrained import get_model
from demucs.apply import apply_model

model = get_model("htdemucs_ft").to("cuda")
for path in glob.glob("tracks/*.wav"):
    wav, sr = torchaudio.load(path)
    sources = apply_model(model, wav.unsqueeze(0).cuda(), overlap=0.25,
                          split=True, segment=7.8)  # auto-chunk for VRAM safety
    for i, name in enumerate(model.sources):
        torchaudio.save(f"stems/{path.split('/')[-1]}_{name}.wav", sources[0, i].cpu(), sr)
    torch.cuda.empty_cache()
```

## Quality Metrics
**SDR** (Signal-to-Distortion Ratio) is the primary benchmark; higher is better. **SIR** measures bleed, **SAR** measures artifacts.

```python
import numpy as np

def sdr(reference: np.ndarray, estimate: np.ndarray) -> float:
    noise = reference - estimate
    return 10 * np.log10(np.sum(reference ** 2) / (np.sum(noise ** 2) + 1e-8))
```

Typical htdemucs_ft on musdb18-HQ: vocals 8.1 dB, drums 8.4 dB, bass 7.7 dB, other 5.6 dB.

## Real-Time vs Offline
| Aspect | Real-time | Offline |
|---|---|---|
| Latency | <50 ms (causal model) | Minutes per track |
| Quality | Lower SDR (no future context) | Best SDR (bidirectional) |
| Use case | Live monitoring, DJ sets | Final mix, stem mastering |

For REVITHION, run offline `htdemucs_ft` for master-quality stems; use real-time for preview only.

## DAW Integration
Route each stem to its own mixer bus for independent EQ/compression/spatial processing.

```python
for stem_path, bus_id in zip(stem_files, [1, 2, 3, 4]):
    session.import_audio(stem_path, target_bus=bus_id, align_to="project_start")
```

## Anti-Patterns
- **Separating already-separated stems** — cascading models multiplies artifacts; always use the original mix.
- **Ignoring overlap** — `overlap=0` creates audible seams at chunk boundaries.
- **FP16 on long tracks** — half-precision accumulates error over millions of samples; use FP32.
- **Skipping VRAM management** — omitting `torch.cuda.empty_cache()` between tracks causes OOM.

## Checklist
- [ ] Select model matching target stem count (4-stem vs 6-stem)
- [ ] Verify GPU memory headroom before processing (segment size × batch)
- [ ] Use `--float32` and `overlap ≥ 0.25` for production renders
- [ ] Validate output with SDR against reference where available
- [ ] Route stems to dedicated mixer buses before applying effects
- [ ] Archive original mix alongside stems for reproducibility
