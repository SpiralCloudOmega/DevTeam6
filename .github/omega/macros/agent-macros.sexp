;; ═══════════════════════════════════════════════════════════════════════
;; OMEGA SYSTEM — Macro Definitions for Agent Configuration Tokens
;; ═══════════════════════════════════════════════════════════════════════
;; These macros provide reusable templates for defining Omega agents,
;; fleets, skills, and inter-agent communication. Expanded at build time
;; by the Omega orchestrator into runtime agent specifications.
;;
;; Follows the REVITHION STUDIO procedural design token pattern:
;;   S-expression tokens → LISP macro expansion → runtime config
;;
;; Macro Syntax:
;;   (defmacro name (pos1 pos2 &key kw1 kw2 &rest rest-param)
;;     "Optional docstring."
;;     (body-template ...))
;;
;; Special Forms:
;;   (or param default)    — Use param if bound, else default
;;   (splice list-param)   — Inline list elements into parent expression
;; ═══════════════════════════════════════════════════════════════════════


;; ── §1  Core Agent Definition ───────────────────────────────────────

(defmacro omega-agent (name role description
                       &key skills tools workspace model
                            heartbeat-interval heartbeat-timeout
                            critic context-budget delegate-to)
  "Core Omega agent specification.
   Defines a single autonomous agent with its role, capabilities,
   workspace isolation, health monitoring, and delegation targets.
   Every agent in the Omega system is instantiated from this form."
  (agent-spec
    :name name
    :role role
    :description description
    :model (or model "claude-sonnet-4-20250514")
    :skills (or skills ())
    :tools (or tools ())
    :workspace (or workspace
      (workspace-config name
        :base-path (string-append ".omega/workspaces/" (symbol->string name))))
    :heartbeat
      (heartbeat-config
        :interval (or heartbeat-interval 30)
        :timeout (or heartbeat-timeout 120))
    :critic (or critic nil)
    :context-budget (or context-budget 200000)
    :delegate-to (or delegate-to ())))


;; ── §2  Critic Agent — LLM Council Peer Review ─────────────────────

(defmacro critic-agent (name description
                        &key review-patterns consensus-model
                             quorum-threshold review-stages
                             scoring-rubric fleet-assignment)
  "Critic agent with LLM Council anonymous peer review.
   Implements a multi-stage review pipeline: each stage independently
   scores agent output against the rubric, then a consensus model
   aggregates scores. Quorum must be met before output is accepted.
   Based on the llm-council-master pattern."
  (agent-spec
    :name name
    :role critic
    :description description
    :model (or consensus-model "claude-sonnet-4-20250514")
    :review-config
      (review-pipeline
        :stages (or review-stages 3)
        :anonymous t
        :quorum-threshold (or quorum-threshold 0.7)
        :scoring-rubric (or scoring-rubric
          (default-rubric
            :correctness 0.35
            :completeness 0.25
            :style 0.15
            :security 0.15
            :performance 0.10))
        :review-patterns (or review-patterns
          ("code-review" "plan-review" "output-review")))
    :fleet-assignment (or fleet-assignment nil)))


;; ── §3  External CLI Terminal Connection ────────────────────────────

(defmacro external-cli (name binary-path
                        &key port protocol capabilities
                             health-endpoint timeout
                             max-sessions env-vars)
  "External CLI tool bridge into the Omega system.
   Maps standalone terminal tools (gemini, opencode, jules, openclaw,
   ollama, kimi-code) into first-class Omega agents via a shim layer.
   The bridge spawns/attaches to the CLI process and translates
   Omega messages into stdin/stdout interactions."
  (cli-bridge
    :name name
    :binary-path binary-path
    :protocol (or protocol "stdio")
    :port (or port nil)
    :capabilities (or capabilities ())
    :health-endpoint (or health-endpoint "/health")
    :timeout (or timeout 60)
    :max-sessions (or max-sessions 1)
    :env-vars (or env-vars ())
    :adapter
      (cli-adapter
        :input-format "json-lines"
        :output-format "json-lines"
        :startup-probe
          (probe :path (or health-endpoint "/health")
                 :initial-delay 5
                 :period 10))))


;; ── §4  Agent Fleet — Shared Config Group ───────────────────────────

