---
name: Omega Fleet Management
description: >
  Parallel agent fleet operations — dispatch patterns, load balancing, context
  partitioning, sub-agent lifecycle management, wave execution, recursive
  delegation, fleet metrics, and graceful degradation under resource pressure.
---

# Omega Fleet Management

Core principle: **Maximize parallelism, minimize wall-clock time.**

Fleet management is the execution layer of the Omega system. While orchestration
handles planning and decomposition, fleet management handles the mechanics of
running 1–100 agents simultaneously, tracking their lifecycle, balancing load,
and recovering from failures without losing progress.

## 1. Parallel Dispatch Patterns

### Fan-Out: Launch All Independent Tasks Simultaneously

```python
# Identify independent tasks (no unmet dependencies)
ready_tasks = query_ready_todos()  # SQL: no pending deps

# Dispatch all in one turn — Copilot CLI supports parallel task() calls
results = []
for task in ready_tasks:
    agent = select_agent(task)  # route by file type, domain, or capability
    results.append(dispatch(agent, task.prompt, task.context))

# All agents run concurrently — collect as they complete
```

### Fan-In: Collect and Validate Results

```python
for task_id, result in collect_results(results):
    if result.success and validates(result, task.criteria):
        mark_done(task_id)
    elif result.success:
        # Output doesn't meet criteria — retry with feedback
        retry(task_id, feedback=f"Output failed criterion: {explain}")
    else:
        # Agent error — reassign to different agent type
        reassign(task_id, fallback_agent(task))
```

### Wave Execution: Dependency-Ordered Batches

```sql
-- Wave N: all tasks with satisfied dependencies
SELECT t.* FROM todos t WHERE t.status = 'pending'
  AND NOT EXISTS (
    SELECT 1 FROM todo_deps td JOIN todos dep ON td.depends_on = dep.id
    WHERE td.todo_id = t.id AND dep.status != 'done'
  );

-- Execute wave → mark done → compute next wave → repeat
```

## 2. Load Balancing

### Agent Selection Strategy

| Factor | Weight | Measurement |
|---|---|---|
| **Capability match** | 40% | Does the agent have skills for this task type? |
| **Current load** | 25% | How many active tasks is this agent type running? |
| **Historical latency** | 20% | Average completion time for similar tasks |
| **Error rate** | 15% | Recent failure rate for this agent type |

```python
def select_agent(task):
    candidates = agents_with_capability(task.required_skills)
    if not candidates:
        return "general-purpose"  # fallback

    scored = []
    for agent in candidates:
        score = (
            0.40 * capability_score(agent, task) +
            0.25 * (1.0 - load_ratio(agent)) +
            0.20 * (1.0 - latency_ratio(agent)) +
            0.15 * (1.0 - error_ratio(agent))
        )
        scored.append((score, agent))

    return max(scored, key=lambda x: x[0])[1]
```

### Overflow Handling

When all agents of a type are busy:
1. **Queue**: Hold the task until an agent is free (default for non-critical)
2. **Upgrade**: Use a more capable agent type (e.g., general-purpose instead of explore)
3. **External dispatch**: Route to an external terminal via HTTP bridge
4. **Defer**: Move to next wave and batch with future work

## 3. Context Partitioning

### Token Budget Estimation

```python
def estimate_tokens(task):
    file_tokens = sum(count_tokens(f) for f in task.files)
    prompt_tokens = count_tokens(task.prompt)
    overhead = 2000  # system prompt + formatting
    return file_tokens + prompt_tokens + overhead

def can_dispatch(task, budget=200000):
    return estimate_tokens(task) <= budget
```

### Context Reduction Strategies

| Strategy | Savings | When to Use |
|---|---|---|
| **File path references** | ~98% | Always — default RLM pattern |
| **Line range restriction** | ~80% | When change is localized |
| **Summary instead of content** | ~90% | For background context files |
| **Split task** | ~50% per sub-task | When single task exceeds budget |

