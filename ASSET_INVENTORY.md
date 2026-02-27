# DevTeam6 — Complete Asset Inventory

> Generated: 2026-02-25 | Environment: WSL2 Ubuntu 24.04 + VS Code on Windows
> VS Code Workspace: `C:\Users\autom\VS Code GitHub Repos\DevTeam6`
> WSL Home: `/home/autom` | WSL Projects: `~/projects/`

---

## 🖥️ Environment & Toolchain

| Tool | Version | Path |
|------|---------|------|
| OS | Ubuntu 24.04.4 LTS | WSL2 Kernel 6.6.87.2 |
| GPU | RTX 3090 24GB | CUDA 13.1, Driver 591.86 |
| Node.js | v22.22.0 (nvm) | `~/.nvm/versions/node/v22.22.0` |
| npm | 10.9.4 | via nvm |
| Python | 3.13.11 | `/usr/bin/python3` |
| pip | 25.3 | `~/.local/bin/pip3` |
| Go | 1.22.2 | `/usr/local/go/bin/go` |
| Rust | 1.93.1 | `~/.cargo/bin/rustc` |
| Java | OpenJDK 21.0.10 | `/usr/bin/java` |
| Docker | 27.5.1 | `/usr/bin/docker` (Docker Desktop WSL2) |
| gh CLI | 2.87.2 | `/usr/bin/gh` |
| Copilot CLI | 0.0.418 | `~/.local/bin/copilot` |
| OpenClaw | v2026.2.21-2 | `/usr/bin/openclaw` |
| Jules CLI | installed | `/usr/bin/jules` |
| pyenv | 2.6.23 | `~/.pyenv/` |
| uv | latest | `~/.local/bin/uv` |
| poetry | installed | `~/.local/bin/poetry` |
| gradle | 8.8 | via SDKMAN |
| maven | installed | `/usr/bin/mvn` |
| cmake | installed | `/usr/bin/cmake` |
| clang | 18 | `/usr/bin/clang` |
| gcc | installed | `/usr/bin/gcc` |
| ripgrep | installed | `/usr/bin/rg` |
| jq / yq | installed | `/usr/bin/jq`, `~/.local/bin/yq` |

### Key Services

| Service | Port | Status |
|---------|------|--------|
| CyborgDB | 8100 | Not running (start with `cyborgdb-service`) |
| OpenClaw Gateway | 18789 | ✅ Running (systemd) |
| PostgreSQL 16 | 5432 | Installed (pgvector enabled) |

### Key Environment Variables (`~/.bashrc`)

```bash
CYBORGDB_API_KEY="cyborg_bfaea874c10d48bab8b69e4bc29da994"
CYBORGDB_SERVICE_URL="http://localhost:8100"
CYBORGDB_DB_TYPE="postgres"
CYBORGDB_CONNECTION_STRING="host=localhost port=5432 dbname=cyborgdb user=cyborgdb password=cyborgdb_secure_2024"
```

### Kali Linux (Separate WSL2 Distro)

- User: `REVITHION`
- kali-linux-everything installed (600+ tools)
- Win-KeX 3.1.3, Gemini CLI 0.11.0, rockyou.txt decompressed
- Access from Ubuntu: `wsl.exe -d kali-linux -- bash -c "command"`

---

## 📁 DevTeam6 Main Repo

**Path (Windows):** `C:\Users\autom\VS Code GitHub Repos\DevTeam6`
**Path (WSL):** `/mnt/c/Users/autom/VS Code GitHub Repos/DevTeam6`
**GitHub:** `SpiralCloudOmega/DevTeam6` (branch: `main`)

### Root Files

| File | Description |
|------|-------------|
| `README.md` | Main repo README (185KB) |
| `package.json` | Root package config |
| `package-lock.json` | Lock file |
| `vite.config.shared.ts` | Shared Vite configuration |
| `devteam6-workspace.code-workspace` | VS Code multi-root workspace |
| `LICENSE` | License file |
| `Copilt.ChatFeed.QFaaS.Transformer.json` | QFaaS Transformer data (1.8MB) |
| `Copilt.ChatFeed.QFaaS.Transformer.md` | QFaaS Transformer docs (865KB) |
| `QFaaS Triple‑Transformer Systems in Sovereign AI Computing.pdf` | Research paper |

### Documentation (Root)

| File | Description |
|------|-------------|
| `AI_VISIBILITY_SOLUTION.md` | AI visibility architecture |
| `DOCUMENTS.md` | Documents index |
| `FILE_INDEX.md` | File index/map |
| `OPTIMIZATION_REPORT.md` | Optimization report |
| `OPTIMIZATION_SUMMARY.md` | Optimization summary |
| `PERFORMANCE.md` | Performance docs |
| `PERFORMANCE_FINAL_SUMMARY.md` | Final performance summary |
| `PERFORMANCE_IMPROVEMENTS.md` | Performance improvements log |
| `PERFORMANCE_OPTIMIZATION.md` | Optimization details |
| `PERFORMANCE_OPTIMIZATION_FASTAPI.md` | FastAPI optimization |
| `PERFORMANCE_OPTIMIZATION_SUMMARY.md` | Optimization summary |
| `WORKSPACE_GUIDE.md` | Workspace setup guide |

### `app/` — React/TypeScript Frontend

```
app/
├── index.html
├── package.json
├── src/
│   ├── App.tsx                          # Main app component
│   ├── main.tsx                         # Entry point
│   ├── index.css                        # Global styles
│   ├── components/
│   │   ├── ide/
│   │   │   ├── StackBlitzEmbed.tsx      # StackBlitz integration
│   │   │   ├── VSCodeEmbed.tsx          # VS Code embed
│   │   │   └── index.ts
│   │   └── ui/
│   │       ├── AnimatedBackground.tsx
│   │       ├── CommandPalette.tsx
│   │       ├── FloatingDock.tsx
│   │       ├── GlassPanel.tsx
│   │       ├── ThemeSwitcher.tsx
│   │       └── index.ts
│   ├── config/
│   │   └── sandpackTemplates.ts         # Sandpack code templates
│   ├── pages/
│   │   ├── AIPlayground.tsx
│   │   ├── CodeWorkstation.tsx
│   │   ├── ControlDeck.tsx
│   │   ├── Dashboard.tsx
│   │   ├── GamificationDashboard.tsx
│   │   ├── GenUIPlayground.tsx
│   │   ├── ImmersiveLanding.tsx
│   │   ├── NodeGraphEditor.tsx
│   │   ├── OnboardingWizard.tsx
│   │   ├── ProjectRoadmap.tsx
│   │   ├── RepoPilot.tsx
│   │   ├── Repomind.tsx
│   │   ├── SemanticKnowledgeHub.tsx
│   │   ├── Settings.tsx
│   │   ├── Templates.tsx
│   │   └── VideoStorytelling.tsx
│   └── utils/
│       ├── controlDeck.ts / .test.ts
│       ├── graph.ts / .test.ts
│       ├── math.ts / .test.ts
│       ├── performance.ts / .test.ts
│       ├── repomind.ts / .test.ts
│       ├── visuals.ts / .test.ts
│       └── youtube.ts / .test.ts
└── public/
    ├── 404.html
    ├── gamification.html
    ├── genui.html
    ├── knowledge-hub.html
    ├── landing.html
    ├── node-graph.html
    ├── onboarding.html
    ├── roadmap.html
    └── videos.html
```

### `projects/` — Vue 3 Node Graph Editor

```
projects/
├── package.json
├── vite.config.ts
├── tsconfig.json / tsconfig.node.json
├── postcss.config.js
├── tailwind.config.js
└── src/
    ├── App.vue
    ├── main.ts
    ├── vite-env.d.ts
    ├── styles/main.css
    ├── components/graph/
    │   ├── NodeEditor.vue
    │   └── NodePalette.vue
    ├── components/nodes/
    │   └── BaseNode.vue
    ├── router/index.ts
    ├── stores/workflow.ts
    ├── types/index.ts
    └── views/
        ├── EditorView.vue
        ├── HomeView.vue
        └── MindmapView.vue
```

### `local-ai/` — Python AI/ML Backend

```
local-ai/
├── README.md
├── requirements.txt
├── agents/
│   ├── __init__.py
│   ├── agent_orchestrator.py
│   ├── agent_registry.py
│   └── base_agent.py
├── api/
│   ├── __init__.py
│   └── main.py                          # FastAPI entry
├── config/
│   ├── __init__.py
│   ├── models.py
│   └── settings.py
├── core/
│   ├── __init__.py
│   ├── context7_sync.py
│   ├── embedding_service.py
│   ├── knowledge_graph.py
│   ├── memory_system.py
│   ├── rag_pipeline.py
│   └── vector_store.py
├── mcp/
│   ├── __init__.py
│   ├── base_server.py
│   ├── memory_server.py
│   ├── rag_server.py
│   └── tool_schemas.py
├── ml/
│   ├── __init__.py
│   ├── config.py
│   ├── advanced/                        # ensemble, neuromorphic, swarm
│   ├── anomaly/                         # autoencoder, isolation forest
│   ├── contrastive/                     # triplet loss, embeddings
│   ├── finetuning/                      # LoRA, QLoRA adapters
│   ├── meta/                            # auto-tuner, hyperparameter opt
│   ├── online/                          # drift detection, incremental
│   ├── reinforcement/                   # PPO, policy network, routing
│   └── supervised/                      # intent, threat, token classifiers
├── services/
│   ├── __init__.py
│   └── ollama_service.py
├── tests/
│   ├── __init__.py
│   └── test_core.py
├── transformers/
│   ├── __init__.py
│   ├── dual_transformer.py
│   ├── security_transformer.py
│   ├── token_transformer.py
│   └── components/
│       ├── __init__.py
│       ├── api_gateway.py
│       ├── auth_manager.py
│       ├── multi_head_attention.py
│       ├── rate_limiter.py
│       └── smart_tokenizer.py
└── utils/
    ├── __init__.py
    ├── chunking.py
    └── formatting.py
```

### `templates/` — Dev Container Templates (7)

| Template | Description |
|----------|-------------|
| `audio-dsp-workspace/` | JUCE audio DSP (C++, CMake, DevContainer) |
| `c-workspace/` | C project (Makefile + CMake, DevContainer) |
| `cpp-workspace/` | C++ project (DevContainer) |
| `dotnet8-api/` | .NET 8 API template |
| `graphics-engine-workspace/` | Graphics engine (C++, DevContainer) |
| `react-starter/` | React starter template |
| `vue3-starter/` | Vue 3 starter template |

### `context7/` — Agent Context

| File | Description |
|------|-------------|
| `agents.md` | Agent context documentation |

### `.github/` — CI/CD & Agents

#### Workflows (7)

| Workflow | File |
|----------|------|
| Auto Merge | `.github/workflows/auto-merge.yml` |
| Auto Update | `.github/workflows/auto-update.yml` |
| CVS Sync | `.github/workflows/cvs-sync.yml` |
| Deploy Pages | `.github/workflows/deploy-pages.yml` |
| Node Graph Sync | `.github/workflows/node-graph-sync.yml` |
| Roadmap Sync | `.github/workflows/roadmap-sync.yml` |
| Static | `.github/workflows/static.yml` |

#### GitHub Copilot Agents (22)