(defmacro agent-fleet (fleet-name role
                       &key agents shared-skills shared-tools
                            workspace-prefix critic
                            heartbeat-config model)
  "Fleet of agents sharing common configuration.
   Expands into multiple omega-agent forms, each inheriting the fleet's
   shared skills, tools, workspace prefix, and critic assignment.
   Individual agent overrides are merged on top of fleet defaults."
  (fleet-spec
    :name fleet-name
    :role role
    :model (or model "claude-sonnet-4-20250514")
    :shared-skills (or shared-skills ())
    :shared-tools (or shared-tools ())
    :workspace-prefix (or workspace-prefix
      (string-append ".omega/fleets/" (symbol->string fleet-name)))
    :critic (or critic nil)
    :heartbeat (or heartbeat-config
      (heartbeat-config :interval 30 :timeout 120))
    :agents
      (map (lambda (agent-override)
        (omega-agent
          :name (get agent-override :name)
          :role role
          :description (get agent-override :description)
          :skills (merge (or shared-skills ())
                         (or (get agent-override :skills) ()))
          :tools (merge (or shared-tools ())
                        (or (get agent-override :tools) ()))
          :workspace
            (workspace-config (get agent-override :name)
              :base-path (string-append
                (or workspace-prefix
                  (string-append ".omega/fleets/" (symbol->string fleet-name)))
                "/" (symbol->string (get agent-override :name))))
          :model (or (get agent-override :model) model)
          :critic critic))
        (or agents ()))))


;; ── §5  Skill Pack — Bundled Capability Set ─────────────────────────

(defmacro skill-pack (name &key skills description category)
  "Named bundle of skills that agents can reference.
   Skill packs group related capabilities (e.g. 'code-review-pack'
   bundles linting, diff analysis, and security scanning). Agents
   reference packs by name; the orchestrator resolves and inlines
   the individual skills at agent instantiation time."
  (skill-bundle
    :name name
    :description (or description
      (string-append "Skill pack: " (symbol->string name)))
    :category (or category "general")
    :skills (or skills ())
    :resolved nil))


;; ── §6  Heartbeat Config — Health Monitoring ────────────────────────

(defmacro heartbeat-config (&key interval timeout max-retries
                                 recovery-delay health-endpoint
                                 on-failure)
  "Health monitoring configuration for an agent or fleet.
   The orchestrator pings agents at the specified interval. If an agent
   fails to respond within the timeout, retries are attempted. After
   max-retries exhausted, the on-failure strategy is executed.
   Strategies: restart, reassign, escalate, ignore."
  (health-monitor
    :interval (or interval 30)
    :timeout (or timeout 120)
    :max-retries (or max-retries 3)
    :recovery-delay (or recovery-delay 10)
    :health-endpoint (or health-endpoint "/health")
    :on-failure (or on-failure "restart")))


;; ── §7  Workspace Config — Agent Isolation ──────────────────────────

(defmacro workspace-config (agent-name
                            &key base-path scratch-dir
                                 context-store planning-dir
                                 max-size-mb)
  "Workspace isolation for a single agent.
   Each agent operates in its own directory tree following the
   planning-with-files pattern: .planning/{uuid}/ per session.
   Includes scratch space, a persistent context store (FTS5-indexed
   SQLite), and a planning directory for session artifacts."
  (workspace-spec
    :agent agent-name
    :base-path (or base-path
      (string-append ".omega/workspaces/" (symbol->string agent-name)))
    :scratch-dir (or scratch-dir
      (string-append (or base-path
        (string-append ".omega/workspaces/" (symbol->string agent-name)))
        "/scratch"))
    :context-store (or context-store
      (string-append (or base-path
        (string-append ".omega/workspaces/" (symbol->string agent-name)))
        "/context.db"))
    :planning-dir (or planning-dir
      (string-append (or base-path
        (string-append ".omega/workspaces/" (symbol->string agent-name)))
        "/.planning"))
    :max-size-mb (or max-size-mb 512)))


;; ── §8  Bridge Endpoint — Inter-Terminal Communication ──────────────

