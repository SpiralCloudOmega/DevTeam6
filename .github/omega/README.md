# 🔱 Omega Agent System

> The recursive multi-agent orchestration layer for REVITHION STUDIO and DevTeam6

## Overview

Omega is the coordination brain that turns 250+ individual agents, 46 skills,
and 12+ CLI terminals into a unified production system. Where a single agent
solves one problem, Omega **decomposes complex work into a DAG of sub-tasks**,
dispatches them to specialist agents in parallel, collects and merges results,
runs critique councils for quality, and commits verified output — all
autonomously.

Key differentiators from running agents individually:

| Capability | Individual Agent | Omega |
|---|---|---|
| Task scope | Single file / feature | Entire project or cross-repo |
| Parallelism | Sequential | Fan-out across 100+ agents |
| Quality gate | None (trust the output) | LLM Council peer review |
| Context cost | Full codebase per agent | Partitioned — paths only (98% savings) |
| Dependency handling | Manual | DAG + topological sort |
| External tools | In-process only | HTTP bridge to any CLI terminal |
| Recovery | Fail and stop | Health-aware rescheduling + retry |

## Architecture

```text
┌─────────────────────────────────────────────────────────┐
│                    OMEGA ORCHESTRATOR                    │
│                                                         │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌────────┐  │
│  │ ChrysaLisp│  │   RLM    │  │ Fulcrum  │  │ Chorus │  │
│  │  Macros   │  │ Context  │  │  Kanban  │  │ Voting │  │
│  └─────┬─────┘  └─────┬────┘  └─────┬────┘  └────┬───┘  │
│        │              │             │             │      │
│        ▼              ▼             ▼             ▼      │
│  ┌──────────────────────────────────────────────────┐   │
│  │              DISPATCH ENGINE                      │   │
│  │   DAG → Topological Sort → Fan-Out → Merge       │   │
│  └──────────────┬───────────────────┬───────────────┘   │
│                 │                   │                    │
│        ┌────────┴────────┐  ┌──────┴──────┐             │
│        │ Internal Agents │  │ HTTP Bridge │             │
│        │  (250+ agents)  │  │  (:18800)   │             │
│        └─────────────────┘  └──────┬──────┘             │
│                                    │                    │
└────────────────────────────────────┼────────────────────┘
                                     │
                    ┌────────────────┼────────────────┐
                    │                │                │
               ┌────▼───┐    ┌──────▼────┐    ┌──────▼────┐
               │ CMake  │    │  Ollama   │    │ gh CLI   │
               │ Build  │    │ :11434    │    │          │
               └────────┘    └───────────┘    └──────────┘
                    ... 12+ external terminals
```

### Components

| Component | Role | Location |
|---|---|---|
| **ChrysaLisp Macros** | Define-once S-expression templates that generate agent configs, bridge routes, and CMake targets | `macros/` |
| **RLM Context** | Recursive Language Model pattern — context passed as env vars, not inline data | `omega-env.sh` |
| **Fulcrum Kanban** | SQL-backed task board tracking every sub-task through backlog → done | `kanban/` |
| **Chorus Voting** | LLM Council consensus system for critique and quality gates | `skills/omega-critique/` |
| **Dispatch Engine** | DAG scheduler with topological sort, fan-out, merge, and retry | `skills/omega-orchestration/` |
| **HTTP Bridge** | Routes commands to external CLI terminals via REST API | `bridge/` |

## Quick Start

```bash
# 1. Source the Omega environment (sets OMEGA_* env vars)
source .github/omega/omega-env.sh

# 2. Generate all configs from SEXP macros
python .github/omega/macros/generate-omega.py --all

# 3. Start the HTTP bridge for external terminal dispatch
python .github/omega/bridge/http-bridge.py &

# 4. Verify the system is healthy
curl http://localhost:18800/health

# 5. Run an orchestrated task
python .github/omega/dispatch.py --task "Refactor audio engine" --strategy domain
```

## Directory Structure

