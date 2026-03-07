---
name: amp-training
description: Training neural network models of guitar amplifiers, pedals, and effects using PyTorch
---

# Neural Amp Model Training Pipeline

End-to-end workflow for capturing, training, and deploying neural models of analog guitar amps and pedals within REVITHION STUDIO.

## Data Capture Setup

Record time-aligned DI/reamped pairs through the target hardware at 48 kHz / 24-bit mono. Keep the DI path clean with no processing. Use diverse playing (chords, single notes, palm mutes) for 3–5 minutes. Leave 3–6 dB headroom to avoid digital clipping.

```bash
soxi -r -b data/raw/DI_signal.wav data/raw/reamped_signal.wav
python scripts/align_audio.py --di data/raw/DI_signal.wav --target data/raw/reamped_signal.wav --out data/aligned/
```

## Dataset Preprocessing

Normalize to [-1, 1], segment into fixed-length chunks, and split 80/10/10.

```python
import numpy as np, soundfile as sf
from sklearn.model_selection import train_test_split

def preprocess(di_path, target_path, chunk=65536):
    di, _ = sf.read(di_path, dtype="float32")
    tgt, _ = sf.read(target_path, dtype="float32")
    di, tgt = di / (np.max(np.abs(di)) + 1e-8), tgt / (np.max(np.abs(tgt)) + 1e-8)
    n = len(di) // chunk
    di_c = di[:n * chunk].reshape(n, chunk, 1)
    tgt_c = tgt[:n * chunk].reshape(n, chunk, 1)
    tr, tmp = train_test_split(np.arange(n), test_size=0.2, random_state=42)
    va, te = train_test_split(tmp, test_size=0.5, random_state=42)
    return {"train": (di_c[tr], tgt_c[tr]), "val": (di_c[va], tgt_c[va]), "test": (di_c[te], tgt_c[te])}
```

## LSTM Model Architecture

Single recurrent layer (hidden 32–64 typical, up to 96 for complex amps) with a linear head.

```python
import torch, torch.nn as nn

class LSTMAmpModel(nn.Module):
    def __init__(self, hidden=40):
        super().__init__()
        self.rec = nn.LSTM(1, hidden, batch_first=True)
        self.head = nn.Linear(hidden, 1)
    def forward(self, x):
        h, _ = self.rec(x)
        return self.head(h)
```

## Loss Functions

```python
def esr_loss(pred, target):
    """Error-to-Signal Ratio — below 0.01 is transparent, below 0.005 excellent."""
    return torch.sum((target - pred) ** 2) / (torch.sum(target ** 2) + 1e-8)

def pre_emphasis_filter(sig, coeff=0.95):
    """Boost high frequencies so the loss penalizes dull pick attack."""
    padded = torch.nn.functional.pad(sig, (0, 0, 1, 0))
    return sig - coeff * padded[:, :-1, :]

def combined_loss(pred, target, pe_weight=0.5):
    pe_pred, pe_tgt = pre_emphasis_filter(pred), pre_emphasis_filter(target)
    return esr_loss(pred, target) + pe_weight * esr_loss(pe_pred, pe_tgt)
```

## Training Loop with LR Scheduling

OneCycleLR for fast convergence; ReduceLROnPlateau as fallback when validation stalls.

```python
def train(model, train_dl, val_dl, epochs=500, lr=5e-4, device="cuda"):
    opt = torch.optim.Adam(model.parameters(), lr=lr)
    cyc = torch.optim.lr_scheduler.OneCycleLR(opt, max_lr=lr*10, epochs=epochs, steps_per_epoch=len(train_dl))
    plat = torch.optim.lr_scheduler.ReduceLROnPlateau(opt, patience=20, factor=0.5)
    best = float("inf")
    for ep in range(epochs):
        model.train()
        for di_b, tgt_b in train_dl:
            di_b, tgt_b = di_b.to(device), tgt_b.to(device)
            loss = combined_loss(model(di_b), tgt_b)
            opt.zero_grad(); loss.backward()
            torch.nn.utils.clip_grad_norm_(model.parameters(), 1.0)
            opt.step(); cyc.step()
        model.eval(); val_esr = 0.0
        with torch.no_grad():
            for d, t in val_dl:
                val_esr += esr_loss(model(d.to(device)), t.to(device)).item()
        val_esr /= len(val_dl); plat.step(val_esr)
        if val_esr < best:
            best = val_esr; torch.save(model.state_dict(), "best.pt")
        if ep % 50 == 0:
            print(f"Epoch {ep:>4d} | ESR {val_esr:.6f} | lr {opt.param_groups[0]['lr']:.2e}")
```

