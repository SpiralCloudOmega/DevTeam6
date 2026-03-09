# 🧠 GitHub Copilot Agent Cheat Sheet
## SpiralCloudOmega / DevTeam6 × REVITHION STUDIO

> **Purpose:** Everything a Copilot agent needs to write proper prompts, stay within context limits,
> understand this codebase's unique architecture, and avoid silent truncation failures.

---

## 📐 CONTEXT WINDOW LIMITS — KNOW YOUR BUDGET

This is the #1 thing Copilot doesn't always self-report accurately. Here are the **actual measured limits**:

### GitHub Copilot Coding Agent (this environment)
| Slot | Limit | Notes |
|------|-------|-------|
| Total context window | ~128K tokens / ~500K characters | Hard cap on everything visible at once |
| `copilot-instructions.md` (system prompt) | **Hard limit: ~8,000 chars** — **safe zone: ≤ 6,000 chars** | Silently truncated beyond ~8K; content after ~6K may be de-prioritized |
| Single user message | ~32K tokens / ~128K characters | Larger messages are chunked |
| Attached/referenced files | ~10 files, ~2MB combined | Excess files are dropped silently |
| Problem statement (issue body) | ~4,000 characters effective | Longer text may lose tail context |
| Repository memory entries | ~50 active entries | Oldest entries rotated out |

### GitHub Copilot Chat (VS Code / Web)
| Mode | Effective Prompt Budget | Truncation Behavior |
|------|------------------------|---------------------|
| Inline completion | **~2,000 chars** surrounding code | Silently drops outer context |
| Chat — `@workspace` | ~60K chars of indexed content | Retrieval, not full scan |
| Chat — attached file | ~100K chars per file | Truncates at end of file |
| Chat — `#selection` | The selection, plus ~4K context | Preferred for precise questions |
| Chat — `/fix`, `/explain` | Same as selection mode | |

### Ollama Local Models (in-repo usage)
| Model | Context | Character Budget |
|-------|---------|-----------------|
| `qwen3:8b` | 32K tokens | ~120K characters |
| `qwen3-coder:30b` | 32K tokens | ~120K characters |
| `KimiCode` (:5494) | **256K tokens** | ~1M characters — use for huge files |

---

## ✂️ TRUNCATION SURVIVAL RULES

**Rule 1 — Front-load the critical information.**
Copilot reads top-to-bottom. If your prompt gets cut, the tail is gone. Put constraints, file paths, and requirements FIRST.

**Rule 2 — The 6K character safe zone for `copilot-instructions.md`.**
`copilot-instructions.md` has a hard truncation at ~8,000 characters, but content after ~6,000 chars can be de-prioritized by the model. Keep it under 6,000 characters for reliable full-file attention. The current file is calibrated to this. Do NOT add large blocks to it — add a link to this cheat sheet instead.

**Rule 3 — Use `#file` references instead of pasting code.**
Pasting a 500-line file into chat costs ~10K characters. Using `#file:path/to/file.cpp` costs ~50 characters and lets Copilot fetch it selectively.

**Rule 4 — Break large tasks into sessions.**
Tasks that require >50K characters of combined context (code + instructions + history) should be split. Use `report_progress` between major phases.

**Rule 5 — Structured prompts survive truncation better.**
```
TASK: [one sentence]
FILES: [comma-separated paths]
CONSTRAINTS: [bullet list]
CONTEXT: [only what's needed, nothing else]
```

---

## 🏗️ REVITHION STUDIO ARCHITECTURE — WHAT COPILOT MAY NOT KNOW

This project is **not a standard web app or plugin**. It is a custom native production suite with
a unique stack that Copilot has never seen assembled this way.