```text
.github/omega/
├── README.md                  # This file — system documentation
├── omega-env.sh               # Environment variables (RLM context)
├── omega.config.sexp          # Master configuration in S-expressions
│
├── agents/                    # Omega-specific agent definitions
│   ├── orchestrator.agent.md  # Top-level orchestrator agent
│   ├── critic-a.agent.md      # Council reviewer instance A
│   ├── critic-b.agent.md      # Council reviewer instance B
│   └── critic-c.agent.md      # Council reviewer instance C
│
├── bridge/                    # External terminal HTTP bridge
│   ├── http-bridge.py         # REST server (:18800)
│   ├── terminals.json         # Terminal registry
│   └── health.py              # Heartbeat monitor
│
├── kanban/                    # Fulcrum Kanban integration
│   ├── board.py               # Board state management
│   └── schema.sql             # SQL schema for task tracking
│
├── macros/                    # ChrysaLisp-style SEXP macros
│   ├── generate-omega.py      # Macro expander + code generator
│   ├── templates/             # SEXP template files
│   └── output/                # Generated configs (gitignored)
│
└── skills/                    # Omega-specific skills
    ├── omega-orchestration/   # Decomposition, DAG, dispatch, merge
    │   └── SKILL.md
    └── omega-critique/        # LLM Council review, scoring, consensus
        └── SKILL.md
```

## Agents

### Internal Agents (Copilot-hosted)

| Agent | Role | Model | Critic |
|---|---|---|---|
| `orchestrator` | Top-level task decomposition and DAG management | claude-opus-4.6 | None (root) |
| `audio-dsp-expert` | Real-time audio processing, filters, FFT | claude-sonnet-4.5 | Council |
| `amp-modeling-expert` | Neural amp modeling, RTNeural, NAM, WaveNet | claude-sonnet-4.5 | Council |
| `cmake-cpp20-expert` | Build systems, C++20/23, cross-platform | claude-sonnet-4.5 | Council |
| `juce-framework-expert` | JUCE 8 plugin/app development | claude-sonnet-4.5 | Council |
| `faust-dsp-expert` | Faust functional DSP, faust2juce | claude-sonnet-4.5 | Council |
| `plugin-format-expert` | VST3, AU, LV2, CLAP, AAX validation | claude-sonnet-4.5 | Council |
| `spectral-analysis-expert` | FFT, STFT, mel spectrograms, source separation | claude-sonnet-4.5 | Council |
| `midi-expert` | MIDI protocol, MPE, MIDI 2.0, arpeggiators | claude-sonnet-4.5 | Council |
| `neural-inference-expert` | RTNeural, ONNX, TensorRT, audio-thread safety | claude-sonnet-4.5 | Council |
| `lisp-token-expert` | SEXP tokens, macro expansion, C++ codegen | claude-sonnet-4.5 | Council |
| `routing-grid-expert` | AXE-FX III 12×4 routing, block types, scenes | claude-sonnet-4.5 | Council |
| `mastering-engineer` | Loudness, EQ, multiband compression, limiting | claude-sonnet-4.5 | Council |
| `mix-engineer` | Gain staging, bus routing, automation, stems | claude-sonnet-4.5 | Council |
| `code-review` | Bug detection, security, logic errors only | claude-sonnet-4.5 | N/A (is critic) |
| `explore` | Fast codebase search and synthesis | claude-haiku-4.5 | None |
| `task` | Build, test, lint — brief output on success | claude-haiku-4.5 | None |
| `general-purpose` | Complex multi-step implementation | claude-sonnet-4.5 | Council |

### External Terminals (HTTP Bridge)

