---
name: Omega Recursive Delegation
description: >
  Sub-agent spawning patterns, context window management using the "gas station"
  strategy, task decomposition for multi-level delegation, result aggregation
  across agent chains, and delegation depth control.
---

# Omega Recursive Delegation

Core principle: **When a task is too big for one agent, delegate down — but never deeper than necessary.**

Recursive delegation is the mechanism that allows the Omega system to handle
tasks of arbitrary complexity. An orchestrator spawns sub-agents, which may
spawn their own sub-agents, forming a tree of delegated work. Each level in the
tree reduces scope and increases specificity. The "gas station" strategy ensures
no agent runs out of context tokens mid-task.

## 1. Sub-Agent Spawning Patterns

### Direct Delegation

The simplest pattern — one agent spawns one sub-agent for a specific task:

```python
# Orchestrator delegates a focused task to a specialist
result = task(
    agent_type="general-purpose",
    prompt="""
    TASK: Implement the biquad filter coefficients for the parametric EQ block
    FILES: src/dsp/biquad.h (lines 1-50), src/dsp/biquad.cpp (lines 1-120)
    CRITERIA:
      1. Correct Butterworth/Chebyshev coefficient calculation
      2. All 6 filter types: lowpass, highpass, bandpass, notch, allpass, peaking
      3. No denormal numbers in output
    """,
)
```

### Fan-Out Delegation

Multiple sub-agents for independent tasks:

```python
# Launch all independent tasks simultaneously
results = parallel([
    task("explore", "Find all files using the old biquad API"),
    task("explore", "Find the test suite for DSP filters"),
    task("general-purpose", "Update biquad.h with new coefficient formulas"),
    task("general-purpose", "Write unit tests for all 6 filter types"),
])
```

### Recursive Delegation

A sub-agent realizes its task is too complex and delegates further:

```text
Orchestrator (Level 0)
  └── "Implement parametric EQ" (general-purpose, Level 1)
        ├── "Research biquad math" (explore, Level 2)
        ├── "Implement coefficients" (general-purpose, Level 2)
        │     ├── "Lowpass/highpass" (task, Level 3)  ← MAX DEPTH
        │     └── "Bandpass/notch/allpass/peaking" (task, Level 3)
        └── "Write tests" (task, Level 2)
```

### Conditional Delegation

Route to different agent types based on task properties:

```python
def delegate(task):
    if task.is_research:
        return task_agent("explore", task.prompt)
    elif task.is_build:
        return task_agent("task", task.prompt)
    elif task.requires_reasoning:
        return task_agent("general-purpose", task.prompt)
    elif task.is_review:
        return task_agent("code-review", task.prompt)
    else:
        return task_agent("general-purpose", task.prompt)
```

## 2. Context Window Management — "Gas Station" Strategy

### The Problem

Each agent has a finite context window (${OMEGA_CONTEXT_BUDGET:-200000} tokens).
As delegation deepens, each level consumes tokens for:
- The delegation prompt (explaining the task)
- The sub-agent's system prompt
- Tool call results (file reads, searches)
- The sub-agent's reasoning and output

If a sub-agent exhausts its context mid-task, work is lost.

### The Gas Station Strategy

Treat context like fuel. Before starting a long journey (complex task), stop
at a "gas station" to fill up:

```text
┌─────────────────────────────────────────────────────────┐
│ Agent Context Window (200K tokens)                       │
│                                                          │
│ ██████░░░░░░░░░░░░░░  System + Task prompt (30K)       │
│ ████████████░░░░░░░░  Tool results loaded (60K)         │
│ ████████████████░░░░  Reasoning + planning (80K)        │
│ ░░░░░░░░░░░░░░░░░░░░  RESERVE for output (30K)         │
│                                                          │
│ ⛽ Gas Station Check at 70% utilization:                 │
│   - Am I on track to finish within budget?               │
│   - Should I delegate remaining work to a fresh agent?   │
│   - Can I compress context by summarizing prior work?    │
└─────────────────────────────────────────────────────────┘
```

### Implementation

```python
def gas_station_check(context_used, budget, task_remaining):
    utilization = context_used / budget

    if utilization < 0.70:
        return "continue"  # plenty of fuel

    if utilization < 0.85 and task_remaining.is_small:
        return "continue"  # can finish on current fuel

    if utilization >= 0.85:
        # Delegate remaining work to a fresh agent with full context window
        return "delegate_remainder"

    # Compress: summarize what we've done so far, shed tool results
    return "compress_and_continue"
```

### Context Compression Techniques

| Technique | Savings | When |
|---|---|---|
| **Summarize prior results** | ~60% | After explore phase, before implementation |
| **Drop tool call details** | ~40% | Keep conclusions, shed raw search results |
| **Delegate to fresh agent** | 100% | Start new agent with only essential context |
| **Narrow file scope** | ~80% | Re-read only the specific lines needed |

## 3. Task Decomposition for Delegation

### Decomposition Criteria

A task should be delegated when:
1. **Scope exceeds single-agent capacity**: >50K tokens of relevant files
2. **Multiple domains involved**: DSP + UI + tests (different expertise needed)
3. **Independent sub-components exist**: Can be parallelized
4. **Time budget allows**: Delegation adds overhead (~10s per agent spawn)

### Decomposition Templates

#### By Function
```python
sub_tasks = [
    {"agent": "audio-dsp-expert", "scope": "DSP implementation"},
    {"agent": "juce-framework-expert", "scope": "Plugin wrapper"},
    {"agent": "task", "scope": "Build and test"},
]
```

