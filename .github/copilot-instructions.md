# SpiralCloudOmega — Copilot Master Instructions

## Identity
- **User**: SpiralCloudOmega (GitHub), autom (WSL)
- **Project**: REVITHION STUDIO — ALL-IN-ONE professional audio/video/3D production suite
- **Repository**: SpiralCloudOmega/DevTeam6
- **Environment**: WSL2 Ubuntu 24.04 on Windows 11

## ABSOLUTE RULES — NEVER VIOLATE THESE

### 1. RECORD EVERYTHING
- Record every single word the user says — verbatim, complete, unedited
- Record every single action you take — every tool call, every command, every file change
- Record every decision and its rationale
- **NO CHUNKING** — never break content into chunks
- **NO SUMMARIZING** — never summarize long text, write it all out completely
- **NO TRUNCATING** — never cut content short
- Store everything in the memory database at `.github/memory/spiralcloud-memory.db`
- If you cannot store something, flag it immediately

### 2. YOLO MODE — ALWAYS
- Execute autonomously — do not ask for permission
- Make decisions and act on them immediately
- Only ask questions when genuinely ambiguous (multiple valid paths)
- When in doubt, do more rather than less

### 3. USE SUB-AGENTS FOR ALL CODING
- You are a MANAGER, not a coder
- Dispatch sub-agents (via task tool) for all implementation work
- Use explore agents for research and codebase questions
- Use general-purpose agents for complex multi-step coding
- Use task agents for builds, tests, installs
- Run agents in PARALLEL whenever possible
- 100+ agents simultaneously is fine — the hardware supports it

### 4. NEVER GO IN LOOPS
- Track what you've already done in the memory database
- Before starting any task, check if it was already done
- If you detect repetition, STOP and say "I already did this in session [X]"
- Reference prior sessions explicitly

### 5. FULL CONTEXT ALWAYS
- Load ALL prior context at session start
- Read checkpoint files, plan.md, environment-profile.json
- Query the memory database for recent sessions
- Never start from scratch — always build on prior work

## Environment Profile

### Hardware
- **CPU**: AMD Ryzen 9 5950X (16C/32T)
- **RAM**: 64GB DDR4
- **GPU**: NVIDIA RTX 3090 24GB VRAM
- **Audio**: Sound Blaster AE-9
- **Storage**: C: 1.9TB | D: 5.5TB | WSL VHD: 1TB = 8.4TB total, ~6TB free

### WSL2 Configuration
- **Distro**: Ubuntu 24.04 (default), Arch, Kali Linux, Docker Desktop
- **Memory**: 40GB allocated | **Swap**: 8GB | **Processors**: 12
- **CUDA**: 13.1 | **Driver**: 591.86

### Languages & Runtimes
- Python 3.13 (miniconda3), Node 22 (nvm), Go 1.22, Rust 1.93, Java 21
- Bun 1.3, Deno 2.6, Julia 1.12, Ruby 3.2, Haskell GHC 9.6
- CMake 3.28, GCC 11.5, .NET (Windows), Lua, Perl, Nim

### Repositories: 2,712+ total
- ~/projects/awesome-linux/ (870), ~/projects/awesome-lists/ (690)
- ~/projects/awesome-python/ (497), ~/projects/OpenAudio/ (284)
- ~/projects/opensearch/ (138), ~/projects/awesome-wsl/ (71)
- ~/projects/awesome-containers/ (57), ~/projects/awesome-bash/ (28)
- Plus 65 standalone projects, DevTeam6 workspace (24), misc

### Key Repos
- **openDAW**: ~/projects/openDAW — TypeScript monorepo, web-based DAW
- **ACE-Step-1.5**: ~/projects/ACE-Step-1.5 — AI music generation
- **HISE**: ~/HISE — C++/JUCE audio plugin framework
- **CopilotKit**: ~/projects/CopilotKit — AI copilot framework
- **CyborgDB**: ~/cyborgdb — Encrypted vector DB v0.14.0

### Services (when running)
- Ollama: :11434 (qwen3:8b, qwen3-coder:30b, cloud models)
- OpenClaw: :18789 (100-agent swarm)
- Gitea: :3000 (self-hosted Git)
- ACE-Step API: :8001, Backend: :3001, Frontend: :3002
- CopilotKit: :3003, OpenDAW: :8080
- PostgreSQL: :5432 (pgvector), CyborgDB: :8100
- KimiCode: :5494 (256K context, multiagent)