| Terminal ID | Target | Port | Capabilities |
|---|---|---|---|
| `cmake-build` | WSL CMake/GCC/Clang | — | C++ compile, link, test |
| `python-repl` | Python 3.13 (conda) | — | Script execution, pip |
| `node-repl` | Node 22 (nvm) | — | JS/TS execution, npm |
| `ollama` | Ollama server | 11434 | LLM inference (qwen3, etc.) |
| `gh-cli` | GitHub CLI | — | PR, issue, repo operations |
| `docker` | Docker Desktop | — | Container build/run |
| `ace-step` | ACE-Step API | 8001 | AI music generation |
| `notebooklm` | NotebookLM CLI | — | Deep research, mind maps |
| `cyborgdb` | CyborgDB | 8100 | Encrypted vector search |
| `gitea` | Gitea server | 3000 | Self-hosted Git operations |
| `kali` | Kali Linux WSL | — | Security tools, pentesting |
| `wsl-bash` | Ubuntu 24.04 WSL | — | General shell commands |

## Configuration

### Master Config (omega.config.sexp)

The master config uses S-expression syntax (ChrysaLisp-inspired):

```lisp
(omega-system
  (version "1.0.0")
  (bridge
    (host "localhost")
    (port 18800)
    (heartbeat-interval 30))

  (agents
    (define-agent orchestrator
      (model "claude-opus-4.6")
      (skills ["omega-orchestration" "omega-critique"])
      (max-concurrent 10))
    (define-agent audio-dsp-expert
      (model "claude-sonnet-4.5")
      (skills ["audio-dsp/*" "juce-framework/*"])
      (critic :council)))

  (terminals
    (define-terminal cmake-build
      (command "cmake")
      (health-check "cmake --version")
      (timeout 600))))
```

### Generation Pipeline

```text
omega.config.sexp → generate-omega.py → {agent.md files, bridge config, CMake targets}
```

The macro expander reads SEXP templates, applies variable substitution, and
generates concrete configuration files. This is the **define-once** principle —
change the SEXP source, regenerate everything.

## Adding a New Agent

1. **Define in SEXP config:**
   ```lisp
   (define-agent my-new-agent
     (model "claude-sonnet-4.5")
     (skills ["relevant-skill/*"])
     (critic :council))
   ```

2. **Regenerate configs:**
   ```bash
   python .github/omega/macros/generate-omega.py --agents
   ```

3. **Add routing rule** in the dispatch engine:
   ```python
   ROUTING["*.new_ext"] = "my-new-agent"
   ```

4. **Verify:** Run a test task that exercises the new agent.

## Adding a New Terminal

1. **Register in terminals.json:**
   ```json
   {
     "id": "my-terminal",
     "command": "/usr/bin/my-tool",
     "health_check": "my-tool --version",
     "timeout": 300
   }
   ```

2. **Add SEXP definition:**
   ```lisp
   (define-terminal my-terminal
     (command "/usr/bin/my-tool")
     (health-check "my-tool --version")
     (timeout 300))
   ```

3. **Restart the bridge:**
   ```bash
   python .github/omega/bridge/http-bridge.py
   ```

4. **Verify health:**
   ```bash
   curl http://localhost:18800/health/my-terminal
   ```

## Health Monitoring

The bridge runs a heartbeat loop that checks every registered terminal:

```text
Every 30s:
  For each terminal in registry:
    Run health_check command
    If exit_code == 0 → mark healthy
    If exit_code != 0 → mark unhealthy, increment failure_count
    If failure_count >= 3 → mark dead, alert

Recovery:
  Dead terminals are retried every 5 minutes
  On recovery → reset failure_count, mark healthy
```

Health endpoint:

```bash
# All terminals
curl http://localhost:18800/health
# {"cmake-build": {"status": "healthy", "last_check": "2025-07-12T..."}, ...}

# Single terminal
curl http://localhost:18800/health/ollama
# {"status": "healthy", "uptime": 3600, "last_check": "2025-07-12T..."}
```

## Critic System

The Omega critique system implements an **LLM Council** for quality assurance:

1. **Three anonymous reviewers** independently evaluate the artifact
2. **No cross-talk** — each reviewer sees only the artifact + rubric
3. **Scoring** on correctness (1–5), security (1–5), performance (1–5), maintainability (1–5)
4. **Quorum voting** — findings need N-of-3 approval (N varies by category)
5. **Signal filtering** — style/formatting comments auto-rejected
6. **Remediation required** — every accepted finding must include a specific fix
7. **Escalation** — deadlocked councils escalate: senior agent → human

