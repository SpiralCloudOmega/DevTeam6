# REVITHION STUDIO — Research Findings
## Session 2026-03-05 — Deep Research & Discovery Phase

---

## Executive Summary

This session conducted comprehensive deep research across all key technologies for REVITHION STUDIO. **Major discovery**: multiple critical components already exist in our projects directory, significantly accelerating the development timeline.

**Key Discoveries:**
1. `acestep.cpp` — Complete C++17 GGML implementation of ACE-Step 1.5 (ALREADY BUILT with CUDA)
2. `ACE-Step-DAW` — Working browser DAW with LEGO pipeline (reference implementation)
3. `OpenDAW Architecture Study` — 797-line analysis already completed
4. ACE-STEP API — 36 endpoints, fully operational on port 8001
5. Complete research on RTNeural, Faust+JUCE, and RLM architecture

---

## 1. ACE-STEP 1.5 — Complete Technical Analysis

### 1.1 API Server (Running)
- **URL**: http://localhost:8001
- **Framework**: FastAPI with Swagger UI at /docs
- **PID**: 91483 (running since March 1)
- **GPU**: RTX 3090, ~15.8GB VRAM active

#### API Endpoints (36 total)
| Category | Endpoints | Key Routes |
|----------|-----------|------------|
| Core | 4 | /health, /v1/init, /v1/reinitialize, /v1/stats |
| Models | 3 | /v1/models, /v1/model_inventory, /v1/chat/completions |
| Audio | 1 | /v1/audio |
| Dataset | 11 | load, save, scan, samples, preprocess (sync+async), auto_label (sync+async) |
| LoRA | 5 | load, unload, toggle, scale, status |
| Training | 6 | start, start_lokr, stop, status, export, load_tensor_info |
| Utility | 4 | create_random_sample, format_input, query_result, release_task |

#### Three-Layer System
1. **Frontend** (React/TypeScript) — Spotify-like UI on port 3000
2. **Backend** (Express/Node.js) — API layer on port 3001
3. **ACE-Step Engine** (Gradio/Python) — ML inference on port 8001

#### Generation Parameters (50+ Gradio args)
- **Core**: caption (prompt), lyrics, BPM, key, time signature, language
- **DiT**: inference_steps (6-15), guidance_scale (7.0), seed, batch_size (1-4)
- **Reference/Cover**: referenceAudioUrl, sourceAudioUrl, taskType (text2music, music2music, cover)
- **Repainting**: repaintingStart, repaintingEnd, instruction, audioCoverStrength
- **LLM Features**: thinking (CoT), enhance, lmTemperature (0.85), lmCfgScale (2.0), negative prompt
- **Advanced**: shift, inference method, custom timesteps, audio format (MP3/FLAC/WAV/OGG/Opus/M4A)

#### LoRA Training Pipeline
```
Upload audio → Scan directory → Auto-label (via Gradio) → Build dataset →
Preprocess (tensors) → Load tensors → Start training → Export weights
```
Training params: rank 64, alpha 128, dropout 0.1, lr 0.0003, epochs 1000, save every 200

### 1.2 acestep.cpp — C++ GGML Implementation

**Location**: `/home/autom/projects/ace-step/acestep.cpp/`
**Status**: ✅ BUILT with CUDA (March 5, 2026)
**Language**: C++17, CMake 3.14+
**Backend**: GGML (submodule) with CUDA/Metal/Vulkan support
**CUDA Architectures**: 75 (Turing), 80 (Ampere), 86 (GA102), 89 (Ada), 120a-121a (Blackwell)