### Stack Overview
```
LAYER               TECHNOLOGY
─────────────────────────────────────────────────────────────────
Presentation        JUCE 8 LookAndFeel + Visage (GPU-accelerated GUI)
Design Tokens       S-expressions → LISP macro expansion → constexpr C++20 headers
Audio Engine        C++17, BoxGraph ECS (ported from OpenDAW TypeScript)
Plugin Format       VST3 / AU / LV2 / CLAP + AAX (via JUCE)
Neural DSP          RTNeural (Eigen/xsimd) + NAM (Neural Amp Modeler)
Circuit Modeling    chowdsp_wdf (wave digital filters)
SIMD                Highway library (cross-arch SIMD abstraction)
Time-Stretch        Rubber Band Library
AI Music Gen        ACE-Step 1.5 (GGML/CUDA, 7.7GB VRAM)
Audio Analysis      Kimi-Audio 7B (4-bit quant, 4GB VRAM)
Copilot AI          CopilotKit + Ollama (qwen3-coder:30b)
Build System        CMake 3.28 + C++20 / C++17 mixed
Target OS           Windows EXE + ISO (built in WSL2 Ubuntu 24.04)
```

### Key Directories
```
DevTeam6/
├── .github/
│   ├── copilot-instructions.md     ← Copilot system prompt (keep <6K chars)
│   ├── COPILOT-CHEATSHEET.md       ← THIS FILE
│   ├── agents/                     ← 236 custom Copilot agents
│   ├── skills/                     ← 41 universal SKILL.md files (6,046 lines)
│   │   ├── audio-dsp/              ← 10 skills: biquad, FFT, dynamics, etc.
│   │   ├── neural-audio/           ← 8 skills: RTNeural, NAM, AI mastering, etc.
│   │   ├── juce-framework/         ← 6 skills: AudioProcessor, LookAndFeel, etc.
│   │   ├── music-production/       ← 6 skills: routing grid, mix, mastering
│   │   ├── cpp-patterns/           ← 6 skills: constexpr, ECS, SIMD, etc.
│   │   └── agent-orchestration/    ← 5 skills: multi-agent, memory, etc.
│   ├── memory/
│   │   ├── spiralcloud-memory.db   ← SQLite FTS5 session memory
│   │   ├── environment-profile.json
│   │   └── rules-and-directives.json
│   └── scripts/
│       └── session-bootstrap.sh
├── REVITHION-STUDIO/
│   ├── CMakeLists.txt
│   ├── src/
│   │   ├── Main.cpp                ← JUCE application entry point
│   │   ├── ai/
│   │   │   ├── CopilotBridge.h/.cpp ← Ollama-backed AI chat (qwen3-coder:30b)
│   │   │   └── ACEStepBridge.h     ← GGML/CUDA music generation
│   │   └── engine/
│   │       ├── BoxGraph.h          ← ECS audio graph (ported from OpenDAW)
│   │       └── AudioGraph.h        ← Topological sort, 128-sample blocks
│   ├── tokens/
│   │   ├── design-tokens.sexp      ← 478L, 46 design tokens in S-expr format
│   │   ├── macros.sexp             ← 128L, 9 LISP macro templates
│   │   └── generate-tokens.py      ← 1,009L S-expr parser + C++20 emitter
│   └── faust/                      ← Faust DSP scripts (EQ, comp, reverb, etc.)
├── app/                            ← React 18 + Three.js workstation (port 3000)
├── projects/                       ← Vue 3 workflow editor (port 5173)
├── local-ai/                       ← Python AI services (FastAPI, port 5000)
│   ├── api/main.py
│   ├── agents/
│   ├── core/                       ← RAG pipeline, knowledge graph, memory
│   └── mcp/                        ← MCP servers (memory, RAG)
├── context7/
│   ├── agents.md                   ← Agent registry
│   └── prompt-guide.md             ← Quick prompt reference (see below)
└── templates/                      ← Codespace starters (C, C++, audio-dsp, etc.)
```

### The Design Token Pipeline (UNIQUE — Copilot won't know this)
```
1. Author tokens in  tokens/design-tokens.sexp  (S-expression format)
2. Define macros in  tokens/macros.sexp
3. Run:  python tokens/generate-tokens.py
4. Output: src/generated/ThemeTokens.h, AmpModels.h, FxChains.h, etc.
5. All C++ constexpr, zero runtime overhead

Example token:
  (token amp-gain-min  :type float  :value 0.0  :unit dB  :group amp-params)
  → constexpr float kAmpGainMin = 0.0f; // dB [amp-params]
```

