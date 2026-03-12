# DevTeam6 - AGENTS.md

> **AI Agent Context File** — Essential information for AI coding agents working on the DevTeam6 repository

---

## Project Overview

**DevTeam6** ("The Omega Tool Kit") is a comprehensive software development encyclopedia and AI-powered multi-agent workspace. It serves as both a curated resource collection (1000+ tools, frameworks, and libraries) and a functional development environment with interactive web applications, code templates, and an advanced AI agent orchestration system.

### Key Characteristics

- **Primary Language**: English (all documentation and comments)
- **Repository Type**: Monorepo with multiple sub-projects
- **License**: MIT License (Copyright 2025 The flamekeeper)
- **GitHub**: `SpiralCloudOmega/DevTeam6`
- **Live Site**: https://spiralcloudomega.github.io/DevTeam6/

### Mission

Transform static repository documentation into a living digital workstation with AI-powered repository analysis, interactive documentation, and intelligent code understanding.

---

## Technology Stack

### Frontend Applications

| Component | Technology | Purpose |
|-----------|------------|---------|
| `app/` | React 18 + TypeScript + Vite | Main 3D demo, GenUI Playground, Node Graph, Dashboards |
| `projects/` | Vue 3 + TypeScript + Vite | Visual workflow automation (n8n-style graph editor) |
| `templates/react-starter` | React 18 + Vite | Starter template (Port 3000) |
| `templates/vue3-starter` | Vue 3 + Pinia + Router | Starter template (Port 5173) |

**Key Frontend Libraries**:
- Three.js + React Three Fiber (3D visualization)
- GSAP (animations)
- Framer Motion (UI animations)
- Chart.js (data visualization)
- Vue Flow (node graph editor)
- Sandpack (live code editor)

### Backend & API

| Component | Technology | Purpose |
|-----------|------------|---------|
| `templates/dotnet8-api` | ASP.NET Core 8 + Swagger | REST API template (Port 5000) |
| `local-ai/` | Python + FastAPI | AI/ML backend with RAG, MCP servers |
| `local-ai/` | PyTorch, Transformers | ML inference and training |

**Key Python Libraries**:
- FastAPI (API framework)
- PyTorch, transformers, sentence-transformers (ML)
- LangChain, LangGraph (AI orchestration)
- OpenSearch (vector search)
- CyborgDB (encrypted vector database)

### AI Agent System

| Component | Technology | Purpose |
|-----------|------------|---------|
| `.github/omega/` | S-expressions, Python, Shell | Multi-agent orchestration layer |
| `.github/agents/` | Markdown configs | 200+ specialized AI agent definitions |
| `.github/skills/` | Markdown docs | Reusable skill modules |

### Development Environment

- **Node.js**: v22+ (via nvm)
- **Python**: 3.13+ (via miniconda)
- **Runtime**: WSL2 Ubuntu 24.04 on Windows 11
- **GPU**: CUDA 13.1 (RTX 3090)

---

## Project Structure