#### Source Architecture (6,000 lines across 19 files)
| File | Lines | Purpose |
|------|-------|---------|
| qwen3-lm.h | 702 | Qwen3 causal LM (4B params) — generates lyrics + audio codes via CoT |
| dit-graph.h | 586 | DiT GGML computation graph construction |
| bpe.h | 553 | BPE tokenizer (bundled in GGUF, no external files) |
| qwen3-enc.h | 474 | Qwen3 text encoder (0.6B) for conditioning |
| dit-sampler.h | 454 | Euler/DPM denoising scheduler |
| dit.h | 398 | DiT model definition (24L, H=2048) |
| vae.h | 558 | AutoencoderOobleck decoder → 48kHz stereo WAV |
| cond-enc.h | 350 | Conditional encoder (text + metadata → DiT input) |
| gguf-weights.h | 343 | GGUF weight loader |
| metadata-fsm.h | 332 | Finite state machine for structured metadata parsing |
| request.cpp | 301 | Request JSON handling, output WAV writing |
| prompt.h | 289 | Prompt template construction |
| fsq-detok.h | 225 | Finite Scalar Quantization detokenizer |
| philox.h | 121 | Philox 4x32-10 PRNG for reproducible sampling |
| request.h | 56 | Request data structures |
| backend.h | 49 | GGML backend abstraction |
| weight-ctx.h | 74 | Weight context management |
| timer.h | 14 | Performance timing |
| debug.h | 115 | Debug utilities |

#### Build Output
- `ace-qwen3` (212 KB) — LLM: request.json → enriched codes + lyrics
- `dit-vae` (197 KB) — DiT+VAE: codes → 48kHz stereo WAV
- `quantize` (27 KB) — GGUF requantizer (BF16 → K-quants)

#### GGUF Models (from HuggingFace: Serveurperso/ACE-Step-1.5-GGUF)
| Model | Architecture | Size (Q8_0) |
|-------|-------------|-------------|
| Qwen3-Embedding-0.6B | Text encoder (28L, H=1024) | 748 MB |
| acestep-5Hz-lm-4B | Qwen3 causal LM | 4.2 GB |
| acestep-v15-turbo | DiT + CondEncoder (24L, H=2048) | 2.4 GB |
| vae | AutoencoderOobleck | 322 MB (BF16) |

**Total**: ~7.7 GB for Q8_0 turbo. LM sizes: 0.6B (fast), 1.7B (balanced), 4B (best quality).

#### Quick Start
```bash
# LLM generates lyrics + audio codes
./build/ace-qwen3 --request request.json --model models/acestep-5Hz-lm-4B-Q8_0.gguf

# DiT+VAE synthesizes audio from codes
./build/dit-vae --request request0.json --model-dit models/acestep-v15-turbo-Q8_0.gguf \
    --model-vae models/vae-BF16.gguf --model-enc models/Qwen3-Embedding-0.6B-Q8_0.gguf
```

### 1.3 ACE-Step-DAW — LEGO Pipeline Reference

**Location**: `/home/autom/projects/ace-step/ACE-Step-DAW/`
**Tech**: Vite + TypeScript + Zustand
**API**: Proxies to ACE-Step backend on port 8001

#### Generation Pipeline (Bottom-Up)
1. **Drums** — generated on silence
2. **Bass** — receives drums mix
3. **Guitar** — receives drums+bass
4. **Synth** — receives drums+bass+guitar
5. **Vocals** — receives full instrumental mix

Track isolation via wave subtraction: `newTrack = currentMix - previousMix`

#### Musical Controls (per-clip, three tiers)
| Mode | BPM/Key/TimeSig Behavior |
|------|--------------------------|
| Auto | ACE-Step infers from audio context |
| Project | Uses project-level setting |
| Manual | Explicit per-clip override |

#### Features
- 13 instrument types, clip-based generation
- Sample mode (description-only prompts)
- Model selection via Settings → ACE-Step API
- Export to stereo WAV
- Keyboard: Space = play/pause, Ctrl+Scroll = zoom

#### Project Structure
```
src/
  components/ (dialogs, generation, layout, timeline, tracks, transport)
  constants/ (defaults, track definitions, key scales)
  engine/    (AudioEngine, TrackNode, wave subtraction, export)
  hooks/     (useAudioEngine, useGeneration, useTransport)
  services/  (API client, generation pipeline, audio storage)
  store/     (Zustand: project, transport, UI, generation)
  types/     (TypeScript interfaces)
  utils/     (WAV encoding, waveform peaks, color, time)
```

