# 🤖 DevTeam6 Agent Registry

> Every agent MUST read this file and update it when adding capabilities. 

## Active Agents

| Agent | Branch | Status |
|-------|--------|--------|
| Documentation Agent | copilot/connect-ai-qfaas-github | Active |
| Workspace Agent | copilot/fix-ui-ux-graphs | Active |
| Performance Agent | copilot/improve-slow-code-performance | Complete |
| UX/UI Agent | copilot/fine-tune-ux-ui-elements | Pending |

## Project Structure

| Folder | Purpose |
|--------|---------|
| app/ | React 18 + Three.js Workstation |
| projects/ | Vue 3 Workflow Editor |
| templates/ | Codespace starters |
| local-ai/ | Python AI services |
| context7/ | Shared agent context |

## Design System

### Colors
- Primary: #6366f1 (Indigo)
- Accent: #22d3ee (Cyan)
- Background: #0f0f23
- Text: #e2e8f0

### Ports
- 3000: React App
- 5173: Vue/React Templates
- 5000: .NET API

## Roadmap

- [x] Codespace templates
- [x] DevContainer workspace
- [x] Rich UX/UI components
- [x] Code Workstation with embedded IDE
- [x] VS Code Web integration (vscode.dev)
- [x] StackBlitz with live preview
- [x] Sandpack code playground
- [x] Professional template configurations
- [x] Phase 4: Dashboard, Templates, Settings
- [x] Global UI integration (AnimatedBackground, CommandPalette, FloatingDock)
- [x] Version 3.5.0
- [ ] Phase 5: Learning Hub (future)
- [ ] WebLLM integration (future)

## Recent Agent Activity

### Documentation Agent (2024-12-15)
**Branch**: `copilot/connect-ai-qfaas-github`  
**Commits**: c34b855, 186e92e  
**Work Completed**:
- Created comprehensive architecture documentation (1,519 lines across 4 files)
- Documented 4-layer system architecture (Knowledge, AI, Apps, DevEx)
- Added QFaaS-inspired AI orchestration patterns (3-tier: Cognitive, Processing, Storage)
- Created GitHub Awesome Copilot integration guide (150+ agents, MCP servers)
- Updated main README with documentation section
- All docs include Mermaid diagrams and code examples

**Next Agent Notes**:
- docs/ structure ready for additional guides (deployment, troubleshooting)
- Architecture documented: ~62k LOC across 331 files
- Local AI system: FastAPI + ChromaDB + Ollama + RAG pipeline
- Interactive apps: Vue 3 + React 18 + Three.js
- All documentation follows existing cyberpunk color scheme

Last Updated: 2024-12-15
