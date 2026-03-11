---
name: Omega Context Efficiency
description: >
  Token-efficient agent operation patterns — RLM environment variable pointers,
  LISP macro generation for code scaffolding, MCP2CLI protocol bridging,
  deferred tool loading, Context Mode integration, and context budget management.
---

# Omega Context Efficiency

Core principle: **Pass pointers, not payloads. The environment carries the knowledge.**

Context efficiency is the foundational optimization of the Omega system. With a
200,000-token context window, every token must earn its place. The RLM pattern
(arXiv:2512.24601) treats the LM's context as a mutable environment in a REPL —
variables point to external state rather than embedding it. This yields ~98%
token savings on file-heavy tasks.

## 1. RLM Environment Variable Pattern

### Core Concept

Instead of embedding file contents in prompts, set environment variables that
point to file paths. The agent reads files on-demand using tools (view, grep).

```bash
# BAD: 50,000 tokens of inline content
prompt = f"Here is the full source code:\n{read_file('src/engine.cpp')}\nNow fix the bug..."

# GOOD: 50 tokens of pointers
export OMEGA_TARGET_FILE="src/engine.cpp"
export OMEGA_TARGET_LINES="142-160"
export OMEGA_BUG_DESCRIPTION="Buffer overflow in processBlock"
prompt = "Fix the bug described in $OMEGA_BUG_DESCRIPTION at $OMEGA_TARGET_FILE:$OMEGA_TARGET_LINES"
```

### Environment Variable Hierarchy

```text
Level 0: System topology     — OMEGA_ROOT, OMEGA_HOME, OMEGA_CONFIG
Level 1: Infrastructure      — OMEGA_BRIDGE_PORT, OMEGA_MEMORY_DB, OMEGA_CLI_*
Level 2: Per-task context     — OMEGA_TASK_ID, OMEGA_WAVE, OMEGA_TARGET_FILE
Level 3: Per-agent workspace  — OMEGA_WS_ORCHESTRATOR, OMEGA_WS_BUILDER
```

Each level adds specificity without repeating higher-level context.

### omega-env.sh as Bootstrap

The `omega-env.sh` file exports the full variable hierarchy. Sourcing it once
gives every script, agent, and tool access to the complete topology:

```bash
source .github/omega/omega-env.sh
# Now $OMEGA_ROOT, $OMEGA_HOME, $OMEGA_BRIDGE_PORT, etc. are all available
```

## 2. LISP Macro Generation

### S-Expression Tokens → Code

The REVITHION token system uses S-expressions (LISP-like syntax) to define
project structure declaratively. The `generate-omega.py` parser expands these
into C++, CMake, Python, and configuration files:

```lisp
;; Define an audio processor
(defblock "compressor"
  :category "dynamics"
  :parameters (
    (param "threshold" :type float :range (-60.0 0.0) :default -20.0)
    (param "ratio" :type float :range (1.0 20.0) :default 4.0)
    (param "attack" :type float :range (0.1 100.0) :default 10.0 :unit "ms")
  )
  :io (stereo-in stereo-out sidechain-in))
```

This token generates:
- C++ class skeleton with parameter declarations
- CMake target registration
- Plugin format wrappers (VST3/AU/CLAP)
- Test harness skeleton
- Documentation stub

### Macro Expansion Pipeline

```text
.sexp token → LISP macro expansion → intermediate AST → target code generator → output files
```

The macros at `.github/omega/macros/agent-macros.sexp` define 10 expansion
rules that transform high-level declarations into concrete implementations.

### Token Budget Impact

One S-expression token (~50 tokens) generates ~500 lines of code across
multiple files. The token-to-code expansion ratio is approximately **1:200**,
meaning macro generation is the most context-efficient code production method.

## 3. MCP2CLI Protocol Bridging

### What It Solves

MCP (Model Context Protocol) tools expect structured JSON-RPC. CLI tools
expect command-line arguments. MCP2CLI bridges the gap:

```text
MCP Tool Call → JSON-RPC → mcp2cli translator → CLI command → subprocess → stdout → JSON-RPC response
```

### Context Efficiency Benefit

Instead of loading every tool into the context window, MCP2CLI allows deferred
tool discovery. Tools are registered by name and schema, but their implementation
lives externally:

```python
# Tool registration (minimal context cost — ~100 tokens per tool)
tools = [
    {"name": "omega_dispatch", "schema": {"terminal": "str", "command": "str"}},
    {"name": "omega_health", "schema": {}},
]

# Tool execution (zero context cost — happens externally)
result = mcp2cli.call("omega_dispatch", terminal="gemini", command="Research X")
```

### Integration Pattern