### BoxGraph ECS Pattern (ported from OpenDAW TypeScript)
```cpp
// C++ equivalent of OpenDAW's Box → Adapter → Processor 3-layer pattern
// Box     = entity with UUID, type-erased data
// Adapter = wraps Box with domain logic (e.g., AmpBlock, ReverbBlock)
// Processor = real-time audio processing node, lock-free

revithion::engine::BoxGraph graph;
auto box = graph.createBox(BoxType::AmpBlock);
box->setParam("gain", 0.0f);
graph.connect(inputBox, box);  // topological sort on commit()
graph.commit();                // rebuilds process order, real-time safe
```

### Routing Grid (AXE-FX III inspired)
```
12 columns × 4 rows = 48 block slots
8 scenes × 4 channels per block = 32 independent states per block
Block types: AMP CAB DELAY REVERB CHORUS FLANGER PHASER COMP EQ
             DRIVE GATE PITCH WAH VOL PAN SPLIT MIX SHUNT
             AI blocks: GENERATE MASTER STEM-SPLIT ANALYZE NEURAL-FX
Signal path: serial (column order) or parallel (split/mix blocks)
```

### CopilotBridge — In-App AI
```cpp
// System prompt injected automatically — do not exceed 2,048 tokens
CopilotConfig cfg;
cfg.modelName  = "qwen3-coder:30b";  // local Ollama
cfg.ollamaUrl  = "http://localhost:11434";
cfg.maxTokens  = 2048;
cfg.temperature = 0.7f;

CopilotBridge bridge(cfg);
bridge.updateContext("bpm", "140");       // injected into every request
bridge.sendMessage("master this track",  // user message
    [](auto& reply){ /* handle */ });
```

---

## 🤖 AGENT ROSTER — PICK THE RIGHT EXPERT

> **Rule:** Always dispatch the most specific expert agent. General-purpose last.

| Use Case | Agent to Call |
|----------|--------------|
| Audio DSP (filters, FFT, dynamics, oscillators) | `audio-dsp-expert` |
| Neural amp modeling, RTNeural, NAM | `amp-modeling-expert` |
| JUCE 8 components, AudioProcessor, LookAndFeel | `juce-framework-expert` |
| Faust DSP language, faust2juce | `faust-dsp-expert` |
| CMake, C++20/23 patterns, cross-platform builds | `cmake-cpp20-expert` |
| AI mastering, loudness, spectral repair | `mastering-engineer` |
| Mixing, bus routing, gain staging | `mix-engineer` |
| AXE-FX routing grid, scenes/channels, block types | `routing-grid-expert` |
| RTNeural, ONNX Runtime, TensorRT, audio-thread safety | `neural-inference-expert` |
| FFT, STFT, mel spectrogram, source separation | `spectral-analysis-expert` |
| LISP tokens, S-expr parser, constexpr generation | `lisp-token-expert` |
| Vulkan compute shaders for GPU audio/FFT | `vulkan-compute-expert` |
| MIDI, MPE, MIDI 2.0, JUCE MIDI API | `midi-expert` |
| VST3, AU, LV2, CLAP, AAX validation | `plugin-format-expert` |
| ACE-Step, GGML, Qwen3 conditioning, LoRA | `ace-step-expert` |
| Python, CUDA, pip, conda, Linux | `linux-nvidia-cuda-python` |
| Session recording, memory persistence | `session-recorder` |
| Loading prior session context | `context-loader` |

---

## ✍️ PROMPT TEMPLATES FOR COMMON TASKS

