# REVITHION STUDIO — Master Implementation Plan
## ALL-IN-ONE Professional Audio/Video Production Suite

> Reengineering Pro Tools + Ableton 12 + AXE-FX III + iZotope + Maxon Video Ultimate
> Built on OpenDAW + ACE-Step foundations | Native C++/CMake/Python/NumPy → EXE/ISO
> Target: WSL2 Ubuntu | Ryzen 9 5950X / 64GB / RTX 3090 24GB / Sound Blaster AE-9

---

## 1. VISION

REVITHION STUDIO is a native, AI-powered, all-in-one production suite combining:

| Reference Product | What We Take | Implementation |
|---|---|---|
| **Avid Pro Tools** | Industry-standard recording, editing, mixing, I/O routing | OpenDAW BoxGraph ECS → C++ native engine |
| **Ableton Live 12** | Session/Arrangement views, clip launching, Wavetable/Operator synths, Max for Live | Dual-view architecture, modular synth engine |
| **Fractal AXE-FX III** | 12×4 grid routing, 30+ block types, scenes/channels, neural amp modeling | Custom routing grid with RTNeural/NAM blocks |
| **iZotope Ozone/RX** | Intelligent mastering chain, spectral repair, stem processing | AI mastering pipeline with Kimi-Audio analysis |
| **Maxon (Cinema 4D)** | 3D visualization, video timeline, motion graphics | Video track + 3D audio spatialization |
| **Nuendo 14** | 319 features: 64-bit engine, MixConsole, MIDI, automation, surround, Control Room | Feature-parity target (220 critical features) |

### Architecture Decision: NATIVE FIRST

```
NOT THIS:  Browser → WebAudio → AudioWorklet (OpenDAW current)
THIS:      CMake → C++17/20 → JUCE 8 → VST3/AU/LV2/Standalone EXE
           Python/NumPy for AI inference pipeline
           acestep.cpp (GGML/CUDA) for music generation
           RTNeural (Eigen/xsimd) for real-time neural DSP
```

OpenDAW's TypeScript codebase (114K LoC, 22 packages) serves as the **architecture blueprint** —
its BoxGraph ECS, topological-sort audio graph, and 3-layer device pattern
(Box→Adapter→Processor) map directly to C++ structs → wrapper classes → audio processors.

`lib-dsp` (pure math DSP: ADSR, biquad, FFT, oscillators, delay, reverb) is **100% portable** —
zero web dependencies, direct C++ translation.

---