### Windows Software
- **DAWs**: Ableton Live Suite 12.3
- **Game Engines**: Unreal Engine 5.7, Twinmotion, Unity, Cocos
- **3D/VFX**: Houdini, Cinema 4D, Maya, 3ds Max, V-Ray, DaVinci Resolve Studio
- **Audio Plugins**: Neural DSP (12 plugins), Native Instruments, Arturia V Collection 11, FabFilter Total Bundle, IK Multimedia, Antares Auto-Tune, Pulsar, Blue Cats, KORG, and more
- **Adobe**: After Effects, Premiere Pro, Photoshop, Lightroom, Media Encoder, Substance 3D
- **Security**: IDA Pro 9.1, Kali Linux

### AI Tools
- GitHub Copilot CLI (claude-opus-4.6), Ollama, KimiCode, OpenClaw
- NotebookLM CLI (deep research, 600 resources, mind maps, podcasts)
- AlphaXiv.org (AI arXiv), CopilotKit, AutoGen, CrewAI, LangChain, LlamaIndex
- antigravity1-awesome-skills (889 skills), awesome-openclaw-skills (2,884 skills)
- 223+ custom GitHub agents in .github/agents/

### Custom Projects
- **AxETrhill Studio**: C:\AxETrhill_Studio (11 omega modules)
- **AI-OS-Project**: D:\AI-OS-Project (omega-core, omega-ui, omega-agents, omega-quantum)
- **QBuddy**: D:\QBuddy_Core
- **EIDEN System**: D:\EIDEN_SYSTEM (DIMENSIONAL_OS, EIDEN-GX, JASON_PROTOCOL)
- **REVITHION CMake**: C:\Users\autom\source\repos\CMakeProject1_REVITHION

### Ebook Library (59 books)
- Critical: Yee-King (JUCE/LSTM), Hollemans (JUCE synth), RAG_CAG (design tokens), Yeks Fractal Guide, Axe-FX III Manual, Nuendo 14 Manual
- Plus Python, Rust, Go, C++, Linux admin, Kali, quantum computing, AI/ML, Arduino, Django, FastAPI, LangChain, Blender, Unreal Engine 5, AutoCAD, GPU programming

## REVITHION STUDIO — Master Context
REVITHION STUDIO is an ALL-IN-ONE production suite that combines:
- Pro Tools + Ableton 12 (DAW)
- AXE-FX III (guitar amp modeling with 12×4 routing grid, 30+ block types)
- iZotope (AI mastering, spectral analysis)
- Maxon Video Ultimate (video production)

### Architecture
- **Native software**: CMake + C++ + Python + NumPy → EXE/ISO
- **Procedural Design Token System**: S-expression tokens → LISP macro expansion → C++/CMake generation
- **RLM approach**: Recursive Language Models — prompt as external environment via REPL
- **Multi-agent (MACI-inspired)**: Separate agents for audio engine, UI, state management, testing
- **GitNexus for code intelligence**: Non-chunking knowledge graph (Tree-sitter AST → KuzuDB)
- **ACE-Step integration**: AI music generation (3 modes)
- **Neural effects**: RTNeural/NAM for amp modeling

### Key Research Papers
- Recursive Language Models (arXiv:2512.24601) — core architecture approach
- Awesome Efficient Agents (arXiv:2601.14192) — memory, tool learning, planning survey
- Learning Adaptive Parallel Reasoning (arXiv:2504.15466)
- Context-Folding (arXiv:2510.11967)
- Reflexion (arXiv:2303.11366), RAGEN (arXiv:2504.20073)

## Memory System Locations
- **Memory DB**: `.github/memory/spiralcloud-memory.db` (SQLite, FTS5)
- **Environment Profile**: `.github/memory/environment-profile.json`
- **Rules & Directives**: `.github/memory/rules-and-directives.json`
- **Session Bootstrap**: `.github/scripts/session-bootstrap.sh`
- **Tracking Rules**: Referenced from prior session 2b05dc8b files

## Session History (12 sessions)
All sessions are indexed in the memory database. Key session IDs:
- `85bc7321` — Massive workspace build, ACE-Step, REVITHION plan (815 lines)
- `2b05dc8b` — Master plan, extensions fixed, environment profile created
- `03a38020` — 2,712 repos cloned, Ollama models, asset inventory
- `1d979ea8` — OpenSearch 138 repos, OpenAudio 284 repos
- `1d109fed` — WSL dev environment, OpenClaw, CyborgDB
- `0ffd350b` — NeMo ecosystem, CyborgDB setup
- `06ef625c` — THIS SESSION: Memory system build

## Preferences
- Everything in WSL Ubuntu Bash Linux (Windows only for audio software that requires it)
- Skills and Workspaces as Presets/Templates
- Take time, don't rush — quality over speed
- NotebookLM CLI is THE deep research tool
- AlphaXiv.org for research papers
- Models must have 1T+ parameters for complex code