```bash
# MCP2CLI config points to omega tools
export MCP2CLI_TOOLS_PATH=".github/omega/bridge"
export MCP2CLI_BRIDGE_URL="http://localhost:${OMEGA_BRIDGE_PORT:-18800}"
```

## 4. Deferred Tool Loading

### Problem

Loading all 236 agents and 41 skills into context wastes tokens. Most tasks
need only 2–5 agents and 1–3 skills.

### Solution: Load on Demand

```python
# Level 1: Load only the agent registry (names + descriptions, ~2000 tokens)
registry = load_agent_registry()  # from .github/agents/ directory listing

# Level 2: Load specific agent when needed (~500 tokens each)
agent = load_agent("omega-builder")  # reads the .agent.md file

# Level 3: Load skill when agent needs it (~800 tokens each)
skill = load_skill("omega-fleet-management")  # reads SKILL.md

# Total: ~3300 tokens instead of ~120,000 for all agents+skills
```

### Implementation in Copilot CLI

```python
# Use explore agent to find the right agent (costs one tool call, not context)
result = task(agent_type="explore", prompt="Which agent handles CMake builds?")
# Returns: "omega-builder" → now load only that agent's definition

# Use glob to find skill files (minimal context cost)
skills = glob("**omega-fleet**/SKILL.md")
# Returns path → read only if needed
```

## 5. Context Mode Integration

### Context Mode from context-mode-main

Context Mode (available at `${OMEGA_CONTEXT_MODE}`) provides a protocol for
passing file references instead of file contents between agents:

```python
# Context Mode reference (minimal tokens)
context_ref = {
    "type": "file_reference",
    "path": "src/audio/engine.cpp",
    "lines": [140, 160],
    "hash": "a1b2c3d4",  # integrity check
}

# Agent receives the reference, reads the file with view tool
# Total context cost: ~30 tokens instead of ~5000 tokens
```

### Integration with Omega

```bash
export OMEGA_CONTEXT_MODE="${OMEGA_CONTEXT_MODE}"  # path to context-mode-main
export OMEGA_CONTEXT_REDUCTION_TARGET=0.02         # target 98% reduction
```

Agents are instructed to:
1. Receive file references, not inline content
2. Use view/grep to read only the lines they need
3. Return diffs, not full file contents
4. Reference env vars for paths instead of hardcoding

## 6. Context Budget Management

### Budget Allocation

Total budget: ${OMEGA_CONTEXT_BUDGET:-200000} tokens per agent instance.

| Component | Budget | Purpose |
|---|---|---|
| System prompt | 2,000 | Agent role, skills, tools |
| Task prompt | 3,000 | Goal, criteria, constraints |
| File references | 500 | Paths and line ranges |
| Tool results | 50,000 | File reads, search results |
| Agent reasoning | 80,000 | Thinking, planning, deciding |
| Output | 30,000 | Code, reports, summaries |
| Reserve | 34,500 | Safety margin |

### Budget Monitoring

```python
def check_budget(agent_context):
    used = count_tokens(agent_context)
    budget = int(os.environ.get("OMEGA_CONTEXT_BUDGET", 200000))
    remaining = budget - used
    utilization = used / budget

    if utilization > 0.85:
        log_warning(f"Context at {utilization:.0%} — consider splitting task")
    if utilization > 0.95:
        log_error(f"Context at {utilization:.0%} — task must be split")

    return remaining
```

## Anti-Patterns

| Anti-Pattern | Fix |
|---|---|
| Embedding full file contents in prompts | Use file path references (§1) |
| Loading all agents/skills upfront | Deferred loading on demand (§4) |
| Hardcoded paths in prompts | Environment variables via omega-env.sh (§1) |
| Generating code manually line-by-line | LISP macro expansion for scaffolding (§2) |
| Every tool loaded into context at start | MCP2CLI deferred tool loading (§3) |
| No budget tracking | Monitor utilization, split at 85% (§6) |
| Full file diffs in agent output | Return minimal unified diffs |
| Repeating context between agent calls | Agents are stateless — context is env vars (§1) |

## Checklist

- [ ] **omega-env.sh sourced** — all OMEGA_* variables available in environment
- [ ] **RLM pattern applied** — file references, not inline content in all prompts
- [ ] **LISP macros used** — code scaffolding via S-expression tokens
- [ ] **MCP2CLI configured** — external tools accessible without context cost
- [ ] **Deferred loading active** — agents/skills loaded only when needed
- [ ] **Context Mode integrated** — file_reference protocol for inter-agent context
- [ ] **Budget monitored** — utilization tracked, alerts at 85% and 95%
- [ ] **Token savings measured** — actual vs. baseline for each task type
- [ ] **Env var hierarchy correct** — Level 0–3 variables set appropriately
- [ ] **Generator tested** — `generate-omega.py --all` produces valid output