## 2. ARCHITECTURE

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                          REVITHION STUDIO v1.0                              │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ┌─────────────────────────────────────────────────────────────────────┐    │
│  │                    PRESENTATION LAYER                               │    │
│  │  JUCE 8 LookAndFeel (procedural design tokens → constexpr C++)     │    │
│  │  ┌──────────┐ ┌────────────┐ ┌──────────┐ ┌────────────────────┐   │    │
│  │  │Arrange   │ │Session/Clip│ │MixConsole│ │Routing Grid Editor │   │    │
│  │  │Timeline  │ │Launch View │ │(Nuendo)  │ │(AXE-FX 12×4)      │   │    │
│  │  └──────────┘ └────────────┘ └──────────┘ └────────────────────┘   │    │
│  │  ┌──────────┐ ┌────────────┐ ┌──────────┐ ┌────────────────────┐   │    │
│  │  │MIDI Key/ │ │AI Copilot  │ │Control   │ │Video Timeline      │   │    │
│  │  │Drum/List │ │(CopilotKit)│ │Room      │ │(Maxon-style)       │   │    │
│  │  └──────────┘ └────────────┘ └──────────┘ └────────────────────┘   │    │
│  └─────────────────────────────────────────────────────────────────────┘    │
│                                                                             │
│  ┌─────────────────────────────────────────────────────────────────────┐    │
│  │                    AI INTEGRATION LAYER                              │    │
│  │  ┌──────────────┐  ┌──────────────┐  ┌───────────────────────┐     │    │
│  │  │ ACE-Step 1.5 │  │ Kimi-Audio   │  │ CopilotKit + Ollama   │     │    │
│  │  │ acestep.cpp  │  │ 7B (4-bit)   │  │ qwen3-coder:30b       │     │    │
│  │  │ GGML/CUDA    │  │ analysis     │  │ conversational copilot │     │    │
│  │  │ 7.7GB VRAM   │  │ 4GB VRAM     │  │ production assistant  │     │    │
│  │  └──────────────┘  └──────────────┘  └───────────────────────┘     │    │
│  └─────────────────────────────────────────────────────────────────────┘    │
│                                                                             │
│  ┌─────────────────────────────────────────────────────────────────────┐    │
│  │                    CORE ENGINE (C++17, real-time safe)               │    │
│  │                                                                     │    │
│  │  ┌─────────────┐  ┌──────────────┐  ┌────────────────────────┐     │    │
│  │  │ BoxGraph    │  │ Audio Graph  │  │ MIDI Engine            │     │    │
│  │  │ ECS (from   │  │ Topological  │  │ Expression Maps,       │     │    │
│  │  │ OpenDAW)    │  │ Sort, 128-   │  │ Note Expression,       │     │    │
│  │  │ Binary      │  │ sample blocks│  │ Drum Maps              │     │    │
│  │  │ serialized  │  │ 48-192kHz    │  │                        │     │    │
│  │  └─────────────┘  └──────────────┘  └────────────────────────┘     │    │
│  │                                                                     │    │
│  │  ┌─────────────────────────────────────────────────────────────┐   │    │
│  │  │              SIGNAL ROUTING GRID (AXE-FX III)               │   │    │
│  │  │  12 columns × 4 rows, expandable                            │   │    │
│  │  │  30+ block types: AMP/CAB/DELAY/REVERB/COMP/EQ/DRIVE/...   │   │    │
│  │  │  + AI blocks: GENERATE/MASTER/STEM-SPLIT/ANALYZE/NEURAL    │   │    │
│  │  │  8 Scenes × 4 Channels per block                            │   │    │
│  │  └─────────────────────────────────────────────────────────────┘   │    │
│  │                                                                     │    │
│  │  ┌─────────────┐  ┌──────────────┐  ┌────────────────────────┐     │    │
│  │  │ MixConsole  │  │ Automation   │  │ Mastering Suite        │     │    │
│  │  │ 16 inserts  │  │ 6 modes      │  │ (iZotope equivalent)   │     │    │
│  │  │ 8 sends     │  │ Touch/Auto/  │  │ Stem EQ, MB Comp,      │     │    │
│  │  │ Channel     │  │ Latch/Write/ │  │ Stereo Imager, Exciter │     │    │
│  │  │ Strip (gate │  │ Trim/Cross   │  │ Maximizer, LUFS meter  │     │    │
│  │  │ +3comp+EQ+  │  │ Over         │  │ Tonal Balance Control  │     │    │
│  │  │ sat+limiter)│  │              │  │                        │     │    │
│  │  └─────────────┘  └──────────────┘  └────────────────────────┘     │    │
│  └─────────────────────────────────────────────────────────────────────┘    │
│                                                                             │
│  ┌─────────────────────────────────────────────────────────────────────┐    │
│  │                    DSP LAYER                                        │    │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────────────┐      │    │
│  │  │ RTNeural     │  │ Faust DSP    │  │ Classic DSP          │      │    │
│  │  │ LSTM/GRU amp │  │ Rapid proto  │  │ from OpenDAW lib-dsp │      │    │
│  │  │ NAM models   │  │ faust2juce   │  │ ADSR, biquad, FFT,   │      │    │
│  │  │ Neural FX    │  │ faust2cpp    │  │ osc, delay, reverb   │      │    │
│  │  └──────────────┘  └──────────────┘  └──────────────────────┘      │    │
│  └─────────────────────────────────────────────────────────────────────┘    │
│                                                                             │
│  ┌─────────────────────────────────────────────────────────────────────┐    │
│  │                    BUILD SYSTEM                                      │    │
│  │  CMake 3.28 + JUCE 8 + RTNeural + GGML + libtorch + Faust          │    │
│  │  Procedural Design Tokens: S-expr → LISP macros → constexpr C++    │    │
│  │  GitNexus: Tree-sitter AST → KuzuDB knowledge graph (no chunking)  │    │
│  │  LLM-Modulo: Planner(ToT) → Executor(ReAct) → Verifier(compiler)  │    │
│  └─────────────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## 3. TECHNOLOGY STACK

### 3.1 Native Core (C++17/20)

| Component | Technology | Source |
|---|---|---|
| Framework | JUCE 8.0.12 | ~/OpenAudio/JUCE/ |
| Audio Engine | Custom (ported from OpenDAW patterns) | BoxGraph ECS + TopologicalSort |
| Neural Inference | RTNeural (Eigen/xsimd) | ~/OpenAudio/RTNeural/ |
| Amp Modeling | Neural Amp Modeler | ~/OpenAudio/neural-amp-modeler-lv2/ |
| DSP Prototyping | Faust → faust2juce/faust2cpp | ~/OpenAudio/faust/ |
| ML Runtime | libtorch (PyTorch C++) | Training + heavy inference |
| Build | CMake 3.28 + Ninja | System installed |
| Compiler | GCC 11.5 / Clang | System installed |
| GPU | CUDA 13.1 | RTX 3090 24GB |

### 3.2 AI Pipeline (Python + C++)

| Component | Technology | VRAM |
|---|---|---|
| Music Generation | acestep.cpp (GGML/CUDA) | 7.7GB (Q8_0) |
| Audio Analysis | Kimi-Audio 7B (4-bit) | 4GB |
| LLM Backend | Ollama (qwen3-coder:30b) | 12GB (shared) |
| AI Copilot | CopilotKit v1.50 | CPU |
| Stem Separation | Demucs/HTDemucs | 2GB |
| Total concurrent | ACE-Step + Kimi + RTNeural | ~12GB fits in 24GB |

### 3.3 ACE-Step v1.5 Architecture (from research)

```
Phase 1: Qwen3 LM (0.6B/1.7B/4B) → CoT metadata + FSQ audio codes (5Hz)
Phase 2: DiT (24L, H=2048, 16 heads) + Conditioning (text+lyric+timbre encoders)
         → Euler flow matching (8 steps) → denoised latents
Phase 3: VAE Decoder (AutoencoderOobleck, 5 upsample blocks, 1920× stride)
         → 48kHz stereo WAV

C++ integration: ace-qwen3 + dit-vae binaries, JSON in / WAV out
LEGO pipeline: Drums→Bass→Guitar→Synth→Vocals (cumulative context + wave subtraction)
Performance: 30s song in <10s on RTX 3090 (turbo, 8 steps)
```