### 1.4 ACE-STEP Ecosystem
| Project | Location | Description |
|---------|----------|-------------|
| ACE-Step-1.5 | ~/projects/ACE-Step-1.5/ | Main Python codebase (Gradio + API) |
| ace-step-ui | ~/projects/ace-step-ui/ | Spotify-like UI (React + Express + Python) |
| acestep.cpp | ~/projects/ace-step/acestep.cpp/ | C++ GGML port (BUILT) |
| ACE-Step-DAW | ~/projects/ace-step/ACE-Step-DAW/ | Browser DAW with LEGO pipeline |
| ace-step-skills | ~/projects/ace-step/ace-step-skills/ | AI agent skills for Claude/Codex |
| awesome-ace-step | ~/projects/ace-step/awesome-ace-step/ | Curated resource list (30+ projects) |
| ACE-Step-ComfyUI | ~/projects/ace-step/ACE-Step-ComfyUI/ | ComfyUI integration |

---

## 2. OpenDAW — Architecture Deep Dive

### 2.1 Monorepo Structure
**Location**: `/home/autom/projects/openDAW/` and `/mnt/c/Users/autom/VS Code GitHub Repos/DevTeam6/openDAW/`
**Stats**: ~1,751 TypeScript files, ~113,864 LOC, 22 packages
**Build**: Turborepo + Vite + esbuild
**License**: AGPL v3 (dual-licensed with commercial option)

### 2.2 BoxGraph ECS — The Architectural Core

**Box = Entity**
- UUID-addressed, typed, contains Fields
- ~70 auto-generated types via ts-morph code generation
- Examples: AudioUnitBox, VaporisateurDeviceBox, TrackBox, NoteEventBox, etc.

**Field = Component**
- Types: PrimitiveField<V> (float, int, string, bool, bytes), PointerField, ArrayField, ObjectField
- Every PrimitiveField is ObservableValue<V> — changes propagate via subscription
- PointerField creates directed graph edges with mandatory/exclusive constraints

**BoxGraph = World**
- Transaction-based mutations: beginTransaction() / endTransaction()
- Binary serialization via custom DataInput/DataOutput (NOT JSON/protobuf)
- Dispatchers<FieldUpdate> for change notification
- Pointer management with automatic cleanup

### 2.3 Three-Thread Audio Model
```
Main Thread          AudioWorklet Thread       Worker Thread
├─ EngineFacade      ├─ EngineProcessor        ├─ OpfsWorker
├─ Project           ├─ BoxGraph (mirror)      ├─ SamplePeakWorker
├─ BoxGraph (source) ├─ AudioUnit[]            └─ TransientDetect
├─ SampleManager     ├─ BlockRenderer
├─ LiveStreamRcvr    ├─ Mixer, Metronome
└─ UI (custom JSX)   └─ Graph<Processor> + TopologicalSort
```

Communication:
- **Communicator RPC** (MessagePort) — type-safe request-response and fire-and-forget
- **SyncSource/SyncTarget** — one-way BoxGraph state sync (binary encoded)
- **SyncStream** (SharedArrayBuffer) — lock-free engine state → main thread
- **LiveStream** (SharedArrayBuffer) — peak meters, spectrum, waveforms

### 2.4 Processing Pipeline (per 128-sample RenderQuantum)
1. Notify ProcessPhase.Before
2. TopologicalSort audio graph (if dirty)
3. BlockRenderer splits quantum into sub-blocks (tempo changes, loops, markers)
4. For each block: process all Processors in topological order
5. Copy primary AudioUnit output
6. Mix metronome
7. Compute peaks + spectrum
8. Notify ProcessPhase.After
9. Flush clip sequencing notifications
10. Write state to SyncStream + LiveStream

