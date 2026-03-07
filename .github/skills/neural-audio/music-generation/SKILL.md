---
name: music-generation
description: AI music generation using ACE-Step with text-to-music, covers, LoRA fine-tuning, and DAW integration
---
# AI Music Generation

Generate production-ready music from text using ACE-Step 1.5. The three-stage pipeline — Qwen3 LM text encoder, DiT diffusion transformer, VAE audio decoder — supports text-to-music, covers/repainting, LoRA fine-tuning, and direct DAW integration.

## Architecture Overview
1. **Qwen3 LM (Text Encoder)** — Encodes lyrics (`[verse]`/`[chorus]` markers) and style tags into conditioning embeddings capturing genre, mood, and temporal structure.
2. **DiT (Diffusion Transformer)** — Denoises latent representations over N steps (default 60, min 20 for drafts). CFG scale 3.0–7.0.
3. **VAE (Decoder)** — Converts the final latent tensor into a 48kHz stereo waveform with full spectral fidelity.

## Text-to-Music Prompting
Tag format: `[genre] [sub-genre] [instruments] [mood/energy] [tempo]`. Always include BPM.

```json
{
  "tags": "progressive metal, djent, 8-string guitar, atmospheric synth pads, aggressive, 140bpm",
  "lyrics": "[verse]\nBeneath the fractured sky we fall\n[chorus]\nWe are the signal in the noise",
  "duration": 180, "cfg_scale": 5.0, "steps": 60
}
```

## ACE-Step API Usage
```python
import requests
from pathlib import Path

def generate_track(tags: str, lyrics: str, duration: int = 120) -> bytes:
    """Generate a full track via the ACE-Step server."""
    resp = requests.post("http://localhost:8001/generate", json={
        "tags": tags, "lyrics": lyrics, "duration": duration,
        "cfg_scale": 5.0, "infer_steps": 60, "seed": -1,
        "output_format": "wav", "sample_rate": 48000,
    }, timeout=300)
    resp.raise_for_status()
    return resp.content

audio = generate_track("cinematic orchestral, epic brass, taiko drums, 90bpm",
                        "[intro]\n(instrumental)\n[verse]\nRise from the ashes")
Path("output.wav").write_bytes(audio)
```

## LEGO Pipeline — Layered Generation
Build tracks stem-by-stem (drums→bass→guitar→synth→vocals), each conditioned on the previous mix.

```python
import subprocess, requests
from pathlib import Path

def generate_layer(tags, lyrics, condition_audio=None):
    """Generate one stem, optionally conditioned on a prior mix."""
    payload = {"tags": tags, "lyrics": lyrics, "duration": 120, "infer_steps": 60}
    files = {}
    if condition_audio and condition_audio.exists():
        files["condition_audio"] = condition_audio.open("rb")
        payload["repaint_strength"] = 0.35
    r = requests.post("http://localhost:8001/generate", data=payload, files=files, timeout=600)
    r.raise_for_status()
    return r.content

def lego_pipeline(out: Path):
    out.mkdir(parents=True, exist_ok=True)
    layers = [("drums", "metal drums, double kick, 160bpm"),
              ("bass", "metal bass, distorted, 160bpm"),
              ("guitar", "8-string djent, polyrhythmic, 160bpm"),
              ("synth", "dark ambient pads, atmospheric, 160bpm"),
              ("vocals", "aggressive vocals, metalcore screams, 160bpm")]
    mix = None
    for name, tags in layers:
        (out / f"{name}.wav").write_bytes(generate_layer(tags, "(instrumental)", mix))
        stems = [s for s in out.glob("*.wav") if s.name != "_mix.wav"]
        mix = out / "_mix.wav"
        ins = " ".join(f'-i "{s}"' for s in stems)
        subprocess.run(f'ffmpeg -y {ins} -filter_complex amix=inputs={len(stems)} "{mix}"',
                       shell=True, capture_output=True)
```

## Covers / Repainting Mode
Style-transfer existing audio by supplying new tags while preserving melody and structure.

