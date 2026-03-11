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
| Houdini | 21.0 (env ready) | `/opt/hfs` (download installer from sidefx.com) |
| Pinokio | 6.0.10 | `DevTeam6/pinokio/` (Electron AI app launcher) |
| CopilotKit | monorepo | `DevTeam6/CopilotKit/` (pnpm + turbo) |
| pnpm | installed | global (for CopilotKit monorepo) |

### Key Services

| Service | Port | Status |
|---------|------|--------|
| CyborgDB | 8100 | Not running (start with `cyborgdb-service`) |
| OpenClaw Gateway | 18789 | ✅ Running (systemd) |
| PostgreSQL 16 | 5432 | Installed (pgvector enabled) |
| Gitea | 3000 | ✅ Running (systemd) — [http://localhost:3000](http://localhost:3000) |
| ACE-Step API | 8001 | AI music generation model (RTX 3090, tier6b) |
| ACE-Step Backend | 3001 | Express API + SQLite database |
| ACE-Step Frontend | 3002 | [http://localhost:3002](http://localhost:3002) — Spotify-like music UI |
| CopilotKit | 3003 | [http://localhost:3003](http://localhost:3003) — AI copilot (Ollama qwen3:8b) |
| OpenDAW Studio | 8080 | [https://localhost:8080](https://localhost:8080) — Web-based DAW |
| Ollama LLM | 11434 | Local LLM server (qwen3:8b, kimi-k2.5:cloud, deepseek-v3.2:cloud) |

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
| **Awesome Lists repos** (`~/projects/awesome-lists/`) | **690** |
| **Awesome WSL repos** (`~/projects/awesome-wsl/`) | **71** |
| **Awesome Bash repos** (`~/projects/awesome-bash/`) | **28** |
| **Awesome LISP repos** (`~/projects/awesome-lisp/`) | **736** |
| **Awesome AI/ML repos** (`~/projects/awesome-ai-ml/`) | **416** |
| **Awesome Python repos** (`~/projects/awesome-python/`) | **497** |
| **Revithion Resources** (`~/projects/revithion-resources/`) | **28** |
| Other repos (`~/projects/`) | 81 |
| DevTeam6 repo (VS Code) | 25 |
| **Total repos** | **~4,200** |
| OpenSearch disk usage | 7.1 GB |
| OpenAudio disk usage | 62 GB |
| Awesome-Linux disk usage | 73 GB |
| Awesome-Containers disk usage | 1.5 GB |
| Awesome Lists disk usage | 13 GB |
| Awesome WSL + Bash disk usage | 2.9 GB |
| Awesome LISP disk usage | 7 GB |
| Awesome AI/ML disk usage | 16 GB |
| Awesome Python disk usage | 12 GB |
| Total disk usage | ~651 GB / 1 TB |
| **AI Tools** | Ollama 0.15.1, Claude CLI 2.1.62, OpenCode 1.2.10 |
| **Ollama Cloud Models** | 4 (kimi-k2.5, glm-5, deepseek-v3.2, minimax-m2.5) |
| **Ollama Local Models** | qwen3-coder:30b (19 GB) |
| **Integration Configs** | 16 (4 integrations × 4 models) |
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

## 🌟 Awesome Lists (Master Collection) — `~/projects/awesome-lists/` (690 repos)

**Location**: `~/projects/awesome-lists/` (flat directory)
**Catalog**: `DevTeam6/awesome/` (VS Code workspace)
**Source**: [SpiralCloudOmega/awesome](https://github.com/SpiralCloudOmega/awesome) (fork of sindresorhus/awesome)
**Disk**: 13 GB
**Cloned**: `--depth 1`

The master collection of all awesome lists — 690 curated lists covering platforms, programming languages, frameworks, tools, databases, gaming, security, and much more. Organized into 26 categories per the original awesome list structure.

**Unavailable repos (5)**: `sponsors/sindresorhus` (not a repo), `catdevnull/awesome-zig` (replaced with `C-BJ/awesome-zig`), and 3 others marked with ⚠️ below.


### Platforms (61 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-nodejs | `sindresorhus/awesome-nodejs` | Async non-blocking event-driven JavaScript runtime built on  |
| 2 | awesome-cross-platform-nodejs | `bcoe/awesome-cross-platform-nodejs` | Writing cross-platform code on Node.js |
| 3 | frontend-dev-bookmarks | `dypsilon/frontend-dev-bookmarks` | Awesome list |
| 4 | awesome-ios | `vsouza/awesome-ios` | Mobile operating system for Apple phones and tablets |
| 5 | awesome-android | `JStumpp/awesome-android` | Mobile operating system developed by Google |
| 6 | awesome-IoT-hybrid | `weblancaster/awesome-IoT-hybrid` | Awesome list |
| 7 | awesome-electron | `sindresorhus/awesome-electron` | Cross-platform native desktop apps using JavaScript/HTML/CSS |
| 8 | awesome-cordova | `busterc/awesome-cordova` | JavaScript API for hybrid apps |
| 9 | awesome-react-native | `jondot/awesome-react-native` | JavaScript framework for writing natively rendering mobile a |
| 10 | awesome-xamarin | `XamSome/awesome-xamarin` | Mobile app development IDE, testing, and distribution |
| 11 | awesome-linux ⚠️ | `inputsh/awesome-linux` | Awesome list |
| 12 | awesome-linux-containers | `Friz-zy/awesome-linux-containers` | Awesome list |
| 13 | awesome-ebpf | `zoidbergwill/awesome-ebpf` | Virtual machine that allows you to write more efficient and  |
| 14 | Awesome-Arch | `PandaFoss/Awesome-Arch` | Linux distributions and projects based on Arch Linux |
| 15 | awesome-appimage | `AppImage/awesome-appimage` | Package apps in a single file that works on various mainstre |
| 16 | awesome-omarchy | `aorumbayev/awesome-omarchy` | Opinionated Arch Linux and Hyprland desktop environment from |
| 17 | awesome-macos-screensavers | `agarrharr/awesome-macos-screensavers` | Awesome list |
| 18 | awesome-mac | `jaywcjlove/awesome-mac` | Awesome list |
| 19 | open-source-mac-os-apps | `serhii-londar/open-source-mac-os-apps` | Awesome list |
| 20 | awesome-watchos | `yenchenlin/awesome-watchos` | Operating system for the Apple Watch |
| 21 | awesome-jvm | `deephacks/awesome-jvm` | Awesome list |
| 22 | awesome-salesforce | `mailtoharshit/awesome-salesforce` | Awesome list |
| 23 | awesome-aws | `donnemartin/awesome-aws` | Awesome list |
| 24 | awesome-windows | `0pandadev/awesome-windows` | Consumer desktop operating system |
| 25 | awesome-powertoys-run-plugins | `hlaueriksson/awesome-powertoys-run-plugins` | Community plugins for the Windows quick launcher |
| 26 | awesome-ipfs | `ipfs/awesome-ipfs` | P2P hypermedia protocol |
| 27 | awesome-fuse | `fuse-compound/awesome-fuse` | Mobile development tools |
| 28 | awesome-heroku | `ianstormtaylor/awesome-heroku` | Cloud platform as a service |
| 29 | awesome-raspberry-pi | `thibmaek/awesome-raspberry-pi` | Credit card-sized computer aimed at teaching kids programmin |
| 30 | awesome-qt | `JesseTG/awesome-qt` | Cross-platform GUI app framework |
| 31 | Awesome-WebExtensions | `fregante/Awesome-WebExtensions` | Cross-browser extension system |
| 32 | awesome-smart-tv | `vitalets/awesome-smart-tv` | Create apps for different TV platforms |
| 33 | awesome-gnome | `Kazhnuz/awesome-gnome` | Simple and distraction-free desktop environment for Linux |
| 34 | awesome-kde | `francoism90/awesome-kde` | A free software community dedicated to creating an open and  |
| 35 | awesome-dotnet | `quozd/awesome-dotnet` | Awesome list |
| 36 | awesome-dotnet-core | `thangchung/awesome-dotnet-core` | Awesome list |
| 37 | awesome-roslyn | `ironcev/awesome-roslyn` | Open-source compilers and code analysis APIs for C# and VB.N |
| 38 | awesome-amazon-alexa | `miguelmota/awesome-amazon-alexa` | Virtual home assistant |
| 39 | awesome-digitalocean | `jonleibowitz/awesome-digitalocean` | Cloud computing platform designed for developers |
| 40 | awesome-flutter | `Solido/awesome-flutter` | Google's mobile SDK for building native iOS and Android apps |
| 41 | awesome-home-assistant | `frenck/awesome-home-assistant` | Open source home automation that puts local control and priv |
| 42 | awesome-ibmcloud | `victorshinya/awesome-ibmcloud` | Cloud platform for developers and companies |
| 43 | awesome-firebase | `jthegedus/awesome-firebase` | App development platform built on Google Cloud |
| 44 | awesome-ros2 | `fkromer/awesome-ros2` | Set of software libraries and tools that help you build robo |
| 45 | awesome-adafruitio | `adafruit/awesome-adafruitio` | Visualize and store data from any device |
| 46 | awesome-cloudflare | `irazasyed/awesome-cloudflare` | CDN, DNS, DDoS protection, and security for your site |
| 47 | awesome-actions-on-google | `ravirupareliya/awesome-actions-on-google` | Developer platform for Google Assistant |
| 48 | awesome-esp | `agucova/awesome-esp` | Low-cost microcontrollers with WiFi and broad IoT applicatio |
| 49 | awesome-deno | `denolib/awesome-deno` | A secure runtime for JavaScript and TypeScript that uses V8  |
| 50 | awesome-dos | `balintkissdev/awesome-dos` | Operating system for x86-based personal computers that was p |
| 51 | awesome-nix | `nix-community/awesome-nix` | Package manager for Linux and other Unix systems that makes  |
| 52 | awesome-integration | `stn1slv/awesome-integration` | Linking together different IT systems (components) to functi |
| 53 | awesome-nodered | `naimo84/awesome-nodered` | A programming tool for wiring together hardware devices, API |
| 54 | awesome-low-code | `zenitysec/awesome-low-code` | Allowing business professionals to address their needs on th |
| 55 | awesome-capacitor | `riderx/awesome-capacitor` | Cross-platform open source runtime for building Web Native a |
| 56 | awesome-arcgis-developer | `Esri/awesome-arcgis-developer` | Mapping and location analysis platform for developers |
| 57 | awesome-ble | `dotintent/awesome-ble` | Low-power wireless communication protocol ideal for IoT, wea |
| 58 | awesome-uno-platform | `MartinZikmund/awesome-uno-platform` | Open-source .NET UI platform for building cross-platform app |
| 59 | awesome-google-cloud | `GoogleCloudPlatform/awesome-google-cloud` | Cloud computing services by Google |
| 60 | awesome-firebase-genkit | `xavidop/awesome-firebase-genkit` | An open-source framework for building AI-powered apps and fe |
| 61 | awesome-backstage | `shano/awesome-backstage` | Open-source platform for building Internal Developer Portals |

### Programming Languages (81 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-javascript | `sorrycc/awesome-javascript` | Awesome list |
| 2 | awesome-promises | `wbinnssmith/awesome-promises` | Awesome list |
| 3 | awesome-standard | `standard/awesome-standard` | Style guide and linter |
| 4 | js-must-watch | `bolshchikov/js-must-watch` | Awesome list |
| 5 | jstips | `loverajoel/jstips` | Awesome list |
| 6 | awesome-network-js | `Kikobeats/awesome-network-js` | Awesome list |
| 7 | awesome-micro-npm-packages | `parro-it/awesome-micro-npm-packages` | Awesome list |
| 8 | awesome-mad-science | `feross/awesome-mad-science` | Impossible sounding projects that exist |
| 9 | maintenance-modules | `maxogden/maintenance-modules` | For npm packages |
| 10 | awesome-npm | `sindresorhus/awesome-npm` | Package manager |
| 11 | awesome-ava | `avajs/awesome-ava` | Test runner |
| 12 | awesome-eslint | `dustinspecker/awesome-eslint` | Linter |
| 13 | awesome-fp-js | `stoeffel/awesome-fp-js` | Awesome list |
| 14 | awesome-observables | `sindresorhus/awesome-observables` | Awesome list |
| 15 | awesome-npm-scripts | `RyanZim/awesome-npm-scripts` | Task runner |
| 16 | 30-seconds-of-code | `30-seconds/30-seconds-of-code` | Code snippets you can understand in 30 seconds |
| 17 | awesome-ponyfills | `Richienb/awesome-ponyfills` | Like polyfills but without overriding native APIs |
| 18 | awesome-swift | `matteocrippa/awesome-swift` | Apple's compiled programming language that is secure, modern |
| 19 | Awesome-Swift-Education | `hsavit1/Awesome-Swift-Education` | Awesome list |
| 20 | Awesome-Swift-Playgrounds | `uraimo/Awesome-Swift-Playgrounds` | Awesome list |
| 21 | awesome-python | `vinta/awesome-python` | General-purpose programming language designed for readabilit |
| 22 | awesome-asyncio | `timofurrer/awesome-asyncio` | Asynchronous I/O in Python 3 |
| 23 | awesome-python-scientific-audio | `faroit/awesome-python-scientific-audio` | Scientific research in audio/music |
| 24 | awesome-circuitpython | `adafruit/awesome-circuitpython` | A version of Python for microcontrollers |
| 25 | awesome-python-data-science | `krzjoa/awesome-python-data-science` | Data analysis and machine learning |
| 26 | awesome-python-typing | `typeddjango/awesome-python-typing` | Optional static typing for Python |
| 27 | awesome-micropython | `mcauser/awesome-micropython` | A lean and efficient implementation of Python 3 for microcon |
| 28 | awesome-rust | `rust-unofficial/awesome-rust` | Awesome list |
| 29 | awesome-pest | `pest-parser/awesome-pest` | Parser generator |
| 30 | awesome-haskell | `krispo/awesome-haskell` | Awesome list |
| 31 | awesome-purescript | `passy/awesome-purescript` | Awesome list |
| 32 | awesome-go | `avelino/awesome-go` | Awesome list |
| 33 | awesome-scala | `lauris/awesome-scala` | Awesome list |
| 34 | awesome-scala-native | `tindzk/awesome-scala-native` | Optimizing ahead-of-time compiler for Scala based on LLVM |
| 35 | awesome-ruby | `markets/awesome-ruby` | Awesome list |
| 36 | awesome-clojure | `razum2um/awesome-clojure` | Awesome list |
| 37 | awesome-clojurescript | `hantuzun/awesome-clojurescript` | Awesome list |
| 38 | awesome-elixir | `h4cc/awesome-elixir` | Awesome list |
| 39 | awesome-elm | `sporto/awesome-elm` | Awesome list |
| 40 | awesome-erlang | `drobakowski/awesome-erlang` | Awesome list |
| 41 | Julia.jl | `svaksha/Julia.jl` | High-level dynamic programming language designed to address  |
| 42 | awesome-lua | `LewisJEllis/awesome-lua` | Awesome list |
| 43 | awesome-c | `inputsh/awesome-c` | Awesome list |
| 44 | awesome-cpp | `fffaraz/awesome-cpp` | General-purpose language with a bias toward system programmi |
| 45 | awesome-R | `qinwf/awesome-R` | Functional programming language and environment for statisti |
| 46 | awesome-r-learning-resources | `iamericfletcher/awesome-r-learning-resources` | Awesome list |
| 47 | awesome-d | `dlang-community/awesome-d` | Awesome list |
| 48 | awesome-cl | `CodyReichert/awesome-cl` | Powerful dynamic multiparadigm language that facilitates ite |
| 49 | awesome-common-lisp-learning | `GustavBertram/awesome-common-lisp-learning` | Awesome list |
| 50 | awesome-perl | `hachiojipm/awesome-perl` | Awesome list |
| 51 | awesome-groovy | `kdabir/awesome-groovy` | Awesome list |
| 52 | awesome-dart | `yissachar/awesome-dart` | Awesome list |
| 53 | awesome-java | `akullpp/awesome-java` | Popular secure object-oriented language designed for flexibi |
| 54 | awesome-rxjava | `eleventigers/awesome-rxjava` | Awesome list |
| 55 | awesome-j2me | `hstsethi/awesome-j2me` | Java specification designed for old keypad phones and PDAs |
| 56 | awesome-kotlin | `KotlinBy/awesome-kotlin` | Awesome list |
| 57 | awesome-ocaml | `ocaml-community/awesome-ocaml` | Awesome list |
| 58 | awesome-coldfusion | `seancoyne/awesome-coldfusion` | Awesome list |
| 59 | awesome-fortran | `rabbiabram/awesome-fortran` | Awesome list |
| 60 | awesome-php | `ziadoz/awesome-php` | Server-side scripting language |
| 61 | awesome-composer | `jakoch/awesome-composer` | Package manager |
| 62 | awesome-pascal | `Fr0sT-Brutal/awesome-pascal` | Awesome list |
| 63 | awesome-AutoHotkey | `ahkscript/awesome-AutoHotkey` | Awesome list |
| 64 | awesome-AutoIt | `J2TeaM/awesome-AutoIt` | Awesome list |
| 65 | awesome-crystal | `veelenga/awesome-crystal` | Awesome list |
| 66 | awesome-frege | `sfischer13/awesome-frege` | Haskell for the JVM |
| 67 | awesome-cmake | `onqtam/awesome-cmake` | Build, test, and package software |
| 68 | awesome-actionscript3 | `robinrodricks/awesome-actionscript3` | Object-oriented language targeting Adobe AIR |
| 69 | awesome-eta | `sfischer13/awesome-eta` | Functional programming language for the JVM |
| 70 | awesome-idris | `joaomilho/awesome-idris` | General purpose pure functional programming language with de |
| 71 | awesome-ada | `ohenley/awesome-ada` | Modern programming language designed for large, long-lived a |
| 72 | awesome-qsharp | `ebraminio/awesome-qsharp` | Domain-specific programming language used for expressing qua |
| 73 | awesome-imba | `koolamusic/awesome-imba` | Programming language inspired by Ruby and Python and compile |
| 74 | awesome-vala | `desiderantes/awesome-vala` | Programming language designed to take full advantage of the  |
| 75 | awesome-coq | `coq-community/awesome-coq` | Formal language and environment for programming and specific |
| 76 | awesome-v | `vlang/awesome-v` | Simple, fast, safe, compiled language for developing maintai |
| 77 | awesome-zig | `catdevnull/awesome-zig` | General-purpose programming language and toolchain for maint |
| 78 | awesome-move | `MystenLabs/awesome-move` | Domain-specific programming language for writing safe smart  |
| 79 | awesome-esolangs | `angrykoala/awesome-esolangs` | Programming languages designed for experimentation or as jok |
| 80 | awesome-vba | `sancarn/awesome-vba` | An event-driven version of Visual Basic 6.0 built into most  |
| 81 | awesome-fsharp | `fsprojects/awesome-fsharp` | A .NET-based language with focus on functional programming |

### Front-End Development (85 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | es6-tools | `addyosmani/es6-tools` | Awesome list |
| 2 | awesome-wpo | `davidsonfellipe/awesome-wpo` | Awesome list |
| 3 | tools | `lvwzhen/tools` | Awesome list |
| 4 | awesome-css | `awesome-css-group/awesome-css` | Style sheet language that specifies how HTML elements are di |
| 5 | critical-path-css-tools | `addyosmani/critical-path-css-tools` | Awesome list |
| 6 | scalable-css-reading-list | `davidtheclark/scalable-css-reading-list` | Awesome list |
| 7 | must-watch-css | `AllThingsSmitty/must-watch-css` | Awesome list |
| 8 | css-protips | `AllThingsSmitty/css-protips` | Awesome list |
| 9 | awesome-css-frameworks | `troxler/awesome-css-frameworks` | Awesome list |
| 10 | awesome-react | `enaqx/awesome-react` | JavaScript library for building user interfaces |
| 11 | awesome-relay | `expede/awesome-relay` | Framework for building data-driven React apps |
| 12 | awesome-react-hooks | `glauberfc/awesome-react-hooks` | Lets you use state and other React features without writing  |
| 13 | awesome-web-components | `web-padawan/awesome-web-components` | Awesome list |
| 14 | awesome-polymer | `Granze/awesome-polymer` | JavaScript library to develop Web Components |
| 15 | awesome-angular | `PatrickJS/awesome-angular` | App framework |
| 16 | awesome-backbone | `sadcitizen/awesome-backbone` | App framework |
| 17 | awesome-html5 | `diegocard/awesome-html5` | Markup language used for websites & web apps |
| 18 | awesome-svg | `willianjusten/awesome-svg` | XML-based vector image format |
| 19 | awesome-canvas | `raphamorim/awesome-canvas` | Awesome list |
| 20 | awesome-knockout | `dnbard/awesome-knockout` | JavaScript library |
| 21 | awesome-dojo | `petk/awesome-dojo` | JavaScript toolkit |
| 22 | Inspire | `NoahBuscher/Inspire` | Awesome list |
| 23 | awesome-ember | `ember-community-russia/awesome-ember` | App framework |
| 24 | awesome-android-ui | `wasabeef/awesome-android-ui` | Awesome list |
| 25 | awesome-ios-ui | `cjwirth/awesome-ios-ui` | Awesome list |
| 26 | awesome-meteor | `Urigo/awesome-meteor` | Awesome list |
| 27 | BEM-resources | `sturobson/BEM-resources` | Awesome list |
| 28 | awesome-flexbox | `afonsopacifer/awesome-flexbox` | Awesome list |
| 29 | typography | `deanhume/typography` | Awesome list |
| 30 | awesome-a11y | `brunopulis/awesome-a11y` | Awesome list |
| 31 | awesome-material | `sachin1092/awesome-material` | Awesome list |
| 32 | awesome-d3 | `wbkd/awesome-d3` | Library for producing dynamic, interactive data visualizatio |
| 33 | awesome-emails | `jonathandion/awesome-emails` | Awesome list |
| 34 | awesome-jquery | `petk/awesome-jquery` | Easy to use JavaScript library for DOM manipulation |
| 35 | jquery-tips-everyone-should-know | `AllThingsSmitty/jquery-tips-everyone-should-know` | Awesome list |
| 36 | awesome-webaudio | `notthetup/awesome-webaudio` | Awesome list |
| 37 | offline-first | `pazguille/offline-first` | Awesome list |
| 38 | awesome-static-website-services | `agarrharr/awesome-static-website-services` | Awesome list |
| 39 | awesome-cyclejs | `cyclejs-community/awesome-cyclejs` | Functional and reactive JavaScript framework |
| 40 | awesome-text-editing | `dok/awesome-text-editing` | Awesome list |
| 41 | motion-ui-design | `fliptheweb/motion-ui-design` | Awesome list |
| 42 | awesome-vue | `vuejs/awesome-vue` | App framework |
| 43 | awesome-marionette | `sadcitizen/awesome-marionette` | App framework |
| 44 | awesome-aurelia | `aurelia-contrib/awesome-aurelia` | App framework |
| 45 | awesome-charting | `zingchart/awesome-charting` | Awesome list |
| 46 | awesome-ionic | `candelibas/awesome-ionic` | Awesome list |
| 47 | awesome-chrome-devtools | `ChromeDevTools/awesome-chrome-devtools` | Awesome list |
| 48 | awesome-postcss | `jdrgomes/awesome-postcss` | CSS tool |
| 49 | awesome-draft-js | `nikgraf/awesome-draft-js` | Rich text editor framework for React |
| 50 | awesome-service-workers | `TalAter/awesome-service-workers` | Awesome list |
| 51 | awesome-progressive-web-apps | `TalAter/awesome-progressive-web-apps` | Awesome list |
| 52 | awesome-choo | `choojs/awesome-choo` | App framework |
| 53 | awesome-redux | `brillout/awesome-redux` | State container for JavaScript apps |
| 54 | awesome-browserify | `browserify/awesome-browserify` | Module bundler |
| 55 | awesome-sass | `Famolus/awesome-sass` | CSS preprocessor |
| 56 | awesome-ant-design | `websemantics/awesome-ant-design` | Enterprise-class UI design language |
| 57 | awesome-less | `LucasBassetti/awesome-less` | CSS preprocessor |
| 58 | awesome-webgl | `sjfricke/awesome-webgl` | JavaScript API for rendering 3D graphics |
| 59 | awesome-preact | `preactjs/awesome-preact` | App framework |
| 60 | progressive-enhancement-resources | `jbmoelker/progressive-enhancement-resources` | Awesome list |
| 61 | awesome-nextjs | `unicodeveloper/awesome-nextjs` | Framework for server-rendered React apps |
| 62 | awesome-lit | `web-padawan/awesome-lit` | Library for building web components with a declarative templ |
| 63 | awesome-jamstack | `automata/awesome-jamstack` | Modern web development architecture based on client-side Jav |
| 64 | awesome-wordpress-gatsby | `henrikwirth/awesome-wordpress-gatsby` | Web development technology stack with WordPress as a back en |
| 65 | awesome-mobile-web-development | `myshov/awesome-mobile-web-development` | Creating a great mobile web experience |
| 66 | awesome-storybook | `lauthieb/awesome-storybook` | Development environment for UI components |
| 67 | awesome-blazor | `AdrienTorris/awesome-blazor` | .NET web framework using C#/Razor and HTML that runs in the  |
| 68 | awesome-pagespeed-metrics | `csabapalfi/awesome-pagespeed-metrics` | Metrics to help understand page speed and user experience |
| 69 | awesome-tailwindcss | `aniftyco/awesome-tailwindcss` | Utility-first CSS framework for rapid UI development |
| 70 | awesome-seed-rs | `seed-rs/awesome-seed-rs` | Rust framework for creating web apps running in WebAssembly |
| 71 | awesome-web-performance-budget | `pajaydev/awesome-web-performance-budget` | Techniques to ensure certain performance metrics for a websi |
| 72 | awesome-web-animation | `sergey-pimenov/awesome-web-animation` | Animations in the browser with JavaScript, CSS, SVG, etc |
| 73 | awesome-yew | `jetli/awesome-yew` | Rust framework inspired by Elm and React for creating multi- |
| 74 | awesome-material-ui | `nadunindunil/awesome-material-ui` | Material Design React components for faster and easier web d |
| 75 | awesome-building-blocks-for-web-apps | `componently-com/awesome-building-blocks-for-web-apps` | Standalone features to be integrated into web apps |
| 76 | awesome-svelte | `TheComputerM/awesome-svelte` | App framework |
| 77 | awesome-design-systems | `klaufel/awesome-design-systems` | Collection of reusable components, guided by rules that ensu |
| 78 | awesome-inertiajs | `innocenzi/awesome-inertiajs` | Make single-page apps without building an API |
| 79 | awesome-mdbootstrap | `mdbootstrap/awesome-mdbootstrap` | Templates, layouts, components, and widgets to rapidly build |
| 80 | awesome-master-css | `master-co/awesome-master-css` | A virtual CSS language with enhanced syntax |
| 81 | awesome-hydrogen | `shopify/awesome-hydrogen` | Edge-first framework for building Shopify storefronts with R |
| 82 | awesome-tiny-js | `thoughtspile/awesome-tiny-js` | Frontend libraries that fit into 2 kB with dependencies |
| 83 | awesome-frontend-gis | `joewdavies/awesome-frontend-gis` | Geographic Information Systems (GIS) for web browsers |
| 84 | awesome-webgpu | `mikbry/awesome-webgpu` | JavaScript API for rendering and compute on GPUs |
| 85 | awesome-webassembly | `idematos/awesome-webassembly` | A portable binary format for running code efficiently across |

### Back-End Development (33 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-flask | `mjhea0/awesome-flask` | Python framework |
| 2 | awesome-docker ⚠️ | `veggiemonk/awesome-docker` | Awesome list |
| 3 | awesome-vagrant | `iJackUA/awesome-vagrant` | Automation virtual machine environment |
| 4 | awesome-pyramid | `uralbash/awesome-pyramid` | Python framework |
| 5 | awesome-play1 | `PerfectCarl/awesome-play1` | Awesome list |
| 6 | awesome-cakephp | `friendsofcake/awesome-cakephp` | PHP framework |
| 7 | awesome-symfony | `sitepoint-editors/awesome-symfony` | PHP framework |
| 8 | awesome-symfony-education | `pehapkari/awesome-symfony-education` | Awesome list |
| 9 | awesome-laravel | `chiraggude/awesome-laravel` | PHP framework |
| 10 | Awesome-Laravel-Education | `fukuball/Awesome-Laravel-Education` | Awesome list |
| 11 | awesome-tall-stack | `livewire/awesome-tall-stack` | Full-stack development solution featuring libraries built by |
| 12 | awesome-rails | `gramantin/awesome-rails` | Web app framework for Ruby |
| 13 | awesome-rails-gem | `hothero/awesome-rails-gem` | Packages |
| 14 | awesome-phalcon | `phalcon/awesome-phalcon` | PHP framework |
| 15 | htaccess | `phanan/htaccess` | Awesome list |
| 16 | nginx-resources | `fcambus/nginx-resources` | Web server |
| 17 | awesome-dropwizard | `stve/awesome-dropwizard` | Java framework |
| 18 | awesome-kubernetes | `ramitsurana/awesome-kubernetes` | Open-source platform that automates Linux container operatio |
| 19 | awesome-lumen | `unicodeveloper/awesome-lumen` | PHP micro-framework |
| 20 | awesome-serverless | `pmuens/awesome-serverless` | Serverless computing and serverless architectures |
| 21 | awesome-wicket | `PhantomYdn/awesome-wicket` | Java web app framework |
| 22 | vertx-awesome | `vert-x3/vertx-awesome` | Toolkit for building reactive apps on the JVM |
| 23 | awesome-terraform | `shuaibiyy/awesome-terraform` | Tool for building, changing, and versioning infrastructure |
| 24 | awesome-vapor | `vapor-community/awesome-vapor` | Server-side development in Swift |
| 25 | awesome-dash | `ucg8j/awesome-dash` | Python web app framework |
| 26 | awesome-fastapi | `mjhea0/awesome-fastapi` | Python web app framework |
| 27 | awesome-cdk | `kolomied/awesome-cdk` | Open-source software development framework for defining clou |
| 28 | awesome-iam | `kdeldycke/awesome-iam` | User accounts, authentication and authorization |
| 29 | awesome-slim | `nekofar/awesome-slim` | PHP framework |
| 30 | awesome-fiber | `gofiber/awesome-fiber` | Web framework built on top of Fasthttp, the fastest HTTP eng |
| 31 | awesome-kustomize | `DevOpsHiveHQ/awesome-kustomize` | Kubernetes native declarative configuration management tool |
| 32 | awesome-opentofu | `virtualroot/awesome-opentofu` | Open-source infrastructure as code tool |
| 33 | awesome-reflex | `reflex-dev/awesome-reflex` | Python web framework for building both your frontend and bac |

### Computer Science (38 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-courses | `prakhar1989/awesome-courses` | Awesome list |
| 2 | awesome-datascience | `academic/awesome-datascience` | Awesome list |
| 3 | awesome-learn-datascience | `siboehm/awesome-learn-datascience` | Awesome list |
| 4 | awesome-machine-learning | `josephmisiti/awesome-machine-learning` | Awesome list |
| 5 | Machine-Learning-Tutorials | `ujjwalkarn/Machine-Learning-Tutorials` | Awesome list |
| 6 | machine-learning-with-ruby | `arbox/machine-learning-with-ruby` | Learning, implementing, and applying Machine Learning using  |
| 7 | Awesome-CoreML-Models | `likedan/Awesome-CoreML-Models` | Models for Apple's machine learning framework |
| 8 | awesome-h2o | `h2oai/awesome-h2o` | Open source distributed machine learning platform written in |
| 9 | awesome-seml | `SE-ML/awesome-seml` | From experiment to production-level machine learning |
| 10 | awesome-ai-in-finance | `georgezouq/awesome-ai-in-finance` | Solving problems in finance with machine learning |
| 11 | awesome-jax | `n2cholas/awesome-jax` | Automatic differentiation and XLA compilation brought togeth |
| 12 | awesome-xai | `altamiracorp/awesome-xai` | Providing insight, explanations, and interpretability to mac |
| 13 | speech-language-processing | `edobashira/speech-language-processing` | Awesome list |
| 14 | awesome-spanish-nlp | `dav009/awesome-spanish-nlp` | Awesome list |
| 15 | nlp-with-ruby | `arbox/nlp-with-ruby` | Awesome list |
| 16 | awesome-qa | `seriousran/awesome-qa` | The science of asking and answering in natural language with |
| 17 | awesome-nlg | `accelerated-text/awesome-nlg` | Generation of text used in data-to-text, conversational agen |
| 18 | awesome-linguistics | `theimpossibleastronaut/awesome-linguistics` | Awesome list |
| 19 | awesome-cryptography | `sobolevn/awesome-cryptography` | Awesome list |
| 20 | awesome-crypto-papers | `pFarb/awesome-crypto-papers` | Theory basics for using cryptography by non-cryptographers |
| 21 | awesome-computer-vision | `jbhuang0604/awesome-computer-vision` | Awesome list |
| 22 | awesome-deep-learning | `ChristosChristofidis/awesome-deep-learning` | Neural networks |
| 23 | awesome-tensorflow | `jtoy/awesome-tensorflow` | Library for machine intelligence |
| 24 | awesome-tensorflow-js | `aaronhma/awesome-tensorflow-js` | WebGL-accelerated machine learning JavaScript library for tr |
| 25 | awesome-tensorflow-lite | `margaretmz/awesome-tensorflow-lite` | Framework that optimizes TensorFlow models for on-device mac |
| 26 | awesome-deep-learning-papers | `terryum/awesome-deep-learning-papers` | The most cited deep learning papers |
| 27 | awesome-deep-learning-resources | `guillaume-chevalier/awesome-deep-learning-resources` | Awesome list |
| 28 | awesome-deep-vision | `kjw0612/awesome-deep-vision` | Awesome list |
| 29 | computer-science | `ossu/computer-science` | Awesome list |
| 30 | awesome-functional-programming | `lucasviola/awesome-functional-programming` | Awesome list |
| 31 | awesome-msr | `dspinellis/awesome-msr` | Evidence-based research on software systems |
| 32 | static-analysis | `analysis-tools-dev/static-analysis` | Awesome list |
| 33 | awesome-information-retrieval | `harpribot/awesome-information-retrieval` | Learn to develop your own search engine |
| 34 | awesome-quantum-computing | `desireevl/awesome-quantum-computing` | Computing that utilizes quantum mechanics and qubits on quan |
| 35 | awesome-theoretical-computer-science | `mostafatouny/awesome-theoretical-computer-science` | The interplay of computer science and pure mathematics, dist |
| 36 | awesome-conversational-ai | `jyguyomarch/awesome-conversational-ai` | Build awesome chatbots and digital assistants |
| 37 | awesome-generative-ai | `steven2358/awesome-generative-ai` | Automatically generates a wide range of unique content in te |
| 38 | awesome-position-based-quantum-cryptography | `Renaller/awesome-position-based-quantum-cryptography` | Theory on quantum cryptography that utilizes special relativ |

### Big Data (9 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-bigdata | `0xnr/awesome-bigdata` | Awesome list |
| 2 | awesome-public-datasets | `awesomedata/awesome-public-datasets` | Awesome list |
| 3 | awesome-hadoop | `youngwookim/awesome-hadoop` | Framework for distributed storage and processing of very lar |
| 4 | awesome-data-engineering | `igorbarinov/awesome-data-engineering` | Awesome list |
| 5 | awesome-streaming | `manuzhang/awesome-streaming` | Awesome list |
| 6 | awesome-spark | `awesome-spark/awesome-spark` | Unified engine for large-scale data processing |
| 7 | awesome-qlik | `ambster-public/awesome-qlik` | Business intelligence platform for data visualization, analy |
| 8 | awesome-splunk | `sduff/awesome-splunk` | Platform for searching, monitoring, and analyzing structured |
| 9 | awesome-network-analysis | `briatte/awesome-network-analysis` | Awesome list |

### Theory (14 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | papers-we-love | `papers-we-love/papers-we-love` | Awesome list |
| 2 | awesome-talks | `JanVanRyswyck/awesome-talks` | Awesome list |
| 3 | awesome-algorithms | `tayllan/awesome-algorithms` | Awesome list |
| 4 | awesome-algorithms-education | `gaerae/awesome-algorithms-education` | Learning and practicing |
| 5 | algovis | `enjalot/algovis` | Awesome list |
| 6 | awesome-artificial-intelligence | `owainlewis/awesome-artificial-intelligence` | Awesome list |
| 7 | search-engine-optimization | `marcobiedermann/search-engine-optimization` | Awesome list |
| 8 | awesome-competitive-programming | `lnishan/awesome-competitive-programming` | Awesome list |
| 9 | awesome-math | `rossant/awesome-math` | Awesome list |
| 10 | awesome-recursion-schemes | `passy/awesome-recursion-schemes` | Traversing nested data structures |
| 11 | awesome-audit-algorithms | `erwanlemerrer/awesome-audit-algorithms` | Algorithmic audits of algorithms |
| 12 | awesome-agi-cocosci | `YuzheSHI/awesome-agi-cocosci` | The reciprocation of Artificial General Intelligence (AGI) a |
| 13 | awesome-complexity | `sellisd/awesome-complexity` | The scientific field studying systems with multiple interact |
| 14 | awesome-vlm-architectures | `gokayfem/awesome-vlm-architectures` | Vision Language Model architectures |

### Books (6 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | free-programming-books | `EbookFoundation/free-programming-books` | Awesome list |
| 2 | GoBooks | `dariubs/GoBooks` | Awesome list |
| 3 | rbooks | `RomanTsegelskyi/rbooks` | Awesome list |
| 4 | Mind-Expanding-Books | `hackerkid/Mind-Expanding-Books` | Awesome list |
| 5 | awesome-book-authoring | `TalAter/awesome-book-authoring` | Awesome list |
| 6 | ElixirBooks | `sger/ElixirBooks` | Awesome list |

### Editors (6 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | sublime-bookmarks | `dreikanter/sublime-bookmarks` | Awesome list |
| 2 | vim-galore | `mhinz/vim-galore` | Awesome list |
| 3 | awesome-neovim | `rockerBOO/awesome-neovim` | Vim-fork focused on extensibility and usability |
| 4 | awesome-emacs | `emacs-tw/awesome-emacs` | Awesome list |
| 5 | awesome-atom | `mehcode/awesome-atom` | Open-source and hackable text editor |
| 6 | awesome-vscode | `viatsko/awesome-vscode` | Cross-platform open-source text editor |

### Gaming (29 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | magictools | `ellisonleao/magictools` | Awesome list |
| 2 | awesome-gametalks | `hzoo/awesome-gametalks` | Awesome list |
| 3 | awesome-godot | `godotengine/awesome-godot` | Game engine |
| 4 | awesome-open-source-games | `michelpereira/awesome-open-source-games` | Awesome list |
| 5 | awesome-unity | `RyanNielson/awesome-unity` | Game engine |
| 6 | awesome-chess | `hkirat/awesome-chess` | Awesome list |
| 7 | awesome-love2d | `love2d-community/awesome-love2d` | Game engine |
| 8 | awesome-PICO-8 | `pico-8/awesome-PICO-8` | Fantasy console |
| 9 | awesome-gbdev | `gbdev/awesome-gbdev` | Awesome list |
| 10 | awesome-construct | `ConstructCommunity/awesome-construct` | Game engine |
| 11 | awesome-gideros | `stetso/awesome-gideros` | Game engine |
| 12 | awesome-minecraft | `bs-community/awesome-minecraft` | Sandbox video game |
| 13 | awesome-computercraft | `tomodachi94/awesome-computercraft` | Minecraft mod that adds programmable computers |
| 14 | game-datasets | `leomaurodesenv/game-datasets` | Materials and datasets for Artificial Intelligence in games |
| 15 | awesome-haxe-gamedev | `Dvergar/awesome-haxe-gamedev` | A high-level strongly typed programming language used to pro |
| 16 | awesome-libgdx | `rafaskb/awesome-libgdx` | Java game framework |
| 17 | awesome-playcanvas | `playcanvas/awesome-playcanvas` | Game engine |
| 18 | awesome-game-remakes | `radek-sprta/awesome-game-remakes` | Actively maintained open-source game remakes |
| 19 | awesome-flame | `flame-engine/awesome-flame` | Game engine for Flutter |
| 20 | awesome-discord-communities | `mhxion/awesome-discord-communities` | Chat with friends and communities |
| 21 | awesome-chip-8 | `tobiasvl/awesome-chip-8` | Virtual computer game machine from the 70s |
| 22 | awesome-games-of-coding | `michelpereira/awesome-games-of-coding` | Learn a programming language by making games |
| 23 | awesome-esports | `strift/awesome-esports` | Video games played as a sport |
| 24 | awesome-learn-gamedev | `notpresident35/awesome-learn-gamedev` | The craft of video game creation |
| 25 | awesome-game-engine-dev | `stevinz/awesome-game-engine-dev` | Building software to speed up game creation |
| 26 | awesome-gamemaker | `bytecauldron/awesome-gamemaker` | Game engine |
| 27 | awesome-game-production | `vhladiienko/awesome-game-production` | Leading the process of designing, developing, testing and di |
| 28 | awesome-babylonjs | `Symbitic/awesome-babylonjs` | Game engine for cross-platform web and native game developme |
| 29 | awesome-roblox | `awesome-roblox/awesome-roblox` | Immersive platform for hosting millions of diverse experienc |

### Development Environment (23 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | quick-look-plugins | `sindresorhus/quick-look-plugins` | For macOS |
| 2 | awesome-devenv | `jondot/awesome-devenv` | Awesome list |
| 3 | awesome-dotfiles | `webpro/awesome-dotfiles` | Awesome list |
| 4 | awesome-shell | `alebcay/awesome-shell` | Awesome list |
| 5 | awsm.fish | `jorgebucaran/awsm.fish` | User-friendly shell |
| 6 | awesome-cli-apps | `agarrharr/awesome-cli-apps` | Awesome list |
| 7 | awesome-zsh-plugins | `unixorn/awesome-zsh-plugins` | Awesome list |
| 8 | awesome-github | `phillipadsmith/awesome-github` | Hosting service for Git repositories |
| 9 | awesome-browser-extensions-for-github | `stefanbuck/awesome-browser-extensions-for-github` | Awesome list |
| 10 | github-cheat-sheet | `tiimgreen/github-cheat-sheet` | Awesome list |
| 11 | awesome-pinned-gists | `matchai/awesome-pinned-gists` | Dynamic pinned gists for your GitHub profile |
| 12 | git-cheat-sheet | `arslanbilal/git-cheat-sheet` | Awesome list |
| 13 | tips | `git-tips/tips` | Awesome list |
| 14 | awesome-git-addons | `stevemao/awesome-git-addons` | Enhance the `git` CLI |
| 15 | awesome-git-hooks | `compscilauren/awesome-git-hooks` | Scripts for automating tasks during `git` workflows |
| 16 | awesome-ssh | `moul/awesome-ssh` | Awesome list |
| 17 | FOSS-for-Dev | `tvvocold/FOSS-for-Dev` | Awesome list |
| 18 | awesome-hyper | `bnb/awesome-hyper` | Cross-platform terminal app built on web technologies |
| 19 | awesome-powershell | `janikvonrotz/awesome-powershell` | Cross-platform object-oriented shell |
| 20 | awesome-alfred-workflows | `alfred-workflows/awesome-alfred-workflows` | Productivity app for macOS |
| 21 | terminals-are-sexy | `k4m4/terminals-are-sexy` | Awesome list |
| 22 | awesome-actions | `sdras/awesome-actions` | Create tasks to automate your workflow and share them with o |
| 23 | awesome-wezterm | `michaelbrusegard/awesome-wezterm` | Powerful cross-platform terminal emulator |

### Entertainment (5 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-scifi | `sindresorhus/awesome-scifi` | Scifi |
| 2 | awesome-fantasy | `RichardLitt/awesome-fantasy` | Awesome list |
| 3 | awesome-geek-podcasts | `ayr-ton/awesome-geek-podcasts` | Awesome list |
| 4 | awesome-newsletters | `zudochkin/awesome-newsletters` | Awesome list |
| 5 | awesome-it-quotes | `victorlaerte/awesome-it-quotes` | Awesome list |

### Databases (19 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-db | `numetriclabz/awesome-db` | Awesome list |
| 2 | awesome-mysql | `shlomi-noach/awesome-mysql` | Awesome list |
| 3 | awesome-sqlalchemy | `dahlia/awesome-sqlalchemy` | Awesome list |
| 4 | awesome-influxdb | `mark-rushakoff/awesome-influxdb` | Awesome list |
| 5 | awesome-neo4j | `neueda/awesome-neo4j` | Awesome list |
| 6 | awesome-mongodb | `ramnes/awesome-mongodb` | NoSQL database |
| 7 | awesome-rethinkdb | `d3viant0ne/awesome-rethinkdb` | Awesome list |
| 8 | awesome-tinkerpop | `mohataher/awesome-tinkerpop` | Graph computing framework |
| 9 | awesome-postgres | `dhamaniasad/awesome-postgres` | Object-relational database |
| 10 | awesome-couchdb | `quangv/awesome-couchdb` | Document-oriented NoSQL database |
| 11 | awesome-hbase | `rayokota/awesome-hbase` | Distributed, scalable, big data store |
| 12 | awesome-nosql-guides | `erictleung/awesome-nosql-guides` | Help on using non-relational, distributed, open-source, and  |
| 13 | awesome-db-tools | `mgramin/awesome-db-tools` | Everything that makes working with databases easier |
| 14 | typedb-awesome | `vaticle/typedb-awesome` | Logical database to organize large and complex networks of d |
| 15 | awesome-cassandra | `Anant/awesome-cassandra` | Open-source, distributed, wide column store, NoSQL database  |
| 16 | awesome-tdengine | `taosdata/awesome-tdengine` | An open-source time-series database with high-performance, s |
| 17 | awesome-supabase | `lyqht/awesome-supabase` | An open-source alternative to Firebase |
| 18 | awesome-pocketbase | `benallfree/awesome-pocketbase` | An open-source, Go-based backend in one file |
| 19 | awesome-neon | `tyaga001/awesome-neon` | An open-source alternative to AWS Aurora |

### Media (14 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-fonts | `brabadu/awesome-fonts` | Awesome list |
| 2 | codeface | `chrissimpkins/codeface` | Text editor fonts |
| 3 | awesome-stock-resources | `neutraltone/awesome-stock-resources` | Awesome list |
| 4 | awesome-gif | `davisonio/awesome-gif` | Image format known for animated images |
| 5 | awesome-music | `ciconia/awesome-music` | Awesome list |
| 6 | awesome-opensource-documents | `44bits/awesome-opensource-documents` | Awesome list |
| 7 | awesome-audio-visualization | `willianjusten/awesome-audio-visualization` | Awesome list |
| 8 | awesome-broadcasting | `ebu/awesome-broadcasting` | Awesome list |
| 9 | awesome-pixel-art | `Siilwyn/awesome-pixel-art` | Pixel-level digital art |
| 10 | awesome-ffmpeg | `transitive-bullshit/awesome-ffmpeg` | Cross-platform solution to record, convert and stream audio  |
| 11 | awesome-icons | `notlmn/awesome-icons` | Downloadable SVG/PNG/font icon projects |
| 12 | awesome-audiovisual | `stingalleman/awesome-audiovisual` | Lighting, audio and video in professional environments |
| 13 | awesome-vlc | `mfkl/awesome-vlc` | Cross-platform media player software and streaming server |
| 14 | awesome-aoip | `Mo-way/awesome-aoip` | Reliably transmit uncompressed, low-latency audio over an IP |

### Learn (16 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-workshopper | `therebelrobot/awesome-workshopper` | Interactive tutorials |
| 2 | learn-to-program | `karlhorky/learn-to-program` | Awesome list |
| 3 | awesome-speaking | `matteofigus/awesome-speaking` | Awesome list |
| 4 | awesome-tech-videos | `lucasviola/awesome-tech-videos` | Awesome list |
| 5 | dive-into-machine-learning | `hangtwenty/dive-into-machine-learning` | Awesome list |
| 6 | awesome-computer-history | `watson/awesome-computer-history` | Awesome list |
| 7 | awesome-programming-for-kids | `HollyAdele/awesome-programming-for-kids` | Awesome list |
| 8 | awesome-steam | `RahulBirCodes/awesome-steam` | Supplements for specific science, technology, engineering, a |
| 9 | awesome-educational-games | `yrgo/awesome-educational-games` | Learn while playing |
| 10 | awesome-javascript-learning | `micromata/awesome-javascript-learning` | Awesome list |
| 11 | awesome-css-learning | `micromata/awesome-css-learning` | Mainly about CSS – the language and the modules |
| 12 | awesome-product-management | `dend/awesome-product-management` | Learn how to be a better product manager |
| 13 | awesome-roadmaps | `liuchong/awesome-roadmaps` | Gives you a clear route to improve your knowledge and skills |
| 14 | awesome-youtubers | `JoseDeFreitas/awesome-youtubers` | Watch video tutorials from YouTubers that teach you about te |
| 15 | awesome-hackathon | `dribdat/awesome-hackathon` | Running fun and productive hackathons |
| 16 | awesome-certificates | `PanXProject/awesome-certificates` | Free computer science certifications to showcase your knowle |

### Security (28 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-appsec | `paragonie/awesome-appsec` | Awesome list |
| 2 | awesome-security | `sbilly/awesome-security` | Awesome list |
| 3 | awesome-ctf | `apsdehal/awesome-ctf` | Capture The Flag |
| 4 | awesome-cyber-security-university | `brootware/awesome-cyber-security-university` | Free educational resources that focus on learning by doing |
| 5 | awesome-malware-analysis | `rshipp/awesome-malware-analysis` | Awesome list |
| 6 | android-security-awesome | `ashishb/android-security-awesome` | Awesome list |
| 7 | awesome-hacking | `carpedm20/awesome-hacking` | Awesome list |
| 8 | awesome-hacking-locations | `daviddias/awesome-hacking-locations` | Awesome list |
| 9 | awesome-honeypots | `paralax/awesome-honeypots` | Deception trap, designed to entice an attacker into attempti |
| 10 | awesome-incident-response | `meirwah/awesome-incident-response` | Awesome list |
| 11 | awesome-vehicle-security | `jaredthecoder/awesome-vehicle-security` | Awesome list |
| 12 | awesome-web-security | `qazbnm456/awesome-web-security` | Security of web apps & services |
| 13 | awesome-lockpicking | `fabacab/awesome-lockpicking` | The art of unlocking a lock by manipulating its components w |
| 14 | awesome-cybersecurity-blueteam | `fabacab/awesome-cybersecurity-blueteam` | Groups of individuals who identify security flaws in informa |
| 15 | awesome-fuzzing | `cpuu/awesome-fuzzing` | Automated software testing technique that involves feeding p |
| 16 | awesome-embedded-and-iot-security | `fkie-cad/awesome-embedded-and-iot-security` | Awesome list |
| 17 | awesome-gdpr | `bakke92/awesome-gdpr` | Regulation on data protection and privacy for all individual |
| 18 | awesome-devsecops | `TaptuIT/awesome-devsecops` | Integration of security practices into [DevOps](https://en.w |
| 19 | awesome-executable-packing | `dhondta/awesome-executable-packing` | Packing and unpacking executable formats |
| 20 | awesome-malware-persistence | `Karneades/awesome-malware-persistence` | Techniques that adversaries use to keep access to systems ac |
| 21 | awesome-evm-security | `kareniel/awesome-evm-security` | Understanding the Ethereum Virtual Machine security ecosyste |
| 22 | awesome-password-cracking | `n0kovo/awesome-password-cracking` | The process of recovering passwords from data that has been  |
| 23 | awesome-security-card-games | `Karneades/awesome-security-card-games` | Train your skills and discuss various security topics |
| 24 | awesome-suricata | `satta/awesome-suricata` | Intrusion detection/prevention system and network security m |
| 25 | awesome-prompt-injection | `FonduAI/awesome-prompt-injection` | A type of vulnerability that specifically targets machine le |
| 26 | awesome-detection-engineering | `infosecB/awesome-detection-engineering` | Design, build, and operate detective cybersecurity controls |
| 27 | awesome-annual-security-reports | `jacobdjwilson/awesome-annual-security-reports` | Exploring cybersecurity trends, insights, and challenges |
| 28 | awesome-cicd-attacks | `TupleType/awesome-cicd-attacks` | Offensive research of systems and processes related to devel |

### Content Management Systems (11 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-umbraco | `umbraco-community/awesome-umbraco` | Awesome list |
| 2 | awesome-refinerycms | `refinerycms-contrib/awesome-refinerycms` | Ruby on Rails CMS |
| 3 | awesome-wagtail | `springload/awesome-wagtail` | Django CMS focused on flexibility and user experience |
| 4 | awesome-textpattern | `drmonkeyninja/awesome-textpattern` | Lightweight PHP-based CMS |
| 5 | awesome-drupal | `nirgn975/awesome-drupal` | Extensible PHP-based CMS |
| 6 | awesome ⚠️ | `craftcms/awesome` | Content-first CMS |
| 7 | Awesome-Sitecore | `MartinMiles/Awesome-Sitecore` | .NET digital marketing platform that combines CMS with tools |
| 8 | awesome-silverstripe-cms | `wernerkrauss/awesome-silverstripe-cms` | PHP MVC framework that serves as a classic or headless CMS |
| 9 | awesome-directus | `directus-community/awesome-directus` | A real-time API and app dashboard for managing SQL database  |
| 10 | awesome-plone | `collective/awesome-plone` | Open source Python CMS |
| 11 | awesome-payload | `DanailMinchev/awesome-payload` | Next.js native and open source headless CMS |

### Hardware (11 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-robotics | `Kiloreux/awesome-robotics` | Awesome list |
| 2 | awesome-iot | `HQarroum/awesome-iot` | Awesome list |
| 3 | awesome-electronics | `kitspace/awesome-electronics` | For electronic engineers and hobbyists |
| 4 | awesome-beacon | `rabschi/awesome-beacon` | Awesome list |
| 5 | guitarspecs | `gitfrage/guitarspecs` | Checklist for building your own electric guitar |
| 6 | awesome-plotters | `beardicus/awesome-plotters` | Computer-controlled drawing machines and other visual art ro |
| 7 | awesome-robotic-tooling | `protontypes/awesome-robotic-tooling` | Free and open tools for professional robotic development |
| 8 | awesome-lidar | `szenergy/awesome-lidar` | Sensor for measuring distances by illuminating the target wi |
| 9 | awesome-open-hardware | `delftopenhardware/awesome-open-hardware` | Open-source hardware projects |
| 10 | awesome-adsb | `rickstaa/awesome-adsb` | Technology broadcasting aircraft's identity, position, and d |
| 11 | awesome-flying-fpv | `Matthias84/awesome-flying-fpv` | Open hardware and software related to drones / UAVs |

### Business (12 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-open-company | `opencompany/awesome-open-company` | Awesome list |
| 2 | PlacesToPostYourStartup | `mmccaff/PlacesToPostYourStartup` | Awesome list |
| 3 | awesome-okr | `domenicosolazzo/awesome-okr` | Goal setting & communication best practices |
| 4 | awesome-indie | `mezod/awesome-indie` | Independent developer businesses |
| 5 | ToolsOfTheTrade | `cjbarber/ToolsOfTheTrade` | Tools used by companies on Hacker News |
| 6 | awesome-clean-tech | `nglgzz/awesome-clean-tech` | Fighting climate change with technology |
| 7 | awesome-wardley-maps | `wardley-maps-community/awesome-wardley-maps` | Provides high situational awareness to help improve strategi |
| 8 | awesome-social-enterprise | `RayBB/awesome-social-enterprise` | Building an organization primarily focused on social impact  |
| 9 | awesome-engineering-team-management | `kdeldycke/awesome-engineering-team-management` | How to transition from software development to engineering m |
| 10 | awesome-developer-first | `agamm/awesome-developer-first` | Products that target developers as the user |
| 11 | awesome-billing | `kdeldycke/awesome-billing` | Payments, invoicing, pricing, accounting, marketplace, fraud |
| 12 | awesome-engineering-strategy | `aleixmorgadas/awesome-engineering-strategy` | How to design and execute engineering strategies for tech le |

### Work (9 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-slack | `matiassingers/awesome-slack` | Team collaboration |
| 2 | awesome-slack | `filipelinhares/awesome-slack` | Awesome list |
| 3 | awesome-remote-job | `lukasz-madon/awesome-remote-job` | Awesome list |
| 4 | awesome-productivity | `jyguyomarch/awesome-productivity` | Awesome list |
| 5 | awesome-job-boards | `tramcar/awesome-job-boards` | Awesome list |
| 6 | awesome-interview-questions | `DopplerHQ/awesome-interview-questions` | Awesome list |
| 7 | awesome-code-review | `joho/awesome-code-review` | Reviewing code |
| 8 | awesome-creative-technology | `j0hnm4r5/awesome-creative-technology` | Businesses & groups that specialize in combining computing,  |
| 9 | awesome-internships | `lodthe/awesome-internships` | CV writing guides and companies that hire interns |

### Networking (6 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-sdn | `sdnds-tw/awesome-sdn` | Awesome list |
| 2 | awesome-pcaptools | `caesar0301/awesome-pcaptools` | Awesome list |
| 3 | awesome-rtc | `rtckit/awesome-rtc` | Network protocols for near simultaneous exchange of media an |
| 4 | awesome-snmp | `eozer/awesome-snmp` | A protocol for collecting, modifying, and organizing informa |
| 5 | awesome-scapy | `secdev/awesome-scapy` | Python-based interactive packet manipulation |
| 6 | awesome-cilium | `seifrajhi/awesome-cilium` | Provides networking and security capabilities for containeri |

### Decentralized Systems (17 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-bitcoin | `igorbarinov/awesome-bitcoin` | Bitcoin services and tools for software developers |
| 2 | awesome-ripple | `vhpoet/awesome-ripple` | Open source distributed settlement network |
| 3 | awesome-non-financial-blockchain | `machinomy/awesome-non-financial-blockchain` | Non-financial blockchain applications |
| 4 | awesome-mastodon | `hyperupcall/awesome-mastodon` | Open source decentralized microblogging network |
| 5 | Awesome-Ethereum | `ttumiel/Awesome-Ethereum` | Distributed computing platform for smart contract developmen |
| 6 | awesome-blockchain-ai | `steven2358/awesome-blockchain-ai` | Blockchain projects for artificial intelligence and machine  |
| 7 | awesome-eosio | `DanailMinchev/awesome-eosio` | A decentralized operating system supporting industrial-scale |
| 8 | awesome-corda | `chainstack/awesome-corda` | Open source blockchain platform designed for business |
| 9 | awesome-waves | `msmolyakov/awesome-waves` | Open source blockchain platform and development toolset for  |
| 10 | awesome-substrate | `substrate-developer-hub/awesome-substrate` | Framework for writing scalable, upgradeable blockchains in R |
| 11 | awesome-golem | `golemfactory/awesome-golem` | Open source peer-to-peer marketplace for computing resources |
| 12 | awesome-stacks-chain | `friedger/awesome-stacks-chain` | A smart contract platform secured by Bitcoin |
| 13 | awesome-algorand | `aorumbayev/awesome-algorand` | An open-source, proof of stake blockchain and smart contract |
| 14 | awesome-zeronet | `zolagonano/awesome-zeronet` | A decentralized web-like network of peer-to-peer users |
| 15 | awesome-cosmos | `cosmos/awesome-cosmos` | Modular framework for building app-specific blockchains in G |
| 16 | awesome-tor | `polycarbohydrate/awesome-tor` | A free overlay network for enabling anonymous communication |
| 17 | awesome-atproto | `atblueprints/awesome-atproto` | Open, decentralized network for building social apps |

### Health and Social Science (13 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-bioie | `caufieldjh/awesome-bioie` | How to extract information from unstructured biomedical data |
| 2 | awesome-computational-neuroscience | `eselkin/awesome-computational-neuroscience` | A multidisciplinary science which uses computational approac |
| 3 | awesome-diversity | `folkswhocode/awesome-diversity` | Creating a more inclusive and diverse tech community |
| 4 | awesome-digital-history | `maehr/awesome-digital-history` | Computer-aided scientific investigation of history |
| 5 | empathy-in-engineering | `KimberlyMunoz/empathy-in-engineering` | Building and promoting more compassionate engineering cultur |
| 6 | awesome-healthcare | `kakoni/awesome-healthcare` | Open source healthcare software for facilities, providers, d |
| 7 | awesome-humane-tech | `humanetech-community/awesome-humane-tech` | Open source projects that help improve society |
| 8 | awesome-mental-health | `dreamingechoes/awesome-mental-health` | Mental health awareness and self-care in the software indust |
| 9 | awesome-neuroscience | `analyticalmonk/awesome-neuroscience` | Study of the nervous system and brain |
| 10 | awesome-digital-humanities | `dh-tech/awesome-digital-humanities` | Software for humanities scholars using quantitative or compu |
| 11 | awesome-lucid-dreams | `IAmCoder/awesome-lucid-dreams` | A dream where one becomes aware they are dreaming |
| 12 | awesome-neuroimaging | `NPACore/awesome-neuroimaging` | Software for analyzing brain data from living subjects |
| 13 | awesome-transgender | `cvyl/awesome-transgender` | Someone whose gender identity differs from their assigned bi |

### Events (3 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-creative-tech-events | `danvoyce/awesome-creative-tech-events` | Events around the globe for creative coding, tech, design, m |
| 2 | awesome-italy-events | `ildoc/awesome-italy-events` | Tech-related events in Italy |
| 3 | awesome-netherlands-events | `awkward/awesome-netherlands-events` | Tech-related events in the Netherlands |

### Testing (11 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-testing | `TheJambo/awesome-testing` | Software testing |
| 2 | awesome-regression-testing | `mojoaxel/awesome-regression-testing` | Ensures changes did not break the functionality or style |
| 3 | awesome-selenium | `christian-bromann/awesome-selenium` | Open-source browser automation framework and ecosystem |
| 4 | awesome-appium | `SrinivasanTarget/awesome-appium` | Test automation tool for apps |
| 5 | awesome-tap | `sindresorhus/awesome-tap` | Test Anything Protocol |
| 6 | awesome-jmeter | `aliesbelik/awesome-jmeter` | Load testing and performance measurement tool |
| 7 | awesome-k6 | `grafana/awesome-k6` | Open-source, developer-centric performance monitoring and lo |
| 8 | awesome-playwright | `mxschmitt/awesome-playwright` | Node.js library to automate Chromium, Firefox and WebKit wit |
| 9 | awesome-quality-assurance-roadmap | `fityanos/awesome-quality-assurance-roadmap` | How to start & build a career in software testing |
| 10 | awesome-gatling | `aliesbelik/awesome-gatling` | Open-source load and performance testing framework based on  |
| 11 | awesome-coderabbit | `coderabbitai/awesome-coderabbit` | AI-powered code review platform |

### Miscellaneous (135 repos)

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | awesome-scientific-writing | `writing-resources/awesome-scientific-writing` | Distraction-free scientific writing with Markdown, reStructu |
| 2 | awesome-json | `burningtree/awesome-json` | Text based data interchange format |
| 3 | awesome-geojson | `tmcw/awesome-geojson` | Awesome list |
| 4 | awesome-json-datasets | `jdorfman/awesome-json-datasets` | Awesome list |
| 5 | awesomeCSV | `secretGeek/awesomeCSV` | A text file format that stores tabular data and uses a comma |
| 6 | discount-for-student-dev | `AchoArnold/discount-for-student-dev` | Awesome list |
| 7 | awesome-radio | `kyleterry/awesome-radio` | Awesome list |
| 8 | awesome ⚠️ | `sindresorhus/awesome` | Recursion illustrated |
| 9 | awesome-analytics | `0xnr/awesome-analytics` | Awesome list |
| 10 | awesome-rest | `marmelab/awesome-rest` | Awesome list |
| 11 | awesome-ciandcd | `cicdops/awesome-ciandcd` | Awesome list |
| 12 | services-engineering | `mmcgrana/services-engineering` | Awesome list |
| 13 | free-for-dev | `ripienaar/free-for-dev` | Awesome list |
| 14 | awesome-answers | `cyberglot/awesome-answers` | Stack Overflow, Quora, etc |
| 15 | awesome-sketch | `diessica/awesome-sketch` | Design app for macOS |
| 16 | awesome-projects-boilerplates | `melvin0008/awesome-projects-boilerplates` | Awesome list |
| 17 | awesome-readme | `matiassingers/awesome-readme` | Awesome list |
| 18 | awesome-github-wiki | `MyHoneyBadger/awesome-github-wiki` | Comprehensive documentation on GitHub beyond README |
| 19 | guides | `NARKOZ/guides` | Awesome list |
| 20 | engineering-blogs | `kilimchoi/engineering-blogs` | Awesome list |
| 21 | awesome-selfhosted ⚠️ | `awesome-selfhosted/awesome-selfhosted` | Awesome list |
| 22 | awesome-foss-apps | `DataDaoDe/awesome-foss-apps` | Awesome list |
| 23 | awesome-gulp | `alferov/awesome-gulp` | Task runner |
| 24 | amas | `sindresorhus/amas` | Ask Me Anything |
| 25 | awesome-ama-answers | `stoeffel/awesome-ama-answers` | Awesome list |
| 26 | awesome-OpenSourcePhotography | `ibaaj/awesome-OpenSourcePhotography` | Awesome list |
| 27 | awesome-opengl | `eug/awesome-opengl` | Cross-platform API for rendering 2D and 3D graphics |
| 28 | awesome-graphql | `chentsulin/awesome-graphql` | Awesome list |
| 29 | urban-and-regional-planning-resources | `APA-Technology-Division/urban-and-regional-planning-resources` | Concerning the built environment and communities |
| 30 | awesome-transit | `CUTR-at-USF/awesome-transit` | Awesome list |
| 31 | awesome-research | `emptymalei/awesome-research` | Awesome list |
| 32 | awesome-dataviz | `javierluraschi/awesome-dataviz` | Awesome list |
| 33 | awesome-microservices | `mfornos/awesome-microservices` | Awesome list |
| 34 | Awesome-Unicode | `jagracey/Awesome-Unicode` | Unicode standards, quirks, packages and resources |
| 35 | awesome-codepoints | `Codepoints/awesome-codepoints` | Awesome list |
| 36 | awesome-for-beginners | `MunGell/awesome-for-beginners` | Awesome list |
| 37 | awesome-katas | `gamontal/awesome-katas` | Awesome list |
| 38 | toolsforactivism | `drewrwilson/toolsforactivism` | Awesome list |
| 39 | citizen-science | `dylanrees/citizen-science` | For community-based and non-institutional scientists |
| 40 | awesome-mqtt | `hobbyquaker/awesome-mqtt` | "Internet of Things" connectivity protocol |
| 41 | awesome4girls | `cristianoliveira/awesome4girls` | Awesome list |
| 42 | awesome-vorpal | `vorpaljs/awesome-vorpal` | Node.js CLI framework |
| 43 | awesome-vulkan | `vinjn/awesome-vulkan` | Low-overhead, cross-platform 3D graphics and compute API |
| 44 | awesome-LaTeX | `egeerardyn/awesome-LaTeX` | Typesetting language |
| 45 | awesome-economics | `antontarasenko/awesome-economics` | An economist's starter kit |
| 46 | awesome-funny-markov | `sublimino/awesome-funny-markov` | Awesome list |
| 47 | Awesome-Bioinformatics | `danielecook/Awesome-Bioinformatics` | Awesome list |
| 48 | awesome-cheminformatics | `hsiaoyi0504/awesome-cheminformatics` | Informatics techniques applied to problems in chemistry |
| 49 | Colorful | `Siddharth11/Colorful` | Choose your next color scheme |
| 50 | awesome-steam | `scholtzm/awesome-steam` | Digital distribution platform |
| 51 | bots | `hackerkid/bots` | Building bots |
| 52 | awesome-sre | `dastergon/awesome-sre` | Awesome list |
| 53 | awesome-dtrace | `xen0l/awesome-dtrace` | Dynamic tracing framework |
| 54 | awesome-userscripts | `bvolpato/awesome-userscripts` | Enhance your browsing experience |
| 55 | awesome-pokemon | `tobiasbueschel/awesome-pokemon` | Pokémon and Pokémon GO |
| 56 | awesome-chatops | `exAspArk/awesome-chatops` | Managing technical and business operations through a chat |
| 57 | awesome-falsehood | `kdeldycke/awesome-falsehood` | Falsehoods programmers believe in |
| 58 | awesome-ddd | `heynickc/awesome-ddd` | Software development approach for complex needs by connectin |
| 59 | awesome-quantified-self | `woop/awesome-quantified-self` | Self-tracking through technology |
| 60 | awesome-saltstack | `hbokh/awesome-saltstack` | Python-based config management system |
| 61 | awesome-web-design | `nicolesaidy/awesome-web-design` | For digital designers |
| 62 | awesome-creative-coding | `terkelg/awesome-creative-coding` | Programming something expressive instead of something functi |
| 63 | awesome-no-login-web-apps | `aviaryan/awesome-no-login-web-apps` | Web apps that work without login |
| 64 | awesome-free-software | `johnjago/awesome-free-software` | Free as in freedom |
| 65 | awesome-framer | `podo/awesome-framer` | Prototyping interactive UI designs |
| 66 | awesome-markdown | `BubuAnabelas/awesome-markdown` | Markup language |
| 67 | awesome-dev-fun | `mislavcimpersak/awesome-dev-fun` | Funny developer projects |
| 68 | awesome-magento2 | `DavidLambauer/awesome-magento2` | Open Source eCommerce built with PHP |
| 69 | awesome-tikz | `xiaohanyu/awesome-tikz` | Graph drawing packages for TeX/LaTeX/ConTeXt |
| 70 | awesome-ad-free | `johnjago/awesome-ad-free` | Ad-free alternatives |
| 71 | awesome-prometheus | `roaldnefs/awesome-prometheus` | Open-source monitoring system |
| 72 | awesome-homematic | `homematic-community/awesome-homematic` | Smart home devices |
| 73 | awesome-ledger | `sfischer13/awesome-ledger` | Double-entry accounting on the command-line |
| 74 | awesome-web-monetization | `thomasbnt/awesome-web-monetization` | A free open web standard service that allows you to send mon |
| 75 | awesome-uncopyright | `johnjago/awesome-uncopyright` | Public domain works |
| 76 | awesome-coins | `Zheaoli/awesome-coins` | Digital currency where encryption is used to regulate the ge |
| 77 | awesome-open-source-supporters | `zachflower/awesome-open-source-supporters` | Companies that offer their tools and services for free to op |
| 78 | awesome-design-principles | `robinstickel/awesome-design-principles` | Create better and more consistent designs and experiences |
| 79 | awesome-theravada | `johnjago/awesome-theravada` | Teachings from the Theravada Buddhist tradition |
| 80 | awesome-inspectit | `inspectit-labs/awesome-inspectit` | Open source Java app performance management tool |
| 81 | awesome-maintainers | `nayafia/awesome-maintainers` | The experience of being an open source maintainer |
| 82 | awesome-calculators | `xxczaki/awesome-calculators` | Calculators for every platform |
| 83 | awesome-captcha | `ZYSzys/awesome-captcha` | A type of challenge–response test used in computing to deter |
| 84 | awesome-jupyter | `markusschanta/awesome-jupyter` | Create and share documents that contain code, equations, vis |
| 85 | awesome-frc | `andrewda/awesome-frc` | International high school robotics championship |
| 86 | awesome-speakers | `karlhorky/awesome-speakers` | Conference and meetup speakers in the programming and design |
| 87 | awesome-board-games | `edm00se/awesome-board-games` | Table-top gaming fun for all |
| 88 | awesome-software-patreons | `uraimo/awesome-software-patreons` | Fund individual programmers or the development of open sourc |
| 89 | awesome-parasite | `ecohealthalliance/awesome-parasite` | Parasites and host-pathogen interactions |
| 90 | awesome-food | `jzarca01/awesome-food` | Food-related projects on GitHub |
| 91 | awesome-bitcoin-payment-processors | `alexk111/awesome-bitcoin-payment-processors` | Start accepting Bitcoin |
| 92 | awesome-scientific-computing | `nschloe/awesome-scientific-computing` | Solving complex scientific problems using computers |
| 93 | awesome-amazon-seller | `ScaleLeap/awesome-amazon-seller` | Awesome list |
| 94 | awesome-agriculture | `brycejohnston/awesome-agriculture` | Open source technology for farming and gardening |
| 95 | awesome-product-design | `ttt30ga/awesome-product-design` | Design a product from the initial concept to production |
| 96 | awesome-prisma | `catalinmiron/awesome-prisma` | Turn your database into a GraphQL API |
| 97 | awesome-software-architecture | `simskij/awesome-software-architecture` | The discipline of designing and building software |
| 98 | awesome-connectivity-info | `stevesong/awesome-connectivity-info` | Better understand who has access to telecommunication and in |
| 99 | awesome-stacks | `stackshareio/awesome-stacks` | Tech stacks for building different apps and features |
| 100 | awesome-cytodata | `cytodata/awesome-cytodata` | Image-based profiling of biological phenotypes for computati |
| 101 | awesome-irc | `davisonio/awesome-irc` | Open source messaging protocol |
| 102 | awesome-ads | `cenoura/awesome-ads` | Advertising and programmatic media for websites |
| 103 | awesome-earth | `philsturgeon/awesome-earth` | Find ways to resolve the climate crisis |
| 104 | awesome-naming | `gruhn/awesome-naming` | Naming things in computer science done right |
| 105 | awesome-web-archiving | `iipc/awesome-web-archiving` | An effort to preserve the Web for future generations |
| 106 | awesome-wp-cli | `schlessera/awesome-wp-cli` | Command-line interface for WordPress |
| 107 | awesome-credit-modeling | `mourarthur/awesome-credit-modeling` | Methods for classifying credit applicants into risk classes |
| 108 | awesome-ansible | `ansible-community/awesome-ansible` | A Python-based, open-source IT configuration management and  |
| 109 | awesome-biological-visualizations | `keller-mark/awesome-biological-visualizations` | Interactive visualization of biological data on the web |
| 110 | awesome-qr-code | `make-github-pseudonymous-again/awesome-qr-code` | A type of matrix barcode that can be used to store and share |
| 111 | awesome-veganism | `sdassow/awesome-veganism` | Making the plant-based lifestyle easy and accessible |
| 112 | awesome-translations | `mbiesiad/awesome-translations` | The transfer of the meaning of a text from one language to a |
| 113 | awesome-scriptable | `dersvenhesse/awesome-scriptable` | An iOS app for automation in JavaScript |
| 114 | awesome-webxr | `msub2/awesome-webxr` | Enables immersive virtual reality and augmented reality cont |
| 115 | awesome-computational-geometry | `atkirtland/awesome-computational-geometry` | Computational approaches for problems in geometry |
| 116 | awesome-openstreetmap | `osmlab/awesome-openstreetmap` | An open data mapping project utilized by many apps and devic |
| 117 | awesome-computational-biology | `inoue0426/awesome-computational-biology` | Computational approaches applied to problems in biology |
| 118 | awesome-read-the-docs | `readthedocs-examples/awesome-read-the-docs` | Example documentation projects to inspire and help bootstrap |
| 119 | awesome-quarto | `mcanouil/awesome-quarto` | Scientific and technical open-source publishing system built |
| 120 | awesome-biological-image-analysis | `hallvaaw/awesome-biological-image-analysis` | Interpreting biological phenomena using images |
| 121 | awesome-chatgpt | `sindresorhus/awesome-chatgpt` | Artificial intelligence chatbot developed by OpenAI |
| 122 | awesome-whisper | `sindresorhus/awesome-whisper` | Open-source AI-powered speech recognition system developed b |
| 123 | awesome-stock-trading | `shi-rudo/awesome-stock-trading` | Purchase and sale of equities of publicly traded companies t |
| 124 | awesome-steam-deck | `airscripts/awesome-steam-deck` | A handheld gaming computer developed by Valve |
| 125 | awesome-astrophotography | `lunohodov/awesome-astrophotography` | Photography of astronomical objects, celestial events, or ar |
| 126 | awesome-hpc | `dstdev/awesome-hpc` | High Performance Computing |
| 127 | awesome-geocaching | `FoxFil/awesome-geocaching` | Outdoor treasure-hunting activity that uses GPS-enabled devi |
| 128 | awesome-regex | `slevithan/awesome-regex` | Specialized language for matching patterns in text |
| 129 | awesome-event-driven-architecture | `lutzh/awesome-event-driven-architecture` | A software architecture approach where services collaborate  |
| 130 | awesome-permacomputing | `idematos/awesome-permacomputing` | Resilient and regenerative computing practices inspired by p |
| 131 | awesome-standards | `donBarbos/awesome-standards` | Standards and proposals that define and enhance software, la |
| 132 | awesome-claude-code | `hesreallyhim/awesome-claude-code` | Terminal-based AI coding assistant by Anthropic |
| 133 | awesome-gemini-cli | `Piebald-AI/awesome-gemini-cli` | Terminal-based AI coding assistant by Google |
| 134 | awesome-sap-commerce | `eminyagiz42/awesome-sap-commerce` | An e-commerce platform built with Java, Spring MVC, and Angu |
| 135 | awesome-tech-ethics | `sampart/awesome-tech-ethics` | Mitigating and avoiding the potential negative effects of te |

**Cloned**: 690 | **Unavailable/Skipped**: 5


---

## 🖥️ Awesome WSL & Linux Bash Commands — 99 repos

**Source catalogs**:
- [SpiralCloudOmega/awesome-wsl](https://github.com/SpiralCloudOmega/awesome-wsl) → `DevTeam6/awesome-wsl/`
- [SpiralCloudOmega/Linux-Bash-Commands](https://github.com/SpiralCloudOmega/Linux-Bash-Commands) → `DevTeam6/Linux-Bash-Commands/`
**Disk**: 2.9 GB | **Cloned**: `--depth 1`
**Unavailable**: 2 repos (`0xbadfca11/miniwsl`, `Awesome-Windows/Awesome`)

### `~/projects/awesome-wsl/` — WSL Tools & Distros (71 repos)

Comprehensive collection of WSL distributions (Alpine, Arch, Fedora, Manjaro, etc.), WSL utilities (wslu, wslpath, wslgit), terminal tools, SSH agents, GUI integration, and the Microsoft WSL kernel source.

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | WSL | `Microsoft/WSL` | Official WSL issue tracker |
| 2 | WSL2-Linux-Kernel | `microsoft/WSL2-Linux-Kernel` | WSL2 Linux kernel source |
| 3 | WSL-DistroLauncher | `Microsoft/WSL-DistroLauncher` | Distro launcher template |
| 4 | terminal | `microsoft/terminal` | Windows Terminal |
| 5 | cuda-samples | `NVIDIA/cuda-samples` | NVIDIA CUDA samples |
| 6 | ArchWSL | `yuk7/ArchWSL` | Arch Linux on WSL |
| 7 | AlpineWSL | `yuk7/AlpineWSL` | Alpine Linux on WSL |
| 8 | CentWSL | `yuk7/CentWSL` | CentOS on WSL |
| 9 | FedoraWSL | `yosukes-dev/FedoraWSL` | Fedora on WSL |
| 10 | ManjaroWSL2 | `sileshn/ManjaroWSL2` | Manjaro on WSL2 |
| 11 | AmazonWSL | `yosukes-dev/AmazonWSL` | Amazon Linux on WSL |
| 12 | DevuanWSL | `VPraharsha03/DevuanWSL` | Devuan on WSL |
| 13 | GentooWSL | `imaandrew/GentooWSL` | Gentoo on WSL |
| 14 | RHWSL | `yosukes-dev/RHWSL` | Red Hat on WSL |
| 15 | WSLackware | `Mohsens22/WSLackware` | Slackware on WSL |
| 16 | WSLFedoraRemix | `WhitewaterFoundry/WSLFedoraRemix` | Fedora Remix for WSL |
| 17 | Pengwin | `WhitewaterFoundry/Pengwin` | Pengwin WSL distro |
| 18 | Pengwin-Enterprise | `WhitewaterFoundry/Pengwin-Enterprise` | Pengwin Enterprise |
| 19 | wsldl | `yuk7/wsldl` | WSL distro launcher |
| 20 | LxRunOffline | `DDoSolitary/LxRunOffline` | WSL distro management |
| 21 | wsl2-distro-manager | `bostrot/wsl2-distro-manager` | WSL2 distro manager GUI |
| 22 | easyWSL | `redcode-labs/easyWSL` | Easy WSL distro installer |
| 23 | WSLInstall | `Biswa96/WSLInstall` | WSL installer tools |
| 24 | WSL_Reverse | `Biswa96/WSL_Reverse` | WSL reverse engineering |
| 25 | WSL-Distribution-Switcher | `RoliSoft/WSL-Distribution-Switcher` | Switch WSL distros |
| 26 | wslu | `wslutilities/wslu` | WSL utilities collection |
| 27 | wslpy | `wslutilities/wslpy` | Python WSL utilities |
| 28 | wslgit | `andy-5/wslgit` | Use git from WSL in Windows |
| 29 | wslpath | `laurent22/wslpath` | Path conversion tool |
| 30 | wslexec | `int128/wslexec` | Execute Windows apps from WSL |
| 31 | wslbridge | `rprichard/wslbridge` | WSL bridge tool |
| 32 | wsl-vpnkit | `sakai135/wsl-vpnkit` | VPN networking for WSL2 |
| 33 | wsl-proxy | `watzon/wsl-proxy` | WSL proxy tool |
| 34 | wsl-open | `4U6U57/wsl-open` | Open files from WSL in Windows |
| 35 | wsl-open-jbwfu | `jbwfu/wsl-open` | WSL open fork |
| 36 | wsl-gui-tool | `emeric-martineau/wsl-gui-tool` | WSL GUI management |
| 37 | wsl-gui-bins | `Konfekt/wsl-gui-bins` | WSL GUI binary helpers |
| 38 | wsl-ui | `octasoft-ltd/wsl-ui` | WSL user interface |
| 39 | wsl-terminal | `goreliu/wsl-terminal` | Terminal for WSL |
| 40 | wsltty | `mintty/wsltty` | Mintty-based WSL terminal |
| 41 | wsl-dotfiles | `Xyene/wsl-dotfiles` | WSL dotfiles |
| 42 | WSL-config | `hsab/WSL-config` | WSL config tool |
| 43 | WSL-Hello-sudo | `nullpo-head/WSL-Hello-sudo` | Windows Hello sudo auth |
| 44 | WSL-Programs | `ethanhs/WSL-Programs` | WSL program list |
| 45 | OpenInWSL-Source | `Opticos/OpenInWSL-Source` | Open In WSL utility |
| 46 | FluentTerminal | `felixse/FluentTerminal` | Fluent design terminal |
| 47 | extraterm | `sedwards2009/extraterm` | Feature-rich terminal |
| 48 | WinCryptSSHAgent | `buptczq/WinCryptSSHAgent` | SSH agent for Windows |
| 49 | wsl2-ssh-pageant | `BlackReloaded/wsl2-ssh-pageant` | SSH pageant for WSL2 |
| 50 | weasel-pageant | `vuori/weasel-pageant` | Pageant for WSL |
| 51 | pinentry-wsl-ps1 | `diablodale/pinentry-wsl-ps1` | GPG pinentry for WSL |
| 52 | xclip-xsel-WSL | `Konfekt/xclip-xsel-WSL` | Clipboard integration |
| 53 | xdg-open-wsl | `cpbotha/xdg-open-wsl` | xdg-open for WSL |
| 54 | vim-wsl-copy-paste | `Konfekt/vim-wsl-copy-paste` | Vim clipboard in WSL |
| 55 | gowinbridge | `Sibikrish3000/gowinbridge` | Go Windows bridge |
| 56 | is-wsl | `sindresorhus/is-wsl` | Detect if running in WSL |
| 57 | setup-wsl | `Vampire/setup-wsl` | GitHub Action for WSL |
| 58 | Ansible-WSL | `Wintus/Ansible-WSL` | Ansible playbooks for WSL |
| 59 | BootShellCredentialProvider | `NathanCastle/BootShellCredentialProvider` | Boot to bash from Windows |
| 60 | community.wsl.sdk | `Gitii/community.wsl.sdk` | WSL SDK for .NET |
| 61 | Windows-Subsystem-For-Linux-Setup | `michaeltreat/Windows-Subsystem-For-Linux-Setup` | WSL setup guide |
| 62 | ubuntu-win-bootstrap | `seapagan/ubuntu-win-bootstrap` | Ubuntu WSL bootstrap |
| 63 | cmd-colors-solarized | `neilpa/cmd-colors-solarized` | Solarized for cmd |
| 64 | acme-wsl | `elrzn/acme-wsl` | Acme editor for WSL |
| 65 | Awesome-UNIX | `sirredbeard/Awesome-UNIX` | Awesome UNIX list |
| 66 | awesome-bash | `awesome-lists/awesome-bash` | Awesome Bash list |
| 67 | awesome-command-line-apps | `herrbischoff/awesome-command-line-apps` | CLI apps list |
| 68 | awesome-wsl | `sirredbeard/awesome-wsl` | This awesome list |
| 69 | curated-programming-resources | `Michael0x2a/curated-programming-resources` | Programming resources |
| 70 | portable-node-guide | `ehmicky/portable-node-guide` | Cross-platform Node.js guide |
| 71 | the-art-of-command-line | `jlevy/the-art-of-command-line` | Master the command line |

### `~/projects/awesome-bash/` — Bash Scripting & CLI Tools (28 repos)

Shell scripting tutorials, cheat sheets, AI-enhanced CLI tools (aichat, aider, plandex, shell_gpt), and curated CLI resource collections.

| # | Repo | Source | Description |
|---|------|--------|-------------|
| 1 | pure-bash-bible | `dylanaraps/pure-bash-bible` | Pure bash snippets for common tasks |
| 2 | bash-it | `bash-it/bash-it` | Community Bash framework |
| 3 | Bash-Snippets | `alexanderepstein/Bash-Snippets` | Useful bash script collection |
| 4 | DevOps-Bash-tools | `HariSekhon/DevOps-Bash-tools` | 1000+ DevOps bash scripts |
| 5 | bash-utils | `pirate/bash-utils` | Hand-crafted bash utilities |
| 6 | introduction-to-bash-scripting | `bobbyiliev/introduction-to-bash-scripting` | Free bash scripting eBook |
| 7 | shell-scripting-tutorial | `techarkit/shell-scripting-tutorial` | Shell scripting guide |
| 8 | 100-shell-script-examples | `epety/100-shell-script-examples` | 100 shell script examples |
| 9 | simple-bash-scripts | `ruanyf/simple-bash-scripts` | Simple bash scripts |
| 10 | shell-examples | `qjcg/shell-examples` | Shell scripting examples |
| 11 | cheat.sh | `chubin/cheat.sh` | Unified cheat sheet service |
| 12 | cheat.sheets | `chubin/cheat.sheets` | Community cheat sheets |
| 13 | awesome-cheatsheets | `LeCoupa/awesome-cheatsheets` | Multi-language cheat sheets |
| 14 | Bash-Cheat-Sheet | `RehanSaeed/Bash-Cheat-Sheet` | Bash cheat sheet |
| 15 | go-lang-cheat-sheet | `a8m/go-lang-cheat-sheet` | Go cheat sheet |
| 16 | learnxinyminutes-docs | `adambard/learnxinyminutes-docs` | Learn X in Y Minutes |
| 17 | aichat | `sigoden/aichat` | All-in-one LLM CLI tool |
| 18 | shell_gpt | `TheR1D/shell_gpt` | GPT-powered CLI tool |
| 19 | aider | `paul-gauthier/aider` | AI pair programming |
| 20 | plandex | `plandex-ai/plandex` | AI-driven terminal development |
| 21 | awesome-terminals | `cdleon/awesome-terminals` | Terminal emulators list |
| 22 | awesome-console-services | `chubin/awesome-console-services` | Console web services |
| 23 | awesome-fetch | `beucismis/awesome-fetch` | System info fetch tools |
| 24 | awesome-ttygames | `ligurio/awesome-ttygames` | Unix ASCII terminal games |
| 25 | awesome-tuis | `rothgar/awesome-tuis` | Terminal user interfaces |
| 26 | cli-apps | `toolleeo/cli-apps` | CLI/TUI application list |
| 27 | open-source-badges | `ellerbrock/open-source-badges` | Open source badges |
| 28 | perl1line.txt | `pkrumins/perl1line.txt` | Perl one-liners |

### `DevTeam6/antigravity1-awesome-skills/` — Copilot Skills Catalog (945 skills)

**GitHub:** [`SpiralCloudOmega/antigravity1-awesome-skills`](https://github.com/SpiralCloudOmega/antigravity1-awesome-skills)
**Path (Windows):** `C:\Users\autom\VS Code GitHub Repos\DevTeam6\antigravity1-awesome-skills`
**Path (WSL):** `/mnt/c/Users/autom/VS Code GitHub Repos/DevTeam6/antigravity1-awesome-skills`

Curated catalog of 945 GitHub Copilot custom skills with a web-app interface, automated categorization, validation scripts, and a full CI/CD pipeline.

#### File Tree

```
antigravity1-awesome-skills/
├── .github/                  # Workflows, issue templates, CI config
├── assets/                   # Project assets (star history)
├── bin/                      # Install scripts (install.js)
├── data/                     # Structured data
│   ├── aliases.json
│   ├── bundles.json
│   ├── catalog.json
│   ├── skills_index.json
│   ├── workflows.json
│   └── package.json
├── docs/                     # 24 documentation files
│   ├── AUDIT.md
│   ├── BUNDLES.md
│   ├── COMMUNITY_GUIDELINES.md
│   ├── FAQ.md
│   ├── GETTING_STARTED.md
│   ├── KIRO_INTEGRATION.md
│   ├── QUALITY_BAR.md
│   ├── SECURITY_GUARDRAILS.md
│   ├── SKILL_ANATOMY.md
│   ├── SKILL_TEMPLATE.md
│   ├── SOURCES.md
│   ├── USAGE.md
│   └── ... (12 more)
├── lib/                      # Utility library (skill-utils.js)
├── scripts/                  # Build & validation scripts
│   ├── auto_categorize_skills.py
│   ├── build-catalog.js
│   ├── fix_dangling_links.py
│   ├── generate_index.py
│   ├── generate_skills_report.py
│   ├── normalize-frontmatter.js
│   ├── validate-skills.js
│   ├── validate_skills.py
│   └── tests/
├── skills/                   # 945 skill definitions
│   ├── 00-andruia-consultant/
│   ├── 10-andruia-skill-smith/
│   ├── agent-orchestration-multi-agent-optimize/
│   ├── ... (942 more skills)
│   └── README.md
├── web-app/                  # React/Vite web interface
│   ├── src/
│   ├── public/
│   ├── package.json
│   ├── vite.config.js
│   └── index.html
├── CATALOG.md                # Full skills catalog (232 KB)
├── CHANGELOG.md              # Release history
├── CONTRIBUTING.md           # Contribution guidelines
├── LICENSE                   # MIT License
├── README.md                 # Main documentation
├── SECURITY.md               # Security policy
├── SKILLS_UPDATE_GUIDE.md    # Skills update guide
├── START_APP.bat             # Windows app launcher
├── package.json              # Root Node.js config
├── package-lock.json
└── skills_index.json         # Master skills index (399 KB)
```

---

### Downloaded Awesome Lists & Reference Repos (56 workspace folders)

These repos were downloaded and added to the DevTeam6 workspace as proper folders with real files.

| # | Folder | Description |
|---|--------|-------------|
| 1 | `500-AI-ML-DL-CV-NLP-Projects` | 500 AI/ML/DL/CV/NLP projects with code |
| 2 | `A-to-Z-Resources-for-Students` | A-to-Z resources for students |
| 3 | `Awesome-Design-Tools` | Curated list of design tools |
| 4 | `Awesome-Hacking1` | Awesome hacking resources |
| 5 | `awesome-actions` | GitHub Actions curated list |
| 6 | `awesome-ai-agents` | AI agents and autonomous systems |
| 7 | `awesome-android-ui` | Android UI components and libraries |
| 8 | `awesome-claude-code` | Claude Code tips and resources |
| 9 | `awesome-compose` | Docker Compose examples |
| 10 | `awesome-courses` | University CS courses list |
| 11 | `awesome-cpp` | Awesome C++ frameworks and libraries |
| 12 | `awesome-cursorrules` | Cursor AI rules (178 rules + 18 .mdc) → converted to 196 custom agents |
| 13 | `awesome-deep-learning` | Deep learning resources |
| 14 | `awesome-deepseek-integration` | DeepSeek AI integration tools |
| 15 | `awesome-design-patterns` | Software design patterns |
| 16 | `awesome-docker` | Docker resources and tools |
| 17 | `awesome-dotnet` | .NET framework resources |
| 18 | `awesome-dotnet-core` | .NET Core resources |
| 19 | `awesome-electron` | Electron.js resources |
| 20 | `awesome-falsehood` | Falsehoods programmers believe |
| 21 | `awesome-flipperzero` | Flipper Zero resources |
| 22 | `awesome-flutter` | Flutter development resources |
| 23 | `awesome-generative-ai-guide` | Generative AI comprehensive guide |
| 24 | `awesome-github-profile-readme` | GitHub profile README examples |
| 25 | `awesome-go` | Go libraries and frameworks |
| 26 | `awesome-ios` | iOS development resources |
| 27 | `awesome-java` | Java frameworks and libraries |
| 28 | `awesome-jupyter` | Jupyter notebook resources |
| 29 | `awesome-linux-customization` | Linux desktop customization |
| 30 | `awesome-machine-learning` | Machine learning resources |
| 31 | `awesome-nodejs` | Node.js frameworks and tools |
| 32 | `awesome-openclaw-skills` | OpenClaw skills collection |
| 33 | `awesome-pentest` | Penetration testing resources |
| 34 | `awesome-php` | PHP frameworks and libraries |
| 35 | `awesome-pydantic` | Pydantic validation resources |
| 36 | `awesome-python` | Python frameworks, libraries, resources (499 GitHub repos) |
| 37 | `awesome-quant` | Quantitative finance libraries |
| 38 | `awesome-react` | React ecosystem resources |
| 39 | `awesome-react-components` | React component libraries |
| 40 | `awesome-rust` | Rust language resources |
| 41 | `awesome-scalability` | Scalability patterns and resources |
| 42 | `awesome-selfhosted` | Self-hosted services |
| 43 | `awesome-shell` | Shell scripting tools and frameworks |
| 44 | `awesome-sysadmin` | System administration tools |
| 45 | `awesome-system-design-resources` | System design learning resources |
| 46 | `awesome-vscode` | VS Code extensions and resources |
| 47 | `css-protips` | CSS professional tips |
| 48 | `dashy` | Self-hosted dashboard application |
| 49 | `data-engineer-handbook` | Data engineering handbook |
| 50 | `epub-reader-vscode` | EPUB reader for VS Code |
| 51 | `frontend-dev-bookmarks` | Frontend development bookmarks |
| 52 | `github-cheat-sheet` | GitHub tips and tricks |
| 53 | `the-book-of-secret-knowledge` | Collection of IT knowledge |
| 54 | `vscode` | VS Code source code |
| 55 | `vscode-copilot-chat` | VS Code Copilot Chat extension |
| 56 | `wifi-densepose` | WiFi-based body pose estimation |

### Custom Copilot Agents (196 agents from awesome-cursorrules)

All 196 cursor rules were converted into GitHub Copilot custom agent files stored in `.github/agents/`.

**Location:** `DevTeam6/.github/agents/`

**Rule Categories:**
- **Frontend**: React, Next.js, Vue, Svelte, Angular, Astro, Qwik, Solid.js (40+ agents)
- **Backend**: Python/FastAPI/Django/Flask, Node.js, Go, Java/Spring, PHP/Laravel, Rails, Elixir (30+ agents)
- **Mobile**: Flutter, React Native, Swift/UIKit, Android Jetpack Compose (8 agents)
- **Testing**: Cypress, Playwright, Jest, Vitest (12 agents)
- **DevOps**: Docker, Kubernetes, Terraform, CI/CD (10+ agents)
- **Languages**: TypeScript, Python, Rust, Go, C++, Scala, Kotlin, R, Solidity (20+ agents)
- **Frameworks**: HTMX, Tailwind, Shadcn UI, Material UI, Chakra UI (15+ agents)
- **General**: Code quality, design patterns, documentation, security (20+ agents)
- **Modern (.mdc)**: Updated rules for Python, React, TypeScript, Vue, Svelte, Rust, etc. (18 agents)

---

### ACE-Step Ecosystem (9 repos — complete org clone)

| Repo | Stars | Description |
|------|-------|-------------|
| `ACE-Step-1.5/` | ★6567 | Main music generation model (installed in `~/projects/ACE-Step-1.5/`) |
| `ACE-Step/` | ★4147 | Original ACE-Step model |
| `ace-step-skills/` | ★37 | Skills & presets for ACE-Step |
| `ACE-Step-ComfyUI/` | ★34 | ComfyUI node integration |
| `ACE-Step-DAW/` | ★0 | DAW integration (new) |
| `acestep.cpp/` | ★5 | Portable C++17 implementation |
| `awesome-ace-step/` | ★17 | Curated community resources |
| `ace-step.github.io/` | ★0 | Project website |
| `ace-step-v1.5.github.io/` | ★12 | v1.5 website |

### Kimi / Moonshot AI Repos

| Repo | Description |
|------|-------------|
| `Kimi-Audio/` | Kimi-Audio-7B: audio understanding, generation & conversation model |
| `Kimi-Linear/` | Kimi Linear 48B-A3B: efficient hybrid linear attention architecture |

### AI Research & Tools

| Repo | Description |
|------|-------------|
| `clawdbot_report/` | ClawdBot research report |
| `rlm/` | Recursive Language Models — task-agnostic infinite context inference |
| `GitNexus/` | Code intelligence platform — knowledge graph for AI agents (MCP) |
| `RAG-Anything/` | Multimodal RAG system (text, images, tables, equations) |
| `notebooklm-cli/` | CLI for Google NotebookLM — deep research, podcasts, quizzes |

### Secrets Management

API keys stored in `.secrets/` folder (git-ignored):
- `.secrets/api-keys.env` — LLM API keys (OpenAI, Anthropic, Google, Moonshot)
- `.secrets/services.env` — Local service URLs and ports

---

## 🤖 AI Tools & Models

### Installed Tools

| Tool | Version | Install Method | Location |
|------|---------|---------------|----------|
| **Ollama** | 0.15.1 | snap | `/snap/ollama/105/bin/ollama` |
| **Claude Code CLI** | 2.1.62 | curl install.sh | `~/.claude/local/claude` |
| **OpenCode** | 1.2.10 | curl install.sh | `~/.opencode/bin/opencode` |

### Ollama Models

| Model | Type | Size | Purpose |
|-------|------|------|---------|
| `kimi-k2.5:cloud` | Cloud (Moonshot AI) | — | Cloud-routed inference via Ollama |
| `glm-5:cloud` | Cloud (Zhipu AI) | — | Cloud-routed inference via Ollama |
| `deepseek-v3.2:cloud` | Cloud (DeepSeek) | — | Cloud-routed inference via Ollama |
| `minimax-m2.5:cloud` | Cloud (MiniMax) | — | Cloud-routed inference via Ollama |
| `qwen3-coder:30b` | Local (Alibaba) | ~19 GB | Local code generation (RTX 3090 24GB) |
| `qwen3:8b` | Local (Alibaba) | 5.2 GB | CopilotKit default agent model |

> **Note**: Cloud models require Ollama account authentication. Run `ollama run <model>` and follow the sign-in URL to activate.

### Ollama Integration Configurations

All 4 integrations configured with all 4 cloud models via `ollama launch <integration> --model <model> --config`:

| Integration | Description | Config Location |
|-------------|-------------|-----------------|
| `claude` | Claude Code CLI | `~/.claude/.claude.json` |
| `opencode` | OpenCode AI | `~/snap/ollama/105/.config/opencode/opencode.json` |
| `codex` | OpenAI Codex CLI | codex config |
| `droid` | Droid AI Assistant | `~/snap/ollama/105/.factory/settings.json` |

**Configured model × integration matrix** (16 combinations):

| Model | claude | opencode | codex | droid |
|-------|--------|----------|-------|-------|
| `kimi-k2.5:cloud` | ✅ | ✅ | ✅ | ✅ |
| `glm-5:cloud` | ✅ | ✅ | ✅ | ✅ |
| `deepseek-v3.2:cloud` | ✅ | ✅ | ✅ | ✅ |
| `minimax-m2.5:cloud` | ✅ | ✅ | ✅ | ✅ |

### API Endpoints

| Endpoint | URL | Description |
|----------|-----|-------------|
| Ollama API | `http://localhost:11434` | Local model serving |
| Ollama Chat | `http://localhost:11434/api/chat` | Chat completions |
| OpenAI-compat | `http://localhost:11434/v1` | OpenAI-compatible API |

### Environment Variables (AI)

| Variable | Value | Purpose |
|----------|-------|---------|
| `ANTHROPIC_AUTH_TOKEN` | `ollama` | Ollama auth token |
| `ANTHROPIC_API_KEY` | `44af...500ef0c.p8MN...` | API key for Anthropic routing |
| `ANTHROPIC_BASE_URL` | `http://localhost:11434` | Route Anthropic calls to Ollama |

### Quick Reference

```bash
# Run a cloud model interactively
ollama run kimi-k2.5:cloud

# Launch Claude Code with a specific model
ollama launch claude --model deepseek-v3.2:cloud

# Launch OpenCode with a model
ollama launch opencode --model glm-5:cloud

# API call to local model
curl http://localhost:11434/api/chat \
  -d '{"model":"qwen3-coder:30b","messages":[{"role":"user","content":"Hello!"}]}'

# OpenCode web UI (from WSL)
opencode web --hostname 0.0.0.0

# OpenCode serve (for Desktop app connection)
opencode serve --hostname 0.0.0.0 --port 4096
```

### Ubuntu Server (WSL2)

**Metapackage**: `ubuntu-server` 1.539.2 installed on Ubuntu 24.04.4 LTS (WSL2)

| Category | Packages |
|----------|----------|
| **Core** | `ubuntu-server`, `cloud-init`, `landscape-common`, `unattended-upgrades` |
| **Remote Access** | `openssh-server`, `byobu`, `tmux`, `screen` |
| **Networking** | `ufw`, `fail2ban`, `net-tools`, `ethtool`, `vlan`, `ifenslave`, `nfs-common`, `cifs-utils`, `chrony` |
| **Storage/RAID** | `lvm2`, `mdadm`, `smartmontools`, `multipath-tools`, `open-iscsi` |
| **Monitoring** | `htop`, `iotop`, `sysstat`, `lm-sensors` |
| **Maintenance** | `logrotate`, `rsync` |

### LibreOffice Build Environment (WSL2)

**Source**: `~/projects/libreoffice/core/` (1.9 GB, 148K+ files)
**Guide**: [DocumentFoundation WSL Build Guide](https://dev.blog.documentfoundation.org/2024/10/17/wsl-for-building-libreoffice/)

| Tool | Version |
|------|---------|
| GCC/G++ | 11.5.0 |
| GNU Make | 4.3 |
| OpenJDK | 21.0.10 |
| ccache | 4.9.1 |
| autoconf/automake/libtool | installed |
| ant | installed |

**Dev Libraries**: GTK3, GStreamer, X11, CUPS, D-Bus, XRandr, Xrender, Xslt, SSL, PNG, JPEG, Expat, cURL + all LibreOffice `build-dep` packages.

```bash
# Build LibreOffice for Linux on WSL2
cd ~/projects/libreoffice/core
./autogen.sh
make
```

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
| `AWESOME_LISTS_HOME` | `~/projects/awesome-lists` | 690 awesome list repos |
| `AWESOME_WSL_HOME` | `~/projects/awesome-wsl` | 71 WSL repos |
| `AWESOME_BASH_HOME` | `~/projects/awesome-bash` | 28 Bash/CLI repos |
| `CYBORGDB_API_URL` | `http://localhost:8100` | CyborgDB vector DB |
| `OPENCLAW_GATEWAY` | `http://localhost:18789` | OpenClaw AI gateway |
| `ANTHROPIC_AUTH_TOKEN` | `ollama` | Ollama auth routing |
| `ANTHROPIC_API_KEY` | `44af...500ef0c...` | Anthropic API key |
| `ANTHROPIC_BASE_URL` | `http://localhost:11434` | Route to Ollama |
| `REPO_COUNT_TOTAL` | `4200` | Total cloned repos (updated 2025-07) |

### Shell Functions

| Function | Description |
|----------|-------------|
| `devteam6_repos` | List all repo collections with counts |
| `devteam6_services` | Show active service endpoints |
| `devteam6_search <query>` | Search across all repos by name |
| `devteam6_stats` | Environment stats (repos, disk, GPU) |