See `skills/omega-critique/SKILL.md` for full details, scoring tables, and SQL
tracking patterns.

## Design Principles

1. **RLM Pattern** — Context passed as environment variables and file paths,
   never inlined. Prompts are pointers to external state, not containers of it.

2. **LISP Macros (Define Once)** — All agent, terminal, and bridge configs
   derive from a single SEXP source. Change the macro, regenerate everything.

3. **Every Agent Gets** — Skills directory, tool access, isolated workspace,
   heartbeat registration, and a critic council assignment.

4. **Recursive Spawning** — Sub-agents can spawn their own sub-agents. The
   orchestrator manages the top-level DAG; each agent manages its local scope.

5. **98% Token Savings** — Context Mode + MCP2CLI means agents receive paths
   and constraints, not file contents. They read what they need on demand.

6. **No Scattered Files** — Everything lives under `.github/omega/`. One
   directory to clone, backup, or version. No hunting across the repo.

7. **Health-First Scheduling** — Never dispatch to a dead terminal. Check
   health before every wave, reassign to fallback on failure.

8. **Critique Everything** — No agent output reaches `main` without passing
   through the LLM Council. The council is the quality gate.

9. **DAG-Driven Execution** — All work is modeled as a directed acyclic graph.
   Topological sort determines execution order. Cycles are detected and rejected.

10. **Kanban Visibility** — Every task is tracked in Fulcrum's SQL-backed board.
    Real-time status: backlog → in_progress → review → done.

## Integration

### Existing Infrastructure

Omega builds on top of the existing DevTeam6 infrastructure:

| System | Count | Integration |
|---|---|---|
| **GitHub Agents** | 241 | Available as dispatch targets via agent_type routing |
| **Skills** | 46 subdirectories | Mounted into agent context per SEXP config |
| **Custom Agents** | 18 REVITHION specialists | Priority routing for domain-specific tasks |
| **Repositories** | 2,712+ | Searchable via explore agents and gh CLI terminal |
| **Ebook Library** | 59 books | Reference material for deep research tasks |
| **Ollama Models** | qwen3:8b, qwen3-coder:30b+ | Local inference via ollama terminal |

### REVITHION STUDIO

Omega is the orchestration layer for REVITHION STUDIO's build pipeline:

```text
Design Tokens (SEXP) → Macro Expansion → C++ Codegen → CMake Build → Plugin Output
       ↑                     ↑                ↑             ↑            ↑
   lisp-token-expert    generate-omega.py  cmake-cpp20    cmake-build   plugin-format
```

Each stage is managed by a specialist agent, coordinated by the Omega
orchestrator, and verified by the critique council before proceeding.

### Key Integrations

- **GitNexus** — Non-chunking knowledge graph for code intelligence (Tree-sitter → KuzuDB)
- **ACE-Step** — AI music generation dispatched via the ace-step terminal
- **CyborgDB** — Encrypted vector search for semantic code queries
- **NotebookLM** — Deep research with 600+ resources, mind maps, podcasts
- **OpenClaw** — 100-agent swarm for parallel exploration tasks

## Fleet

The Omega Fleet is the integration layer that wires **every agent** into the
orchestration system with critic assignments, heartbeat monitoring, skill
mappings, and workspace provisioning.

### Agent Census

| Metric | Count |
|---|---|
| **Custom agents** (`.github/agents/`) | 236 |
| **Omega agents** (`.github/omega/agents/`) | 7 |
| **Total fleet** | **243** |

### Category Breakdown