### 3.4 Procedural Design Token System

```
S-expression tokens (design-tokens.sexp)
    ↓ cmacro-style pattern→template expansion
    ↓ LISP macros read env vars: REVITHION_COLOR_MODE, REVITHION_BASE_HUE, etc.
    ↓ Generate constexpr C++ headers at CMake build time (Jinja2 templates)
    ↓ RAGGeneratedLookAndFeel : LookAndFeel_V4
    ↓ Hot-reload via FileChangeWatcher (dev) / zero-overhead constexpr (prod)

Color: 8-layer Oklab/OKLCH system (parameter-semantic: freq=hue, amp=sat, mod=purple)
Typography: Harmonic ratio scales (1.333/1.618/1.125), 14px base, tabular numerals
Spacing: Fibonacci × base unit, 3 density levels
Export: constexpr C++, CSS custom properties, Figma JSON, HISE themes
```

### 3.5 Build Orchestration (LLM-Modulo)

```
┌──────────────────┐     ┌──────────────────┐     ┌──────────────────┐
│  PLANNER          │────▶│  EXECUTOR         │────▶│  VERIFIER         │
│  Tree-of-Thoughts │     │  ReAct loop       │     │  CMake compile    │
│  Least-to-Most    │     │  Sub-agent swarm  │     │  CTest suite      │
│  Task decompose   │     │  (MACI pattern)   │     │  Static analysis  │
│                    │     │                    │     │  Audio latency    │
│  If fail:          │     │  Agents:           │     │  benchmark        │
│  PlaSma re-plan   │     │  - Audio Engine    │     │                    │
│  with procedural   │     │  - UI/UX           │     │  If fail:          │
│  knowledge         │     │  - State Mgmt      │     │  → back to PLANNER│
│                    │     │  - Testing          │     │  with error context│
└──────────────────┘     │  - Integration      │     └──────────────────┘
                          └──────────────────┘
```

---

## 4. AXE-FX III ROUTING GRID

### 4.1 Grid Architecture
- 12 columns × 4 rows (expandable)
- Each cell = one processing block or empty
- Left-to-right flow with splits (1→N) and merges (N→1)
- 8 Scenes (snapshots of block bypass/channel states)
- 4 Channels per block (instant parameter preset switching)

### 4.2 Block Types (36 total)

**Signal Flow (5):** INPUT, OUTPUT, MIXER, SEND/RETURN, FEEDBACK SEND/RETURN

**Amp & Cabinet (3):** AMP (RTNeural LSTM), CAB (IR + DynaCab neural), TONE MATCH

**Dynamics (4):** COMPRESSOR (multi-style), GATE/EXPANDER, LIMITER (IRC-5), ENHANCER

**Time-Based (4):** DELAY (8 modes), REVERB (7 algorithms), MEGATAP, LOOPER

**Modulation (5):** CHORUS, FLANGER, PHASER, TREMOLO, ROTARY (Leslie)

**Pitch (5):** PITCH (harmonizer), VOCODER, RING MOD, RESONATOR, FORMANT

**EQ & Filter (4):** EQ (parametric/graphic/dynamic), FILTER (LP/HP/BP/notch), CROSSOVER, TILT EQ

**Distortion (4):** DRIVE, WAVESHAPER (neural), BITCRUSHER, FOLD

**AI Blocks (6):** AI GENERATE (ACE-Step), AI MASTER, AI STEM SPLIT, AI ANALYZE (Kimi-Audio), NEURAL EFFECT, STYLE TRANSFER

### 4.3 Neural Amp Modeling — Training Targets (from Yeks Guide)

**Tier 1 — Must Have (10 models):**
MESA Boogie IIC+, Soldano SLO-100, Marshall Plexi, Fender Twin Reverb,
Fender Deluxe Reverb, Marshall JCM800, EVH 5150-III, MESA Rectifier,
VOX AC30, Dumble Overdrive Special

**Tier 2 — Important (15 models):**
Marshall JTM45, Fender Bassman, Orange OR120, Bogner Ecstasy,
Friedman BE-100, Diezel VH4, ENGL Savage, Peavey 6505,
Matchless DC-30, Dr. Z Maz 38, Hiwatt DR103, Sunn Model T,
Supro Thunderbolt, Ampeg SVT, Two-Rock Classic Reverb

**Tier 3 — Extended (78+ models):**
Full Yeks catalog: 103+ unique amp models documented with circuit details

**Training pipeline (from Yee-King book):**
```
1. Capture DI + processed pairs from amp sims (NAM/SmartGuitarAmp)
2. Train: PyTorch → SimpleLSTM + Dense (0.5s seqs, batch=50)
3. Export: TorchScript (.pt) OR RTNeural JSON
4. Deploy: RTNeural ModelT<float,1,1,LSTMLayerT,DenseT> — single-sample forward()
5. RTNeural is 2-3× faster than TorchScript, audio-thread safe (zero alloc)
```

---

## 5. NUENDO-PARITY FEATURE MAP (319 features → 220 critical)