## Export to RTNeural JSON

RTNeural expects a JSON with layer descriptors and flat weight arrays.

```python
import json

def export_rtneural(model, path="model.json"):
    sd, H = model.state_dict(), model.rec.hidden_size
    def s(key, a, b): return sd[key][a:b].flatten().tolist()
    def bias(a, b): return (sd["rec.bias_ih_l0"][a:b] + sd["rec.bias_hh_l0"][a:b]).tolist()
    json.dump({"in_shape": [1], "layers": [
        {"type": "lstm", "shape": [1, H], "weights": {
            "W_ii": s("rec.weight_ih_l0", 0, H), "W_if": s("rec.weight_ih_l0", H, 2*H),
            "W_ig": s("rec.weight_ih_l0", 2*H, 3*H), "W_io": s("rec.weight_ih_l0", 3*H, 4*H),
            "W_hi": s("rec.weight_hh_l0", 0, H), "W_hf": s("rec.weight_hh_l0", H, 2*H),
            "W_hg": s("rec.weight_hh_l0", 2*H, 3*H), "W_ho": s("rec.weight_hh_l0", 3*H, 4*H),
            "b_i": bias(0, H), "b_f": bias(H, 2*H), "b_g": bias(2*H, 3*H), "b_o": bias(3*H, 4*H)}},
        {"type": "dense", "shape": [H, 1],
         "weights": sd["head.weight"].flatten().tolist(), "bias": sd["head.bias"].tolist()}
    ]}, open(path, "w"))
```

## NAM Workflow

Neural Amp Modeler uses its own `.nam` format. Capture with the standard NAM input signal, then train via CLI.

```bash
pip install neural-amp-modeler
nam train --input data/aligned/DI.wav --target data/aligned/reamped.wav \
    --architecture lstm-40 --epochs 500 --out models/my_amp.nam
```

## A/B Testing

1. **Blind listening** — render a passage through real amp and model, randomize labels, 3+ listeners rate 1–5.
2. **Spectral comparison** — overlay magnitude spectra; check harmonic rolloff above 4 kHz.
3. **ESR on test set** — confirm < 0.01.

```bash
sox reamped.wav -n spectrogram -o spec_real.png
sox predicted.wav -n spectrogram -o spec_model.png
```

## Anti-Patterns

- **Clipped captures** — digital clipping teaches the wrong nonlinearity. Always leave headroom.
- **Skipping pre-emphasis** — model sounds dull despite low ESR because transients are under-fit.
- **Single-riff training data** — model fails on unseen styles. Use diverse input material.
- **Ignoring alignment** — one sample offset inflates ESR. Cross-correlate before training.
- **Oversized hidden layers** — hidden > 96 rarely helps but doubles real-time CPU cost.

## Checklist

- [ ] DI and target WAVs are 48 kHz / 24-bit mono, matched length
- [ ] Peak-normalized to [-1, 1], no digital clipping
- [ ] Sub-sample aligned via cross-correlation
- [ ] Dataset split 80/10/10 train/val/test
- [ ] LSTM hidden size selected (32–96 range)
- [ ] ESR + pre-emphasis combined loss active
- [ ] Gradient clipping at max_norm=1.0
- [ ] OneCycleLR + ReduceLROnPlateau configured
- [ ] Best checkpoint saved on val ESR improvement
- [ ] Test ESR < 0.01 confirmed
- [ ] Exported to RTNeural JSON or NAM `.nam`
- [ ] Blind A/B test passed (≥ 4/5 fidelity)
- [ ] Real-time inference profiled within CPU budget