### 2.5 Three-Layer Device Pattern
```
Box (Data Schema, auto-generated) → Adapter (Domain Logic, both threads) → Processor (DSP, audio thread only)
```

#### Instruments (7)
- Vaporisateur (subtractive synth), Playfield (drum machine), Nano (sampler)
- Tape (audio playback), Soundfont (SF2 player), MIDI Output, Audio Bus

#### Effects (12)
- Stereo Tool, Delay, Crusher, Cheap Reverb, Dattorro Reverb, Revamp (EQ)
- Compressor, Gate, Maximizer, Fold, Tidal, Neural Amp

#### MIDI Effects (4)
- Arpeggio, Pitch, Velocity, Zeitgeist

### 2.6 Native Port Analysis (from Architecture Study)
**Recommended: Option A — Full C++ Rewrite**

| What | Web | Native Replacement |
|------|-----|-------------------|
| AudioWorklet | AudioWorkletProcessor | Native audio thread (RtAudio/PortAudio) |
| Web Audio | AudioContext | Direct audio device access |
| SharedArrayBuffer | Cross-thread comms | std::atomic, lock-free queues |
| MessagePort | postMessage() | Message queues, ZeroMQ |
| Workers | new Worker() | std::thread / thread pool |
| JSX UI | lib-jsx → DOM | JUCE GUI / Dear ImGui |
| IndexedDB/OPFS | Storage | Native filesystem I/O |

**Portable (direct translation to C++):**
- lib-dsp (128 files, pure math DSP)
- lib-box (46 files, ECS core)
- Processing model (Block/ProcessInfo/Processor)
- Voicing system, TopologicalSort
- Binary serialization, AutomatableParameter

---

## 3. RTNeural — Neural Amp Modeling Pipeline

### 3.1 Overview
- **Author**: Jatin Chowdhury (CCRMA, Stanford)
- **GitHub**: github.com/jatinchowdhury18/RTNeural
- **License**: BSD-3
- **Language**: C++ header-only library

### 3.2 Supported Layers
Dense, LSTM, GRU, Conv1D, Conv2D, BatchNorm1D, Activation (tanh, relu, sigmoid, softmax, elu, prelu)

### 3.3 Backends
- **Eigen** — General purpose, good performance
- **xsimd** — SIMD-optimized (SSE/AVX/NEON), best performance
- **Accelerate** — Apple platforms only
- **STL** — No dependencies, portable

### 3.4 Workflow
```
1. TRAIN (Python):    PyTorch/TF → LSTM(hidden=32-64, layers=1) → train on clean→amp pairs
2. EXPORT:           model_utils.py → model_weights.json
3. LOAD (C++):       auto model = RTNeural::json_parser::parseJson<double>(stream)
4. PROCESS:          model->forward(input) → output (per-sample, real-time safe)
```

### 3.5 Key Properties
- **Real-time safe**: No allocations on audio thread
- **Per-sample processing**: forward() called for each audio sample
- **Template-based**: Compile-time model architecture for zero overhead
- **Hidden state management**: LSTM/GRU state persists across processBlock calls

### 3.6 Training Data Requirements
- 1-15 minutes of varied guitar playing through physical amp
- Clean signal (DI) + processed signal (amp output) paired
- Sequence length: 0.5s (22,050 samples at 44.1kHz)
- Batch size: 50 sequences per update

### 3.7 For REVITHION
- Replace libtorch with RTNeural for inference (smaller, faster, real-time safe)
- Train from Yeks catalog: 103+ amp models
- Architecture: LSTM(input=1, hidden=32-64, layers=1) + Dense output
- Tone control: add as second input channel to LSTM
- Integrate as JUCE AudioProcessor subclass

---

## 4. Faust → JUCE Integration

### 4.1 Overview
- **Faust**: Functional Audio Stream language (domain-specific for DSP)
- **faust2juce**: Compiler tool that generates JUCE AudioProcessor from .dsp file
- **Output**: .jucer project file ready for Projucer → IDE → build