### 5.1 Audio Engine
- Sample rates: 44.1–192kHz per project
- Bit depth: 16/24/32-float/64-float recording
- 64-bit float internal processing
- ASIO driver + ASIO-Guard (pre-processing for non-RT paths)
- Audio Performance Monitor (CPU/disk)
- Track freeze/disable
- Pan law options (0dB, -3dB, -6dB, equal power)

### 5.2 Track Types (16)
Audio, Instrument, Sampler, Drum, MIDI, Group, FX, VCA Fader,
Folder, Marker, Ruler, Tempo, Signature, Arranger, Transpose, Video

### 5.3 Signal Routing
- Input/Output busses (mono/stereo/surround)
- 16 insert slots per channel (pre/post fader split)
- 8 sends per channel (pre/post fader)
- Direct Routing (8 destinations, automatable switching)
- Side-chain routing to any VST3 effect
- External effects/instruments with latency compensation
- Automatic downmixing between channel widths

### 5.4 MixConsole (Nuendo-class)
- Multiple MixConsole windows
- Channel Strip: Gate → Compressor(×3 styles) → EQ → Saturation → Limiter
- 4-band parametric EQ per channel with graphical editor
- Channel visibility agents and linking
- MixConsole snapshots (save/recall entire mixer state)
- Copy/move channel settings via drag-drop
- 8 Quick Controls per track

### 5.5 Effects Processing
- VST3 plugin hosting (and VST2 legacy)
- Direct Offline Processing (non-destructive, unlimited undo)
- 32+ built-in audio processes (envelope, gain, normalize, pitch shift, time stretch, reverse)
- élastique time-stretching (real-time), Free Warp, AudioWarp quantize

### 5.6 MIDI System
- Key Editor, Drum Editor, List Editor, Score Editor
- Expression Maps (articulation switching)
- Note Expression (per-note parameter control, VST3)
- Drum Maps with custom mappings
- MIDI Logical Editor (programmable MIDI transforms)
- Chord Track + Chord Pads

### 5.7 Automation
- 6 modes: Read, Touch, Auto-Latch, Cross-Over, Write, Trim
- Automation Panel with global controls
- Per-parameter curves with node editing
- Automation follows events option

### 5.8 Mastering & Metering
- EBU R128 loudness metering (Integrated, Short-term, Momentary, True Peak)
- Loudness Track (real-time measurement over time)
- Supervision metering plugin (spectrum, phase, loudness)
- Dither on output

### 5.9 Surround / Immersive
- Up to 7.1.6 channel configurations
- Dolby Atmos (via Renderer for Dolby Atmos)
- Ambisonics support
- 3D panner with height channels

### 5.10 Control Room
- Monitor sources and outputs
- Multiple monitor sets (A/B switching)
- 4 Cue mixes for performers
- Talkback with auto-dim
- Reference level calibration

### 5.11 Post-Production (Nuendo Exclusive)
- ADR (Automated Dialogue Replacement) system
- ReConform (re-edit audio to new video cut)
- Game Audio Connect (Wwise/FMOD integration)
- Direct Offline Processing for film workflows

---

## 6. AI MASTERING SUITE (iZotope Ozone Equivalent)

```
MASTERING ASSISTANT (AI — Kimi-Audio analyzes → recommends chain)
  │
  ├─ STEM EQ — AI-powered per-stem equalization (vocals/drums/bass/instruments)
  ├─ DYNAMIC EQ — Frequency-dependent compression (neural model)
  ├─ MULTIBAND COMPRESSOR — 4-band with auto-threshold
  ├─ STEREO IMAGER — Frequency-dependent width, mid/side processing
  ├─ EXCITER / ENHANCER — Tube/tape/transistor harmonic saturation
  ├─ MAXIMIZER / LIMITER — IRC-5 transparent limiting, target LUFS
  └─ TONAL BALANCE CONTROL — Reference-aware frequency analysis, A/B comparison

Meta-Controller ("torchknob" from Yee-King):
  "warm and punchy" → {drive: 0.6, bass: +3dB, comp_ratio: 4:1, ...}
  Train regression model: text description → parameter values
```

---

## 7. BUILD SYSTEM & INFRASTRUCTURE

### 7.1 CMake Super-Project

```cmake
cmake_minimum_required(VERSION 3.28)
project(REVITHION VERSION 1.0.0 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)

# Dependencies
add_subdirectory(external/JUCE)
add_subdirectory(external/RTNeural)
add_subdirectory(external/ggml)          # For acestep.cpp
add_subdirectory(external/Faust)

# Design Token Generation (build-time)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/generated/DesignTokens.h
    COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/generate-tokens.py
    DEPENDS ${CMAKE_SOURCE_DIR}/tokens/design-tokens.sexp
    COMMENT "Generating constexpr design tokens from S-expressions"
)

# Core engine library
add_library(revithion-core STATIC
    src/engine/BoxGraph.cpp
    src/engine/AudioGraph.cpp
    src/engine/RoutingGrid.cpp
    src/engine/MixConsole.cpp
    src/engine/MIDIEngine.cpp
    src/engine/Automation.cpp
    src/dsp/ADSR.cpp src/dsp/Biquad.cpp src/dsp/FFT.cpp
    src/dsp/Oscillator.cpp src/dsp/Delay.cpp src/dsp/Reverb.cpp
    src/neural/NeuralEffect.cpp src/neural/AmpModel.cpp
    src/ai/ACEStepBridge.cpp src/ai/KimiAudioBridge.cpp
)

# Standalone application
juce_add_gui_app(REVITHION
    PRODUCT_NAME "REVITHION STUDIO"
    COMPANY_NAME "SpiralCloudOmega"
)
target_link_libraries(REVITHION PRIVATE
    revithion-core juce::juce_audio_utils juce::juce_dsp
    RTNeural ggml torch
)

# VST3 plugin exports
juce_add_plugin(REVITHION-Amp VST3 AU LV2 ...)
juce_add_plugin(REVITHION-Master VST3 AU LV2 ...)
juce_add_plugin(REVITHION-FX VST3 AU LV2 ...)
```