| Agent File | Specialization |
|------------|---------------|
| `context7.agents.md` | Context7 integration |
| `my-bugfixer.agent.md` | Bug fixing |
| `my-c++.agent.md` | C++ development |
| `my-content.eval.agent.md` | Content evaluation |
| `my-context7.agent.md` | Context7 |
| `my-electron.agent.md` | Electron apps |
| `my-gpt5beast.agent.md` | GPT-5 power agent |
| `my-hlbpa.agent.md` | HLBPA |
| `my-idea.generator.agent.md` | Idea generation |
| `my-javamcp.agent.md` | Java MCP |
| `my-mentor.agent.md` | Mentoring |
| `my-neo4j.agent.md` | Neo4j graph DB |
| `my-nextjs.agent.md` | Next.js |
| `my-php.agent.md` | PHP |
| `my-planner.agent.md` | Planning |
| `my-prd.agent.md` | Product requirements |
| `my-python.mcp.server.agent.md` | Python MCP server |
| `my-react.agent.md` | React |
| `my-research.agent.md` | Research |
| `my-software.expert.agent.md` | Software expertise |
| `my-specification.agent.md` | Specifications |
| `my-terraform.agent.md` | Terraform/IaC |
| `my-transcendent.coding.agent.md` | Transcendent coding |

#### Agent Personas (5): devops, master, python, react, research
#### Agent Rules (5): devops, master, python, react, research
#### Agent Logs (6): devops, master, python, react, research, shared-learnings
#### Agent Memory: `embeddings.json`, `knowledge-graph.json`
#### Badges (8): node-graph-stats, phase-* (7 phases)

### `.devcontainer/`

| File | Description |
|------|-------------|
| `devcontainer.json` | Dev container configuration |
| `post-create.sh` | Post-create setup script |

---

## 📦 WSL Repos — `~/projects/` (57 Git Repos)

### SpiralCloudOmega Repos (37)

| # | Repo | GitHub | Path |
|---|------|--------|------|
| 1 | aios-core | `SpiralCloudOmega/aios-core` | `~/projects/aios-core` |
| 2 | cagent | `SpiralCloudOmega/cagent` | `~/projects/cagent` |
| 3 | catnip | `SpiralCloudOmega/catnip` | `~/projects/catnip` |
| 4 | CopilotKit | `SpiralCloudOmega/CopilotKit` | `~/projects/CopilotKit` |
| 5 | DiligentFX | `SpiralCloudOmega/DiligentFX` | `~/projects/DiligentFX` |
| 6 | dotnet-docs-samples | `SpiralCloudOmega/dotnet-docs-samples` | `~/projects/dotnet-docs-samples` |
| 7 | erhe | `SpiralCloudOmega/erhe` | `~/projects/erhe` |
| 8 | functions-samples | `SpiralCloudOmega/functions-samples` | `~/projects/functions-samples` |
| 9 | gemini-flow | `SpiralCloudOmega/gemini-flow` | `~/projects/gemini-flow` |
| 10 | gemini-kit | `SpiralCloudOmega/gemini-kit` | `~/projects/gemini-kit` |
| 11 | gemini-plan-commands | `SpiralCloudOmega/gemini-plan-commands` | `~/projects/gemini-plan-commands` |
| 12 | gh-aw | `SpiralCloudOmega/gh-aw` | `~/projects/gh-aw` |
| 13 | gimp | `SpiralCloudOmega/gimp` | `~/projects/gimp` |
| 14 | gogcli | `SpiralCloudOmega/gogcli` | `~/projects/gogcli` |
| 15 | HeroicGamesLauncher | `SpiralCloudOmega/HeroicGamesLauncher` | `~/projects/HeroicGamesLauncher` |
| 16 | implot3d | `SpiralCloudOmega/implot3d` | `~/projects/implot3d` |
| 17 | inkscape | `SpiralCloudOmega/inkscape` | `~/projects/inkscape` |
| 18 | JavaScriptKit | `SpiralCloudOmega/JavaScriptKit` | `~/projects/JavaScriptKit` |
| 19 | kdenlive | `SpiralCloudOmega/kdenlive` | `~/projects/kdenlive` |
| 20 | kimi-cli | `SpiralCloudOmega/kimi-cli` | `~/projects/kimi-cli` |
| 21 | lightrag | `SpiralCloudOmega/lightrag` | `~/projects/lightrag` |
| 22 | numpy | `SpiralCloudOmega/numpy` | `~/projects/numpy` |
| 23 | open-webui | `SpiralCloudOmega/open-webui` | `~/projects/open-webui` |
| 24 | openDAW | `SpiralCloudOmega/openDAW` | `~/projects/openDAW` |
| 25 | panda3d | `SpiralCloudOmega/panda3d` | `~/projects/panda3d` |
| 26 | playwright-cli | `SpiralCloudOmega/playwright-cli` | `~/projects/playwright-cli` |
| 27 | python-docs-samples | `SpiralCloudOmega/python-docs-samples` | `~/projects/python-docs-samples` |
| 28 | pytorch | `SpiralCloudOmega/pytorch` | `~/projects/pytorch` |
| 29 | qRemote | `SpiralCloudOmega/qRemote` | `~/projects/qRemote` |
| 30 | ralph | `SpiralCloudOmega/ralph` | `~/projects/ralph` |
| 31 | RendererEngine | `SpiralCloudOmega/RendererEngine` | `~/projects/RendererEngine` |
| 32 | run-long-command | `SpiralCloudOmega/run-long-command` | `~/projects/run-long-command` |
| 33 | scipy | `SpiralCloudOmega/scipy` | `~/projects/scipy` |
| 34 | self-command | `SpiralCloudOmega/self-command` | `~/projects/self-command` |
| 35 | syncthing | `SpiralCloudOmega/syncthing` | `~/projects/syncthing` |
| 36 | vitejs | `SpiralCloudOmega/vitejs` | `~/projects/vitejs` |
| 37 | WasmKit | `SpiralCloudOmega/WasmKit` | `~/projects/WasmKit` |

### Upstream / Third-Party Repos (24)

| # | Repo | GitHub | Path |
|---|------|--------|------|
| 1 | A2A | `a2aproject/A2A` | `~/projects/A2A` |
| 2 | ace-step-ui | `fspecii/ace-step-ui` | `~/projects/ace-step-ui` |
| 3 | adk-python | `google/adk-python` | `~/projects/adk-python` |
| 4 | autogen | `microsoft/autogen` | `~/projects/autogen` |
| 5 | crewAI | `crewAIInc/crewAI` | `~/projects/crewAI` |
| 6 | dynamo | `ai-dynamo/dynamo` | `~/projects/dynamo` |
| 7 | fastapi | `tiangolo/fastapi` | `~/projects/fastapi` |
| 8 | langchain | `langchain-ai/langchain` | `~/projects/langchain` |
| 9 | llama_index | `run-llama/llama_index` | `~/projects/llama_index` |
| 10 | mem0 | `mem0ai/mem0` | `~/projects/mem0` |
| 11 | minio | `minio/minio` | `~/projects/minio` |
| 12 | modelcontextprotocol | `modelcontextprotocol/modelcontextprotocol` | `~/projects/modelcontextprotocol` |
| 13 | NeMo-Agent-Toolkit | `SpiralCloudOmega/NeMo-Agent-Toolkit` | `~/projects/NeMo-Agent-Toolkit` |
| 14 | opentelemetry-python | `open-telemetry/opentelemetry-python` | `~/projects/opentelemetry-python` |
| 15 | phoenix | `arize-ai/phoenix` | `~/projects/phoenix` |
| 16 | ragas | `explodinggradients/ragas` | `~/projects/ragas` |
| 17 | redis-py | `redis/redis-py` | `~/projects/redis-py` |
| 18 | sdk-python | `strands-agents/sdk-python` | `~/projects/sdk-python` |
| 19 | semantic-kernel | `microsoft/semantic-kernel` | `~/projects/semantic-kernel` |
| 20 | uv | `astral-sh/uv` | `~/projects/uv` |
| 21 | weave | `wandb/weave` | `~/projects/weave` |
| 22 | youtube-to-docs | `SpiralCloudOmega/youtube-to-docs` | `~/projects/youtube-to-docs` |
| 23 | zvec | `SpiralCloudOmega/zvec` | `~/projects/zvec` |
| 24 | antigravity-awesome-skills | `sickn33/antigravity-awesome-skills` | `~/projects/antigravity-awesome-skills` |

### Standalone Repos (1)

| # | Repo | GitHub | Path |
|---|------|--------|------|
| 1 | stitch-skills | `SpiralCloudOmega/stitch-skills` | `~/projects/stitch-skills` |

### Non-Repo Files in `~/projects/`

| File | Type |
|------|------|
| `Miniconda3-latest-Linux-x86_64.sh` | Miniconda installer |
| `awscliv2.zip` | AWS CLI v2 installer |
| `cuda-repo-wsl-ubuntu-13-1-local_13.1.1-1_amd64.deb` | CUDA 13.1 local repo |
| `dotnet-install.sh` | .NET install script |
| `linux-install.sh` | Linux install script |
| `nodesource_setup.sh` | NodeSource setup |
| `firebase-debug.log` | Firebase debug log |
| `cs` | Coursier binary (ELF) |
| `kubectl` | Kubernetes CLI (ELF) |
| `OpenAudio/` | Directory (no git) |
| `aws/` | AWS CLI directory |
| `bin/` | Local binaries |
| `y/` | Misc directory |

---

## 🔧 AI/ML Python Stack (installed via pip)

| Package | Version |
|---------|---------|
| PyTorch | 2.10.0 |
| transformers | 5.2.0 |
| sentence-transformers | 5.2.3 |
| opensearch-py | 3.1.0 |
| opensearch-py-ml | latest |
| LangChain | latest |
| LangGraph | latest |
| langchain-nvidia-ai-endpoints | 1.0.4 |
| cyborgdb + cyborgdb-service | 0.14.0 |
| scikit-learn | 1.8.0 |
| scipy | 1.17.1 |

---

## 🔎 OpenSearch Project — `~/projects/opensearch/` (138 repos)