| Category | Agents | Heartbeat | Critic | Quorum | Review Focus |
|---|---|---|---|---|---|
| `web-frontend` | 82 | 30s | omega-critic | 1 | accessibility, performance, type safety |
| `web-backend` | 51 | 30s | omega-critic | 2 | API correctness, auth, SQL injection |
| `personal` | 22 | 30s | omega-critic | 1 | correctness, project-specific |
| `testing` | 13 | 60s | omega-critic | 1 | coverage, flakiness, determinism |
| `mobile` | 11 | 30s | omega-critic | 1 | platform guidelines, battery, accessibility |
| `audio` | 8 | 15s | omega-critic | 2 | DSP correctness, latency, SIMD safety |
| `infrastructure` | 7 | 15s | omega-critic | 2 | build reproducibility, memory safety |
| `devops` | 7 | 30s | omega-security | 2 | secrets, permissions, idempotency |
| `documentation` | 7 | 60s | omega-critic | 1 | accuracy, completeness, clarity |
| `quality` | 7 | 60s | omega-critic | 1 | consistency, SOLID principles |
| `omega` | 7 | 10s | omega-orchestrator | 2 | system integrity, recursion safety |
| `security` | 6 | 30s | omega-security | 3 | vulnerabilities, secrets, injection |
| `data-ml` | 5 | 30s | omega-critic | 2 | numerical stability, reproducibility |
| `game-dev` | 5 | 30s | omega-critic | 1 | frame budget, physics determinism |
| `agent-system` | 3 | 5s | omega-security | 2 | context integrity, memory persistence |
| `revithion` | 2 | 15s | omega-critic | 2 | macro correctness, C++ codegen safety |

### Fleet Files

| File | Format | Purpose |
|---|---|---|
| `fleet/agent-registry.sexp` | S-expression | Master registry — every agent with category, skills, critic, heartbeat |
| `fleet/fleet-manifest.json` | JSON | Machine-readable manifest for external tooling |
| `fleet/critic-assignments.sexp` | S-expression | Category → critic wiring with quorum and review focus |
| `fleet/heartbeat-config.sexp` | S-expression | Heartbeat intervals, timeouts, and failure thresholds per category |
| `fleet/skill-assignments.sexp` | S-expression | Skill → agent category mapping (46 base + 6 omega = 52 skills) |
| `fleet/workspace-layout.sh` | Bash | Provisions `.planning/` workspace directories with README.md per category |

### Adding a New Agent to the Fleet

1. **Create the agent file** in `.github/agents/my-agent.agent.md`
2. **Add an entry** to `fleet/agent-registry.sexp`:
   ```lisp
   (agent-entry :name "my-agent"
                :path ".github/agents/my-agent.agent.md"
                :category "web-backend"
                :skills ["python" "fastapi" "rest-api" "database"]
                :critic "omega-critic"
                :heartbeat-interval 30)
   ```
3. **Add to `fleet/fleet-manifest.json`** (or regenerate from registry)
4. **Verify** the agent inherits its category's critic and heartbeat config

### Critic System

Each category is assigned a critic (`omega-critic` or `omega-security`) with a
quorum threshold:

- **Quorum 1** — Any single reviewer can approve (low-risk: frontend, testing, docs)
- **Quorum 2** — Majority agreement required (standard: audio, backend, infrastructure)
- **Quorum 3** — Unanimous agreement required (security-critical: security, blockchain)

The `omega-orchestrator` self-governs the Omega agents (no external critic).

### Heartbeat Monitoring

Agents are monitored at four tiers:

| Tier | Interval | Categories | Rationale |
|---|---|---|---|
| **Meta-critical** | 5s | agent-system | Context/memory loss is catastrophic |
| **Real-time** | 15s | audio, infrastructure, revithion | Latency-sensitive workloads |
| **Standard** | 30s | web, mobile, backend, devops, data-ml, game-dev | Normal operation |
| **Low-priority** | 60s | testing, documentation, quality | Batch workloads |

Failure policy: 3 missed heartbeats → unhealthy, 5 → dead. Dead agents are
retried every 5 minutes and restored to the dispatch pool on recovery.

### Workspace Provisioning

Run the workspace provisioner to create planning directories:

```bash
bash .github/omega/fleet/workspace-layout.sh
```

This creates `.planning/<category>/` for all 20 workspaces (16 agent categories
+ orchestrator + critic + heartbeat), each with a README.md documenting its
purpose, agent count, heartbeat interval, and critic assignment.