### 7.2 Directory Structure

```
REVITHION-STUDIO/
├── CMakeLists.txt                    # Super-project
├── docs/
│   ├── REVITHION-STUDIO-MASTER-PLAN.md  # This file
│   ├── OPENDAW-ARCHITECTURE-STUDY.md    # OpenDAW analysis (797 lines)
│   ├── nuendo-14-feature-extraction.md  # 319 features (753 lines)
│   └── REVITHION-Book-Extractions.md    # Yee-King + Yeks (805 lines)
├── src/
│   ├── engine/                       # Core engine (BoxGraph ECS, audio graph)
│   │   ├── BoxGraph.h/.cpp           # Entity-Component-System (from OpenDAW)
│   │   ├── AudioGraph.h/.cpp         # Topological sort DSP graph
│   │   ├── RoutingGrid.h/.cpp        # AXE-FX III 12×4 grid
│   │   ├── MixConsole.h/.cpp         # Nuendo-class mixer
│   │   ├── MIDIEngine.h/.cpp         # Full MIDI system
│   │   ├── Automation.h/.cpp         # 6-mode automation
│   │   ├── ControlRoom.h/.cpp        # Monitor management
│   │   └── Transport.h/.cpp          # Play/record/loop/punch
│   ├── dsp/                          # Pure DSP (ported from OpenDAW lib-dsp)
│   │   ├── ADSR.h, Biquad.h, FFT.h, Oscillator.h
│   │   ├── Delay.h, Reverb.h, Compressor.h, Gate.h
│   │   ├── Chorus.h, Flanger.h, Phaser.h
│   │   └── Resampler.h, AudioBuffer.h
│   ├── neural/                       # Neural audio effects
│   │   ├── NeuralEffect.h/.cpp       # RTNeural base class
│   │   ├── AmpModel.h/.cpp           # LSTM amp emulation
│   │   ├── CabModel.h/.cpp           # Neural cabinet + IR
│   │   └── MetaController.h/.cpp     # Text→parameters (torchknob)
│   ├── ai/                           # AI integration bridges
│   │   ├── ACEStepBridge.h/.cpp      # acestep.cpp subprocess management
│   │   ├── KimiAudioBridge.h/.cpp    # Audio analysis API client
│   │   ├── MasteringAssistant.h/.cpp # AI mastering chain
│   │   └── CopilotBridge.h/.cpp      # LLM copilot integration
│   ├── blocks/                       # Routing grid block implementations
│   │   ├── AmpBlock.h/.cpp
│   │   ├── CabBlock.h/.cpp
│   │   ├── DelayBlock.h/.cpp
│   │   ├── ReverbBlock.h/.cpp
│   │   ├── CompressorBlock.h/.cpp
│   │   ├── ... (36 block types)
│   │   └── AIGenerateBlock.h/.cpp
│   ├── ui/                           # JUCE UI components
│   │   ├── ArrangeView.h/.cpp
│   │   ├── SessionView.h/.cpp        # Ableton-style clip launcher
│   │   ├── MixConsoleView.h/.cpp
│   │   ├── RoutingGridView.h/.cpp
│   │   ├── MIDIEditorView.h/.cpp
│   │   ├── ControlRoomView.h/.cpp
│   │   └── DesignTokens.h            # Auto-generated from S-expressions
│   └── plugins/                      # Standalone VST3/AU/LV2 exports
│       ├── amp/
│       ├── mastering/
│       └── fx/
├── tokens/                           # Procedural design token definitions
│   ├── design-tokens.sexp            # S-expression token source
│   ├── color-palettes.sexp
│   ├── typography.sexp
│   └── spacing.sexp
├── tools/                            # Build-time tools
│   ├── generate-tokens.py            # S-expr → constexpr C++ generator
│   └── forge-boxes.py                # Box schema → C++ class generator
├── models/                           # ML model storage
│   ├── amp/                          # NAM/RTNeural amp models (.nam, .json)
│   ├── cab/                          # Cabinet IRs + neural cab models
│   ├── mastering/                    # Mastering preset models
│   └── acestep/                      # ACE-Step GGUF models (7.7GB)
├── training/                         # Model training scripts
│   ├── amp-capture/                  # DI → amp capture workflow
│   ├── neural-effects/               # Effect model training
│   └── lora/                         # ACE-Step LoRA fine-tuning
├── faust/                            # Faust DSP source files
│   ├── parametric-eq.dsp
│   ├── multiband-comp.dsp
│   ├── tape-delay.dsp
│   ├── plate-reverb.dsp
│   ├── phaser-chorus-flanger.dsp
│   └── vocoder.dsp
├── external/                         # Git submodules
│   ├── JUCE/
│   ├── RTNeural/
│   ├── ggml/
│   └── Faust/
└── tests/
    ├── engine/
    ├── dsp/
    ├── neural/
    └── integration/
```

