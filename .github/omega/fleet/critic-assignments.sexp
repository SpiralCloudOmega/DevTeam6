;; ═══════════════════════════════════════════════════════════════════════
;; REVITHION STUDIO — Omega Critic Assignments
;; ═══════════════════════════════════════════════════════════════════════
;; Maps each agent category to its critic, quorum threshold, and
;; review focus areas. The critic runs an LLM Council (3 anonymous
;; reviewers, no cross-talk, scored rubric, quorum voting).
;;
;; Quorum levels:
;;   1 = any single reviewer can approve (low-risk categories)
;;   2 = majority agreement required (standard categories)
;;   3 = unanimous agreement required (security-critical categories)
;; ═══════════════════════════════════════════════════════════════════════

(critic-assignment :category "agent-system"
                   :agent-count 3
                   :critic "omega-security"
                   :quorum 2
                   :review-focus "context integrity, memory persistence, recursion depth, token budget")

(critic-assignment :category "audio"
                   :agent-count 8
                   :critic "omega-critic"
                   :quorum 2
                   :review-focus "DSP correctness, latency budget, SIMD safety, sample-rate handling")

(critic-assignment :category "data-ml"
                   :agent-count 5
                   :critic "omega-critic"
                   :quorum 2
                   :review-focus "numerical stability, data leakage, reproducibility, GPU memory")

(critic-assignment :category "devops"
                   :agent-count 7
                   :critic "omega-security"
                   :quorum 2
                   :review-focus "secrets exposure, permission scoping, idempotency, rollback safety")

(critic-assignment :category "documentation"
                   :agent-count 7
                   :critic "omega-critic"
                   :quorum 1
                   :review-focus "accuracy, completeness, clarity, actionability")

(critic-assignment :category "game-dev"
                   :agent-count 5
                   :critic "omega-critic"
                   :quorum 1
                   :review-focus "frame budget, memory allocation, physics determinism, input latency")

(critic-assignment :category "general"
                   :agent-count 0
                   :critic "omega-critic"
                   :quorum 1
                   :review-focus "correctness, safety, best practices")

(critic-assignment :category "infrastructure"
                   :agent-count 7
                   :critic "omega-critic"
                   :quorum 2
                   :review-focus "build reproducibility, ABI stability, memory safety, thread safety")

(critic-assignment :category "mobile"
                   :agent-count 11
                   :critic "omega-critic"
                   :quorum 1
                   :review-focus "platform guidelines, memory management, battery efficiency, accessibility")

(critic-assignment :category "personal"
                   :agent-count 22
                   :critic "omega-critic"
                   :quorum 1
                   :review-focus "correctness, safety, project-specific conventions")

(critic-assignment :category "quality"
                   :agent-count 7
                   :critic "omega-critic"
                   :quorum 1
                   :review-focus "consistency, readability, maintainability, SOLID principles")

(critic-assignment :category "revithion"
                   :agent-count 2
                   :critic "omega-critic"
                   :quorum 2
                   :review-focus "macro correctness, token schema, C++ codegen safety, AXE-FX compatibility")

(critic-assignment :category "security"
                   :agent-count 6
                   :critic "omega-security"
                   :quorum 3
                   :review-focus "vulnerabilities, secrets, injection, reentrancy, access control")

(critic-assignment :category "testing"
                   :agent-count 13
                   :critic "omega-critic"
                   :quorum 1
                   :review-focus "test coverage, flakiness, assertion quality, determinism")

(critic-assignment :category "web-backend"
                   :agent-count 51
                   :critic "omega-critic"
                   :quorum 2
                   :review-focus "API correctness, authentication, SQL injection, rate limiting")

(critic-assignment :category "web-frontend"
                   :agent-count 82
                   :critic "omega-critic"
                   :quorum 1
                   :review-focus "accessibility, performance, type safety, responsive design")

;; ═══════════════════════════════════════════════════════════════════════
;; Omega agents: self-governing (orchestrator reviews, no external critic)
;; ═══════════════════════════════════════════════════════════════════════

(critic-assignment :category "omega"
                   :agent-count 7
                   :critic "omega-orchestrator"
                   :quorum 2
                   :review-focus "system integrity, recursion safety, dispatch correctness, token budget")

