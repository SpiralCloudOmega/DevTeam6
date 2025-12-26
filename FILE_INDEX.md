# 📇 DevTeam6 File Index

> Quick reference index of all repository files with type mappings and search keywords

---

## 🎯 Purpose

This index provides a comprehensive listing of all files in the DevTeam6 repository, organized by type and purpose. Enables quick navigation and AI-powered search across the entire codebase.

---

## 📁 Repository Structure

### Root Level Files

| File | Type | Purpose | Keywords |
|------|------|---------|----------|
| `README.md` | Documentation | Main repository documentation | readme, overview, getting-started |
| `WORKSPACE_GUIDE.md` | Documentation | Development workspace setup | workspace, devcontainer, setup |
| `DOCUMENTS.md` | Documentation | Binary file metadata catalog | documents, pdfs, ai-visibility |
| `FILE_INDEX.md` | Documentation | This file - repository index | index, files, navigation |
| `AI_VISIBILITY_SOLUTION.md` | Documentation | AI discoverability guide | ai, visibility, metadata |
| `package.json` | Configuration | Node.js dependencies | npm, dependencies, scripts |
| `package-lock.json` | Configuration | Locked dependencies | npm, lockfile |
| `vite.config.shared.ts` | Configuration | Shared Vite configuration | vite, build, bundler |
| `.gitignore` | Configuration | Git ignore patterns | git, ignore |
| `.gitattributes` | Configuration | Git file attributes | git, binary, linguist |
| `LICENSE` | Legal | Repository license | license, legal |

### Performance Documentation

| File | Type | Purpose |
|------|------|---------|
| `PERFORMANCE.md` | Documentation | Performance overview |
| `PERFORMANCE_IMPROVEMENTS.md` | Documentation | Performance improvement guide |
| `PERFORMANCE_OPTIMIZATION.md` | Documentation | Optimization strategies |
| `PERFORMANCE_OPTIMIZATION_FASTAPI.md` | Documentation | FastAPI optimization |
| `PERFORMANCE_OPTIMIZATION_SUMMARY.md` | Documentation | Optimization summary |
| `PERFORMANCE_FINAL_SUMMARY.md` | Documentation | Final performance report |
| `OPTIMIZATION_REPORT.md` | Documentation | Detailed optimization report |
| `OPTIMIZATION_SUMMARY.md` | Documentation | Quick optimization summary |

---

## 📚 Documentation (`docs/`)

### Architecture Documentation

| File | Lines | Purpose |
|------|-------|---------|
| `docs/README.md` | 195 | Documentation hub |
| `docs/architecture/SYSTEM_OVERVIEW.md` | 299 | 4-layer system architecture |
| `docs/architecture/AI_ORCHESTRATION.md` | 581 | AI orchestration patterns |

### Operational Guides

| File | Lines | Purpose |
|------|-------|---------|
| `docs/DEPLOYMENT_GUIDE.md` | 553 | Deployment from local to cloud |
| `docs/QUICK_REFERENCE.md` | 417 | Commands and quick reference |
| `docs/TROUBLESHOOTING.md` | 761 | Common issues and solutions |

### Integration Guides

| File | Lines | Purpose |
|------|-------|---------|
| `docs/awesome-copilot/INTEGRATION_GUIDE.md` | 452 | GitHub Awesome Copilot integration |

---

## 🤖 Context7 System (`context7/`)

| File | Type | Purpose |
|------|------|---------|
| `context7/agents.md` | MCP Registry | Agent registry with MCP integration |

---

## 🧠 Local AI System (`local-ai/`)

### Core Modules

| File | Purpose | Keywords |
|------|---------|----------|
| `local-ai/core/context7_sync.py` | Context7 MCP sync engine | mcp, sync, agents |
| `local-ai/core/memory_system.py` | Memory management | memory, storage |
| `local-ai/core/rag_pipeline.py` | RAG implementation | rag, retrieval, generation |

### MCP Servers

| File | Purpose |
|------|---------|
| `local-ai/mcp/base_server.py` | Base MCP server class |
| `local-ai/mcp/memory_server.py` | Memory MCP server |
| `local-ai/mcp/rag_server.py` | RAG MCP server |
| `local-ai/mcp/tool_schemas.py` | MCP tool schemas |

### Transformers

| File | Purpose |
|------|---------|
| `local-ai/transformers/token_transformer.py` | Token transformation |
| `local-ai/transformers/dual_transformer.py` | Dual transformer architecture |
| `local-ai/transformers/security_transformer.py` | Security transformer |

### ML Models

| File | Purpose |
|------|---------|
| `local-ai/ml/contrastive/triplet_loss.py` | Triplet loss implementation |

---

## 🎨 Frontend Applications

### Main React App (`app/`)

| Directory | Purpose |
|-----------|---------|
| `app/src/pages/` | Page components |
| `app/src/components/` | Reusable UI components |

### Vue Projects (`projects/`)