### 7.3 GitNexus Integration (Code Intelligence)

```
GitNexus provides non-chunking code intelligence:
- Tree-sitter AST parsing → KuzuDB knowledge graph
- Community detection (Leiden clustering) for code modules
- Execution flow traces for process understanding
- 7 MCP tools for querying the knowledge graph
- Supports: C, C++, Python, TypeScript, JavaScript, Go, Rust

Use: Index entire REVITHION codebase → query relationships, dependencies,
     call graphs without chunking or embedding loss.
NOTE: PolyForm Noncommercial license — use for development tooling only.
```

### 7.4 Port Allocation

| Port | Service |
|---|---|
| 3000 | Gitea (self-hosted Git) |
| 3003 | CopilotKit (AI copilot backend) |
| 5494 | KimiCode (coding agent) |
| 8001 | ACE-Step Python API (dev/proto) |
| 8002 | Kimi-Audio API (analysis) |
| 8003 | acestep.cpp API (native gen) |
| 8004 | GPU Sidecar (RTNeural inference) |
| 8080 | OpenDAW (reference) |
| 8100 | CyborgDB (encrypted vectors) |
| 11434 | Ollama (LLM backend) |
| 18789 | OpenClaw (agent swarm) |

---

## 8. DEVELOPMENT PHASES

### Phase 1: Foundation — Core Engine + Routing Grid
**Deliverable:** Standalone C++ app with AXE-FX routing grid, basic DSP, audio I/O

- [ ] 1.1 Initialize CMake super-project with JUCE 8 + RTNeural submodules
- [ ] 1.2 Port BoxGraph ECS from OpenDAW TypeScript → C++ (Box, Field, BoxGraph, transactions)
- [ ] 1.3 Port lib-dsp from OpenDAW → C++ (ADSR, Biquad, FFT, Oscillator, AudioBuffer)
- [ ] 1.4 Implement AudioGraph with TopologicalSort (128-sample blocks, PPQN timing)
- [ ] 1.5 Implement RoutingGrid (12×4, cell management, connection topology)
- [ ] 1.6 Implement 10 core blocks: INPUT, OUTPUT, MIXER, EQ, COMP, GATE, DELAY, REVERB, DRIVE, FILTER
- [ ] 1.7 Implement Scene system (8 snapshots) and Channel system (4 presets/block)
- [ ] 1.8 Build JUCE standalone app shell with RoutingGridView
- [ ] 1.9 ASIO/CoreAudio driver integration via JUCE
- [ ] 1.10 Basic transport (play, stop, loop, BPM)
- [ ] 1.11 Write CTest suite for engine + DSP

### Phase 2: Faust Effects + Extended Blocks
**Deliverable:** 30+ effect blocks, Faust integration

- [ ] 2.1 Set up Faust → C++ compilation pipeline in CMake
- [ ] 2.2 Faust DSP: Parametric EQ (4/8/16 band), Dynamic EQ
- [ ] 2.3 Faust DSP: Multiband compressor (4 bands), Limiter (IRC-5 style)
- [ ] 2.4 Faust DSP: Tape delay, Dattorro plate reverb, Spring reverb, Convolution
- [ ] 2.5 Faust DSP: Chorus, Flanger, Phaser, Tremolo, Rotary (Leslie)
- [ ] 2.6 Faust DSP: Pitch shifter, Vocoder (16-band), Ring modulator
- [ ] 2.7 Faust DSP: Drive/OD/Fuzz suite, Waveshaper, Bitcrusher, Fold
- [ ] 2.8 Implement Megatap delay, Enhancer, Formant, Resonator, Crossover
- [ ] 2.9 Build SplitterProcessor (1→N) and MixerProcessor (N→1)
- [ ] 2.10 All 36 block types working in routing grid

### Phase 3: Neural Audio Effects
**Deliverable:** RTNeural amp/cab modeling, neural FX running real-time

- [ ] 3.1 Integrate RTNeural into CMake build (Eigen + xsimd backends)
- [ ] 3.2 Implement NeuralEffect base class (RTNeural ModelT, JSON weight loading)
- [ ] 3.3 Build AMP block with RTNeural LSTM (single-sample forward())
- [ ] 3.4 Build CAB block (IR loader + neural DynaCab)
- [ ] 3.5 Port SmartGuitarAmp LSTM model as first neural amp
- [ ] 3.6 Implement NAM (.nam model format) loader
- [ ] 3.7 Train Tier 1 amp models (10 amps from Yeks catalog)
- [ ] 3.8 Build Meta-Controller (torchknob: text→parameters regression model)
- [ ] 3.9 Build NEURAL EFFECT block (load any RTNeural .json as custom FX)
- [ ] 3.10 Benchmark: all neural models running within 128-sample deadline at 48kHz

### Phase 4: ACE-Step Integration
**Deliverable:** AI music generation from within DAW