### 4.2 Workflow
```bash
# Write DSP in Faust
echo 'import("stdfaust.lib"); process = fi.lowpass(1, hslider("cutoff", 1000, 20, 20000, 1));' > filter.dsp

# Compile to JUCE project
faust2juce filter.dsp

# Open in Projucer, export to IDE, build
# Output: VST3/AU/AAX/Standalone plugin
```

### 4.3 Key Flags
- `-scn base_dsp` — Rename base class to avoid juce::dsp namespace clash
- `-standalone` — Generate standalone app instead of plugin
- Standard Faust UI elements (hslider, nentry, button) → host-automatable parameters

### 4.4 For REVITHION
- Use Faust for rapid DSP prototyping: EQ, compressor, delay, reverb, phaser, vocoder
- faust2juce generates initial JUCE code, then customize LookAndFeel
- Can combine with Plugin GUI Magic for advanced UIs
- All 36 routing grid block types can be prototyped in Faust first

---

## 5. RLM — Recursive Language Models (arXiv:2512.24601)

### 5.1 Core Concept
Prompt = external environment variable (not direct model input)
LLM generates code to manipulate/decompose the prompt recursively

### 5.2 LISP Macro Analogy
| LISP | RLM |
|------|-----|
| Source code | Prompt (data) |
| Macro interpreter | LLM |
| Macro expansion | Recursive llm_query() calls |
| Evaluated result | Final answer |

### 5.3 Key Primitive
`llm_query(prompt_snippet)` — recursive self-invocation on sub-segment