```python
def repaint_cover(source: str, new_tags: str, strength: float = 0.6) -> bytes:
    """Repaint a track with new style. strength: 0.0=original, 1.0=fully new."""
    with open(source, "rb") as f:
        r = requests.post("http://localhost:8001/repaint", files={"audio": f},
                          data={"tags": new_tags, "repaint_strength": strength,
                                "infer_steps": 80, "cfg_scale": 4.5}, timeout=600)
    r.raise_for_status()
    return r.content
```

## LoRA Fine-Tuning
Train an adapter on 30+ clips (10–30s each) with `metadata.jsonl` sidecar.

```python
lora_config = {
    "base_model": "ace-step-v1.5", "dataset_dir": "./training_data/my_style/",
    "output_dir": "./lora_adapters/my_style/", "epochs": 40,
    "batch_size": 2, "learning_rate": 1e-4, "lora_rank": 64, "lora_alpha": 128,
    "lora_target_modules": ["q_proj", "v_proj", "k_proj", "o_proj"],
    "gradient_accumulation_steps": 4, "mixed_precision": "bf16",
}
# metadata.jsonl: {"file": "001.wav", "tags": "...", "lyrics": "..."}
```

## GGML/CUDA Inference — acestep.cpp
```bash
cd ~/projects/ACE-Step-1.5/acestep.cpp
cmake -B build -DACESTEP_CUDA=ON -DCMAKE_CUDA_ARCHITECTURES=86
cmake --build build --config Release -j$(nproc)
# Quantize: Q4_K_M (~8GB, drafts) | Q5_K_M (~12GB, prod) | Q8_0 (~18GB) | F16 (~24GB)
./build/bin/acestep-quantize models/f16.gguf models/Q5_K_M.gguf Q5_K_M
# Server with LoRA adapter
./build/bin/acestep-server \
  --model models/Q5_K_M.gguf --lora lora_adapters/my_style/adapter.gguf \
  --port 8001 --ctx-size 4096 --n-gpu-layers 99 --threads 12
```

## Latency & Streaming Preview
```python
def stream_preview(tags: str, lyrics: str, chunk_cb):
    """Stream audio chunks for real-time DAW preview (20 steps for speed)."""
    with requests.post("http://localhost:8001/generate", json={
        "tags": tags, "lyrics": lyrics, "duration": 30,
        "infer_steps": 20, "stream": True,
    }, stream=True, timeout=120) as resp:
        for chunk in resp.iter_content(chunk_size=48000 * 2 * 2):
            if chunk:
                chunk_cb(chunk)
```

## DAW Integration
```bash
# Render tempo-locked stems at 48kHz for Ableton import
for stem in drums bass guitar vocals; do
  curl -s -X POST http://localhost:8001/generate \
    -H "Content-Type: application/json" \
    -d "{\"tags\":\"$stem, progressive metal, 140bpm\",\"duration\":180,\"sample_rate\":48000}" \
    -o "/mnt/c/Users/autom/Music/Ableton/User Library/Samples/ai_${stem}.wav"
done
```

## Anti-Patterns
- **Vague tags** — `"cool music"` produces noise. Specify genre, instruments, mood, and BPM.
- **Single-pass full mix** — Loses stem separation. Use LEGO layered pipeline instead.
- **LoRA over-training** — Beyond 60 epochs on small datasets causes mode collapse.
- **F16 for drafts** — Burns 24GB VRAM, 4× slower. Use Q4_K_M for iteration, Q5_K_M for finals.
- **Random seeds** — Makes A/B comparison impossible. Pin seeds during prompt refinement.
- **Sample rate mismatch** — 44.1kHz into 48kHz session causes pitch drift. Match the DAW.
- **No streaming** — Full renders kill creative flow. Use `stream: true` with 20 steps.

## Checklist
- [ ] ACE-Step server on `:8001` with `--n-gpu-layers 99`
- [ ] Model quantized to Q5_K_M (Q4_K_M for drafts)
- [ ] Tags specify genre, instruments, mood, and explicit BPM
- [ ] LEGO order: drums → bass → guitar → synth → vocals
- [ ] LoRA: 30+ clips, rank 64, ≤40 epochs, bf16 precision
- [ ] Output sample rate matches DAW session (48kHz default)
- [ ] Streaming enabled for previews (`infer_steps: 20`)
- [ ] Seeds pinned during prompt iteration
- [ ] Stems exported to DAW import directory