- [ ] 4.1 Build acestep.cpp with CUDA on RTX 3090
- [ ] 4.2 Implement ACEStepBridge (subprocess management, JSON→WAV pipeline)
- [ ] 4.3 Build AI GENERATE block for routing grid
- [ ] 4.4 Build generation UI panel (prompt, lyrics, duration, BPM, key, steps)
- [ ] 4.5 Implement LEGO pipeline (drums→bass→guitar→synth→vocals, wave subtraction)
- [ ] 4.6 Real-time preview (reduced inference steps)
- [ ] 4.7 Inpainting/repainting (regenerate sections)
- [ ] 4.8 Variation generation (batch N)
- [ ] 4.9 ACE-Step LoRA fine-tuning pipeline (user's audio → custom style models)
- [ ] 4.10 Audio file import into arrangement timeline from generation

### Phase 5: MixConsole + Arrangement
**Deliverable:** Nuendo-class mixing console and arrangement timeline

- [ ] 5.1 Implement 16 track types (Audio, Instrument, Sampler, Drum, MIDI, Group, FX, VCA, etc.)
- [ ] 5.2 Build MixConsole with 16 inserts, 8 sends, channel strip
- [ ] 5.3 Channel Strip: Gate → 3 compressor types → EQ → Saturation → Limiter
- [ ] 5.4 4-band parametric EQ per channel with graphical curve editor
- [ ] 5.5 Implement Direct Routing (8 destinations, automatable)
- [ ] 5.6 Build Arrangement Timeline view (regions, clips, crossfades)
- [ ] 5.7 Build Session/Clip Launch View (Ableton-style)
- [ ] 5.8 Implement 6-mode automation system with per-parameter curves
- [ ] 5.9 MixConsole snapshots (save/recall entire mixer state)
- [ ] 5.10 VCA fader linking (nested VCA support)
- [ ] 5.11 Channel visibility agents and filter presets

### Phase 6: MIDI System
**Deliverable:** Full MIDI editing and instrument hosting

- [ ] 6.1 MIDI event engine (notes, CC, aftertouch, pitch bend, sysex)
- [ ] 6.2 Key Editor (piano roll) with velocity/CC lanes
- [ ] 6.3 Drum Editor with drum maps
- [ ] 6.4 List Editor (event list)
- [ ] 6.5 Expression Maps (articulation switching)
- [ ] 6.6 Note Expression (per-note VST3 parameter control)
- [ ] 6.7 Chord Track and Chord Pads
- [ ] 6.8 MIDI Logical Editor (programmable transforms)
- [ ] 6.9 Arpeggiator and MIDI effects
- [ ] 6.10 VST3 instrument hosting (JUCE AudioPluginHost patterns)

### Phase 7: AI Mastering Suite
**Deliverable:** iZotope Ozone equivalent with AI assistant

- [ ] 7.1 Implement stem separation (Demucs integration)
- [ ] 7.2 Build Stem EQ module (per-stem frequency adjustment)
- [ ] 7.3 Build multiband dynamics processor
- [ ] 7.4 Build stereo imager (frequency-dependent width, mid/side)
- [ ] 7.5 Build harmonic exciter (tube/tape/transistor neural models)
- [ ] 7.6 Build transparent limiter (IRC-5 style)
- [ ] 7.7 Build tonal balance analyzer with reference matching
- [ ] 7.8 EBU R128 loudness metering (Integrated/Short-term/Momentary/True Peak)
- [ ] 7.9 Implement Mastering Assistant AI (Kimi-Audio analysis → chain recommendation)
- [ ] 7.10 A/B comparison system with commercial references
- [ ] 7.11 Build AI MASTER block for routing grid

### Phase 8: Control Room + Monitoring
**Deliverable:** Professional monitoring environment

- [ ] 8.1 Control Room with multiple monitor outputs
- [ ] 8.2 Monitor A/B switching
- [ ] 8.3 4 Cue mixes for performers
- [ ] 8.4 Talkback with auto-dim
- [ ] 8.5 Reference level calibration
- [ ] 8.6 Surround monitoring up to 7.1.6
- [ ] 8.7 Headphone monitoring with crossfeed

### Phase 9: CopilotKit AI Copilot
**Deliverable:** Conversational AI assistant embedded in DAW

- [ ] 9.1 Integrate CopilotKit with JUCE app (embedded browser or native chat panel)
- [ ] 9.2 Build generateTrack action (triggers ACE-Step)
- [ ] 9.3 Build analyzeAudio action (triggers Kimi-Audio)
- [ ] 9.4 Build masterTrack action (triggers mastering suite)
- [ ] 9.5 Build suggestEffects action (effect chain recommendation)
- [ ] 9.6 Build mixAssistant action (level, pan, EQ suggestions)
- [ ] 9.7 Connect to Ollama backend (qwen3-coder:30b) with REVITHION system prompt
- [ ] 9.8 Streaming generation feedback (AG-UI protocol)

### Phase 10: Plugin Export + Packaging
**Deliverable:** VST3/AU/LV2 plugins + standalone EXE/ISO

- [ ] 10.1 JUCE plugin projects: REVITHION-Amp (neural amp suite)
- [ ] 10.2 JUCE plugin projects: REVITHION-Master (mastering chain)
- [ ] 10.3 JUCE plugin projects: REVITHION-FX (multi-effect)
- [ ] 10.4 Build installers (NSIS for Windows, .deb/.AppImage for Linux)
- [ ] 10.5 VST3 plugin hosting within REVITHION (load 3rd party plugins)
- [ ] 10.6 Test plugins in: REAPER, Bitwig, FL Studio, Ardour
- [ ] 10.7 Build ISO image for distribution
- [ ] 10.8 Procedural design token LookAndFeel applied to all plugins

---

## 9. EXISTING RESOURCES (Ready to Use)

### Source Code
| Resource | Location | Status |
|---|---|---|
| OpenDAW (blueprint) | DevTeam6/openDAW/ + ~/projects/openDAW/ | Running :8080 |
| ACE-Step Python | ~/projects/ACE-Step-1.5/ | Installed |
| acestep.cpp | DevTeam6/acestep.cpp/ | Ready to build |
| ACE-Step-DAW | DevTeam6/ACE-Step-DAW/ | Reference |
| JUCE 8.0.12 | ~/OpenAudio/JUCE/ | Ready |
| HISE | ~/HISE/ | Ready |
| RTNeural | ~/OpenAudio/RTNeural/ | Ready |
| NAM (LV2) | ~/OpenAudio/neural-amp-modeler-lv2/ | Ready |
| Faust | ~/OpenAudio/faust/ | Ready |
| SmartGuitarAmp | ~/OpenAudio/SmartGuitarAmp/ | Ready |
| Surge | ~/OpenAudio/surge/ | Architecture ref |
| Vital | ~/OpenAudio/vital/ | Architecture ref |
| DawDreamer | ~/OpenAudio/DawDreamer/ | Python DAW ref |
| tracktion_engine | ~/OpenAudio/tracktion_engine/ | C++ DAW ref |
| CopilotKit | DevTeam6/CopilotKit/ | Demo on :3003 |
| Kimi-Audio | DevTeam6/Kimi-Audio/ | Cloned |
| GitNexus | DevTeam6/GitNexus/ | Needs npm install |
| cmacro | ~/projects/cmacro/ | Cloned |
| eslisp | ~/projects/eslisp/ | Cloned |

### Research Docs (Generated)
| Document | Location | Lines |
|---|---|---|
| OpenDAW Architecture Study | REVITHION-STUDIO/docs/OPENDAW-ARCHITECTURE-STUDY.md | 797 |
| Nuendo 14 Feature Extraction | docs/research/nuendo-14-feature-extraction.md | 753 |
| Book Extractions (Yee-King+Yeks) | docs/research/REVITHION-Book-Extractions.md | 805 |
| LLM Planning Research | ~/projects/awesome-llm-planning-reasoning/ | Cloned |
| ASSET_INVENTORY.md | DevTeam6/ASSET_INVENTORY.md | 3,252 |

### Reference Software (D:\New Downloads)
FabFilter Total Bundle, iZotope Ozone 12, Arturia V11, Neural DSP full suite,
NI Kontakt 8, Ableton 12, FL Studio 25, Bitwig 6, REAPER 7, IDA Pro 9.1

### Skills
- 889 antigravity-awesome-skills (442 relevant)
- 2,884 OpenClaw skills (claw-swarm, audio-gen, audio-cog, agent-memory-ultimate)
- 235 GitHub agents (Architect, Phoenix, Transcendent Coder)
- NotebookLM CLI (deep research, 600 resources)
- AlphaXiv.org (AI research papers)

---

## 10. VRAM BUDGET (RTX 3090 24GB)

| Component | VRAM | When |
|---|---|---|
| ACE-Step (Q8_0 turbo + 4B LM) | 7.7 GB | During generation |
| Kimi-Audio (4-bit) | 4.0 GB | During analysis |
| RTNeural amp models | <0.1 GB | Always (real-time) |
| Demucs stem separation | 2.0 GB | During mastering |
| **Concurrent max** | **~14 GB** | **Fits in 24GB** |

---

## 11. SUCCESS METRICS

### MVP (Phase 1-4)
- [ ] Routing grid with 36 blocks running in standalone C++ app
- [ ] Neural amp modeling real-time at 48kHz (<128 sample latency)
- [ ] ACE-Step generates tracks from within DAW
- [ ] Basic Faust effects library (EQ, comp, delay, reverb, chorus)
- [ ] ASIO audio at 48kHz with <10ms round-trip latency

### Production (Phase 1-8)
- [ ] Full Nuendo-class MixConsole with channel strip
- [ ] 16 track types with full MIDI system
- [ ] AI mastering chain with assistant workflow
- [ ] 30+ neural amp/cab models included (Tier 1+2)
- [ ] Control Room with surround monitoring
- [ ] 6-mode automation system

### Release (Phase 1-10)
- [ ] VST3/AU/LV2 plugin suite (Amp, Master, FX)
- [ ] Standalone EXE installer for Windows + .deb for Linux
- [ ] CopilotKit conversational AI fully integrated
- [ ] LEGO multi-track AI generation pipeline
- [ ] Procedural design token UI system
- [ ] ISO distribution image

---

*Generated: 2026-03-03 | SpiralCloudOmega / REVITHION STUDIO*
*Research: 8 parallel agents, 21 completed research tasks, 2,355 lines of analysis docs*
*References: OpenDAW (114K LoC), Nuendo 14 (1,806pp), Yee-King (362pp), Yeks (301pp), RAG_CAG (113pp)*
