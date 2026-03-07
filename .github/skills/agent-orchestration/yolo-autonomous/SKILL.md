---
name: YOLO Autonomous Execution
description: >
  Universal skill for autonomous agent execution in DevTeam6. Agents execute first
  and report results — never block on permission for non-destructive operations.
---
# YOLO Autonomous Execution

## 1. Decision-Making Without Permission
Act first, report results. YOLO mode means execute autonomously. Only ask when genuinely
ambiguous (multiple valid paths with different trade-offs). Default to doing **more**.

```text
GOOD: "Set up tests" → Create config, write 14 tests, run them, report "14/14 passing"
BAD:  "Set up tests" → "Should I use Jest or Vitest?" → Just pick and go.
```

## 2. When to Ask vs When to Act
**ASK**: delete repos, drop databases, ambiguous architecture, security-sensitive changes.
**ACT**: create files, install packages, run builds, code changes, git commits, launch agents.

```text
ACT: npm install zod                          # non-destructive, just do it
ASK: "Migration requires dropping all tables?" # destructive, confirm first
```

## 3. Error Recovery Strategies
Never retry the same failing approach twice. Escalate, pivot, or fall back:
1. **Different approach** — `npm install` fails → try `yarn` or `pnpm`
2. **Escalate model** — haiku → sonnet → opus
3. **Alternative tools** — grep fails on binary → use `strings` instead
4. **Manual fallback** — sub-agent fails twice → do it yourself
5. **Document failures** — record what failed and why in session database

```sql
INSERT INTO todos (id, title, description, status) VALUES
  ('fix-build', 'Fix build', 'tsc failed: missing UserProps type.', 'in_progress');
UPDATE todos SET status='done', description=description||' RESOLVED via types.ts' WHERE id='fix-build';
```

## 4. Progress Tracking (SQL Todos)
Use `todos` with descriptive kebab-case IDs. Update status as you work.

```sql
INSERT INTO todos (id, title, description, status) VALUES
  ('setup-jest', 'Configure Jest', 'Install jest + ts-jest. Create jest.config.ts.', 'pending'),
  ('write-tests', 'Auth tests', 'Cover login, logout, token refresh.', 'pending');
INSERT INTO todo_deps (todo_id, depends_on) VALUES ('write-tests', 'setup-jest');
UPDATE todos SET status = 'in_progress' WHERE id = 'setup-jest';
UPDATE todos SET status = 'done' WHERE id = 'setup-jest';
```

## 5. Verbatim Recording
Record everything the user says — completely, unedited. No chunking, no summarizing, no truncating.

```text
GOOD: Store full quote "Create 11 skill files with YAML frontmatter and practical examples"
BAD:  Store "Create skill files" — lost all critical details
```

## 6. Parallel Execution Patterns
Dispatch 5–10+ agents simultaneously. Hardware supports 100+ concurrent agents.

```text
task(explore): "Find all TypeScript configs"       # all four launch simultaneously
task(explore): "List all test files and frameworks" # results collected as each
task(explore): "Map dependency graph of src/core/"  # finishes — no sequential
task(explore): "Find all API endpoint definitions"  # bottleneck
```

## 7. Fleet Deployment
Launch large agent batches for bulk operations. Track completion, handle partial failures.

```text
for skill in [memory, routing, fleet, recovery, ...]:
  task(general-purpose, background): "Create SKILL.md for {skill}"
# 9 done, 1 in_progress, 1 failed → re-dispatch failed with alt model
```

## 8. 503 Retry Patterns
When rate limits hit: log failure, continue other tasks, retry with backoff (30s → 60s → 120s),
swap models if needed (opus → sonnet → haiku), queue if all models fail.

## 9. Proactive Backup Agents
For critical tasks, launch primary + backup on a faster model. First to finish wins.

```text
task(general-purpose, model="claude-opus-4.6"):   "Generate arch doc"  # primary
task(general-purpose, model="claude-sonnet-4.5"): "Generate arch doc"  # backup
# 2x cost, guaranteed fast delivery. Reserve for critical-path items only.
```

## Anti-Patterns

| Anti-Pattern | Correct Approach |
|---|---|
| Asking permission for every action | Act first, report results |
| Sequential independent tasks | Dispatch agents simultaneously |
| Giving up after first failure | Retry with different approach/model |
| No progress tracking | Use todos table with status updates |
| Summarizing user input | Store complete text verbatim |
| Retrying same failing command | Change approach, escalate, try alt tools |
| Idling on blocked task | Work on other ready tasks in parallel |

## Checklist
- [ ] Autonomous execution verified — no unnecessary permission requests
- [ ] Progress tracked in SQL — all tasks in todos with current status
- [ ] Parallel dispatch used — independent tasks launched simultaneously
- [ ] Error recovery tested — retry-with-different-approach occurred
- [ ] Verbatim recording confirmed — user input stored complete
- [ ] Fleet deployment, 503 handling, and backup agents operational