### Adding a New DSP Block to the Routing Grid
```
TASK: Add a [BLOCK_NAME] block type to the REVITHION routing grid.
AGENT: routing-grid-expert + audio-dsp-expert
FILES: REVITHION-STUDIO/src/engine/BoxGraph.h, REVITHION-STUDIO/CMakeLists.txt
PATTERN: Follow existing AMP block pattern (Box → Adapter → Processor)
CONSTRAINTS:
  - Real-time safe (no allocations, no locks in process())
  - Parameters defined via design tokens in tokens/design-tokens.sexp
  - Expose as CLAP + VST3 via JUCE AudioProcessor wrapper
CONTEXT: BoxGraph ECS, 12×4 routing grid, 8 scenes × 4 channels per block
```

### Updating the Design Token System
```
TASK: Add [N] new tokens for [CATEGORY] to the design token pipeline.
AGENT: lisp-token-expert
FILES: REVITHION-STUDIO/tokens/design-tokens.sexp, tokens/macros.sexp, tokens/generate-tokens.py
PATTERN: (token name :type TYPE :value VAL :unit UNIT :group GROUP)
CONSTRAINTS:
  - Must generate valid constexpr C++20 header
  - Group name maps to generated filename (e.g., amp-params → AmpModels.h)
  - Run generate-tokens.py to verify output before committing
```

### Adding a Neural DSP Model
```
TASK: Integrate a [MODEL_TYPE] neural model for real-time audio processing.
AGENT: neural-inference-expert + amp-modeling-expert
FILES: REVITHION-STUDIO/src/ai/, REVITHION-STUDIO/CMakeLists.txt
CONSTRAINTS:
  - Use RTNeural for LSTM/GRU/Conv1D (Eigen backend, no heap in process())
  - Models ship as JSON (RTNeural format) or GGUF (for GGML models)
  - Target: RTX 3090 24GB VRAM, Ryzen 9 5950X 16C/32T
  - Must pass pluginval 1.0.4 validation
```

### React / TypeScript (app/)
```
TASK: [description of UI change]
AGENT: my-react.agent or general-purpose
FILES: app/src/[relevant files]
STACK: React 18, TypeScript, Three.js, Vite, port 3000
THEME: Cyberpunk — cyan #00f0ff, magenta #ff00ff, green #00ff88, bg #0f0f23
CONSTRAINTS: No new npm dependencies unless absolutely required
```

### Python AI Service (local-ai/)
```
TASK: [description of AI service change]
AGENT: linux-nvidia-cuda-python
FILES: local-ai/[relevant files]
STACK: FastAPI, Python 3.13, SQLite, pgvector, Ollama REST API (port 11434)
CONSTRAINTS: Async endpoints, pydantic models, no blocking I/O in hot path
```

---

## ⚡ PROMPT ENGINEERING QUICK TIPS

### DO
- ✅ **Name the specific agent** — `AGENT: audio-dsp-expert` tells Copilot which `.agent.md` to load
- ✅ **List exact file paths** — reduces retrieval errors
- ✅ **State the pattern** — "follow the AMP block pattern" is 5 words that replace 50 lines
- ✅ **Declare constraints first** — truncation kills the tail, not the head
- ✅ **Reference SKILL.md files** — e.g., "see `.github/skills/audio-dsp/biquad-filter/SKILL.md`"
- ✅ **Use structured YAML/markdown format** — easier for the model to parse reliably
- ✅ **Say "real-time safe"** explicitly — the model needs this signal for audio code
- ✅ **Specify the CMake target** — `add_to target: RevithionStudio` avoids ambiguity

### DON'T
- ❌ Paste large code blocks when you can use `#file:path` or a file reference
- ❌ Leave the agent type vague — "help me with audio" hits general-purpose, not the DSP expert
- ❌ Write requirements at the end of a long prompt — they may be truncated away
- ❌ Ask multiple unrelated things in one message — spawn parallel agents instead
- ❌ Forget to say "no heap allocation in process()" for any audio-thread code
- ❌ Use `std::function` or `std::shared_ptr` in real-time audio paths
- ❌ Assume Copilot knows the ECS BoxGraph pattern — it's custom; always link the file

---

## 🔑 KEY INVARIANTS (Copilot must never break these)

