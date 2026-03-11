---
name: Omega Orchestration
description: >
  Recursive multi-agent orchestration — task decomposition, DAG scheduling,
  fan-out/fan-in dispatch, context partitioning, conflict resolution, external
  terminal bridge, health-aware scheduling, and Kanban integration via Fulcrum.
---

# Omega Orchestration

Core loop: **decompose → build DAG → partition context → dispatch → merge → critique → commit**.

Omega extends the base multi-agent-coordinator skill with external terminal
dispatch, RLM environment injection, Fulcrum Kanban integration, and
health-aware scheduling across 250+ agents and 12+ CLI terminals.

## 1. Task Decomposition Strategies

| Strategy | Split axis | Best when | Example |
|---|---|---|---|
| **Functional** | Module / feature | Clear ownership boundaries | Audio engine vs. UI vs. tests |
| **Layer** | Stack layer | Full-stack changes | CMake → C++ → Python bindings |
| **File-based** | One agent per file | Many independent edits | Bulk refactor across 20 files |
| **Domain** | Expert domain | Specialist knowledge required | DSP agent for biquad, security agent for auth |

Choose the strategy that **maximises independent sub-tasks** — independence = parallelism.

```python
def decompose(task, strategy="domain"):
    if strategy == "functional":
        return split_by_module(task, modules=["audio-engine", "ui", "state", "tests"])
    if strategy == "layer":
        return split_by_layer(task, layers=["cmake", "cpp", "python", "docs"])
    if strategy == "file":
        return [SubTask(file=f) for f in task.changed_files]
    if strategy == "domain":
        return assign_by_expertise(task, agent_registry)
```

## 2. Dependency Graph Management

Model every plan as a DAG in the SQL todo system:

```sql
INSERT INTO todos (id, title, status) VALUES
  ('schema',    'Define SEXP token schema',    'pending'),
  ('macro',     'Write LISP macro expander',   'pending'),
  ('codegen',   'Generate C++ from expanded',  'pending'),
  ('cmake',     'Update CMakeLists.txt',       'pending'),
  ('tests',     'Integration tests',           'pending');

INSERT INTO todo_deps (todo_id, depends_on) VALUES
  ('macro',   'schema'),
  ('codegen', 'macro'),
  ('cmake',   'codegen'),
  ('tests',   'cmake');
```

**Cycle detection** — abort if this returns rows:

```sql
WITH RECURSIVE chain(task, root) AS (
  SELECT todo_id, depends_on FROM todo_deps
  UNION ALL
  SELECT td.todo_id, c.root FROM todo_deps td JOIN chain c ON td.depends_on = c.task
)
SELECT DISTINCT task FROM chain WHERE task = root;
```

**Topological sort** — find the next ready wave:

```sql
SELECT t.* FROM todos t WHERE t.status = 'pending'
  AND NOT EXISTS (
    SELECT 1 FROM todo_deps td JOIN todos dep ON td.depends_on = dep.id
    WHERE td.todo_id = t.id AND dep.status != 'done'
  );
```

## 3. Agent Delegation Patterns

### Fan-Out / Fan-In
Dispatch independent agents in a single response, collect all results:

```python
ready = query_ready_tasks()
results = fan_out(ready)  # parallel task() calls
for task_id, result in results:
    if result.success:
        mark_done(task_id)
    else:
        retry = fan_out_single(task_id, refined_prompt(result.error))
        mark_done(task_id) if retry.success else mark_blocked(task_id, retry.error)
merged = merge(results, strategy="concatenation")
```

### Recursive Delegation
A general-purpose agent may spawn its own sub-agents:

```text
Manager → general-purpose agent → spawns explore + task agents → returns merged result
```

### Conditional Delegation
Route based on task properties:

```python
ROUTING = {
    "*.cpp": "cmake-cpp20-expert",
    "*.py":  "python-modern",
    "*.ts":  "typescript-modern",
    "*.md":  "explore",
    "test_*": "task",
}
def route(file_path):
    for pattern, agent in ROUTING.items():
        if fnmatch(file_path, pattern): return agent
    return "general-purpose"
```

## 4. Result Merging Strategies

| Strategy | Use case | Dedup |
|---|---|---|
| **Concatenation** | Independent file edits | No |
| **Union** | Combine unique findings (lint, review) | Yes |
| **Intersection** | Keep only items confirmed by 2+ agents | Yes |
| **Priority** | Domain expert's result wins on conflict | N/A |

## 5. Conflict Resolution