```python
def partition_context(task, budget):
    if estimate_tokens(task) <= budget:
        return [task]  # fits in one agent

    # Split by file — each sub-task gets a subset of files
    file_groups = chunk_files(task.files, max_tokens=budget - 5000)
    return [
        SubTask(
            files=group,
            prompt=task.prompt,
            criteria=task.criteria,
        )
        for group in file_groups
    ]
```

## 4. Sub-Agent Lifecycle

### States

```
SPAWNED → RUNNING → COMPLETED
                  → FAILED → RETRYING → COMPLETED | DEAD
                  → TIMEOUT → RETRYING → COMPLETED | DEAD
```

### Lifecycle Tracking

```sql
CREATE TABLE IF NOT EXISTS fleet_agents (
  id TEXT PRIMARY KEY,
  task_id TEXT NOT NULL,
  agent_type TEXT NOT NULL,
  status TEXT DEFAULT 'spawned',
  spawned_at TEXT DEFAULT (datetime('now')),
  completed_at TEXT,
  duration_ms INTEGER,
  retry_count INTEGER DEFAULT 0,
  error TEXT
);

-- Find stuck agents (running > 5 minutes)
SELECT * FROM fleet_agents
WHERE status = 'running'
AND julianday('now') - julianday(spawned_at) > 5.0/1440.0;
```

### Timeout and Retry Policy

| Agent Type | Default Timeout | Max Retries | Retry Strategy |
|---|---|---|---|
| explore | 60s | 1 | Refine prompt |
| task | 120s | 2 | Same prompt, different model |
| general-purpose | 300s | 2 | Decompose into smaller tasks |
| External terminal | 600s | 1 | Fall back to different terminal |

## 5. Recursive Delegation

When a task is too complex for a single fleet wave:

```text
Fleet Commander (level 0)
  ├── Wave 1: Research tasks (3 explore agents)
  ├── Wave 2: Implementation tasks (5 general-purpose agents)
  │     └── Sub-Fleet Commander (level 1)
  │           ├── Wave 2a: Core module (2 agents)
  │           └── Wave 2b: Tests (2 agents)
  └── Wave 3: Integration (1 general-purpose agent)
```

Rules for recursive delegation:
- Maximum delegation depth: 3 levels
- Each level reduces the fleet size by 50% (100 → 50 → 25)
- Parent commander owns the timeout for the entire sub-tree
- Results bubble up through the hierarchy — child reports to parent

## Anti-Patterns

| Anti-Pattern | Fix |
|---|---|
| Sequential dispatch of independent tasks | Fan-out all ready tasks in parallel (§1) |
| One agent type for all tasks | Load-balanced routing by capability (§2) |
| Full file content in every prompt | Context partitioning with paths only (§3) |
| Fire-and-forget agent spawning | Lifecycle tracking with timeout detection (§4) |
| Infinite retry loops | Max retries + escalation to orchestrator |
| Monolithic tasks exceeding context | Split into sub-tasks under budget (§3) |
| Ignoring agent health before dispatch | Pre-flight health check every wave |
| No metrics collection | Track timing, success rate, and retries in SQL |

## Checklist

- [ ] **Ready tasks identified** — SQL query returns tasks with all deps satisfied
- [ ] **Agent selection scored** — capability, load, latency, error rate weighted
- [ ] **Context budgeted** — every task under ${OMEGA_CONTEXT_BUDGET:-200000} tokens
- [ ] **RLM pattern applied** — file paths, not inline content
- [ ] **Fleet agents tracked** — SQL table with status, timing, retries
- [ ] **Timeouts set** — per agent type with max retries defined
- [ ] **Health checked** — bridge /health queried before wave dispatch
- [ ] **Results validated** — output checked against acceptance criteria
- [ ] **Metrics stored** — wave timing, success rate, fleet utilization
- [ ] **Failure handled** — retry → reassign → block → escalate