```
DevTeam6/
├── README.md                          # Main documentation (1000+ resources)
├── AGENTS.md                          # This file
├── package.json                       # Root workspace configuration
│
├── app/                               # 🔷 React/Three.js main application
│   ├── src/
│   │   ├── App.tsx                    # Main 3D vortex component
│   │   ├── pages/                     # Route pages (16 interactive experiences)
│   │   │   ├── GenUIPlayground.tsx    # AI-powered UI generator
│   │   │   ├── NodeGraphEditor.tsx    # n8n-style workflow editor
│   │   │   ├── SemanticKnowledgeHub.tsx
│   │   │   ├── GamificationDashboard.tsx
│   │   │   ├── Repomind.tsx           # AI repository workstation
│   │   │   └── ...
│   │   ├── components/                # Shared UI components
│   │   └── utils/                     # Utility functions + tests
│   └── package.json                   # React 18 + Vite + Three.js
│
├── projects/                          # 📊 Vue 3 Node Graph Editor
│   ├── src/
│   │   ├── components/graph/          # Node editor components
│   │   └── stores/workflow.ts         # Pinia state management
│   └── package.json                   # Vue 3 + Vue Flow
│
├── templates/                         # 📦 Development workspace templates
│   ├── react-starter/                 # React 18 + TypeScript + Vite
│   ├── vue3-starter/                  # Vue 3 + Router + Pinia
│   ├── dotnet8-api/                   # ASP.NET Core 8 Web API
│   ├── cpp-workspace/                 # C++20 + CMake + DevContainer
│   ├── c-workspace/                   # C17 + Makefile + DevContainer
│   ├── audio-dsp-workspace/           # JUCE + VST3 + DevContainer
│   └── graphics-engine-workspace/     # Vulkan + GLFW + DevContainer
│
├── local-ai/                          # 🤖 Python AI/ML backend
│   ├── api/main.py                    # FastAPI entry point
│   ├── agents/                        # Agent orchestrator
│   ├── core/                          # RAG pipeline, memory, vector store
│   ├── mcp/                           # MCP (Model Context Protocol) servers
│   └── ml/                            # ML modules (ensemble, RL, etc.)
│
├── .github/                           # 🤖 GitHub configuration & AI agents
│   ├── workflows/                     # CI/CD workflows (7 files)
│   │   ├── deploy-pages.yml           # Deploy 3D demo to GitHub Pages
│   │   ├── cvs-sync.yml               # Hourly CVS sync
│   │   ├── roadmap-sync.yml           # 6-hour roadmap sync
│   │   └── ...
│   ├── agents/                        # 200+ AI agent definitions (.agent.md)
│   ├── skills/                        # Reusable skill modules
│   │   ├── agent-orchestration/
│   │   ├── ai-integration/
│   │   ├── audio-dsp/
│   │   ├── cpp-patterns/
│   │   └── ...
│   ├── omega/                         # 🔱 Omega multi-agent system
│   │   ├── omega-env.sh               # Environment configuration
│   │   ├── macros/                    # ChrysaLisp S-expression macros
│   │   ├── agents/                    # Omega-specific agents
│   │   ├── skills/                    # Omega skills (orchestration, critique)
│   │   └── bridge/                    # HTTP bridge for terminal dispatch
│   └── copilot-instructions.md        # GitHub Copilot master instructions
│
├── .devcontainer/                     # 🐳 GitHub Codespaces configuration
│   ├── devcontainer.json              # Node 20, .NET 8, Docker
│   └── post-create.sh                 # Auto-setup script
│
├── context7/                          # Agent context documentation
│
├── awesome-* (100+ directories)       # 📚 Curated awesome lists
│   # awesome-python, awesome-rust, awesome-ai-agents, etc.
│
├── ACE-Step*/                         # 🎵 AI music generation project
├── openclaw-main/                     # 🎮 OpenClaw game reimplementation
├── openDAW/                           # 🎹 Web-based DAW
├── radium-master/                     # 🎛️ Radium music tracker
├── voicebox-main/                     # 🗣️ Voice synthesis
├── moonshine-main/                    # 🌙 Moonshine speech recognition
│
└── [Root Documentation Files]
    ├── DOCUMENTS.md                   # PDF/research paper catalog
    ├── FILE_INDEX.md                  # AI file discovery guide
    ├── WORKSPACE_GUIDE.md             # Development setup guide
    ├── ASSET_INVENTORY.md             # Complete asset inventory
    ├── START_HERE.md                  # Open Claude Cowork research
    └── *.pdf                          # Technical books and research papers
```

---

## Build and Development Commands

### Root Workspace Commands

```bash
# Install all dependencies
npm run install:all

# Run all applications simultaneously
npm run dev                          # React (3000) + Vue (5173) + .NET (5000)
npm run dev:frontend                 # React + Vue only
npm run dev:app                      # Main app only
npm run dev:projects                 # Projects folder only

# Build all templates
npm run build

# Clean all node_modules and build outputs
npm run clean
```

### Individual Template Commands

```bash
# React Starter (Port 3000)
cd templates/react-starter
npm install
npm run dev
npm run build

# Vue 3 Starter (Port 5173)
cd templates/vue3-starter
npm install
npm run dev
npm run build

# .NET 8 API (Port 5000)
cd templates/dotnet8-api
dotnet restore
dotnet run
dotnet watch run
```

### Python AI Backend Commands

```bash
cd local-ai

# Install dependencies
pip install -r requirements.txt

# Run FastAPI server
uvicorn api.main:app --reload --port 8000

# Run tests
pytest tests/
```

### Omega Agent System Commands

```bash
# Source Omega environment
source .github/omega/omega-env.sh

# Generate configs from S-expression macros
python .github/omega/macros/generate-omega.py --all

# Start HTTP bridge
python .github/omega/bridge/http-bridge.py &

# Check health
curl http://localhost:18800/health
```