Full clone of the [opensearch-project](https://github.com/opensearch-project) GitHub org.
Organized into 9 categories. All cloned with `--depth 1` for space efficiency (7.1GB total).

### `core/` — Search Engine & Dashboards (2 repos)

| Repo | Language | Stars | Description |
|------|----------|-------|-------------|
| OpenSearch | Java | 12,443 | Distributed RESTful search engine |
| OpenSearch-Dashboards | TypeScript | 2,005 | Visualization dashboards |

### `clients/` — Language Clients (10 repos)

| Repo | Language | Description |
|------|----------|-------------|
| opensearch-py | Python | Python client |
| opensearch-go | Go | Go client |
| opensearch-js | JavaScript | Node.js client |
| opensearch-java | Java | Java client |
| opensearch-rs | Rust | Rust client |
| opensearch-net | C# | .NET client |
| opensearch-php | PHP | PHP client |
| opensearch-ruby | Ruby | Ruby client |
| opensearch-cli | Go | CLI tool |
| opensearch-ruby-aws-sigv4 | Ruby | AWS SigV4 wrapper for Ruby |

### `ai-ml/` — AI, ML & Vector Search (13 repos)

| Repo | Language | Stars | Description |
|------|----------|-------|-------------|
| k-NN | Java | 206 | k-nearest neighbors vector search |
| ml-commons | Java | 143 | Common ML algorithms (k-means, etc.) |
| neural-search | Java | 112 | Neural/vector search (text→vectors) |
| opensearch-py-ml | Python | 55 | Python ML extensions |
| flow-framework | Java | 50 | AI application builder framework |
| skills | Java | 15 | ML agent skills |
| anomaly-detection | Java | 87 | Anomaly detection with auto-notifications |
| opensearch-mcp-server-py | Python | 102 | MCP server for OpenSearch |
| opensearch-jvector | Java | 21 | JVector integration |
| remote-vector-index-builder | Python | 8 | Remote vector index builder |
| skills-eval | TypeScript | 4 | Skills evaluation framework |
| agent-health | TypeScript | 3 | Agent health monitoring |
| oscar-ai-bot | Python | 1 | AI chatbot (AWS Bedrock) |

### `plugins/` — Server-Side Plugins (25 repos)

| Repo | Language | Stars | Description |
|------|----------|-------|-------------|
| security | Java | 236 | TLS, auth backends, data masking |
| sql | Java | 163 | SQL & PPL query language |
| alerting | Kotlin | 78 | Monitor-based alerting |
| index-management | Kotlin | 73 | Periodic data operations |
| observability | Kotlin | 64 | Logs, traces, metrics visualization |
| cross-cluster-replication | Kotlin | 63 | Cross-cluster data sync |
| security-analytics | Java | 95 | Security threat detection |
| geospatial | Java | 46 | Geospatial features |
| notifications | Kotlin | 38 | Notification routing |
| query-insights | Java | 25 | Query analysis & optimization |
| user-behavior-insights | Java | 34 | User behavior analytics |
| search-processor | Java | 26 | Search request pipeline |
| asynchronous-search | Java | 33 | Background query execution |
| reporting | Kotlin | 32 | PNG/PDF/CSV report export |
| search-relevance | Java | 13 | Search relevance tuning |
| custom-codecs | Java | 14 | Custom Lucene codecs |
| job-scheduler | Java | 31 | Periodic job runner |
| opensearch-storage-encryption | Java | 9 | Data-at-rest encryption |
| opensearch-system-templates | Java | 3 | System configuration templates |
| opensearch-oci-object-storage | Java | 9 | OCI Object Storage plugin |
| simple-schema | Java | 8 | User-domain schema enhancement |
| time-series-db | Java | 8 | Time-series database |
| cluster-etcd | Java | 4 | Clusterless node config via etcd |
| opensearch-traffic-gateway | Java | 4 | Request governance gateway |
| opensearch-olap | — | 1 | OLAP analytics |

### `dashboards-plugins/` — Dashboard UI Plugins (21 repos)

| Repo | Language | Description |
|------|----------|-------------|
| security-dashboards-plugin | TypeScript | Security management UI |
| anomaly-detection-dashboards-plugin | TypeScript | Anomaly detection UI |
| alerting-dashboards-plugin | JavaScript | Alert management UI |
| dashboards-observability | TypeScript | Observability visualization |
| dashboards-search-relevance | TypeScript | Search relevance tools |
| dashboards-assistant | TypeScript | AI assistant for dashboards |
| dashboards-maps | TypeScript | Map visualization |
| dashboards-flow-framework | TypeScript | AI app designer UI |
| index-management-dashboards-plugin | TypeScript | Index management UI |
| ml-commons-dashboards | TypeScript | ML commons UI |
| dashboards-query-workbench | TypeScript | SQL/PPL query workbench |
| dashboards-notifications | TypeScript | Notification management UI |
| dashboards-visualizations | TypeScript | Custom visualizations |
| dashboards-reporting | HTML | Report generation UI |
| oui | TypeScript | OpenSearch UI Framework |
| query-insights-dashboards | TypeScript | Query insights UI |
| dashboards-investigation | TypeScript | Investigation tools |
| dashboards-i18n | TypeScript | Internationalization |
| security-analytics-dashboards-plugin | TypeScript | Security analytics UI |
| dashboards-desktop | HTML | Desktop application |
| dashboards-notebooks | TypeScript | Notebook visualization (archived) |

### `infrastructure/` — Build, Deploy & Test (17 repos)

| Repo | Language | Description |
|------|----------|-------------|
| opensearch-build | Python | Build systems |
| helm-charts | Go Template | Kubernetes Helm charts |
| opensearch-k8s-operator | Go | Kubernetes operator |
| docker-images | Shell | Docker image definitions |
| opensearch-benchmark | Python | Performance benchmarking |
| opensearch-benchmark-workloads | Python | Benchmark workloads |
| ansible-playbook | — | Ansible automation |
| terraform-provider-opensearch | Go | Terraform provider |
| opensearch-cluster-cdk | TypeScript | AWS CDK for clusters |
| opensearch-ci | TypeScript | CI/CD automation |
| dashboards-anywhere | Python | Cloud-native dashboards infra |
| opensearch-metrics | Java | Metrics collection |
| automation-app | TypeScript | GitHub repo automation |
| opensearch-build-libraries | Groovy | Build system libraries |
| opensearch-dashboards-functional-test | JavaScript | Functional tests |
| opensearch-dashboards-test-library | JavaScript | Test utilities & page objects |
| jenkins-gitops | Dockerfile | Jenkins CI/CD infra |

### `tools/` — Data Pipelines & Analysis (16 repos)

| Repo | Language | Description |
|------|----------|-------------|
| data-prepper | Java | Data ingestion & transformation |
| opensearch-migrations | Java | Cluster migration tool |
| opensearch-spark | Scala | Spark acceleration framework |
| performance-analyzer | Java | Cluster performance metrics |
| performance-analyzer-rca | Java | Root cause analysis framework |
| performance-analyzer-commons | Java | Performance analyzer shared libs |
| opensearch-prometheus-exporter | Java | Prometheus metrics exporter |
| opensearch-hadoop | Java | Hadoop integration |
| logstash-output-opensearch | Ruby | Logstash output plugin |
| logstash-input-opensearch | Ruby | Logstash input plugin |
| perftop | JavaScript | Performance metrics CLI |
| observability-stack | Python | Observability infrastructure |
| traffic-comparator | Python | Traffic comparison tool |
| traffic-replayer | Python | Traffic replay tool |
| reporting-cli | JavaScript | Report generation CLI |
| sql-odbc | C++ | ODBC driver for SQL |

### `sdk/` — SDKs & Libraries (15 repos)

| Repo | Language | Description |
|------|----------|-------------|
| common-utils | Kotlin | Shared utilities for plugins |
| opensearch-sdk-java | Java | Java extensions SDK |
| opensearch-sdk-py | Python | Python extensions SDK |
| opensearch-api-specification | TypeScript | API specification |
| opensearch-plugin-template-java | Java | Plugin template |
| opensearch-testcontainers | Java | Testcontainers integration |
| spring-data-opensearch | Java | Spring Data integration |
| sql-jdbc | Java | JDBC driver |
| sql-cli | Python | SQL CLI tool |
| opensearch-dsl-py | Python | Python DSL (deprecated → opensearch-py) |
| opensearch-protobufs | TypeScript | Protobuf/gRPC APIs |
| opensearch-remote-metadata-sdk | Java | Remote metadata SDK |
| opensearch-net-abstractions | C# | .NET shared utilities (archived) |
| opensearch-dashboards-sdk-js | — | Dashboards JS SDK |
| sql-odbc | C++ | ODBC driver |

### `docs/` — Documentation & Community (19 repos)

| Repo | Description |
|------|-------------|
| documentation-website | Official docs website |
| opensearch-catalog | Developer catalog & integrations |
| project-website | opensearch.org website source |
| opensearch-devops | DevOps tools & integrations |
| opensearch-plugins | Plugin management resources |
| piped-processing-language | PPL specification |
| opensearch-research | Research papers |
| maps | Map resources |
| github | Org templates & resources |
| community | Community standards & governance |
| foundation-website | Linux Foundation website |
| ux | UX research & resources |
| demos | Demo projects |
| project-meta | Org management tools |
| project-tools | Project/org utilities |
| project-website-search | Docs search experience |
| oui-docs-cdk | UI docs infrastructure |
| security-response | Vulnerability reporting |
| data-prepper-documentation-website | Data Prepper docs |

---

## 📊 Summary

| Category | Count |
|----------|-------|
| **OpenSearch repos** (`~/projects/opensearch/`) | **138** |
| **OpenAudio repos** (`~/OpenAudio/`) | **284** |
| **Awesome-Linux + Free Apps repos** (`~/projects/awesome-linux/`) | **870** |
| **Awesome-Containers repos** (`~/projects/awesome-containers/`) | **57** |
| Other repos (`~/projects/`) | 62 |
| DevTeam6 repo (VS Code) | 1 |
| **Total repos** | **1,412** |
| OpenSearch disk usage | 7.1 GB |
| OpenAudio disk usage | 36 GB |
| Awesome-Linux disk usage | 73 GB |
| Awesome-Containers disk usage | 1.5 GB |
| Total disk usage | 199 GB / 1 TB |
| DevTeam6 app pages (React) | 16 |
| DevTeam6 templates | 7 |
| DevTeam6 GitHub agents | 22 |
| DevTeam6 CI workflows | 7 |
| Local-AI ML modules | 8 categories |
| Local-AI Python files | 50+ |
| WSL dev tools | 30+ |
| Kali Linux tools | 600+ |
| **RLM Environment Variables** | `~/projects/devteam6-env.sh` |

### OpenSearch by Category

| Category | Repos | Key Capabilities |
|----------|-------|-------------------|
| `core/` | 2 | Search engine + dashboards |
| `clients/` | 10 | Python, Go, JS, Java, Rust, C#, PHP, Ruby, CLI |
| `ai-ml/` | 13 | k-NN vectors, ML commons, neural search, MCP server, flow framework |
| `plugins/` | 25 | Security, SQL, alerting, geospatial, anomaly detection, encryption |
| `dashboards-plugins/` | 21 | All dashboard UI plugins |
| `infrastructure/` | 17 | Helm, K8s operator, Terraform, Docker, benchmarking, CI/CD |
| `tools/` | 16 | Data Prepper, Spark, migrations, Prometheus, Logstash |
| `sdk/` | 15 | SDKs, Spring Data, Testcontainers, JDBC, protobuf |
| `docs/` | 19 | Documentation, catalog, community, research |

### Flatpak & Flathub

| Component | Version / Status |
|-----------|-----------------|
| Flatpak | 1.14.6 |
| Flathub remote | ✅ Configured (system) |
| GNOME Software plugin | ✅ Installed |

---

## 🎵 OpenAudio — `~/OpenAudio/` (284 repos, 36GB)

Full collection of open-source audio plugins, apps, libraries and frameworks from [OpenAudio](https://github.com/SpiralCloudOmega/OpenAudio) + [awesome-linuxaudio](https://github.com/SpiralCloudOmega/awesome-linuxaudio). All cloned with `--depth 1`.

### Audio Plugins (95 repos)

| # | Repo | Type | Framework | Description |
|---|------|------|-----------|-------------|
| 1 | 3HSPlug | Instrument | JUCE | GM/GS FM/PCM Synthesizer |
| 2 | 8ridgelite | Instrument | JUCE | 8-string guitar virtual instrument |
| 3 | ADLplug | Instrument | JUCE | FM Chip Synthesizer (OPL & OPN) |
| 4 | airwindows | Effect | N/A | Various experimental effect plugins |
| 5 | ampli-Fe | Effect | Rust VST | Cross-platform VST2 plugin in Rust |
| 6 | amsynth | Instrument | GTK | Analog-modelling synthesizer |
| 7 | andes | Instrument | JUCE | Perlin noise synthesizer |
| 8 | argotlunar | Effect | JUCE | Real-time delay-line granulator |
| 9 | AudibleInstruments | Misc | N/A | Eurorack modules for VCV Rack |
| 10 | AudiblePlanets | Instrument | JUCE | Quasi-Ptolemaic semi-modular synth |
| 11 | Audio-Effects | Effect | JUCE | Textbook audio effect implementations |
| 12 | binaural-vst | Effect | JUCE | 3D binaural audio positioning |
| 13 | blocks | Instrument | JUCE | Polyphonic modular synth |
| 14 | C1Bitcrusher | Effect | N/A | Bit reduction effect |
| 15 | calf | Effect | GTK | LV2 and Jack audio plug-ins |
| 16 | Cardinal | Instrument | DPF | Virtual modular synthesizer |
| 17 | CHOW | Effect | JUCE | Maximally truculent distortion |
| 18 | ChorusExMachina | Instrument | NIH-Plug | Physically modelled chorus synth |
| 19 | CloudSeed | Effect | N/A | Algorithmic reverb for huge spaces |
| 20 | cocoa-delay | Effect | WDL-OL | Warm delay effect |
| 21 | Convolver | Effect | JUCE | Impulse-response reverb |
| 22 | cStop | Effect | JUCE | Tape stop effect |
| 23 | dexed | Instrument | JUCE | DX7 FM plugin synth |
| 24 | DigitsVst | Instrument | VSTGUI | Casio CZ-inspired synth |
| 25 | distrho-mverb | Effect | DPF | DPF-based MVerb |
| 26 | dragonfly-reverb | Effect | DPF | Hall-style reverb |
| 27 | drops | Instrument | DPF | Single file audio player |
| 28 | flutterbird | Effect | iPlug2 | Simple pitch fluctuation |
| 29 | gigaverb | Effect | VSTGUI | Reverb |
| 30 | glBars | Effect | DPF | OpenGL bars visualization |
| 31 | helm | Instrument | JUCE | Polyphonic synth with modulation |
| 32 | hexter | Instrument | DPF | Yamaha DX7 modeling FM synth |
| 33 | HiLoFilter | Effect | JUCE | Single-knob hipass/lopass filter |
| 34 | hue | Effect | Rust VST | Noise mixer effect |
| 35 | jc303 | Instrument | JUCE | Roland TB-303 clone |
| 36 | JuceOPLVSTi | Instrument | JUCE | FM synth (Yamaha OPL2/3) |
| 37 | JuicePlugins | Misc | DPF | DISTRHO plugin collection |
| 38 | K-Meter | Effect | JUCE | K-System loudness meter |
| 39 | kars | Instrument | DPF | Karplus-strong plucked string synth |
| 40 | KickMaker | Instrument | VSTGUI | Kick-drum synth |
| 41 | LameVST | Effect | N/A | LameMP3 encoder as effect |
| 42 | Magical8bitPlug2 | Instrument | JUCE | 8bit game console synth |
| 43 | mda-plugins-juce | Misc | JUCE | Classic MDA plugins in JUCE |
| 44 | midi-chord-reader | Misc | JUCE | MIDI chord detection |
| 45 | mika-micro | Instrument | WDL-OL | Simple subtractive synth |
| 46 | mini-series | Effect | DPF | Small plugin collection |
| 47 | modEQ | Effect | JUCE | EQ with modulation |
| 48 | MultiMeter | Effect | JUCE | Comprehensive audio analysis tools |
| 49 | MVerb | Effect | DPF | DPF version of MVerb reverb |
| 50 | mverb | Effect | VSTGUI | Dattorro reverb |
| 51 | ndc-Plugs | Effect | DPF | ndc plugins in DPF |
| 52 | nekobi | Instrument | DPF | TB-303 clone |
| 53 | nih-plug | Misc | Rust | Rust VST3/CLAP plugin framework |
| 54 | ninjas2 | Instrument | DPF | Sample slicer |
| 55 | NotNotchFilter | Effect | JUCE | DJ-style mid-EQ replacement |
| 56 | OB-Xd | Instrument | JUCE | Oberheim OB-X emulation |
| 57 | OctaSine | Instrument | Rust VST | FM synth in Rust |
| 58 | odin2 | Instrument | JUCE | Polyphonic synth |
| 59 | OwlBass | Instrument | JUCE | Additive bass synth |
| 60 | oxefmsynth | Instrument | VSTGUI | 8-operator FM synth |
| 61 | plugdata | Misc | JUCE | Pure Data as plugin |
| 62 | ProM | Effect | DPF | Music visualizer (projectM) |
| 63 | pulp-fiction | Effect | JUCE | Multi-effects rack |
| 64 | PulseDelay | Effect | N/A | Timed delay effect |
| 65 | QtVST | Effect | Qt | Qt-based VST plugins |
| 66 | Reach | Effect | JUCE | Extraterrestrial reverb |
| 67 | regrader | Effect | VSTGUI | Degenerative delay |
| 68 | Roth-AIR | Effect | JUCE | Airy presence mixing tool |
| 69 | Schrammel_OJD | Effect | JUCE | Guitar distortion pedal model |
| 70 | setekh | Effect | JUCE | Minimalistic distortion |
| 71 | sfizz | Instrument | VSTGUI | SFZ parser and instrument |
| 72 | ShoutVST | Effect | VSTGUI | VST for streaming to Icecast |
| 73 | SmartGuitarAmp | Instrument | JUCE | ML guitar amplifier |
| 74 | SmartGuitarPedal | Effect | JUCE | ML guitar pedal |
| 75 | Sound-Analyser | Effect | JUCE | Real-time audio analysis |
| 76 | sound-of-music | Effect | JUCE | Multiband bitcrusher |
| 77 | SPARTA | Effect | SAF/JUCE | Spatial audio plugin suite |
| 78 | Squeezer | Effect | JUCE | Compressor with side-chain |
| 79 | StoneMistress | Effect | JUCE | Phase shifter and chorus |
| 80 | surge | Instrument | VSTGUI | Subtractive wavetable synth |
| 81 | temper | Effect | Faust/JUCE | Digital distortion |
| 82 | the-source | Instrument | JUCE | Subtractive synth |
| 83 | there-will-be-blood | Effect | JUCE | Drum bleed reduction |
| 84 | traKmeter | Effect | JUCE | Tracking loudness meter |
| 85 | tunefish | Instrument | JUCE | Virtual analog synth |
| 86 | Vaporizer2 | Instrument | JUCE | Wavetable additive/subtractive synth |
| 87 | vital | Instrument | JUCE | Spectral warping wavetable synth |
| 88 | vst2413 | Instrument | N/A | FM synth (YM2413/OPLL) |
| 89 | VSTPlugins | Effect | JUCE | VST plugin collection |
| 90 | VSTSID | Instrument | N/A | SID synthesizer |
| 91 | Wavetable | Instrument | JUCE | 2-oscillator wavetable synth |
| 92 | wolf-shaper | Effect | DPF | Waveshaper with spline editor |
| 93 | ykchorus | Effect | DPF | Chorus effect (TAL-based) |
| 94 | zam-plugins | Effect | DPF | High-quality audio plugins |
| 95 | zynaddsubfx | Instrument | FLTK | Additive/Subtractive FM synth |

### Audio Apps (28 repos)

| # | Repo | Description |
|---|------|-------------|
| 1 | adsr | Web-based ADSR synth |
| 2 | ardour | DAW |
| 3 | audacity | Audio editor |
| 4 | BespokeSynth | Node-based modular synth |
| 5 | boscaceoil | Beginner synthesiser |
| 6 | butterDAWg | DAW (WIP) |
| 7 | Carla | GNU/Linux Plugin Host |
| 8 | chuck | Music programming language |
| 9 | coincident-spectra | Spectral-microtuning additive synth |
| 10 | drumbeatrepo | Interactive drum patterns |
| 11 | Element | Advanced Audio Plugin Host |
| 12 | elementary_grid | Tonegrid synth |
| 13 | furnace | Tracker sequencer with game chips |
| 14 | GainPlugin | Experimental JUCE/React plugin |
| 15 | giada | Loop machine |
| 16 | harmonoid | Cross-platform music player |
| 17 | helio-workstation | Sequencer |
| 18 | HISE | Sample-based instrument framework |
| 19 | hydrogen | Drum machine |
| 20 | javascript-software-synthesizer | JS Software Synth |
| 21 | lmms | DAW |
| 22 | MuseScore | Notation and sheet music |
| 23 | musical-garden | Web music instrument |
| 24 | NewMixer | Mixing software |
| 25 | openmpt | Popular module tracker |
| 26 | OpenUtau | Singing synthesis platform |
| 27 | OwlPlug | Audio plugin manager |
| 28 | patchcab | Modular Eurorack web synth |
| 29 | Rack | VCV Rack modular synth |
| 30 | SampleScanner | Auto-sampling tool |
| 31 | score | Intermedia sequencer |
| 32 | Shape-Your-Music | Drawing music web app |
| 33 | sonic-pi | Code-based music tool |
| 34 | soundshed-app | Tone browser for smart amps |
| 35 | stargate | All-in-one DAW |
| 36 | studiorack-app | Audio plugin manager |
| 37 | supercollider | Audio synthesis platform |
| 38 | yabridge | Linux bridge for Windows VSTs |
| 39 | zrythm | DAW |

### Libraries & Frameworks (37 repos)

| # | Repo | Language | Description |
|---|------|----------|-------------|
| 1 | AudioKit | Swift | Audio synthesis for iOS/macOS |
| 2 | AudioKitSynthOne | Swift | Open-source iOS synthesizer |
| 3 | AudioPlugSharp | C# | .NET VST3 plugin framework |
| 4 | avendish | C++ | C++20 media object framework |
| 5 | awesome-juce | Markdown | Curated list of JUCE resources |
| 6 | cabbage | Csound | Audio plugins with Csound |
| 7 | clap | C | CLever Audio Plugin standard |
| 8 | cmajor | C++ | Audio programming language |
| 9 | csound | C | Sound synthesis programming language |
| 10 | dawdreamer | Python | Audio-processing Python framework |
| 11 | DawVert | Python | DAW project file converter |
| 12 | DPF | C++ | Cross-platform audio plugin framework |
| 13 | Dplug | D | Audio plugins in D language |
| 14 | elementary | JavaScript | JS runtime for native audio apps |
| 15 | faust | C++ | Functional audio signal processing |
| 16 | FFTConvolver | C++ | Real-time audio convolution |
| 17 | FlexASIO | C++ | Universal ASIO driver for Windows |
| 18 | fluidsynth | C | SoundFont 2 real-time synth |
| 19 | hvcc | Python | Heavy compiler for Pure Data patches |
| 20 | HyMPS | Markdown | Multimedia production suite list |
| 21 | iPlug2 | C++ | Cross-platform audio plugin framework |
| 22 | JUCE | C++ | Cross-platform audio framework |
| 23 | KlangFalter | C++ | Convolution reverb plugin |
| 24 | library | Misc | VCV Rack plugin library |
| 25 | libspatialaudio | C++ | Ambisonic encoding/decoding |
| 26 | m1-spatialsystem | Misc | Mach1 spatial/multichannel plugins |
| 27 | nallely-midi | Python | MIDI device scripting |
| 28 | neural-amp-modeler-lv2 | C++ | Neural amplifier modeler (LV2) |
| 29 | Qubx | Rust | Real-time parallel audio streams |
| 30 | r-audio | JavaScript | React components for Web Audio |
| 31 | rtaudio | C++ | Cross-platform real-time audio I/O |
| 32 | rust-vst | Rust | VST 2.4 in Rust |
| 33 | SAF | C/C++ | Spatial Audio Framework |
| 34 | SOUL | C++ | SOUnd Language for audio code |
| 35 | Soundpipe | C | Lightweight music DSP library |
| 36 | Spatial_Audio_Framework | C | Spatial audio algorithms |
| 37 | Syntage | C# | .NET study synth |
| 38 | Tone.js | JavaScript | Web Audio music framework |
| 39 | tracktion_engine | C++ | DAW engine module |
| 40 | vstgui | C++ | Audio plugin UI toolkit |
| 41 | wdl-ol | C++ | Cross-platform plugin framework |
| 42 | web-audio-api | JavaScript | W3C Web Audio API spec |
| 43 | webcz101 | JavaScript | Virtual CZ101 synth (browser) |
| 44 | webdx7 | JavaScript | Virtual DX7 synth (browser) |
| 45 | webmidi | JavaScript | MIDI for web browsers |

### Open Data (1 repo)

| Repo | Description |
|------|-------------|
| moises-db | Source separation public dataset |

### awesome-linuxaudio additions (97 repos)

Cloned from the [awesome-linuxaudio](https://github.com/SpiralCloudOmega/awesome-linuxaudio) catalog — Linux-specific audio tools, plugins, utilities, and resources not already in OpenAudio.

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | airwave | phantom-code | VST bridge for Linux |
| 2 | airwin2rack | baconpaul | Airwindows plugins for VCV Rack |
| 3 | alsa | markc | ALSA configuration scripts |
| 4 | alsa-control | sezanzeb | ALSA control utility |
| 5 | Ami-Sampler-VST | astriiddev | Amiga-style sampler VST |
| 6 | ams-lv2 | blablack | AMS LV2 plugin set |
| 7 | assets | LMMS | LMMS sample/preset assets |
| 8 | avwlv2 | mfisher31 | AVW LV2 plugin |
| 9 | awesome-linuxaudio | SpiralCloudOmega | Linux audio catalog (this list) |
| 10 | Awesome-Linux-Software | LewisVo | Curated Linux software list |
| 11 | awesome-selfhosted | awesome-selfhosted | Self-hosted software list |
| 12 | beast | tim-janik | Music synthesizer & composition |
| 13 | beatslash-lv2 | blablack | Beat slicer LV2 plugin |
| 14 | Camomile | pierreguillot | Pure Data plugins in DAWs |
| 15 | cantata | cdrummond | Qt MPD client |
| 16 | caps-lv2 | moddevices | CAPS LV2 plugins |
| 17 | CheeseCutter | theyamo | SID music editor |
| 18 | DeeFuzzer | yomguy | Audio/video streaming tool |
| 19 | delvj | delvj | VJ software |
| 20 | discodos | JOJ0 | DJ record collection tool |
| 21 | DISTRHO-Ports | DISTRHO | Collection of ported audio plugins |
| 22 | DPF-Plugins | DISTRHO | DPF plugin collection |
| 23 | drmr | nicklan | Drum machine LV2 plugin |
| 24 | drumlabooh | psemiletov | Drum machine LV2/VST3 |
| 25 | dvgrab | ddennedy | DV video grabber |
| 26 | fadecut | micressor | Audio fade/cut tool |
| 27 | foo-yc20 | sampov2 | Yamaha YC-20 organ emulation |
| 28 | Freaked | pjotrompet | Audio effect plugin |
| 29 | ft2-clone | 8bitbubsy | Fasttracker II clone |
| 30 | gbsplay | mmitch | Game Boy Sound player |
| 31 | go-dsp-guitar | andrepxx | Guitar effects in Go |
| 32 | GxPlugins.lv2 | brummer10 | Guitar amp LV2 plugins |
| 33 | horgand | ycollet | Organ synthesizer |
| 34 | hyperion | tvdzwan | Ambient lighting system |
| 35 | jack_capture | kmatheussen | JACK audio capture |
| 36 | jack_link | rncbc | Ableton Link for JACK |
| 37 | jack_midi_clock | x42 | JACK MIDI clock |
| 38 | jackaudio.github.com | jackaudio | JACK audio website |
| 39 | JackAss | falkTX | JACK-ASIO bridge |
| 40 | jalv_select | brummer10 | LV2 plugin host selector |
| 41 | Just-a-Sample | BOBONA | Simple sampler plugin |
| 42 | klystrack | kometbomb | Chiptune tracker |
| 43 | ladspa | swh | LADSPA plugins by Steve Harris |
| 44 | lamb-rs | magnetophon | Lookahead limiter (Rust) |
| 45 | LinVst | osxmidi | Linux VST bridge |
| 46 | linuxwave | orhun | Generate music from entropy |
| 47 | luz | lighttroupe | Live VJ performance tool |
| 48 | lv2 | swh | LV2 plugins by Steve Harris |
| 49 | Mamba | brummer10 | Virtual MIDI keyboard |
| 50 | mandelbulber2 | buddhi1980 | 3D fractal generator |
| 51 | mda-lv2 | moddevices | MDA plugins as LV2 |
| 52 | mhwaveedit | magnush | Audio wave editor |
| 53 | MidiJoystick | osune | Joystick to MIDI |
| 54 | mod-distortion | mod-audio | MOD distortion plugin |
| 55 | mod-host | moddevices | LV2 plugin host |
| 56 | mod-pitchshifter | mod-audio | MOD pitch shifter |
| 57 | mod-utilities | moddevices | MOD utility plugins |
| 58 | noise-repellent | lucianodato | Noise reduction LV2 plugin |
| 59 | openAV-ArtyFX | openAVproductions | ArtyFX effect plugins |
| 60 | openAV-Fabla2 | openAVproductions | Drum sampler |
| 61 | openAV-Luppp | openAVproductions | Live looping tool |
| 62 | openAV-Sorcer | openAVproductions | Wavetable synth |
| 63 | Paraphrasis | VirtualAnalogy | Spectral morphing synth |
| 64 | PeakEater | vvvar | Waveshaping limiter |
| 65 | phasex | williamweston | Phase harmonic synth |
| 66 | psi-plugins | ycollet | PSI audio plugins |
| 67 | pulsemixer | GeorgeFilipkin | PulseAudio terminal mixer |
| 68 | pwvucontrol | saivert | PipeWire volume control |
| 69 | pydub | jiaaro | Python audio manipulation |
| 70 | qloud | molke-productions | Speaker measurement tool |
| 71 | quodlibet | quodlibet | Music player & tagger |
| 72 | radium_compressor | kmatheussen | Audio compressor |
| 73 | realtimeconfigquickscan | raboof | RT Linux config checker |
| 74 | rkrlv2 | ssj71 | Rakarrack effects as LV2 |
| 75 | SendMIDI | gbevin | Command-line MIDI sender |
| 76 | sequencer64 | ahlstromcj | MIDI sequencer |
| 77 | sfarkxtc | raboof | sfArk extractor |
| 78 | shoopdaloop | SanderVocke | Live looper |
| 79 | showq | evandelisle | Cue-based audio player |
| 80 | sointu | vsariola | Modular synth for 4k intros |
| 81 | sonata | multani | MPD client |
| 82 | sonic | waywardgeek | Speed/pitch audio tool |
| 83 | sooperlooper-lv2-plugin | moddevices | SooperLooper as LV2 |
| 84 | soundcraft-utils | lack | Soundcraft mixer control |
| 85 | studio-controls | ovenwerks | Linux studio setup tool |
| 86 | superboucle | Vampouille | Loop trigger tool |
| 87 | sync-audio-tracks | alopatindev | Audio track synchronizer |
| 88 | ToneTwistPlugs | brummer10 | Tone shaping LV2 plugins |
| 89 | valentine | tote-bag-labs | Compressor plugin |
| 90 | vht | rdybka | Vaporwave tracker |
| 91 | vst-bridge | abique | VST Linux bridge |
| 92 | wine-vst | Sangeppato | Run Windows VSTs via Wine |
| 93 | winff | WinFF | Video converter GUI |
| 94 | wolf-spectrum | wolf-plugins | Spectrum analyzer |
| 95 | x42-plugins | x42 | Professional LV2 plugin suite |
| 96 | ymuse | yktoo | MPD client (Go/GTK) |
| 97 | zytrax | reduz | Tracker DAW |

---

## 🐧 Awesome-Linux-Software (764 repos)

**Location**: `~/projects/awesome-linux/`
**Disk**: 62 GB
**Source**: [SpiralCloudOmega/Awesome-Linux-Software](https://github.com/SpiralCloudOmega/Awesome-Linux-Software)

Comprehensive collection of open-source Linux software across every category — applications, development tools, desktop environments, window managers, command-line utilities, and more. All cloned with `--depth 1`.

| # | Repo | Description |
|---|------|-------------|
| 1 | 2bwm | Linux software |
| 2 | AFFiNE | Linux software |
| 3 | ANGRYsearch | Linux software |
| 4 | Ant | Linux software |
| 5 | AppImageLauncher | Linux software |
| 6 | ArrowDL | Linux software |
| 7 | AstroNvim | Linux software |
| 8 | Atoms | Linux software |
| 9 | Awesome-Linux-Software-zh_CN | Linux software |
| 10 | Beaver-Notes | Linux software |
| 11 | BetterDiscord | Linux software |
| 12 | BoilR | Linux software |
| 13 | BoostNote-App | Linux software |
| 14 | Buka | Linux software |
| 15 | Buku | Linux software |
| 16 | CPU-X | Linux software |
| 17 | Celestia | Linux software |
| 18 | Cemu | Linux software |
| 19 | Cinnamon | Linux software |
| 20 | Clementine | Linux software |
| 21 | Clipboard | Linux software |
| 22 | Cockatrice | Linux software |
| 23 | Collision | Linux software |
| 24 | ColourPicker | Linux software |
| 25 | CopyQ | Linux software |
| 26 | CudaText | Linux software |
| 27 | Cura | Linux software |
| 28 | Curlew | Linux software |
| 29 | Curtail | Linux software |
| 30 | Daily-Reddit-Wallpaper | Linux software |
| 31 | Desktop-App | Linux software |
| 32 | Discover | Linux software |
| 33 | Ebook-Viewer | Linux software |
| 34 | ElectronMail | Linux software |
| 35 | EmuDeck | Linux software |
| 36 | Epic-Asset-Manager | Linux software |
| 37 | FBReader | Linux software |
| 38 | FFmpeg | Linux software |
| 39 | FeedTheMonkey | Linux software |
| 40 | Flat-Remix | Linux software |
| 41 | Flat-Remix-GTK | Linux software |
| 42 | FlaxEngine | Linux software |
| 43 | Fluent-icon-theme | Linux software |
| 44 | FreeCAD | Linux software |
| 45 | FreeRDP | Linux software |
| 46 | FreeTube | Linux software |
| 47 | GDevelop | Linux software |
| 48 | Gisto | Linux software |
| 49 | Gittyup | Linux software |
| 50 | Gnome-Pie | Linux software |
| 51 | Go-For-It | Linux software |
| 52 | Gradience | Linux software |
| 53 | Graphite-gtk-theme | Linux software |
| 54 | Gruvbox-GTK-Theme | Linux software |
| 55 | HandBrake | Linux software |
| 56 | Hardcode-Tray | Linux software |
| 57 | Hyprland | Linux software |
| 58 | ImageMagick | Linux software |
| 59 | Imagine | Linux software |
| 60 | KDiskMark | Linux software |
| 61 | KVIrc | Linux software |
| 62 | Kaku | Linux software |
| 63 | Kimi | Linux software |
| 64 | KomodoEdit | Linux software |
| 65 | Kooha | Linux software |
| 66 | Launcher | Linux software |
| 67 | Layan-gtk-theme | Linux software |
| 68 | LightTable | Linux software |
| 69 | LinuxTimeline | Linux software |
| 70 | LuminanceHDR | Linux software |
| 71 | LunarVim | Linux software |
| 72 | MEGAsync | Linux software |
| 73 | MangoHud | Linux software |
| 74 | Markdown-Tutorial | Linux software |
| 75 | Marker | Linux software |
| 76 | Mindustry | Linux software |
| 77 | Mojave-gtk-theme | Linux software |
| 78 | Moosync | Linux software |
| 79 | Motrix | Linux software |
| 80 | Mousai | Linux software |
| 81 | Mudlet | Linux software |
| 82 | MusicBot | Linux software |
| 83 | Nordic | Linux software |
| 84 | NotepadNext | Linux software |
| 85 | Notes | Linux software |
| 86 | NvChad | Linux software |
| 87 | OmniDB | Linux software |
| 88 | OpenRA | Linux software |
| 89 | OpenRCT2 | Linux software |
| 90 | OpenTTD | Linux software |
| 91 | Orchis-theme | Linux software |
| 92 | Parabolic | Linux software |
| 93 | PathPicker | Linux software |
| 94 | PhotoGIMP | Linux software |
| 95 | Photoshop-CC2022-Linux | Linux software |
| 96 | Pixelitor | Linux software |
| 97 | Pixelorama | Linux software |
| 98 | Play- | Linux software |
| 99 | PrismLauncher | Linux software |
| 100 | Proton | Linux software |
| 101 | ProtonPlus | Linux software |
| 102 | ProtonUp-Qt | Linux software |
| 103 | PrusaSlicer | Linux software |
| 104 | QOwnNotes | Linux software |
| 105 | Qogir-icon-theme | Linux software |
| 106 | Qogir-theme | Linux software |
| 107 | RawTherapee | Linux software |
| 108 | Red-DiscordBot | Linux software |
| 109 | Remmina | Linux software |
| 110 | Repository | Linux software |
| 111 | RetroArch | Linux software |
| 112 | RetroDECK | Linux software |
| 113 | Reversal-icon-theme | Linux software |
| 114 | RingRacers | Linux software |
| 115 | Rocket.Chat | Linux software |
| 116 | Sigil | Linux software |
| 117 | Slic3r | Linux software |
| 118 | Solaar | Linux software |
| 119 | Soundux | Linux software |
| 120 | Sourcetrail | Linux software |
| 121 | SpaceVim | Linux software |
| 122 | Stacer | Linux software |
| 123 | Sweet | Linux software |
| 124 | SysMonTask | Linux software |
| 125 | TMSU | Linux software |
| 126 | Tangram | Linux software |
| 127 | Tela-icon-theme | Linux software |
| 128 | TiddlyDesktop | Linux software |
| 129 | Trimage | Linux software |
| 130 | Twake | Linux software |
| 131 | Ulauncher | Linux software |
| 132 | Vencord | Linux software |
| 133 | Ventoy | Linux software |
| 134 | VeraCrypt | Linux software |
| 135 | Vesktop | Linux software |
| 136 | Viper4Linux-GUI | Linux software |
| 137 | Vita3K | Linux software |
| 138 | WBOT | Linux software |
| 139 | Warp | Linux software |
| 140 | Waterfox | Linux software |
| 141 | WebCord | Linux software |
| 142 | WhiteSur-gtk-theme | Linux software |
| 143 | WhiteSur-icon-theme | Linux software |
| 144 | WickedEngine | Linux software |
| 145 | Wine-Builds | Linux software |
| 146 | WineZGUI | Linux software |
| 147 | Workbench | Linux software |
| 148 | Zafiro-icons | Linux software |
| 149 | ZeroBraneStudio | Linux software |
| 150 | ZeroTierOne | Linux software |
| 151 | actiona | Linux software |
| 152 | activitywatch | Linux software |
| 153 | age | Linux software |
| 154 | airpaste | Linux software |
| 155 | alacritty | Linux software |
| 156 | alarm-clock | Linux software |
| 157 | albert | Linux software |
| 158 | anbox | Linux software |
| 159 | anoise | Linux software |
| 160 | antimicrox | Linux software |
| 161 | anytype-ts | Linux software |
| 162 | app | Linux software |
| 163 | appimagepool | Linux software |
| 164 | arc-theme | Linux software |
| 165 | arduino-eclipse-plugin | Linux software |
| 166 | arduino-ide | Linux software |
| 167 | ares | Linux software |
| 168 | aria2 | Linux software |
| 169 | asciinema | Linux software |
| 170 | aseprite | Linux software |
| 171 | astromenace | Linux software |
| 172 | authpass | Linux software |
| 173 | auto-cpufreq | Linux software |
| 174 | autokey | Linux software |
| 175 | awesome | Linux software |
| 176 | backintime | Linux software |
| 177 | bandwhich | Linux software |
| 178 | bar | Linux software |
| 179 | barrier | Linux software |
| 180 | base | Linux software |
| 181 | bash2048 | Linux software |
| 182 | bastet | Linux software |
| 183 | bat | Linux software |
| 184 | bauh | Linux software |
| 185 | bcal | Linux software |
| 186 | beets | Linux software |
| 187 | bevy | Linux software |
| 188 | bibisco | Linux software |
| 189 | bibletime | Linux software |
| 190 | bismuth | Linux software |
| 191 | blackboxwm | Linux software |
| 192 | blanket | Linux software |
| 193 | bleachbit | Linux software |
| 194 | blocklist-ipsets | Linux software |
| 195 | bomi | Linux software |
| 196 | bookworm | Linux software |
| 197 | bottom | Linux software |
| 198 | boxtron | Linux software |
| 199 | bpytop | Linux software |
| 200 | brackets | Linux software |
| 201 | brasero | Linux software |
| 202 | brave-browser | Linux software |
| 203 | breaktimer-app | Linux software |
| 204 | broot | Linux software |
| 205 | bspwm | Linux software |
| 206 | btop | Linux software |
| 207 | budgie-desktop | Linux software |
| 208 | bup | Linux software |
| 209 | buttercup-desktop | Linux software |
| 210 | bzip3 | Linux software |
| 211 | caja | Linux software |
| 212 | calibre | Linux software |
| 213 | calibre-web | Linux software |
| 214 | candy-icons | Linux software |
| 215 | caprine | Linux software |
| 216 | cartridges | Linux software |
| 217 | cassandra | Linux software |
| 218 | cava | Linux software |
| 219 | ccat | Linux software |
| 220 | cdm | Linux software |
| 221 | cdogs-sdl | Linux software |
| 222 | celluloid | Linux software |
| 223 | cerebro | Linux software |
| 224 | chatterino2 | Linux software |
| 225 | chatty | Linux software |
| 226 | cheat | Linux software |
| 227 | chemtool | Linux software |
| 228 | cherrytree | Linux software |
| 229 | cinelerra-cv | Linux software |
| 230 | clamav-devel | Linux software |
| 231 | clapper | Linux software |
| 232 | clifm | Linux software |
| 233 | cloc | Linux software |
| 234 | cmus | Linux software |
| 235 | code | Linux software |
| 236 | colorls | Linux software |
| 237 | conky | Linux software |
| 238 | console-tdm | Linux software |
| 239 | contour | Linux software |
| 240 | cool-retro-term | Linux software |
| 241 | couchdb | Linux software |
| 242 | cpupower-gui | Linux software |
| 243 | croc | Linux software |
| 244 | cryptomator | Linux software |
| 245 | curseradio | Linux software |
| 246 | cutter | Linux software |
| 247 | czkawka | Linux software |
| 248 | darktable | Linux software |
| 249 | dbeaver | Linux software |
| 250 | dconf-editor | Linux software |
| 251 | dde-file-manager | Linux software |
| 252 | ddgr | Linux software |
| 253 | deadbeef | Linux software |
| 254 | deb-get | Linux software |
| 255 | decky-loader | Linux software |
| 256 | deepin-music | Linux software |
| 257 | defold | Linux software |
| 258 | desktop | Linux software |
| 259 | desktop-linux | Linux software |
| 260 | desmume | Linux software |
| 261 | devilbox | Linux software |
| 262 | dialect | Linux software |
| 263 | dino | Linux software |
| 264 | diskonaut | Linux software |
| 265 | distrobox | Linux software |
| 266 | dnscrypt-proxy | Linux software |
| 267 | dokuwiki | Linux software |
| 268 | dolphin | Linux software |
| 269 | doomemacs | Linux software |
| 270 | dosbox-staging | Linux software |
| 271 | draw | Linux software |
| 272 | drawing | Linux software |
| 273 | droidcam | Linux software |
| 274 | duc | Linux software |
| 275 | duckstation | Linux software |
| 276 | duf | Linux software |
| 277 | duplicacy | Linux software |
| 278 | dwm | Linux software |
| 279 | easyeffects | Linux software |
| 280 | easystroke | Linux software |
| 281 | easytag | Linux software |
| 282 | edb-debugger | Linux software |
| 283 | edex-ui | Linux software |
| 284 | element-web | Linux software |
| 285 | emacs | Linux software |
| 286 | emoji-keyboard | Linux software |
| 287 | entrance | Linux software |
| 288 | espanso | Linux software |
| 289 | etcher | Linux software |
| 290 | exifcleaner | Linux software |
| 291 | extension-manager | Linux software |
| 292 | eza | Linux software |
| 293 | fail2ban | Linux software |
| 294 | fast-cli | Linux software |
| 295 | fastfetch | Linux software |
| 296 | fd | Linux software |
| 297 | ferdium-app | Linux software |
| 298 | firehol | Linux software |
| 299 | firejail | Linux software |
| 300 | firewalld | Linux software |
| 301 | fish-shell | Linux software |
| 302 | fisher | Linux software |
| 303 | fishfry | Linux software |
| 304 | fkill-cli | Linux software |
| 305 | flameshot | Linux software |
| 306 | flare-engine | Linux software |
| 307 | flatseal | Linux software |
| 308 | flixel | Linux software |
| 309 | flowblade | Linux software |
| 310 | flox | Linux software |
| 311 | fluent-reader | Linux software |
| 312 | fluxbox | Linux software |
| 313 | flycast | Linux software |
| 314 | foliate | Linux software |
| 315 | fontforge | Linux software |
| 316 | franz | Linux software |
| 317 | freac | Linux software |
| 318 | freeciv | Linux software |
| 319 | freedoom | Linux software |
| 320 | fritzing-app | Linux software |
| 321 | fsearch | Linux software |
| 322 | fzf | Linux software |
| 323 | gallery-dl | Linux software |
| 324 | gambit | Linux software |
| 325 | gamemode | Linux software |
| 326 | gamescope | Linux software |
| 327 | gap | Linux software |
| 328 | gaphor | Linux software |
| 329 | gdm | Linux software |
| 330 | gdm-settings | Linux software |
| 331 | geeqie | Linux software |
| 332 | geogebra | Linux software |
| 333 | ghidra | Linux software |
| 334 | ghostty | Linux software |
| 335 | ghostwriter | Linux software |
| 336 | gifcurry | Linux software |
| 337 | git | Linux software |
| 338 | git-cola | Linux software |
| 339 | gitlabhq | Linux software |
| 340 | gitolite | Linux software |
| 341 | gkill | Linux software |
| 342 | glances | Linux software |
| 343 | glow | Linux software |
| 344 | gnome-books | Linux software |
| 345 | gnome-builder | Linux software |
| 346 | gnome-pomodoro | Linux software |
| 347 | gnome-shell-extension-gsconnect | Linux software |
| 348 | gogs | Linux software |
| 349 | googler | Linux software |
| 350 | gopass | Linux software |
| 351 | goverlay | Linux software |
| 352 | gparted | Linux software |
| 353 | gpick | Linux software |
| 354 | gping | Linux software |
| 355 | gpodder | Linux software |
| 356 | gpt4all | Linux software |
| 357 | gramps | Linux software |
| 358 | green-recorder | Linux software |
| 359 | gtk | Linux software |
| 360 | gtop | Linux software |
| 361 | guake | Linux software |
| 362 | gufw | Linux software |
| 363 | gummi | Linux software |
| 364 | gzdoom | Linux software |
| 365 | haxe | Linux software |
| 366 | heaps | Linux software |
| 367 | helix | Linux software |
| 368 | herbstluftwm | Linux software |
| 369 | higan | Linux software |
| 370 | hledger | Linux software |
| 371 | hors | Linux software |
| 372 | htop | Linux software |
| 373 | hyper | Linux software |
| 374 | hyperfine | Linux software |
| 375 | i2pd | Linux software |
| 376 | i3 | Linux software |
| 377 | icewm | Linux software |
| 378 | imgp | Linux software |
| 379 | indicator-sound-switcher | Linux software |
| 380 | indicator-sysmonitor | Linux software |
| 381 | insomnia | Linux software |
| 382 | intellij-community | Linux software |
| 383 | iprange | Linux software |
| 384 | ipscan | Linux software |
| 385 | ipython | Linux software |
| 386 | irssi | Linux software |
| 387 | itch | Linux software |
| 388 | jellyfin-media-player | Linux software |
| 389 | joplin | Linux software |
| 390 | k3b | Linux software |
| 391 | kakoune | Linux software |
| 392 | kando | Linux software |
| 393 | keepassxc | Linux software |
| 394 | keeweb | Linux software |
| 395 | kermit | Linux software |
| 396 | keynav | Linux software |
| 397 | kitty | Linux software |
| 398 | kmon | Linux software |
| 399 | komorebi | Linux software |
| 400 | kopia | Linux software |
| 401 | korkut | Linux software |
| 402 | krohnkite | Linux software |
| 403 | ksnip | Linux software |
| 404 | la-capitaine-icon-theme | Linux software |
| 405 | lapce | Linux software |
| 406 | lazydocker | Linux software |
| 407 | lazygit | Linux software |
| 408 | lemurs | Linux software |
| 409 | lf | Linux software |
| 410 | libretime | Linux software |
| 411 | lightdm | Linux software |
| 412 | linux-hardened | Linux software |
| 413 | linux-router | Linux software |
| 414 | linux-tkg | Linux software |
| 415 | liquidprompt | Linux software |
| 416 | liquorix-package | Linux software |
| 417 | liteide | Linux software |
| 418 | localsend | Linux software |
| 419 | logisim-evolution | Linux software |
| 420 | logseq | Linux software |
| 421 | lolcat | Linux software |
| 422 | lossless-cut | Linux software |
| 423 | lsd | Linux software |
| 424 | lutris | Linux software |
| 425 | luxtorpeda | Linux software |
| 426 | lxqt | Linux software |
| 427 | ly | Linux software |
| 428 | lynis | Linux software |
| 429 | magic-wormhole | Linux software |
| 430 | mailnag | Linux software |
| 431 | mame | Linux software |
| 432 | manuskript | Linux software |
| 433 | mapscii | Linux software |
| 434 | marble | Linux software |
| 435 | marktext | Linux software |
| 436 | material-ocean | Linux software |
| 437 | maxautoclicker | Linux software |
| 438 | mc | Linux software |
| 439 | mcpelauncher-manifest | Linux software |
| 440 | mdm | Linux software |
| 441 | melonDS | Linux software |
| 442 | menyoki | Linux software |
| 443 | micro | Linux software |
| 444 | min | Linux software |
| 445 | mindforger | Linux software |
| 446 | minetest | Linux software |
| 447 | minigalaxy | Linux software |
| 448 | miro | Linux software |
| 449 | mitmproxy | Linux software |
| 450 | mixxx | Linux software |
| 451 | moka-icon-theme | Linux software |
| 452 | mongo | Linux software |
| 453 | moodle | Linux software |
| 454 | mopidy | Linux software |
| 455 | mortar | Linux software |
| 456 | mosh | Linux software |
| 457 | movie-monad | Linux software |
| 458 | mozilla-vpn-client | Linux software |
| 459 | mps-youtube | Linux software |
| 460 | mpv | Linux software |
| 461 | muffon | Linux software |
| 462 | mullvadvpn-app | Linux software |
| 463 | multibootusb | Linux software |
| 464 | museeks | Linux software |
| 465 | mycli | Linux software |
| 466 | mycroft-core | Linux software |
| 467 | mypaint | Linux software |
| 468 | mysql-server | Linux software |
| 469 | mysql-workbench | Linux software |
| 470 | nativefier | Linux software |
| 471 | nautilus | Linux software |
| 472 | navi | Linux software |
| 473 | nemo | Linux software |
| 474 | neovide | Linux software |
| 475 | neovim | Linux software |
| 476 | nestopia | Linux software |
| 477 | netdata | Linux software |
| 478 | neverball | Linux software |
| 479 | newsboat | Linux software |
| 480 | nheko | Linux software |
| 481 | nitroshare-desktop | Linux software |
| 482 | nix | Linux software |
| 483 | nmap | Linux software |
| 484 | nnn | Linux software |
| 485 | nodm | Linux software |
| 486 | nomacs | Linux software |
| 487 | notepadqq | Linux software |
| 488 | notes | Linux software |
| 489 | notesnook | Linux software |
| 490 | nsnake | Linux software |
| 491 | nuclear | Linux software |
| 492 | numix-icon-theme | Linux software |
| 493 | nushell | Linux software |
| 494 | nvtop | Linux software |
| 495 | nxengine-evo | Linux software |
| 496 | nylas-mail | Linux software |
| 497 | nyrna | Linux software |
| 498 | o3de | Linux software |
| 499 | obs-studio | Linux software |
| 500 | oceanbase | Linux software |
| 501 | oh-my-fish | Linux software |
| 502 | oh-my-zsh | Linux software |
| 503 | oil | Linux software |
| 504 | ojo | Linux software |
| 505 | okular | Linux software |
| 506 | olive | Linux software |
| 507 | onefetch | Linux software |
| 508 | onenote | Linux software |
| 509 | onionshare | Linux software |
| 510 | openbox | Linux software |
| 511 | openmw | Linux software |
| 512 | openshot-qt | Linux software |
| 513 | opensnitch | Linux software |
| 514 | opensurge | Linux software |
| 515 | ops | Linux software |
| 516 | osdlyrics | Linux software |
| 517 | pacman4console.debian | Linux software |
| 518 | padloc | Linux software |
| 519 | papirus-icon-theme | Linux software |
| 520 | parlatype | Linux software |
| 521 | paru | Linux software |
| 522 | pb-for-desktop | Linux software |
| 523 | pdd | Linux software |
| 524 | pdfarranger | Linux software |
| 525 | pdfsam | Linux software |
| 526 | pdfslicer | Linux software |
| 527 | peek | Linux software |
| 528 | pencil | Linux software |
| 529 | percona-server | Linux software |
| 530 | percona-server-mongodb | Linux software |
| 531 | percona-xtradb-cluster | Linux software |
| 532 | pfetch | Linux software |
| 533 | pgcli | Linux software |
| 534 | phockup | Linux software |
| 535 | photivo | Linux software |
| 536 | photoshopCClinux | Linux software |
| 537 | phototonic | Linux software |
| 538 | pi-hole | Linux software |
| 539 | picard | Linux software |
| 540 | picom | Linux software |
| 541 | pingus | Linux software |
| 542 | piper | Linux software |
| 543 | piskel | Linux software |
| 544 | pithos | Linux software |
| 545 | planify | Linux software |
| 546 | plotinus | Linux software |
| 547 | pmm-server | Linux software |
| 548 | podman | Linux software |
| 549 | pods | Linux software |
| 550 | pokete | Linux software |
| 551 | polybar | Linux software |
| 552 | polychromatic | Linux software |
| 553 | popcorn-desktop | Linux software |
| 554 | popsicle | Linux software |
| 555 | portmaster | Linux software |
| 556 | postgres | Linux software |
| 557 | powertop | Linux software |
| 558 | ppsspp | Linux software |
| 559 | proton-ge-custom | Linux software |
| 560 | protontricks | Linux software |
| 561 | protonup | Linux software |
| 562 | proxychains | Linux software |
| 563 | pulsar | Linux software |
| 564 | pulseaudio | Linux software |
| 565 | pywal | Linux software |
| 566 | qBittorrent | Linux software |
| 567 | qBittorrent-Enhanced-Edition | Linux software |
| 568 | qTox | Linux software |
| 569 | qdirstat | Linux software |
| 570 | qmc2-mame-fe | Linux software |
| 571 | qt-creator | Linux software |
| 572 | qtile | Linux software |
| 573 | quickemu | Linux software |
| 574 | quickviewer | Linux software |
| 575 | qutebrowser | Linux software |
| 576 | rabbitvcs | Linux software |
| 577 | radare2 | Linux software |
| 578 | radiotray-ng | Linux software |
| 579 | rainbowstream | Linux software |
| 580 | ranger | Linux software |
| 581 | rclip | Linux software |
| 582 | rclone | Linux software |
| 583 | redshift | Linux software |
| 584 | remarkable | Linux software |
| 585 | restic | Linux software |
| 586 | retext | Linux software |
| 587 | rhythmbox | Linux software |
| 588 | ripgrep | Linux software |
| 589 | river | Linux software |
| 590 | rmlint | Linux software |
| 591 | roberta | Linux software |
| 592 | rofi | Linux software |
| 593 | rsnapshot | Linux software |
| 594 | rssguard | Linux software |
| 595 | rustdesk | Linux software |
| 596 | ryzomcore | Linux software |
| 597 | s-tui | Linux software |
| 598 | sage | Linux software |
| 599 | sc-controller | Linux software |
| 600 | scantailor | Linux software |
| 601 | scratch-flash | Linux software |
| 602 | scrcpy | Linux software |
| 603 | screenFetch | Linux software |
| 604 | screencloud | Linux software |
| 605 | scribus | Linux software |
| 606 | scummvm | Linux software |
| 607 | sddm | Linux software |
| 608 | seafile | Linux software |
| 609 | shadowsocks-qt5 | Linux software |
| 610 | shadps4 | Linux software |
| 611 | shell | Linux software |
| 612 | shellcheck | Linux software |
| 613 | shotcut | Linux software |
| 614 | shutter | Linux software |
| 615 | silentcast | Linux software |
| 616 | simutrans | Linux software |
| 617 | sirikali | Linux software |
| 618 | skribisto | Linux software |
| 619 | sleek | Linux software |
| 620 | slides | Linux software |
| 621 | slim-display-manager | Linux software |
| 622 | snake | Linux software |
| 623 | snes9x | Linux software |
| 624 | sourcegit | Linux software |
| 625 | spacefm | Linux software |
| 626 | spacemacs | Linux software |
| 627 | spectrwm | Linux software |
| 628 | spicetify-cli | Linux software |
| 629 | spot | Linux software |
| 630 | spotify-downloader | Linux software |
| 631 | spotube | Linux software |
| 632 | spy | Linux software |
| 633 | sqlitebrowser | Linux software |
| 634 | ssr | Linux software |
| 635 | starship | Linux software |
| 636 | steam-deck-utilities | Linux software |
| 637 | steam-repo-manager | Linux software |
| 638 | steam-rom-manager | Linux software |
| 639 | steamtinkerlaunch | Linux software |
| 640 | stella | Linux software |
| 641 | stk-code | Linux software |
| 642 | strawberry | Linux software |
| 643 | streamlink | Linux software |
| 644 | streamlink-twitch-gui | Linux software |
| 645 | stride | Linux software |
| 646 | studio3 | Linux software |
| 647 | sugar | Linux software |
| 648 | super-productivity | Linux software |
| 649 | supertux | Linux software |
| 650 | sway | Linux software |
| 651 | synfig | Linux software |
| 652 | tabby | Linux software |
| 653 | tabletop-club | Linux software |
| 654 | tanks-of-freedom-ii | Linux software |
| 655 | taskbook | Linux software |
| 656 | taskwarrior | Linux software |
| 657 | tdesktop | Linux software |
| 658 | tealdeer | Linux software |
| 659 | terminator | Linux software |
| 660 | terminology | Linux software |
| 661 | termit | Linux software |
| 662 | termshark | Linux software |
| 663 | testdisk | Linux software |
| 664 | texstudio | Linux software |
| 665 | textadept | Linux software |
| 666 | texworks | Linux software |
| 667 | the_silver_searcher | Linux software |
| 668 | thefuck | Linux software |
| 669 | themix-gui | Linux software |
| 670 | tidal-hifi | Linux software |
| 671 | tilda | Linux software |
| 672 | tilix | Linux software |
| 673 | timecop | Linux software |
| 674 | timeshift | Linux software |
| 675 | tldr | Linux software |
| 676 | tmpmail | Linux software |
| 677 | tmux | Linux software |
| 678 | todo.txt-android | Linux software |
| 679 | todoist-linux | Linux software |
| 680 | tomboy | Linux software |
| 681 | toolbox | Linux software |
| 682 | touche | Linux software |
| 683 | trackma | Linux software |
| 684 | transgui | Linux software |
| 685 | translate-shell | Linux software |
| 686 | trelby | Linux software |
| 687 | triplea | Linux software |
| 688 | trojita | Linux software |
| 689 | tty-solitaire | Linux software |
| 690 | tusk | Linux software |
| 691 | twin | Linux software |
| 692 | ubunsys | Linux software |
| 693 | ubuntu-cleaner | Linux software |
| 694 | ugene | Linux software |
| 695 | uktools | Linux software |
| 696 | ukui-desktop-environment | Linux software |
| 697 | uncrustify | Linux software |
| 698 | undistract-me | Linux software |
| 699 | unetbootin | Linux software |
| 700 | ungoogled-chromium | Linux software |
| 701 | unix-notes | Linux software |
| 702 | unknown-horizons | Linux software |
| 703 | unsnap | Linux software |
| 704 | upscayl | Linux software |
| 705 | vagrant | Linux software |
| 706 | variety | Linux software |
| 707 | vaultwarden | Linux software |
| 708 | veyon | Linux software |
| 709 | vhs | Linux software |
| 710 | vidcutter | Linux software |
| 711 | video-downloader | Linux software |
| 712 | vifm | Linux software |
| 713 | vim | Linux software |
| 714 | virt-manager | Linux software |
| 715 | visidata | Linux software |
| 716 | visualboyadvance-m | Linux software |
| 717 | vmail | Linux software |
| 718 | vnstat | Linux software |
| 719 | vocal | Linux software |
| 720 | vokoscreen | Linux software |
| 721 | vorta | Linux software |
| 722 | vpuppr | Linux software |
| 723 | vscode | Linux software |
| 724 | vscodium | Linux software |
| 725 | warpinator | Linux software |
| 726 | warzone2100 | Linux software |
| 727 | waveterm | Linux software |
| 728 | waydroid | Linux software |
| 729 | wayfire | Linux software |
| 730 | webtorrent-desktop | Linux software |
| 731 | weechat | Linux software |
| 732 | wesnoth | Linux software |
| 733 | wezterm | Linux software |
| 734 | wine-ge-custom | Linux software |
| 735 | wine-tkg-git | Linux software |
| 736 | wine-wayland | Linux software |
| 737 | winetricks | Linux software |
| 738 | wizqtclient | Linux software |
| 739 | workrave | Linux software |
| 740 | worm | Linux software |
| 741 | wpgtk | Linux software |
| 742 | xbmc | Linux software |
| 743 | xclicker | Linux software |
| 744 | xdm | Linux software |
| 745 | xemu | Linux software |
| 746 | xmonad | Linux software |
| 747 | xsv | Linux software |
| 748 | yay | Linux software |
| 749 | yewtube | Linux software |
| 750 | youtube-dl | Linux software |
| 751 | youtube-music | Linux software |
| 752 | yt-dlp | Linux software |
| 753 | ytdownloader | Linux software |
| 754 | ytfzf | Linux software |
| 755 | ytmdl | Linux software |
| 756 | z.lua | Linux software |
| 757 | zaproxy | Linux software |
| 758 | zathura | Linux software |
| 759 | zeal | Linux software |
| 760 | zellij | Linux software |
| 761 | zen-kernel | Linux software |
| 762 | zimfw | Linux software |
| 763 | zoxide | Linux software |
| 764 | zulip | Linux software |

Total: 764 repos cloned

---

## 📱 Awesome-Free-Apps (106 new repos)

**Location**: `~/projects/awesome-linux/` (merged with Awesome-Linux-Software)
**Catalog**: `DevTeam6/awesome-free-apps/` (VS Code workspace)
**Source**: [SpiralCloudOmega/awesome-free-apps](https://github.com/SpiralCloudOmega/awesome-free-apps)
**Disk**: +11 GB (73 GB total for awesome-linux collection)

Cross-platform free & open-source apps catalog. 155 GitHub repos extracted (desktop + mobile), 48 already existed from Awesome-Linux-Software. 106 new repos cloned, 1 unavailable.

**Categories covered**: Audio, Browsers, Communication, Compression, Customization, Data Management, Developer Tools (API/Database/Network/Game Engines/Virtualization), Documents, Note Taking, Text Editors, Download Managers, Games, Emulators, Graphics, 3D Modeling, Security, Media, Streaming, System, Utilities, VPN, and Mobile apps (Android/iOS).

**Combined awesome-linux totals**: 870 repos (764 Awesome-Linux-Software + 106 Awesome-Free-Apps)

---

## 🐳 Awesome Linux Containers — `~/projects/awesome-containers/` (57 repos)

**Location**: `~/projects/awesome-containers/` (11 subdirectories by category)
**Catalog**: `DevTeam6/awesome-linux-containers/` (VS Code workspace)
**Source**: [SpiralCloudOmega/awesome-linux-containers](https://github.com/SpiralCloudOmega/awesome-linux-containers)
**Disk**: 1.5 GB
**Cloned**: `--depth 1`

Curated collection of Linux container tools, runtimes, security tools, and ecosystem projects. 58 GitHub repos extracted from the awesome-linux-containers catalog; 57 cloned successfully (1 unavailable: `salihciftci/liman`).

### `foundations/` — Specs & Standards (3 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | specs | `opencontainers/specs` | OCI container specifications |
| 2 | cnab-spec | `deislabs/cnab-spec` | Cloud Native Application Bundle spec |
| 3 | atomicapp | `projectatomic/atomicapp` | Nulecule multi-container packaging |

### `clouds/` — Cloud & Build Tools (1 repo)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | kaniko | `GoogleContainerTools/kaniko` | Build container images in Kubernetes |

### `operating-systems/` — Container OSes (1 repo)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | photon | `vmware/photon` | VMware minimal container-optimized Linux |

### `hypervisors/` — MicroVMs & Hypervisors (3 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | firecracker | `firecracker-microvm/firecracker` | Lightweight microVM (AWS Lambda/Fargate) |
| 2 | gvisor | `google/gvisor` | Application kernel for container sandboxing |
| 3 | runv | `hyperhq/runv` | Hypervisor-based OCI runtime |

### `containers/` — Runtimes & Engines (20 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | moby | `moby/moby` | Docker engine (Moby project) |
| 2 | runc | `opencontainers/runc` | OCI container runtime |
| 3 | youki | `containers/youki` | Container runtime in Rust |
| 4 | libpod | `containers/libpod` | Podman container engine library |
| 5 | lxc | `lxc/lxc` | Linux Containers userspace tools |
| 6 | lxd | `lxc/lxd` | LXD system container manager |
| 7 | rkt | `coreos/rkt` | CoreOS container engine |
| 8 | railcar | `oracle/railcar` | Rust OCI container runtime |
| 9 | cc-oci-runtime | `01org/cc-oci-runtime` | Intel Clear Containers OCI runtime |
| 10 | sysbox | `nestybox/sysbox` | Next-gen container runtime (systemd, Docker-in-Docker) |
| 11 | lithos | `tailhook/lithos` | Process supervisor for containers |
| 12 | vagga | `tailhook/vagga` | Containerization tool without daemons |
| 13 | lmctfy | `google/lmctfy` | Google open-source container stack |
| 14 | libct | `xemul/libct` | Container management library |
| 15 | bocker | `p8952/bocker` | Docker in ~100 lines of bash |
| 16 | warden | `cloudfoundry/warden` | Cloud Foundry container manager |
| 17 | multidocker | `marty90/multidocker` | Multiple Docker environments |
| 18 | plash | `ihucos/plash` | Container build system |
| 19 | dgr | `blablacar/dgr` | Container build & runtime tool |
| 20 | udocker | `indigo-dc/udocker` | Execute Docker containers as unprivileged user |

### `sandboxes/` — Sandboxing & Isolation (7 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | bubblewrap | `projectatomic/bubblewrap` | Unprivileged sandboxing tool |
| 2 | nsjail | `google/nsjail` | Light-weight process isolation (Linux namespaces) |
| 3 | subuser | `subuser-security/subuser` | Run desktop apps in Docker containers |
| 4 | lxroot | `parke/lxroot` | Lightweight Linux containers |
| 5 | pyspaces | `Friz-zy/pyspaces` | Python Linux namespaces library |
| 6 | python-nsenter | `zalando/python-nsenter` | Python nsenter wrapper |
| 7 | singularity | `singularityware/singularity` | HPC container platform |

### `filesystem/` — Image & Registry Tools (8 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | dive | `wagoodman/dive` | Explore Docker image layers |
| 2 | container-diff | `GoogleCloudPlatform/container-diff` | Diff container images |
| 3 | go-containerregistry | `google/go-containerregistry` | Go library for container registries |
| 4 | buildah | `projectatomic/buildah` | Build OCI container images |
| 5 | skopeo | `projectatomic/skopeo` | Copy container images between registries |
| 6 | img | `jessfraz/img` | Standalone Dockerfile builder |
| 7 | docker-pushrm | `christian-korneck/docker-pushrm` | Push README to container registries |
| 8 | Whaler | `P3GLEG/Whaler` | Reverse-engineer Docker images |

### `dashboard/` — UI & Management (2 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | portainer | `portainer/portainer` | Container management UI |
| 2 | swarmpit | `swarmpit/swarmpit` | Docker Swarm management UI |

### `security/` — Security & Compliance (7 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | docker-bench-security | `docker/docker-bench-security` | Docker CIS benchmark checker |
| 2 | oci-seccomp-bpf-hook | `containers/oci-seccomp-bpf-hook` | Seccomp profile generator |
| 3 | sockguard | `buildkite/sockguard` | Docker socket proxy guard |
| 4 | drydock | `zuBux/drydock` | Docker security auditing |
| 5 | container-compliance | `OpenSCAP/container-compliance` | Container compliance checking |
| 6 | bane | `jfrazelle/bane` | AppArmor profile generator for Docker |
| 7 | footloose | `weaveworks/footloose` | VM-like containers |

### `tools/` — Utilities (3 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | goss | `aelsabbahy/goss` | Quick server validation tool |
| 2 | docker-explorer | `google/docker-explorer` | Docker filesystem forensics |
| 3 | porto | `yandex/porto` | Yandex container management system |

### `ecosystem/` — Meta & Collections (2 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-docker | `veggiemonk/awesome-docker` | Curated Docker resources |
| 2 | sysdig-container-ecosystem | `draios/sysdig-container-ecosystem` | Container ecosystem map |

---

## 🧠 RLM Environment Variables

**File**: `~/projects/devteam6-env.sh` (auto-sourced via `~/.bashrc`)
**Paradigm**: [Recursive Language Models (arXiv:2512.24601)](https://arxiv.org/abs/2512.24601)

All resources are exposed as environment variables — symbolic pointers that agents can "peek" into without loading full context. This follows the RLM paper's REPL pattern where the prompt is treated as an external environment.

### Key Variables

| Variable | Value | Purpose |
|----------|-------|---------|
| `DEVTEAM6_HOME` | `~/projects/DevTeam6` | Main project root |
| `DEVTEAM6_VSCODE` | `C:\Users\autom\VS Code GitHub Repos\DevTeam6` | VS Code workspace |
| `OPENSEARCH_HOME` | `~/projects/opensearch` | 138 OpenSearch repos |
| `OPENAUDIO_HOME` | `~/OpenAudio` | 284 audio repos |
| `AWESOME_LINUX_HOME` | `~/projects/awesome-linux` | 870 Linux/free app repos |
| `AWESOME_CONTAINERS_HOME` | `~/projects/awesome-containers` | 57 container repos |
| `CYBORGDB_API_URL` | `http://localhost:8100` | CyborgDB vector DB |
| `OPENCLAW_GATEWAY` | `http://localhost:18789` | OpenClaw AI gateway |
| `REPO_COUNT_TOTAL` | `1410` | Total cloned repos |

### Shell Functions

| Function | Description |
|----------|-------------|
| `devteam6_repos` | List all repo collections with counts |
| `devteam6_services` | Show active service endpoints |
| `devteam6_search <query>` | Search across all repos by name |
| `devteam6_stats` | Environment stats (repos, disk, GPU) |

