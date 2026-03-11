;; ═══════════════════════════════════════════════════════════════════════
;; REVITHION STUDIO — Omega Agent System Master Configuration
;; ═══════════════════════════════════════════════════════════════════════
;; S-expression topology definition for the multi-agent orchestration
;; system. Parsed by omega-bootstrap.py at system init to instantiate
;; agents, fleets, bridges, and health monitors.
;;
;; Macro Syntax (inherited from REVITHION token system):
;;   (defmacro name (pos1 pos2 &key kw1 kw2 &rest rest-param)
;;     "Optional docstring."
;;     (body-template ...))
;;
;; Special Forms:
;;   (or param default)    — Use param if bound, else default
;;   (splice list-param)   — Inline list elements into parent expression
;;   (require path)        — Load macro definitions from another file
;; ═══════════════════════════════════════════════════════════════════════

(require "../macros/agent-macros.sexp")
(require "../../REVITHION-STUDIO/tokens/macros.sexp")


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  SYSTEM METADATA                                                  ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(omega-system
  :name        "REVITHION Omega"
  :version     "1.0.0"
  :description "Multi-agent orchestration topology for REVITHION STUDIO —
                ALL-IN-ONE professional audio/video/3D production suite.
                Coordinates internal Copilot agents, external CLI terminals,
                fleet dispatch, heartbeat monitoring, and critic review."
  :repository  "SpiralCloudOmega/DevTeam6"
  :owner       "SpiralCloudOmega"
  :environment "WSL2 Ubuntu 24.04 / Windows 11"

  :paths
  (paths :root       "/mnt/c/Users/autom/VS Code GitHub Repos/DevTeam6"
         :omega-dir  ".github/omega"
         :agents-dir ".github/omega/agents"
         :skills-dir ".github/omega/skills"
         :macros-dir ".github/omega/macros"
         :bridge-dir ".github/omega/bridge"
         :kanban-dir ".github/omega/kanban"
         :memory-db  ".github/memory/spiralcloud-memory.db"
         :planning   ".planning"
         :revithion  "REVITHION-STUDIO"
         :tokens     "REVITHION-STUDIO/tokens")


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  AGENT MACROS — Templates for common agent patterns               ║
;; ╚═══════════════════════════════════════════════════════════════════╝

;; ── Core Agent Template ──────────────────────────────────────────────

(defmacro omega-agent (name role &key skills tools workspace model
                       heartbeat-interval heartbeat-timeout heartbeat-retry
                       critic max-context-tokens description)
  "Base agent template. All omega agents inherit from this form.
   Provides heartbeat defaults, workspace isolation, and critic binding."
  (agent :name name
         :role role
         :description (or description "")
         :skills (or skills [])
         :tools (or tools [])
         :workspace (or workspace (concat ".planning/" name))
         :model (or model "claude-sonnet-4-20250514")
         :max-context-tokens (or max-context-tokens 200000)
         :critic (or critic "omega-critic")
         :heartbeat
         (heartbeat :interval (or heartbeat-interval 30)
                    :timeout  (or heartbeat-timeout 120)
                    :retry    (or heartbeat-retry 3))))

;; ── Fleet Lead Template ──────────────────────────────────────────────

(defmacro fleet-lead (name fleet-id &key skills tools model description)
  "Fleet lead agent — coordinates a group of specialist sub-agents.
   Higher heartbeat frequency, larger context window."
  (omega-agent name "lead"
    :skills skills
    :tools (or tools ["task" "explore" "general-purpose" "bash" "grep" "glob"])
    :model (or model "claude-sonnet-4-20250514")
    :max-context-tokens 200000
    :heartbeat-interval 15
    :heartbeat-timeout 90
    :heartbeat-retry 5
    :description (or description (concat "Fleet lead for " fleet-id))
    :workspace (concat ".planning/fleets/" fleet-id)))

;; ── External Terminal Template ───────────────────────────────────────

(defmacro external-terminal (name binary &key port protocol auth-method
                             health-endpoint startup-cmd version description)
  "External CLI terminal — bridges to non-Copilot AI tools.
   Each terminal has its own health check and bridge config."
  (terminal :name name
            :binary binary
            :version (or version "latest")
            :description (or description "")
            :port (or port nil)
            :protocol (or protocol "stdio")
            :auth-method (or auth-method "none")
            :health-endpoint (or health-endpoint "/health")
            :startup-cmd (or startup-cmd binary)
            :bridge
            (bridge :enabled true
                    :protocol (or protocol "stdio")
                    :timeout 30
                    :retry 3)))


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  SKILL PACKS — Bundled capabilities referenced by agents          ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(skill-packs

  (skill-pack :id "audio-core"
    :description "Core audio DSP, amp modeling, and signal processing"
    :skills ["audio-dsp-expert" "amp-modeling-expert" "faust-dsp-expert"
             "spectral-analysis-expert" "mastering-engineer" "mix-engineer"])

  (skill-pack :id "audio-extended"
    :description "Extended audio — MIDI, plugins, neural inference, GPU compute"
    :skills ["midi-expert" "plugin-format-expert" "neural-inference-expert"
             "vulkan-compute-expert" "ace-step-expert"])

  (skill-pack :id "code-systems"
    :description "Systems programming — C++, Rust, CMake, JUCE"
    :skills ["cpp-modern" "cmake-cpp20-expert" "juce-framework-expert"
             "rust-modern" "clean-code-modern"])

  (skill-pack :id "code-web"
    :description "Web and scripting — Python, TypeScript, Node, Go"
    :skills ["python-modern" "typescript-modern" "node-express-modern"
             "go-backend-scalability" "fastapi-modern"])

  (skill-pack :id "code-frontend"
    :description "Frontend frameworks — React, Next.js, Svelte, Tailwind"
    :skills ["react-modern" "nextjs-modern" "svelte-modern"
             "tailwind-modern" "vue-modern"])

  (skill-pack :id "devops-infra"
    :description "Infrastructure — Docker, CI/CD, Git, deployment"
    :skills ["awesome-docker" "gitflow-modern" "github-code-quality"
             "git-conventional-commit-messages"])

  (skill-pack :id "research-tools"
    :description "Deep research, paper analysis, knowledge synthesis"
    :skills ["context-loader" "memory-keeper" "session-recorder"])

  (skill-pack :id "review-quality"
    :description "Code review, quality assurance, testing"
    :skills ["code-review" "codequality-modern" "javascript-typescript-code-quality"
             "jest-unit-testing" "vitest-unit-testing" "playwright-e2e-testing"])

  (skill-pack :id "lisp-tokens"
    :description "LISP-based design token system — S-expr parsing, macro expansion"
    :skills ["lisp-token-expert" "routing-grid-expert"])

  (skill-pack :id "database-ops"
    :description "Database and vector storage operations"
    :skills ["database-modern" "python-containerization"]))


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CORE AGENTS  (7 system-level agents)                             ║
;; ╚═══════════════════════════════════════════════════════════════════╝

;; ── Omega Orchestrator ───────────────────────────────────────────────
;; Master coordinator — dispatches work to fleets and sub-agents,
;; manages task decomposition, resolves dependencies, tracks state.
(omega-agent "omega-orchestrator" "orchestrator"
  :description "Master orchestrator — decomposes tasks, dispatches to fleets,
                tracks global state via SQLite todos and kanban board.
                Runs in YOLO mode: autonomous execution, no confirmation."
  :skills ["audio-core" "code-systems" "lisp-tokens" "research-tools"]
  :tools  ["task" "explore" "general-purpose" "code-review" "bash"
           "grep" "glob" "sql" "github-mcp" "web-search"]
  :model  "claude-opus-4-20250514"
  :max-context-tokens 200000
  :heartbeat-interval 10
  :heartbeat-timeout 60
  :heartbeat-retry 5
  :critic "omega-critic"
  :workspace ".planning/orchestrator")

;; ── Omega Critic ─────────────────────────────────────────────────────
;; Universal code reviewer — uses LLM Council multi-model consensus.
;; Reviews all agent output before merge/commit.
(omega-agent "omega-critic" "critic"
  :description "Universal reviewer — multi-model LLM Council consensus.
                Evaluates code quality, correctness, security, performance.
                Minimum 3-model agreement required for approval."
  :skills ["review-quality" "code-systems" "code-web" "clean-code-modern"]
  :tools  ["code-review" "explore" "bash" "grep" "glob" "sql"]
  :model  "claude-opus-4-20250514"
  :max-context-tokens 200000
  :heartbeat-interval 30
  :heartbeat-timeout 120
  :heartbeat-retry 3
  :critic nil  ;; self-reviews via council consensus
  :workspace ".planning/critic"
  :council
  (llm-council :quorum 3
               :models ["claude-opus-4-20250514" "gpt-5.1" "gemini-3-pro-preview"]
               :consensus-threshold 0.67
               :escalation-model "claude-opus-4-20250514"))

;; ── Omega Heartbeat ──────────────────────────────────────────────────
;; Health monitor — pings all agents and terminals, restarts failed ones,
;; reports status to orchestrator.
(omega-agent "omega-heartbeat" "monitor"
  :description "Health monitor — continuous liveness probes for all agents
                and external terminals. Triggers restart on timeout,
                escalates to orchestrator after max retries exceeded."
  :skills ["devops-infra"]
  :tools  ["bash" "sql"]
  :model  "claude-haiku-4-20250514"
  :max-context-tokens 50000
  :heartbeat-interval 5
  :heartbeat-timeout 30
  :heartbeat-retry 10
  :critic "omega-critic"
  :workspace ".planning/heartbeat")

;; ── Omega Audio Lead ─────────────────────────────────────────────────
;; Coordinates the audio fleet — amp modeling, DSP, mastering, MIDI,
;; spectral analysis, plugin formats, neural inference, GPU compute.
(fleet-lead "omega-audio-lead" "audio-fleet"
  :description "Audio fleet coordinator — routes audio tasks to specialists.
                Manages amp model tokens, DSP chains, mastering presets,
                MIDI routing, spectral analysis, and neural inference."
  :skills ["audio-core" "audio-extended" "lisp-tokens"]
  :tools  ["task" "explore" "general-purpose" "bash" "grep" "glob" "sql"]
  :model  "claude-sonnet-4-20250514")

;; ── Omega Code Lead ──────────────────────────────────────────────────
;; Coordinates the code fleet — Python, TypeScript, Rust, Go, C++, CMake.
(fleet-lead "omega-code-lead" "code-fleet"
  :description "Code fleet coordinator — routes implementation tasks to
                language specialists. Manages build systems, test suites,
                dependency resolution, and cross-language integration."
  :skills ["code-systems" "code-web" "code-frontend" "devops-infra"]
  :tools  ["task" "explore" "general-purpose" "bash" "grep" "glob" "sql"]
  :model  "claude-sonnet-4-20250514")

;; ── Omega Research Lead ──────────────────────────────────────────────
;; Coordinates the research fleet — deep research, paper analysis,
;; knowledge synthesis, memory management.
(fleet-lead "omega-research-lead" "research-fleet"
  :description "Research fleet coordinator — manages deep research tasks
                via Gemini, NotebookLM, ArXiv scanning. Synthesizes
                findings into actionable knowledge for other agents."
  :skills ["research-tools" "database-ops"]
  :tools  ["task" "explore" "bash" "web-search" "sql" "web-fetch"]
  :model  "claude-sonnet-4-20250514")

;; ── Omega Builder ────────────────────────────────────────────────────
;; Runs builds, tests, deployments, and infrastructure operations.
(omega-agent "omega-builder" "builder"
  :description "Build and deploy agent — executes CMake builds, npm/pip
                installs, test suites, Docker images, CI/CD pipelines.
                Reports pass/fail with full logs on failure."
  :skills ["devops-infra" "code-systems" "code-web"]
  :tools  ["task" "bash" "sql" "github-mcp"]
  :model  "claude-haiku-4-20250514"
  :max-context-tokens 100000
  :heartbeat-interval 20
  :heartbeat-timeout 300
  :heartbeat-retry 3
  :critic "omega-critic"
  :workspace ".planning/builder")


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  EXTERNAL CLI TERMINALS  (5 bridges to non-Copilot tools)         ║
;; ╚═══════════════════════════════════════════════════════════════════╝

;; ── Gemini Terminal ──────────────────────────────────────────────────
;; Google Gemini CLI — deep research, multi-modal reasoning, 1M+ context.
(external-terminal "gemini-terminal" "/usr/local/bin/gemini"
  :description "Google Gemini CLI — deep research with 1M+ token context.
                Used for multi-modal analysis, long-document synthesis,
                and cross-referencing research papers."
  :protocol    "stdio"
  :auth-method "api-key"
  :version     "latest"
  :startup-cmd "gemini --model gemini-2.5-pro"
  :health-endpoint nil)

;; ── OpenCode Terminal ────────────────────────────────────────────────
;; OpenCode — fast multi-model coding terminal with session persistence.
(external-terminal "opencode-terminal" "/home/autom/.opencode/bin/opencode"
  :description "OpenCode CLI — fast multi-model coding with session state.
                Supports Claude, GPT, Gemini backends. Optimized for
                rapid iteration on code changes."
  :protocol    "stdio"
  :auth-method "config-file"
  :version     "latest"
  :startup-cmd "opencode"
  :health-endpoint nil)

;; ── JULES Terminal ───────────────────────────────────────────────────
;; Google JULES — full VM sandboxed execution for untrusted code.
(external-terminal "jules-terminal" "/usr/bin/jules"
  :description "Google JULES — full VM execution environment. Sandboxed
                compute for untrusted code, long-running builds, and
                integration tests requiring isolated environments."
  :protocol    "stdio"
  :auth-method "oauth"
  :version     "latest"
  :startup-cmd "jules"
  :health-endpoint nil)

;; ── OpenClaw Terminal ────────────────────────────────────────────────
;; OpenClaw — 100-agent swarm coordination on port 18789.
(external-terminal "openclaw-terminal" "/usr/bin/openclaw"
  :description "OpenClaw agent swarm — up to 100 concurrent agents.
                HTTP API on port 18789. Used for embarrassingly parallel
                tasks: mass refactoring, bulk analysis, search indexing."
  :port        18789
  :protocol    "http"
  :auth-method "bearer-token"
  :version     "latest"
  :startup-cmd "openclaw serve --port 18789 --max-agents 100"
  :health-endpoint "/api/v1/health")

;; ── Ollama Terminal ──────────────────────────────────────────────────
;; Ollama — local model inference, zero-latency for small tasks.
(external-terminal "ollama-terminal" "/usr/local/bin/ollama"
  :description "Ollama local inference — zero-latency model serving.
                Models: qwen3:8b (fast), qwen3-coder:30b (code).
                RTX 3090 24GB VRAM for local GPU inference."
  :port        11434
  :protocol    "http"
  :auth-method "none"
  :version     "0.17.4"
  :startup-cmd "ollama serve"
  :health-endpoint "/api/tags")


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  FLEET DEFINITIONS — Groups of agents with shared config          ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(fleets

  ;; ── Audio Fleet ────────────────────────────────────────────────────
  ;; All audio/DSP specialists — the core of REVITHION STUDIO
  (fleet :id "audio-fleet"
    :lead "omega-audio-lead"
    :critic "omega-critic"
    :description "Audio production specialists — amp modeling, DSP, mastering,
                  MIDI, spectral analysis, plugin formats, neural inference,
                  Faust DSP, Vulkan compute, ACE-Step AI generation."
    :agents
    [(agent-ref :name "amp-modeling-specialist"
                :agent-type "amp-modeling-expert"
                :model "claude-sonnet-4-20250514"
                :skills ["audio-core" "lisp-tokens"])
     (agent-ref :name "dsp-specialist"
                :agent-type "audio-dsp-expert"
                :model "claude-sonnet-4-20250514"
                :skills ["audio-core"])
     (agent-ref :name "mastering-specialist"
                :agent-type "mastering-engineer"
                :model "claude-sonnet-4-20250514"
                :skills ["audio-core"])
     (agent-ref :name "mix-specialist"
                :agent-type "mix-engineer"
                :model "claude-sonnet-4-20250514"
                :skills ["audio-core"])
     (agent-ref :name "midi-specialist"
                :agent-type "midi-expert"
                :model "claude-haiku-4-20250514"
                :skills ["audio-extended"])
     (agent-ref :name "spectral-specialist"
                :agent-type "spectral-analysis-expert"
                :model "claude-sonnet-4-20250514"
                :skills ["audio-extended"])
     (agent-ref :name "plugin-specialist"
                :agent-type "plugin-format-expert"
                :model "claude-sonnet-4-20250514"
                :skills ["audio-extended"])
     (agent-ref :name "faust-specialist"
                :agent-type "faust-dsp-expert"
                :model "claude-sonnet-4-20250514"
                :skills ["audio-core"])
     (agent-ref :name "neural-inference-specialist"
                :agent-type "neural-inference-expert"
                :model "claude-sonnet-4-20250514"
                :skills ["audio-extended"])
     (agent-ref :name "vulkan-compute-specialist"
                :agent-type "vulkan-compute-expert"
                :model "claude-sonnet-4-20250514"
                :skills ["audio-extended"])
     (agent-ref :name "ace-step-specialist"
                :agent-type "ace-step-expert"
                :model "claude-sonnet-4-20250514"
                :skills ["audio-extended"])]
    :heartbeat
    (heartbeat :interval 30 :timeout 120 :retry 3))

  ;; ── Code Fleet ─────────────────────────────────────────────────────
  ;; Language-specific implementation specialists
  (fleet :id "code-fleet"
    :lead "omega-code-lead"
    :critic "omega-critic"
    :description "Implementation specialists — Python, TypeScript, Rust, Go,
                  C++/CMake, JUCE framework. Handles all coding tasks
                  dispatched by the orchestrator."
    :agents
    [(agent-ref :name "python-specialist"
                :agent-type "python-modern"
                :model "claude-sonnet-4-20250514"
                :skills ["code-web"])
     (agent-ref :name "typescript-specialist"
                :agent-type "typescript-modern"
                :model "claude-sonnet-4-20250514"
                :skills ["code-web" "code-frontend"])
     (agent-ref :name "rust-specialist"
                :agent-type "rust-modern"
                :model "claude-sonnet-4-20250514"
                :skills ["code-systems"])
     (agent-ref :name "go-specialist"
                :agent-type "go-backend-scalability"
                :model "claude-sonnet-4-20250514"
                :skills ["code-web"])
     (agent-ref :name "cpp-specialist"
                :agent-type "cpp-modern"
                :model "claude-sonnet-4-20250514"
                :skills ["code-systems"])
     (agent-ref :name "cmake-specialist"
                :agent-type "cmake-cpp20-expert"
                :model "claude-sonnet-4-20250514"
                :skills ["code-systems"])
     (agent-ref :name "juce-specialist"
                :agent-type "juce-framework-expert"
                :model "claude-sonnet-4-20250514"
                :skills ["code-systems" "audio-core"])]
    :heartbeat
    (heartbeat :interval 30 :timeout 120 :retry 3))

  ;; ── Research Fleet ─────────────────────────────────────────────────
  ;; Deep research, paper analysis, knowledge synthesis
  (fleet :id "research-fleet"
    :lead "omega-research-lead"
    :critic "omega-critic"
    :description "Research and knowledge agents — Gemini deep research,
                  NotebookLM synthesis, ArXiv paper scanning, memory
                  management, and context loading."
    :agents
    [(agent-ref :name "gemini-researcher"
                :agent-type "explore"
                :terminal "gemini-terminal"
                :model "gemini-3-pro-preview"
                :skills ["research-tools"])
     (agent-ref :name "notebooklm-researcher"
                :agent-type "explore"
                :terminal "ollama-terminal"
                :model "claude-sonnet-4-20250514"
                :skills ["research-tools"])
     (agent-ref :name "arxiv-scanner"
                :agent-type "explore"
                :model "claude-haiku-4-20250514"
                :skills ["research-tools"])
     (agent-ref :name "memory-manager"
                :agent-type "memory-keeper"
                :model "claude-haiku-4-20250514"
                :skills ["research-tools" "database-ops"])
     (agent-ref :name "context-manager"
                :agent-type "context-loader"
                :model "claude-haiku-4-20250514"
                :skills ["research-tools"])]
    :heartbeat
    (heartbeat :interval 60 :timeout 180 :retry 3))

  ;; ── DevOps Fleet ───────────────────────────────────────────────────
  ;; Infrastructure, CI/CD, containerization
  (fleet :id "devops-fleet"
    :lead "omega-builder"
    :critic "omega-critic"
    :description "Infrastructure and deployment — Docker builds, CI/CD
                  pipelines, Git workflow, dependency management."
    :agents
    [(agent-ref :name "docker-specialist"
                :agent-type "task"
                :model "claude-haiku-4-20250514"
                :skills ["devops-infra"])
     (agent-ref :name "ci-specialist"
                :agent-type "task"
                :model "claude-haiku-4-20250514"
                :skills ["devops-infra"])
     (agent-ref :name "git-specialist"
                :agent-type "task"
                :model "claude-haiku-4-20250514"
                :skills ["devops-infra"])]
    :heartbeat
    (heartbeat :interval 30 :timeout 120 :retry 3))

  ;; ── Review Fleet ───────────────────────────────────────────────────
  ;; Code review, quality assurance, testing specialists
  (fleet :id "review-fleet"
    :lead "omega-critic"
    :critic nil  ;; reviews are self-validating via council
    :description "Quality assurance — code review, testing, static analysis,
                  security scanning. Uses LLM Council for consensus."
    :agents
    [(agent-ref :name "code-reviewer"
                :agent-type "code-review"
                :model "claude-sonnet-4-20250514"
                :skills ["review-quality"])
     (agent-ref :name "test-specialist"
                :agent-type "task"
                :model "claude-haiku-4-20250514"
                :skills ["review-quality"])
     (agent-ref :name "security-reviewer"
                :agent-type "code-review"
                :model "claude-opus-4-20250514"
                :skills ["review-quality"])]
    :heartbeat
    (heartbeat :interval 30 :timeout 120 :retry 3)))


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  HEARTBEAT CONFIGURATION — Global defaults + per-agent overrides  ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(heartbeat-config
  :description "Global heartbeat settings. Per-agent overrides take precedence."

  :global-defaults
  (heartbeat :interval 30      ;; seconds between pings
             :timeout  120     ;; seconds before declared unresponsive
             :retry    3       ;; retries before escalation
             :backoff  "exponential"  ;; retry backoff strategy
             :max-backoff 300)        ;; max 5 minutes between retries

  :escalation-policy
  (escalation :on-timeout   "restart-agent"
              :on-max-retry "notify-orchestrator"
              :on-cascade   "pause-fleet"       ;; if >50% of fleet down
              :cascade-threshold 0.5)

  ;; Per-agent overrides (agents with stricter/looser requirements)
  :overrides
  [(heartbeat-override :agent "omega-orchestrator"
                       :interval 10 :timeout 60 :retry 5)
   (heartbeat-override :agent "omega-heartbeat"
                       :interval 5  :timeout 30 :retry 10)
   (heartbeat-override :agent "omega-builder"
                       :interval 20 :timeout 300 :retry 3)
   (heartbeat-override :agent "openclaw-terminal"
                       :interval 15 :timeout 60 :retry 5)
   (heartbeat-override :agent "ollama-terminal"
                       :interval 10 :timeout 45 :retry 5)])


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  INTER-TERMINAL BRIDGE — HTTP/stdio bridge for external tools     ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(bridge-config
  :description "Configuration for bridges between Copilot agents and
                external CLI terminals. HTTP bridges for networked tools,
                stdio bridges for local CLI tools."

  :http-bridge
  (http-bridge :port-range [19000 19100]  ;; reserved port range
               :protocol   "http/1.1"
               :auth       "bearer-token"
               :tls        false          ;; local only, no TLS needed
               :cors       false
               :timeout    30
               :max-payload-mb 50)

  :stdio-bridge
  (stdio-bridge :buffer-size  65536       ;; 64KB line buffer
                :encoding     "utf-8"
                :timeout      60
                :max-lines    10000)

  ;; Bridge routing — which terminal handles which request type
  :routes
  [(bridge-route :pattern "research/*"     :terminal "gemini-terminal")
   (bridge-route :pattern "code/edit/*"    :terminal "opencode-terminal")
   (bridge-route :pattern "code/sandbox/*" :terminal "jules-terminal")
   (bridge-route :pattern "swarm/*"        :terminal "openclaw-terminal")
   (bridge-route :pattern "inference/*"    :terminal "ollama-terminal")
   (bridge-route :pattern "build/*"        :terminal "omega-builder")
   (bridge-route :pattern "review/*"       :terminal "omega-critic")])


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CRITIC ASSIGNMENTS — Which critic reviews which fleet            ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(critic-assignments
  :description "Maps fleets and agents to their assigned critic.
                omega-critic is the universal fallback reviewer."

  :assignments
  [(critic-assign :fleet "audio-fleet"    :critic "omega-critic"
                  :review-scope ["correctness" "dsp-accuracy" "performance"
                                 "latency" "bit-exactness"])
   (critic-assign :fleet "code-fleet"     :critic "omega-critic"
                  :review-scope ["correctness" "style" "security"
                                 "performance" "test-coverage"])
   (critic-assign :fleet "research-fleet" :critic "omega-critic"
                  :review-scope ["accuracy" "completeness" "citations"
                                 "actionability"])
   (critic-assign :fleet "devops-fleet"   :critic "omega-critic"
                  :review-scope ["reliability" "security" "idempotency"
                                 "rollback-safety"])
   (critic-assign :fleet "review-fleet"   :critic nil
                  :review-scope ["consensus-achieved"])]

  :council-config
  (llm-council :min-reviewers  3
               :consensus      0.67
               :timeout        120
               :models         ["claude-opus-4-20250514"
                                "gpt-5.1"
                                "gemini-3-pro-preview"]
               :fallback-model "claude-sonnet-4-20250514"))


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  WORKSPACE ISOLATION — Per-agent scratch dirs under .planning/    ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(workspace-config
  :description "Each agent gets an isolated workspace for scratch files,
                intermediate results, and session state. Workspaces are
                cleaned on agent restart unless persist is true."

  :base-path ".planning"
  :auto-create true
  :cleanup-on-restart false

  :workspaces
  [(workspace :agent "omega-orchestrator" :path ".planning/orchestrator"
              :persist true  :max-size-mb 100)
   (workspace :agent "omega-critic"       :path ".planning/critic"
              :persist true  :max-size-mb 50)
   (workspace :agent "omega-heartbeat"    :path ".planning/heartbeat"
              :persist false :max-size-mb 10)
   (workspace :agent "omega-audio-lead"   :path ".planning/fleets/audio-fleet"
              :persist true  :max-size-mb 200)
   (workspace :agent "omega-code-lead"    :path ".planning/fleets/code-fleet"
              :persist true  :max-size-mb 200)
   (workspace :agent "omega-research-lead" :path ".planning/fleets/research-fleet"
              :persist true  :max-size-mb 500)
   (workspace :agent "omega-builder"      :path ".planning/builder"
              :persist false :max-size-mb 500)])


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CONTEXT MANAGEMENT — Memory, FTS5, planning-with-files           ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(context-config
  :description "Context management integrations — FTS5 memory database,
                context-mode for session loading, planning-with-files
                for structured workspace artifacts."

  :memory
  (memory-config :database ".github/memory/spiralcloud-memory.db"
                 :engine   "sqlite-fts5"
                 :tables   ["sessions" "turns" "checkpoints" "session_files"
                            "session_refs" "search_index"]
                 :auto-record true
                 :verbatim  true       ;; no summarizing, no chunking
                 :max-db-size-mb 500)

  :context-mode
  (context-mode :enabled true
                :source  "context-mode-main"
                :auto-load-on-session-start true
                :load-order ["environment-profile" "rules-and-directives"
                             "recent-sessions" "pending-work"])

  :planning-with-files
  (planning-files :enabled true
                  :source  "planning-with-files-master"
                  :plan-file   ".planning/plan.md"
                  :context-dir ".planning/context"
                  :artifacts   [".planning/plan.md"
                                ".planning/context/environment.md"
                                ".planning/context/progress.md"
                                ".planning/context/decisions.md"])

  :environment-profile
  (env-profile :path ".github/memory/environment-profile.json"
               :auto-refresh-interval 3600  ;; refresh hourly
               :include ["hardware" "languages" "services" "repositories"
                         "ai-tools" "custom-projects"]))


;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  EXTERNAL TOOL REGISTRY — gh CLI, git, and system tools           ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(tool-registry
  :description "System-level CLI tools available to all agents."

  :tools
  [(tool :name "gh"        :binary "/usr/bin/gh"
         :version "2.87.2" :description "GitHub CLI — PRs, issues, actions")
   (tool :name "git"       :binary "/usr/bin/git"
         :version "2.43.0" :description "Git version control")
   (tool :name "python"    :binary "/home/autom/miniconda3/bin/python"
         :version "3.13"   :description "Python 3.13 via Miniconda")
   (tool :name "node"      :binary "/home/autom/.nvm/current/bin/node"
         :version "22"     :description "Node.js 22 via nvm")
   (tool :name "rustc"     :binary "/home/autom/.cargo/bin/rustc"
         :version "1.93"   :description "Rust compiler")
   (tool :name "go"        :binary "/usr/local/go/bin/go"
         :version "1.22"   :description "Go compiler")
   (tool :name "cmake"     :binary "/usr/bin/cmake"
         :version "3.28"   :description "CMake build system")
   (tool :name "gcc"       :binary "/usr/bin/gcc"
         :version "11.5"   :description "GNU C/C++ compiler")
   (tool :name "docker"    :binary "/usr/bin/docker"
         :version "latest" :description "Docker container runtime")
   (tool :name "bun"       :binary "/home/autom/.bun/bin/bun"
         :version "1.3"    :description "Bun JavaScript runtime")])


) ;; end omega-system