---

## Code Style Guidelines

### JavaScript/TypeScript (React/Vue)

- **Formatter**: Prettier with default config
- **Linter**: ESLint with React/Vue plugins
- **TypeScript**: Strict mode enabled
- **Naming**:
  - Components: PascalCase (e.g., `GenUIPlayground.tsx`)
  - Utilities: camelCase (e.g., `controlDeck.ts`)
  - Constants: UPPER_SNAKE_CASE
- **Imports**: Group by external → internal → relative
- **Comments**: JSDoc for public APIs

### Python

- **Style**: PEP 8 compliant
- **Type Hints**: Required for function signatures
- **Docstrings**: Google-style docstrings
- **Naming**:
  - Classes: PascalCase
  - Functions/variables: snake_case
  - Constants: UPPER_SNAKE_CASE

### C++

- **Standard**: C++20
- **Build System**: CMake 3.28+
- **Naming**:
  - Classes: PascalCase
  - Functions: camelCase
  - Variables: snake_case or m_camelCase for members
  - Macros: UPPER_SNAKE_CASE

### C# (.NET)

- **Standard**: C# 12 with .NET 8
- **Naming**: PascalCase for methods/classes, camelCase for variables

---

## Testing Strategy

### Frontend Testing

```bash
# React/Vue unit tests with Vitest
cd app && npm run test
cd projects && npm run test

# Linting
cd templates/react-starter && npm run lint
cd templates/vue3-starter && npm run lint
```

### Python Testing

```bash
cd local-ai
pytest tests/ -v
```

### VS Code Debug Configurations

The workspace includes pre-configured debug setups:
- React App (Chrome debugger, port 3000)
- Vue 3 App (Chrome debugger, port 5173)
- .NET API (CoreCLR debugger, port 5000)
- Full Stack (all apps simultaneously)

---

## CI/CD and Deployment

### GitHub Actions Workflows