### 5.4 Properties
- Handles 10M+ token tasks with minimal context rot
- Out-of-core processing (data doesn't fit in context window)
- Constant-size history (no unbounded growth)
- Comparable or lower cost than RAG/summarization

### 5.5 For REVITHION Build System
```
Design Tokens (S-expr) → RLM recursive expansion → constexpr C++ generation

Example:
(define-palette "dark-steel"
  :bg-primary    (color 0x1A 0x1A 0x2E)
  :accent        (color 0x00 0xD4 0xFF)
  :knob-body     (gradient radial :center (0.4 0.3)
                   (stop 0.0 (lighten :bg-primary 0.15))
                   (stop 1.0 (darken :bg-primary 0.05))))

↓ RLM expansion (recursive macro processing) ↓

constexpr Color kBgPrimary{0x1A, 0x1A, 0x2E, 0xFF};
constexpr Color kAccent{0x00, 0xD4, 0xFF, 0xFF};
constexpr GradientDef kKnobBody{GradientType::Radial, {0.4f, 0.3f}, ...};
```

### 5.6 Implementations
- github.com/alexzhang13/rlm-minimal — Minimal implementation
- github.com/fullstackwebdev/rlm_repl — REPL implementation

---

## 6. Nuendo 14 Feature Matrix (from extraction doc)

### 6.1 Track Types (14 total, Priority-rated)
| Track Type | Priority | Description |
|------------|----------|-------------|
| Audio | Critical | Mono/stereo/multichannel audio recording + editing |
| MIDI | Critical | MIDI event sequencing |
| Instrument | Critical | VSTi hosting with MIDI input |
| Group | Critical | Bus/subgroup routing |
| FX | Critical | Send effect returns |
| VCA Fader | Critical | Voltage-controlled amplifier grouping |
| Sampler | Useful | Quick sample playback |
| Drum | Useful | Drum map editing |
| Folder | Useful | Organization |
| Marker | Useful | Timeline markers |
| Tempo | Useful | Tempo automation |
| Signature | Useful | Time signature changes |
| Chord | Later | Chord progression tracking |
| Video | Later | Video playback sync |

### 6.2 MixConsole Features
- 8 sends per channel (pre/post-fader)
- 4-band parametric EQ per channel
- Channel strip: Gate → Compressor → EQ → Saturation → Limiter
- Direct Offline Processing (non-destructive effect chains)
- 64-bit float internal processing
- 44.1-192 kHz sample rate support

### 6.3 Automation Modes
1. Read — playback only
2. Touch — writes while touching, returns to previous
3. Auto-Latch — writes while touching, holds last value
4. Cross-Over — advanced mode with pass-through
5. Write — overwrites everything
6. Trim — offset adjustment on existing curves

---

## 7. Neural Amp Catalog (from Book Extractions)

### 7.1 Tier 1 — Priority Models (10)
Must-have amp models for initial release:
1. MESA Mark IIC+ (high gain legend)
2. Soldano SLO-100 (hot-rodded clean to high gain)
3. Marshall Plexi 1959SLP (classic British crunch)
4. Fender Twin Reverb (clean American standard)
5. Fender '59 Bassman (tweed warmth)
6. Vox AC30 (Class A jangle)
7. Marshall JCM800 (80s metal/rock)
8. EVH 5150-III (modern high gain)
9. MESA Dual Rectifier (modern metal)
10. Bogner Ecstasy (boutique versatility)

### 7.2 Training Architecture
- LSTM: input_size=1, hidden_size=32-64, num_layers=1
- Training data: 1-15 min varied guitar playing through real amp
- Export: TorchScript (.pt) or RTNeural JSON
- Inference: RTNeural forward() per sample at 48kHz

### 7.3 Full Catalog (103+ from Yeks Guide)
Categories: Fender (15+), Marshall (12+), Vox/Class-A (8+), High-Gain (20+), 
Boutique (15+), Bass (8+), Acoustic (3+), Misc (20+)

---

## 8. Existing Reference Documents

| Document | Location | Lines | Content |
|----------|----------|-------|---------|
| REVITHION-STUDIO-MASTER-PLAN.md | DevTeam6/ | 762 | Complete 10-phase development plan |
| OPENDAW-ARCHITECTURE-STUDY.md | REVITHION-STUDIO/docs/ | 797 | Deep OpenDAW architecture analysis |
| nuendo-14-feature-extraction.md | REVITHION-STUDIO/docs/ | 753 | Nuendo 14 feature matrix with priorities |
| REVITHION-Book-Extractions.md | REVITHION-STUDIO/docs/ | 805 | Neural amp modeling + JUCE plugin reference |
| NVIDIA_CUDA_X_MANIFEST.md | REVITHION-STUDIO/Local_C_Drive/ | 285 | All installed NVIDIA libraries |

---

## 9. Revised Development Strategy

### What Changed
The discovery of `acestep.cpp`, `ACE-Step-DAW`, and the existing architecture study dramatically accelerates the timeline:

1. **acestep-bridge todo → integrate existing acestep.cpp** (weeks → days)
2. **LEGO pipeline → port from ACE-Step-DAW** (design → reference implementation exists)
3. **OpenDAW study → already done** (saved weeks of analysis)
4. **Models → download GGUF files** (no training needed for music gen)

### Revised Phase 1 Priority
1. ✅ Build acestep.cpp with CUDA — DONE
2. Download GGUF models (~7.7GB) — IN PROGRESS
3. Test acestep.cpp end-to-end generation
4. Set up CMake super-project with JUCE 8 + acestep.cpp as subproject
5. Create JUCE standalone app with ACE-STEP generate button
6. Port LEGO pipeline logic from ACE-Step-DAW TypeScript → C++

### Key Architecture Decision
Use **acestep.cpp** (GGML) for offline/batch generation AND keep the Python API server for real-time interactive generation. The JUCE app talks to both:
- GGML for fast turbo generation (no Python overhead)
- Python API for LoRA training, dataset management, advanced features

---

*Research conducted: 2026-03-05*
*All findings documented verbatim. No chunking. No summarizing.*
*Session: 06ef625c-da1c-4046-b763-7a3dfcb9ae91*