| Rule | Scope | When |
|---|---|---|
| **Domain expert wins** | DSP code, security code | Expert agent vs. general agent |
| **Last-write-wins** | Non-structural (comments, docs) | Low-risk changes |
| **Quorum vote** | Architecture, dependency changes | High-stakes decisions |
| **Escalate to human** | Structural conflicts | Cannot auto-resolve |

## 6. External Terminal Dispatch via HTTP Bridge

Omega routes work to external CLI terminals through the HTTP bridge:

```python
import requests

BRIDGE = "http://localhost:18800"

def dispatch_terminal(terminal_id, command, timeout=300):
    resp = requests.post(f"{BRIDGE}/dispatch", json={
        "terminal": terminal_id,
        "command": command,
        "timeout": timeout,
    })
    return resp.json()  # {"status": "ok", "output": "...", "exit_code": 0}

# Example: run C++ build in the CMake terminal
result = dispatch_terminal("cmake-build", "cmake --build build/ --parallel 12")
```

Registered terminals: `gh-cli`, `cmake-build`, `python-repl`, `node-repl`,
`ollama`, `notebooklm`, `docker`, `wsl-bash`, `kali`, `gitea`, `cyborgdb`, `ace-step`.

## 7. Context Partitioning

Each agent receives **only** the context it needs — pointers, not payloads:

```python
context = {
    "goal": sub_task.description,
    "files": sub_task.relevant_paths,       # paths only, agent reads them
    "constraints": sub_task.constraints,     # e.g. "preserve ABI"
    "output_format": "unified diff",
    "env": {                                 # RLM pattern: context as env vars
        "OMEGA_TASK_ID": sub_task.id,
        "OMEGA_WAVE": current_wave,
        "OMEGA_STRATEGY": merge_strategy,
    },
}
```

This is the **RLM pattern** — prompts reference external environment, not inline data.
Token savings: ~98% vs. embedding full file contents in every prompt.

## 8. Kanban Integration via Fulcrum

Fulcrum provides the Kanban board that tracks Omega tasks:

```sql
-- Fulcrum columns map to todo statuses
-- backlog → pending | in_progress → in_progress | review → in_progress | done → done

UPDATE todos SET status = 'in_progress' WHERE id = 'codegen';
-- Agent completes work...
UPDATE todos SET status = 'done' WHERE id = 'codegen';

-- Dashboard query: progress by wave
SELECT
  CASE WHEN status = 'done' THEN '✅' WHEN status = 'in_progress' THEN '🔄' ELSE '⏳' END as icon,
  id, title, status
FROM todos ORDER BY created_at;
```

## 9. Health-Aware Scheduling

Before dispatching, check agent/terminal health:

```python
def schedule(ready_tasks):
    health = requests.get(f"{BRIDGE}/health").json()
    for task in ready_tasks:
        agent = route(task)
        if health.get(agent, {}).get("status") != "healthy":
            task.reassign(fallback_agent(agent))
        dispatch(task)
```

Skip unhealthy agents, reassign to fallback. Log all skips for post-mortem.

## Anti-Patterns

| Anti-Pattern | Fix |
|---|---|
| Sequential dispatch of independent tasks | Fan-out in parallel (§3) |
| Full codebase in every agent prompt | Context partitioning with paths only (§7) |
| No dependency tracking between tasks | DAG + SQL todos + cycle detection (§2) |
| Ignoring terminal health before dispatch | Health check + fallback routing (§9) |
| Hardcoded agent routing | Pattern-based routing table (§3) |
| No retry on agent failure | Retry once with refined prompt, then block (§3) |
| Circular task dependencies | CTE cycle detection before first wave (§2) |
| Monolithic task descriptions | Decompose to maximise parallelism (§1) |

## Checklist

- [ ] **Decomposition strategy chosen** — functional / layer / file / domain
- [ ] **DAG validated** — cycle detection CTE returns zero rows
- [ ] **Context partitioned** — each agent gets only relevant paths + constraints
- [ ] **RLM env vars set** — OMEGA_TASK_ID, OMEGA_WAVE, OMEGA_STRATEGY
- [ ] **Merge strategy selected** — concatenation / union / intersection / priority per result type
- [ ] **Conflict resolution rule** — domain-expert / last-write / quorum / escalate
- [ ] **Health check enabled** — bridge /health queried before each wave
- [ ] **Fulcrum Kanban synced** — todo statuses reflect actual progress
- [ ] **Failure handling** — retry policy defined, blocked tasks logged
- [ ] **Terminal dispatch tested** — bridge responds to /dispatch for all registered terminals
