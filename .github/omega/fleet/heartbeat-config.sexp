;; ═══════════════════════════════════════════════════════════════════════
;; REVITHION STUDIO — Omega Heartbeat Configuration
;; ═══════════════════════════════════════════════════════════════════════
;; Defines heartbeat check intervals per category. Agents that miss
;; 3 consecutive heartbeats are marked unhealthy; 5 misses = dead.
;; Dead agents are removed from the dispatch pool and retried every 5m.
;;
;; Tiers:
;;   5s  — meta-critical (agent-system: context, memory, session)
;;   15s — real-time critical (audio, infrastructure, revithion)
;;   30s — standard (web, backend, mobile, devops, game-dev, data-ml)
;;   60s — low-priority (testing, documentation, quality)
;; ═══════════════════════════════════════════════════════════════════════

(heartbeat-policy
  :max-missed      3
  :dead-threshold  5
  :retry-interval  300
  :alert-channel   "omega-heartbeat")

;; ── META-CRITICAL (5s) ──────────────────────────────────

(heartbeat-config :category "agent-system"
                  :agent-count 3
                  :interval 5
                  :timeout 15
                  :max-missed 3
                  :dead-threshold 5)

;; ── REAL-TIME CRITICAL (15s) ──────────────────────────────────

(heartbeat-config :category "audio"
                  :agent-count 8
                  :interval 15
                  :timeout 45
                  :max-missed 3
                  :dead-threshold 5)

(heartbeat-config :category "infrastructure"
                  :agent-count 7
                  :interval 15
                  :timeout 45
                  :max-missed 3
                  :dead-threshold 5)

(heartbeat-config :category "revithion"
                  :agent-count 2
                  :interval 15
                  :timeout 45
                  :max-missed 3
                  :dead-threshold 5)

;; ── STANDARD (30s) ──────────────────────────────────

(heartbeat-config :category "data-ml"
                  :agent-count 5
                  :interval 30
                  :timeout 90
                  :max-missed 3
                  :dead-threshold 5)

(heartbeat-config :category "devops"
                  :agent-count 7
                  :interval 30
                  :timeout 90
                  :max-missed 3
                  :dead-threshold 5)

(heartbeat-config :category "game-dev"
                  :agent-count 5
                  :interval 30
                  :timeout 90
                  :max-missed 3
                  :dead-threshold 5)

(heartbeat-config :category "general"
                  :agent-count 0
                  :interval 30
                  :timeout 90
                  :max-missed 3
                  :dead-threshold 5)

(heartbeat-config :category "mobile"
                  :agent-count 11
                  :interval 30
                  :timeout 90
                  :max-missed 3
                  :dead-threshold 5)

(heartbeat-config :category "personal"
                  :agent-count 22
                  :interval 30
                  :timeout 90
                  :max-missed 3
                  :dead-threshold 5)

(heartbeat-config :category "security"
                  :agent-count 6
                  :interval 30
                  :timeout 90
                  :max-missed 3
                  :dead-threshold 5)

(heartbeat-config :category "web-backend"
                  :agent-count 51
                  :interval 30
                  :timeout 90
                  :max-missed 3
                  :dead-threshold 5)

(heartbeat-config :category "web-frontend"
                  :agent-count 82
                  :interval 30
                  :timeout 90
                  :max-missed 3
                  :dead-threshold 5)

;; ── LOW-PRIORITY (60s) ──────────────────────────────────

(heartbeat-config :category "documentation"
                  :agent-count 7
                  :interval 60
                  :timeout 180
                  :max-missed 3
                  :dead-threshold 5)

(heartbeat-config :category "quality"
                  :agent-count 7
                  :interval 60
                  :timeout 180
                  :max-missed 3
                  :dead-threshold 5)

(heartbeat-config :category "testing"
                  :agent-count 13
                  :interval 60
                  :timeout 180
                  :max-missed 3
                  :dead-threshold 5)

;; ── OMEGA SYSTEM (10s) ─────────────────────────────────────

(heartbeat-config :category "omega"
                  :agent-count 7
                  :interval 10
                  :timeout 30
                  :max-missed 2
                  :dead-threshold 3)