| Workflow | Trigger | Purpose |
|----------|---------|---------|
| `deploy-pages.yml` | Push to `main` (app/** changes) | Deploy 3D demo to GitHub Pages |
| `static.yml` | Push to `main` | Deploy static content |
| `cvs-sync.yml` | Hourly cron | Sync CVS data |
| `roadmap-sync.yml` | 6-hour cron | Update roadmap badges |
| `node-graph-sync.yml` | 4-hour cron | Sync node graph stats |
| `auto-update.yml` | Scheduled | Auto-update dependencies |
| `auto-merge.yml` | PR events | Auto-merge eligible PRs |

### Deployment Targets

- **GitHub Pages**: https://spiralcloudomega.github.io/DevTeam6/
- **3D Demo**: https://spiralcloudomega.github.io/DevTeam6/
- **GenUI**: https://spiralcloudomega.github.io/DevTeam6/genui
- **Node Graph**: https://spiralcloudomega.github.io/DevTeam6/node-graph
- **Roadmap**: https://spiralcloudomega.github.io/DevTeam6/roadmap

---

## Key Configuration Files

| File | Purpose |
|------|---------|
| `package.json` | Root workspace scripts and dependencies |
| `vite.config.shared.ts` | Shared Vite configuration |
| `devteam6-workspace.code-workspace` | VS Code multi-root workspace |
| `.devcontainer/devcontainer.json` | GitHub Codespaces configuration |
| `.github/copilot-instructions.md` | GitHub Copilot custom instructions |
| `.github/omega/omega-env.sh` | Omega agent environment variables |
| `.github/omega/omega.config.sexp` | Omega master configuration (S-expressions) |

---

## Security Considerations

### Environment Variables

- Never commit sensitive data to the repository
- Use `.env.local` files (already gitignored)
- For Omega: Use `.github/memory/spiralcloud-memory.db` for session tracking

### API Keys and Secrets

- Store in environment variables
- For local-ai: Use `CYBORGDB_API_KEY` format
- For Ollama: No API key required for local models

### CORS Configuration

- .NET API has CORS enabled for development (`AllowAnyOrigin()`)
- For production: Restrict to specific domains

---

## AI Agent Context

### Omega Agent System

The repository includes a sophisticated multi-agent orchestration system (Omega) that:

1. **Decomposes tasks** into DAGs of sub-tasks
2. **Dispatches** to specialist agents in parallel (100+ agents)
3. **Runs critique councils** for quality gates
4. **Commits verified output** autonomously

### Available Agent Types

- **Orchestrator**: Task decomposition and scheduling
- **Critic**: Quality review and feedback
- **Builder**: Implementation agents
- **Researcher**: Information gathering
- **Security**: Security analysis

### Skill Modules

Located in `.github/skills/`:
- `agent-orchestration/` - Multi-agent coordination patterns
- `ai-integration/` - AI model integration (ACE-Step, Whisper, etc.)
- `audio-dsp/` - Digital signal processing
- `cpp-patterns/` - C++ design patterns
- `juce-framework/` - JUCE audio framework
- `music-production/` - Music production techniques
- `neural-audio/` - Neural network audio processing

### GitHub Copilot Agents

Located in `.github/agents/` (200+ specialized agents):
- Language-specific agents (react.agent.md, python.agent.md, rust.agent.md)
- Framework agents (nextjs.agent.md, fastapi.agent.md)
- Task agents (my-research.agent.md, my-bugfixer.agent.md)

---

## Development Workflow

### Getting Started

1. **Clone and setup**:
   ```bash
   git clone https://github.com/SpiralCloudOmega/DevTeam6.git
   cd DevTeam6
   npm run install:all
   ```

2. **Open workspace**:
   ```bash
   code devteam6-workspace.code-workspace
   ```

3. **Run development servers**:
   ```bash
   npm run dev
   ```

4. **Access applications**:
   - React: http://localhost:3000
   - Vue: http://localhost:5173
   - .NET API: http://localhost:5000/swagger

### Making Changes

1. Create a feature branch
2. Make changes in the appropriate directory
3. Test locally with `npm run dev`
4. Run linters: `npm run lint`
5. Commit with descriptive messages
6. Push and create PR

### Commit Message Conventions

- `feat:` New feature
- `fix:` Bug fix
- `docs:` Documentation changes
- `style:` Code style changes (formatting)
- `refactor:` Code refactoring
- `test:` Test additions/changes
- `chore:` Build/tooling changes

---

## Troubleshooting

### Common Issues

**Port already in use**:
```bash
lsof -i :3000
kill -9 <PID>
```

**Dependencies not installing**:
```bash
npm cache clean --force
npm run clean
npm run install:all
```

**.NET build fails**:
```bash
cd templates/dotnet8-api
dotnet clean
dotnet restore
dotnet build
```

### Getting Help

- Check `WORKSPACE_GUIDE.md` for setup issues
- Check `START_HERE.md` for Open Claude Cowork research
- Create an issue on GitHub for bugs

---

## External Resources

### Documentation

- [README.md](README.md) - Main repository documentation
- [WORKSPACE_GUIDE.md](WORKSPACE_GUIDE.md) - Development workspace guide
- [DOCUMENTS.md](DOCUMENTS.md) - PDF and research paper catalog
- [FILE_INDEX.md](FILE_INDEX.md) - AI file discovery guide
- [ASSET_INVENTORY.md](ASSET_INVENTORY.md) - Complete asset inventory

### Live Demos

- Main Site: https://spiralcloudomega.github.io/DevTeam6/
- 3D Demo: https://spiralcloudomega.github.io/DevTeam6/
- GenUI: https://spiralcloudomega.github.io/DevTeam6/genui
- Node Graph: https://spiralcloudomega.github.io/DevTeam6/node-graph

---

## Notes for AI Agents

1. **This is a resource repository** - Most directories are curated collections (awesome lists). Be careful not to modify these unless explicitly instructed.

2. **Main development areas**:
   - `app/` - React/Three.js application
   - `projects/` - Vue 3 workflow editor
   - `templates/` - Starter templates
   - `local-ai/` - Python AI backend
   - `.github/omega/` - Agent orchestration system

3. **Testing is minimal** - The project focuses on documentation and examples rather than comprehensive test suites.

4. **GitHub Pages deployment** - The `app/` directory is built and deployed automatically on push to `main`.

5. **Large files** - Many PDFs and binary files exist in the root. Use `DOCUMENTS.md` for metadata about these files.

6. **Memory system** - Omega uses `.github/memory/` for session tracking. Check this for context from previous sessions.

---

*Last updated: 2026-03-11*
*For questions or updates to this file, create an issue or PR.*
