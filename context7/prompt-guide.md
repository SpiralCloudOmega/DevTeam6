# ⚡ Copilot Prompt Guide — Quick Reference
## DevTeam6 × REVITHION STUDIO

> Fast lookup for writing correct prompts. Full details: `.github/COPILOT-CHEATSHEET.md`

---

## 📐 Context Limits at a Glance

| Where | Effective Budget | Truncation |
|-------|-----------------|------------|
| `copilot-instructions.md` | **6,000 chars max** | Silent — tail gone |
| Coding agent total | ~500K chars | Hard cap |
| VS Code inline | ~2,000 chars | Silent — outer context lost |
| Chat `@workspace` | ~60K chars indexed | Retrieval-based |
| Ollama qwen3-coder:30b | ~120K chars | Error at limit |
| KimiCode (:5494) | ~1M chars | Best for huge files |

**Golden Rule:** Put constraints at the TOP. The tail gets cut, not the head.

---

## 🎯 Pick Your Agent

```
Audio DSP code        → audio-dsp-expert
Neural amps / RTNeural → amp-modeling-expert
JUCE 8 components     → juce-framework-expert
CMake / C++20         → cmake-cpp20-expert
Faust DSP             → faust-dsp-expert
AI mastering / LUFS   → mastering-engineer
Mix / bus routing     → mix-engineer
Routing grid blocks   → routing-grid-expert
LISP tokens / sexp    → lisp-token-expert
ACE-Step / GGML       → ace-step-expert
Python / CUDA / pip   → linux-nvidia-cuda-python
Load prior session    → context-loader
```

---

## 📝 Prompt Template

```
TASK: [one sentence]
AGENT: [agent-name]
FILES: [path/to/file.cpp, path/to/other.h]
PATTERN: [existing pattern to follow, if any]
CONSTRAINTS:
  - [constraint 1]
  - [constraint 2]
CONTEXT: [only what is strictly needed]
```

---

## 🔑 Magic Words for Audio Code

Always include when writing DSP / audio-thread code:
- `real-time safe` — no allocations, no locks, no dynamic_cast in processBlock()
- `no heap in process()` — reinforces the above
- `follow BoxGraph ECS pattern` — for routing grid / engine changes
- `generate constexpr header from token pipeline` — for new constants
- `pluginval-compliant` — ensures plugin passes validation

---

## 🏗️ Architecture in 30 Seconds

```
Design Tokens  (.sexp) → generate-tokens.py → constexpr C++20 headers
Audio Engine   BoxGraph ECS → topological sort → 128-sample real-time blocks
Routing Grid   12×4 blocks, 8 scenes × 4 channels, AXE-FX III inspired
Neural DSP     RTNeural (JSON models) + NAM + GGML/CUDA (ACE-Step)
GUI            JUCE 8 + Visage (GPU) + procedural LookAndFeel from tokens
Plugins        VST3 / AU / LV2 / CLAP via JUCE
Build          CMake 3.28, C++20 new / C++17 JUCE-adjacent, WSL2 → EXE/ISO
```

---

## 📁 Most-Referenced Files

```
.github/copilot-instructions.md          ← system prompt (keep <6K chars)
.github/COPILOT-CHEATSHEET.md            ← full reference (this doc's parent)
.github/skills/{category}/{skill}/SKILL.md ← 41 universal skill files
REVITHION-STUDIO/src/engine/BoxGraph.h   ← ECS audio graph
REVITHION-STUDIO/src/ai/CopilotBridge.h  ← in-app AI (Ollama)
REVITHION-STUDIO/tokens/design-tokens.sexp ← all design constants
REVITHION-STUDIO/tokens/generate-tokens.py ← S-expr → C++ emitter
context7/agents.md                       ← agent registry + project state
```

---

## 🚫 Never Do This

- ❌ `new` / `delete` in `processBlock()`
- ❌ `std::mutex::lock()` on the audio thread
- ❌ Hardcode numeric constants — use generated token headers
- ❌ Add requirements at the bottom of a long prompt (truncation eats them)
- ❌ Paste large files into chat — use `#file:` reference instead
- ❌ Set `cmake_minimum_required` below 3.25

---

*Full cheat sheet → `.github/COPILOT-CHEATSHEET.md`*