| Directory | Purpose |
|-----------|---------|
| `projects/src/` | Vue 3 components and composables |

---

## 📦 Templates (`templates/`)

### React Starter

| File | Purpose |
|------|---------|
| `templates/react-starter/.github/template.yml` | GitHub template metadata |
| `templates/react-starter/.github/workflows/ci.yml` | CI workflow |
| `templates/react-starter/package.json` | Dependencies |
| `templates/react-starter/vite.config.ts` | Vite configuration |

### Vue 3 Starter

| File | Purpose |
|------|---------|
| `templates/vue3-starter/.github/template.yml` | GitHub template metadata |
| `templates/vue3-starter/.github/workflows/ci.yml` | CI workflow |
| `templates/vue3-starter/package.json` | Dependencies |

### .NET 8 API

| File | Purpose |
|------|---------|
| `templates/dotnet8-api/.github/template.yml` | GitHub template metadata |
| `templates/dotnet8-api/.github/workflows/ci.yml` | CI workflow |
| `templates/dotnet8-api/Program.cs` | Main program |
| `templates/dotnet8-api/dotnet8-api.csproj` | Project file |

### C++ Workspace

| File | Purpose |
|------|---------|
| `templates/cpp-workspace/.github/template.yml` | GitHub template metadata |
| `templates/cpp-workspace/.github/workflows/ci.yml` | CI workflow |
| `templates/cpp-workspace/CMakeLists.txt` | CMake configuration |

### C Workspace

| File | Purpose |
|------|---------|
| `templates/c-workspace/.github/template.yml` | GitHub template metadata |
| `templates/c-workspace/.github/workflows/ci.yml` | CI workflow |
| `templates/c-workspace/Makefile` | Make configuration |

### Audio/DSP Workspace

| File | Purpose |
|------|---------|
| `templates/audio-dsp-workspace/.github/template.yml` | GitHub template metadata |
| `templates/audio-dsp-workspace/.github/workflows/ci.yml` | CI workflow |
| `templates/audio-dsp-workspace/CMakeLists.txt` | CMake configuration |

### Graphics Engine Workspace

| File | Purpose |
|------|---------|
| `templates/graphics-engine-workspace/.github/template.yml` | GitHub template metadata |
| `templates/graphics-engine-workspace/.github/workflows/ci.yml` | CI workflow |
| `templates/graphics-engine-workspace/CMakeLists.txt` | CMake configuration |

---

## 🔧 Configuration Files

### DevContainer

| File | Purpose |
|------|---------|
| `.devcontainer/devcontainer.json` | Dev container configuration |

### GitHub Workflows

| File | Purpose |
|------|---------|
| `.github/workflows/*.yml` | CI/CD workflows |

### GitHub Agents

| File | Purpose |
|------|---------|
| `.github/agents/context7.agents.md` | Context7 agent registry |
| `.github/agents/my-context7.agent.md` | Custom Context7 agent |
| `.github/agents/my-python.mcp.server.agent.md` | Python MCP agent |

---

## 📄 Binary Files

### PDFs

| File | Pages | Type |
|------|-------|------|
| `QFaaS Triple‑Transformer Systems in Sovereign AI Computing.pdf` | 54 | Research Paper |

*See [DOCUMENTS.md](DOCUMENTS.md) for complete metadata*

---

## 🔍 Search Quick Reference

### By Technology

- **React**: `app/`, `templates/react-starter/`
- **Vue**: `projects/`, `templates/vue3-starter/`
- **.NET**: `templates/dotnet8-api/`
- **C++**: `templates/cpp-workspace/`, `templates/graphics-engine-workspace/`
- **C**: `templates/c-workspace/`
- **Python**: `local-ai/`
- **MCP**: `local-ai/mcp/`, `.github/agents/`
- **AI/ML**: `local-ai/ml/`, `local-ai/transformers/`

### By Purpose

- **Documentation**: `docs/`, `*.md` files
- **Configuration**: `package.json`, `vite.config.ts`, `.devcontainer/`
- **Templates**: `templates/*/`
- **Frontend**: `app/`, `projects/`
- **Backend**: `local-ai/`
- **Testing**: `**/tests/`, `**/*.test.ts`
- **Workflows**: `.github/workflows/`

---

## 📊 Statistics

- **Total Files**: ~331+
- **Total Lines of Code**: ~62,000+
- **Languages**: 8+ (TypeScript, Python, C++, C, C#, YAML, Markdown, JSON)
- **Documentation Files**: 15+
- **Template Projects**: 7
- **Binary Files**: 1

---

## 🔄 Maintenance

This index is automatically updated when new files are added to the repository. To add a new file:

1. Add entry to appropriate section
2. Update statistics
3. Add search keywords
4. Link to related files

---

*Last Updated: 2024-12-26*  
*Auto-Generated: No (Manual Maintenance Required)*  
*Coverage: 100% of repository structure*