#### By Layer
```python
sub_tasks = [
    {"agent": "cmake-cpp20-expert", "scope": "Build system"},
    {"agent": "general-purpose", "scope": "Core C++ implementation"},
    {"agent": "general-purpose", "scope": "Python bindings"},
    {"agent": "explore", "scope": "Documentation"},
]
```

#### By File
```python
# When many files need similar changes (bulk refactor)
sub_tasks = [
    {"agent": "general-purpose", "scope": f"Update {f}"}
    for f in changed_files
]
```

### Acceptance Criteria Propagation

Every delegated task must include explicit acceptance criteria that the parent
can verify:

```python
delegation = {
    "task": "Implement lowpass biquad filter",
    "files": ["src/dsp/biquad.cpp"],
    "criteria": [
        "Correct Butterworth coefficient calculation",
        "Unit test passes with <0.01dB error at reference frequencies",
        "No denormal numbers in output (test with very small input)",
    ],
    "output": "Modified biquad.cpp + test file",
}
```

## 4. Result Aggregation Across Agent Chains

### Collection Pattern

```python
def aggregate_delegation_results(sub_results):
    merged = {"files_changed": [], "tests_added": [], "issues": []}

    for result in sub_results:
        if result.success:
            merged["files_changed"].extend(result.files_changed)
            merged["tests_added"].extend(result.tests_added)
        else:
            merged["issues"].append({
                "task": result.task_id,
                "error": result.error,
                "agent": result.agent_type,
            })

    # Dedup files (multiple agents may touch the same file)
    merged["files_changed"] = list(set(merged["files_changed"]))

    return merged
```

### Conflict Detection

When multiple sub-agents modify the same file:

```python
def detect_conflicts(sub_results):
    file_edits = defaultdict(list)
    for result in sub_results:
        for file_path in result.files_changed:
            file_edits[file_path].append(result.agent_id)

    conflicts = {f: agents for f, agents in file_edits.items() if len(agents) > 1}
    if conflicts:
        # Route to orchestrator for conflict resolution
        escalate("File conflicts detected", conflicts)
    return conflicts
```

### Result Validation Chain

```text
Sub-agent output
  → Parent validates against acceptance criteria
    → Pass: integrate into parent's result
    → Fail: retry with feedback OR escalate to orchestrator
      → All retries exhausted: block task, alert orchestrator
```

## 5. Delegation Depth Control

### Maximum Depth

| Delegation Level | Allowed | Typical Use |
|---|---|---|
| Level 0 | Always | Orchestrator → specialist agents |
| Level 1 | Always | Specialist → explore/task agents |
| Level 2 | When needed | Complex sub-tasks needing further decomposition |
| Level 3 | Maximum | Only for very large, multi-domain tasks |

### Depth Enforcement

```python
OMEGA_MAX_DELEGATION_DEPTH = 3

def delegate(task, current_depth=0):
    if current_depth >= OMEGA_MAX_DELEGATION_DEPTH:
        # At max depth — must execute directly, no further delegation
        return execute_directly(task)

    if should_delegate(task):
        sub_tasks = decompose(task)
        return fan_out([
            delegate(st, current_depth + 1)
            for st in sub_tasks
        ])

    return execute_directly(task)
```

### Depth Metadata

Every delegation includes depth metadata for tracking:

```python
delegation_prompt = f"""
OMEGA_DELEGATION_DEPTH: {current_depth + 1}
OMEGA_MAX_DEPTH: {OMEGA_MAX_DELEGATION_DEPTH}
OMEGA_PARENT_TASK: {parent_task_id}
OMEGA_ROOT_TASK: {root_task_id}

You are at delegation level {current_depth + 1} of {OMEGA_MAX_DELEGATION_DEPTH}.
{"You MAY delegate further." if current_depth + 1 < OMEGA_MAX_DELEGATION_DEPTH else "You MUST NOT delegate further — execute directly."}

TASK: {task.description}
"""
```

## Anti-Patterns

| Anti-Pattern | Fix |
|---|---|
| Delegating without acceptance criteria | Every delegation includes verifiable criteria (§3) |
| Unlimited delegation depth | Max depth = 3 with enforcement (§5) |
| No context budget check before delegating | Gas station check at 70% utilization (§2) |
| Agent exhausts context mid-task | Delegate remainder to fresh agent (§2) |
| Multiple agents editing same file without detection | Conflict detection before merge (§4) |
| Sequential delegation of independent tasks | Fan-out parallel delegation (§1) |
| No depth metadata in delegation prompts | Include OMEGA_DELEGATION_DEPTH in every prompt (§5) |
| Retrying at same level indefinitely | Escalate after 2 retries — different agent or human |

## Checklist

- [ ] **Decomposition criteria met** — task genuinely needs delegation (scope, domain, parallelism)
- [ ] **Acceptance criteria propagated** — every sub-task has verifiable criteria
- [ ] **Delegation depth tracked** — OMEGA_DELEGATION_DEPTH in every prompt
- [ ] **Max depth enforced** — level 3 agents execute directly, never delegate
- [ ] **Gas station check active** — context utilization monitored at 70% and 85%
- [ ] **Context compression ready** — summarize/delegate/narrow strategies available
- [ ] **Conflict detection enabled** — multi-agent file edits detected before merge
- [ ] **Result validation chain** — parent validates every sub-agent result
- [ ] **Parallel where possible** — independent sub-tasks fan-out simultaneously
- [ ] **Escalation path defined** — retry → different agent → orchestrator → human