| Invariant | Rule |
|-----------|------|
| Audio thread safety | No `new`/`delete`, no `std::mutex` lock, no `dynamic_cast` inside `processBlock()` |
| Token pipeline | All C++ constants come from generated headers — never hardcode magic numbers |
| Plugin validation | All DSP blocks must pass `pluginval --strictness-level 5` |
| JUCE threading | GUI only on message thread; audio only on audio thread; use `juce::AsyncUpdater` to bridge |
| Design tokens | Group names in `.sexp` must exactly match generated header filenames |
| CMake minimum | `cmake_minimum_required(VERSION 3.25)` — never lower |
| C++ standard | C++20 for new code; C++17 for JUCE-adjacent code (JUCE 8 is C++17 internally) |
| Faust integration | Faust DSP → `faust2juce` → scriptnode-compatible wrapper |
| Memory DB | Session facts stored in `.github/memory/spiralcloud-memory.db` (SQLite FTS5) |

---

## 🔧 LOCAL SERVICES REFERENCE

| Service | Port | Command to Start |
|---------|------|-----------------|
| React App (DevTeam6) | 3000 | `cd app && npm run dev` |
| Vue 3 Workflow Editor | 5173 | `cd projects && npm run dev` |
| Python AI API (FastAPI) | 5000 | `cd local-ai && uvicorn api.main:app --reload` |
| Ollama (local LLM) | 11434 | `ollama serve` |
| ACE-Step API | 8001 | `cd ~/projects/ACE-Step-1.5 && python api.py` |
| OpenClaw (100 agents) | 18789 | `openclaw start` |
| KimiCode (256K ctx) | 5494 | `kimicode serve` |
| CopilotKit | 3003 | `cd ~/projects/CopilotKit && npm run dev` |
| PostgreSQL + pgvector | 5432 | `sudo service postgresql start` |
| CyborgDB | 8100 | `cd ~/cyborgdb && cargo run --release` |

---

## 📦 SKILLS LIBRARY — QUICK REFERENCE

Skills are `.github/skills/{category}/{skill-name}/SKILL.md` — YAML frontmatter + code + anti-patterns + checklist.

| Category | Count | Key Skills |
|----------|-------|-----------|
| `audio-dsp` | 10 | biquad-filter, fft-processor, dynamics, oscillators, delay-reverb, saturation, spatial, metering |
| `neural-audio` | 8 | rtneural-inference, amp-training, wavenet, cab-modeling, ai-mastering, voice, music-gen, stem-sep |
| `juce-framework` | 6 | audio-processor, lookandfeel, parameter-layout, midi-input, plugin-state, unit-testing |
| `music-production` | 6 | routing-grid, mix-console, mastering-chain, automation, session-view, clip-launch |
| `cpp-patterns` | 6 | constexpr-generation, ecs-entity, simd-vectors, lock-free-queue, memory-pool, template-meta |
| `agent-orchestration` | 5 | multi-agent-coordinator, context-injection, skill-routing, memory-retrieval, parallel-dispatch |

**To reference a skill in a prompt:**
```
See skill: .github/skills/audio-dsp/biquad-filter/SKILL.md
```

---

## 🚨 EMERGENCY CONTEXT — When Something Goes Wrong

### "Copilot produced code that ignores the architecture"
→ Add to prompt: `MANDATORY: Follow BoxGraph ECS pattern. See REVITHION-STUDIO/src/engine/BoxGraph.h`

### "The token pipeline output looks wrong"
→ Run: `python REVITHION-STUDIO/tokens/generate-tokens.py --dry-run`

### "Agent timed out / HTTP 503"
→ Known issue: agents running >25 min consistently 503. Always launch backup fast agents (<10 min tasks) for long research.

### "Context was lost between sessions"
→ Run context-loader agent first: it reads `.github/memory/spiralcloud-memory.db`

### "Copilot doesn't know about a recent change"
→ Use `store_memory` tool with category + citations. Memory DB retains ~50 recent entries.

---

*Last updated: 2026-03-09 | Maintained by SpiralCloudOmega*
