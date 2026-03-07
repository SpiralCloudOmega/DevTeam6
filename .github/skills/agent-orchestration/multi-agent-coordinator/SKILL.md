---
name: Multi-Agent Coordinator
description: >
  Patterns for orchestrating multiple AI agents — task decomposition, dependency
  graphs, context partitioning, conflict resolution, and result merging.
---

# Multi-Agent Coordinator

Core loop: **decompose → assign → schedule → dispatch → merge → verify**.

## 1. Task Decomposition Patterns

| Strategy | Split axis | Best when |
|---|---|---|
| **Functional** | By module / feature | Clear module boundaries |
| **Layer** | Frontend / backend / infra | Full-stack changes |
| **File-based** | One agent per file | Many independent edits |
| **Domain** | Audio engine, UI, tests | Domain expertise matters |

```yaml
tasks:
  - { id: api-schema,   description: "OpenAPI schema for /sessions" }
  - { id: api-handler,  depends_on: [api-schema],  description: "Handler in src/api/" }
  - { id: ui-component, depends_on: [api-schema],  description: "SessionList component" }
  - { id: e2e-tests,    depends_on: [api-handler, ui-component], description: "Playwright tests" }
```

Maximise **independent** sub-tasks — more independence means more parallelism.
## 2. Agent Role Assignment
```python
def assign_agent(sub_task):
    if sub_task.needs_understanding:    return "explore"          # read-only, cheap
    if sub_task.complexity == "high":   return "general-purpose"  # full toolset
    if sub_task.is_build_or_test:       return "task"             # CLI, brief output
    if sub_task.domain in CUSTOM:       return CUSTOM[sub_task.domain]
    return "general-purpose"
```
> Prefer **custom agents** over built-in when both can handle the task.
## 3. Dependency Graph (DAG)

Model dependencies with the SQL todo system:
```sql
INSERT INTO todos (id, title, status) VALUES
  ('api-schema', 'Define API schema', 'pending'),
  ('api-handler', 'Implement handler', 'pending'),
  ('ui-component', 'Build SessionList', 'pending'),
  ('e2e-tests', 'Write E2E tests', 'pending');
INSERT INTO todo_deps (todo_id, depends_on) VALUES
  ('api-handler', 'api-schema'),   ('ui-component', 'api-schema'),
  ('e2e-tests', 'api-handler'),    ('e2e-tests', 'ui-component');
```
Detect cycles with a recursive CTE — non-empty result means abort:
```sql
WITH RECURSIVE chain(task, root) AS (
  SELECT todo_id, depends_on FROM todo_deps
  UNION ALL
  SELECT td.todo_id, c.root FROM todo_deps td JOIN chain c ON td.depends_on = c.task
)
SELECT DISTINCT task FROM chain WHERE task = root;
```
## 4. Topological Sort Execution

Execute in waves — each wave contains tasks whose dependencies are all **done**:
```sql
SELECT t.* FROM todos t WHERE t.status = 'pending'
  AND NOT EXISTS (
    SELECT 1 FROM todo_deps td JOIN todos dep ON td.depends_on = dep.id
    WHERE td.todo_id = t.id AND dep.status != 'done');
```
```python
while has_pending_tasks():
    ready = query_ready_tasks()
    if not ready: raise CircularDependencyError()
    for task_id, result in fan_out(ready):
        mark_done(task_id) if result.success else mark_blocked(task_id)
```
## 5. Parallel Fan-Out / Fan-In

Dispatch independent agents in a **single response**:
```text
task(agent_type="explore",         prompt="Find all REST endpoints in src/api/")
task(agent_type="general-purpose", prompt="Refactor logger.ts to structured JSON")
task(agent_type="task",            prompt="Run npm test -- --coverage")
```
Never silently ignore a failed agent — retry once, then mark blocked:
```python
for f in [r for r in results if not r.success]:
    retry_with_refined_prompt(f)
    if still_failing: mark_blocked(f.task_id, reason=f.error)
```
## 6. Context Partitioning

Each agent gets **only** what it needs — never the full codebase:
```python
context = {
    "goal": sub_task.description,
    "files": sub_task.relevant_file_paths,   # paths, NOT contents
    "constraints": ["preserve API compat"],
    "output_format": "unified diff",
}
```
Provide file **paths** (not contents) and state **output format** and **constraints**.
## 7. Output Merging Strategies

| Strategy | Use case |
|---|---|
| **Concatenation** | Independent file edits — append diffs |
| **Union** | Combine unique items (lint warnings) |
| **Intersection** | Keep only items found by 2+ agents |
| **Priority** | Domain expert's result wins on conflict |

```python
def merge(results, strategy):
    if strategy == "union":       return list(set().union(*(r.items for r in results)))
    if strategy == "intersection":
        return list(set.intersection(*(set(r.items) for r in results)))
```
## 8. Conflict Resolution

1. **Domain expert wins** — audio agent overrides general agent on DSP code.
2. **Last-write-wins** — only for non-structural changes (comments, formatting).
3. **Manual resolution** — structural conflicts flagged for human review.

```python
def resolve_conflict(a, b, path):
    if is_domain_expert(a.agent, path): return a
    if is_domain_expert(b.agent, path): return b
    if is_non_structural(a, b):         return b       # last-write-wins
    raise ConflictError(f"Manual resolution needed: {path}")
```
## 9. Quorum Thresholds

Require N-of-M agreement for high-stakes decisions:
```python
QUORUM = {"architecture": 3, "dependency_add": 2, "style": 1}  # out of 3
def check_quorum(votes, kind):
    return sum(1 for v in votes if v.approve) >= QUORUM[kind]
```
## Anti-Patterns

| Anti-Pattern | Fix |
|---|---|
| Sequential execution of independent tasks | Fan-out in parallel (§5) |
| Giving every agent full codebase context | Partition context per agent (§6) |
| No dependency tracking | DAG + SQL todos (§3) |
| Ignoring agent failures | Retry once, then mark blocked (§5) |
| Circular dependencies | Cycle detection CTE before execution (§3) |
## Checklist

- [ ] **DAG validated** — cycle detection returns zero rows
- [ ] **Parallel dispatch** — independent tasks run simultaneously
- [ ] **Context partitioned** — each agent gets only relevant paths/constraints
- [ ] **Conflict resolution** — strategy chosen (domain-expert / last-write / manual)
- [ ] **Quorum set** — high-stakes decisions require N-of-M agreement
- [ ] **Failure handling** — retry policy and escalation path defined
- [ ] **Merge strategy** — concatenation / union / intersection per result type
- [ ] **Status tracking** — SQL todos updated after each wave