(defmacro bridge-endpoint (name &key host port protocol
                                     auth-token routes)
  "HTTP bridge endpoint for inter-terminal communication.
   Defines a network endpoint that agents use to communicate across
   terminal boundaries (e.g. WSL ↔ Windows, local ↔ remote).
   Routes map Omega message types to HTTP paths. Auth is via
   bearer token in the Authorization header."
  (endpoint-spec
    :name name
    :host (or host "127.0.0.1")
    :port (or port 18800)
    :protocol (or protocol "http")
    :auth-token (or auth-token nil)
    :routes (or routes
      (default-routes
        (:message  "/api/v1/message")
        (:health   "/api/v1/health")
        (:delegate "/api/v1/delegate")
        (:status   "/api/v1/status")
        (:stream   "/api/v1/stream")))))


;; ── §9  Context Config — Context Management ─────────────────────────

(defmacro context-config (&key max-tokens reduction-target
                               fts5-tokenizer chunk-max-bytes
                               memory-db)
  "Context management configuration for agent memory.
   Integrates with Context Mode (context-mode-main) for intelligent
   context window management. When an agent approaches max-tokens,
   the reduction strategy compresses context to reduction-target.
   FTS5-indexed SQLite stores long-term memory across sessions."
  (context-spec
    :max-tokens (or max-tokens 200000)
    :reduction-target (or reduction-target 0.6)
    :fts5-tokenizer (or fts5-tokenizer "unicode61")
    :chunk-max-bytes (or chunk-max-bytes 4096)
    :memory-db (or memory-db ".omega/memory/context.db")
    :strategy
      (reduction-strategy
        :method "hierarchical-summary"
        :preserve-recent 0.3
        :preserve-pinned t
        :compress-ratio 0.4)))


;; ── §10  Delegation Rule — Recursive Agent Spawning ─────────────────

(defmacro delegation-rule (from-agent to-agent
                           &key conditions priority max-depth)
  "Rule governing when one agent can delegate work to another.
   The Omega orchestrator evaluates conditions before allowing
   delegation. Max-depth prevents infinite recursion. Priority
   determines which rule fires when multiple rules match.
   Conditions are predicate expressions evaluated at delegation time."
  (delegation-spec
    :from from-agent
    :to to-agent
    :priority (or priority 50)
    :max-depth (or max-depth 3)
    :conditions (or conditions (always-true))
    :policy
      (delegation-policy
        :allow-recursive t
        :inherit-context t
        :inherit-workspace nil
        :timeout-inherit t
        :result-propagation "merge")))


;; ═══════════════════════════════════════════════════════════════════════
;; Composite Macros — Built from primitives above
;; ═══════════════════════════════════════════════════════════════════════

;; ── Convenience: Agent with full stack config ───────────────────────

(defmacro full-agent (name role description
                      &key model skills tools
                           heartbeat workspace context
                           critic delegate-to)
  "Fully configured agent combining workspace, context, and heartbeat.
   Convenience macro that wires together omega-agent, workspace-config,
   context-config, and heartbeat-config into a single declaration."
  (omega-agent name role description
    :model (or model "claude-sonnet-4-20250514")
    :skills (or skills ())
    :tools (or tools ())
    :heartbeat-interval (or (get heartbeat :interval) 30)
    :heartbeat-timeout (or (get heartbeat :timeout) 120)
    :workspace (or workspace
      (workspace-config name))
    :context-budget (or (get context :max-tokens) 200000)
    :critic (or critic nil)
    :delegate-to (or delegate-to ())))

;; ── Convenience: CLI fleet from a list of binaries ──────────────────

(defmacro cli-fleet (fleet-name &rest cli-specs)
  "Fleet of external CLI tools registered as Omega agents.
   Each cli-spec is (name binary-path &key ...props) passed directly
   to external-cli. The fleet wraps all CLIs with shared health
   monitoring and a unified bridge endpoint."
  (fleet-spec
    :name fleet-name
    :role external-cli
    :agents
      (map (lambda (spec)
        (apply external-cli spec))
        (splice cli-specs))
    :bridge
      (bridge-endpoint fleet-name
        :port 18800
        :protocol "http")))


;; ═══════════════════════════════════════════════════════════════════════
;; EOF — agent-macros.sexp
;; ═══════════════════════════════════════════════════════════════════════
