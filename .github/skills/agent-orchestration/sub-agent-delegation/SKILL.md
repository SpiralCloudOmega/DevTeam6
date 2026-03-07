---
name: Sub-Agent Delegation
description: Patterns for delegating work to sub-agents via the task tool — prompt engineering, parallel dispatch, error handling, and result validation.
---

# Sub-Agent Delegation

Effective sub-agent orchestration turns a slow, serial workflow into a fast, parallel one. This skill covers when to delegate, how to write prompts that succeed on the first try, and how to validate results.

## When to Delegate vs Do Yourself

**Delegate** complex multi-file changes, codebase research, builds, and tests. **Do yourself** single-line edits, reading a known file, or simple git commands. Rule of thumb: if it needs more than 3 tool calls, delegate it.

| Scenario | Action | Why |
|---|---|---|
| Refactor function in 12 files | `general-purpose` agent | Multi-file, full toolset |
| Read `src/config.ts` line 40 | `view` tool directly | One call, known path |
| Find all usages of `AuthProvider` | `explore` agent | Research needing synthesis |
| Run `npm test` | `task` agent | Verbose output stays out of context |

## Prompt Engineering for Sub-Agents

Sub-agents are stateless — they see nothing from your conversation. Every prompt must be self-contained. Specific always beats vague.

**Bad — vague, no context:**
```
task(agent_type="general-purpose", prompt="Fix the auth bug", description="Fix auth")
```

**Good — specific paths, constraints, acceptance criteria:**
```
task(
  agent_type="general-purpose",
  prompt="In /mnt/c/project/src/auth/token.ts, refreshToken() on line 45 throws an unhandled rejection on expired tokens. Wrap the fetch in try/catch returning null on 401. Do NOT modify TokenStore. Run 'npm test -- --grep auth' after.",
  description="Fix token refresh"
)
```

Include: absolute file paths, what to change, what NOT to change, error messages verbatim, acceptance criteria.

## Context Window Management

Each sub-agent gets a fresh context window. Front-load everything needed, but don't over-stuff. **Include:** repo root, file paths, problem statement, code snippets, error output, constraints. **Exclude:** full conversation history, entire large files (give paths instead), unrelated context.

```
task(
  agent_type="explore",
  prompt="In /mnt/c/Users/autom/VS Code GitHub Repos/DevTeam6, find all files importing '@lib/database' and explain how connection pooling is configured. Check src/lib/database/pool.ts for settings. Return: importing files, pool config, and any files bypassing the pool.",
  description="Trace DB pool usage"
)
```

## Parallel Dispatch Patterns

Independent tasks must be dispatched simultaneously. `explore` agents are always safe to parallelize. Use `mode="background"` to keep working while agents run.

**Parallel research (all three in ONE response):**
```
task(agent_type="explore", prompt="Find all REST endpoints in src/api/ ...", description="Find endpoints")
task(agent_type="explore", prompt="Find all DB models in src/models/ ...", description="Find models")
task(agent_type="explore", prompt="Find all middleware in src/middleware/ ...", description="Find middleware")
```
**Background build:**
```
task(agent_type="task", prompt="Run 'npm run build' and report results.", description="Build", mode="background")
```
**Parallel implementation (ONLY when agents touch different files):**
```
task(agent_type="general-purpose", prompt="Create src/utils/logger.ts ...", description="Create logger")
task(agent_type="general-purpose", prompt="Create src/utils/validator.ts ...", description="Create validator")
```

## Error Handling

Build a retry ladder: refine prompt → escalate model → decompose → do it yourself.

1. **Refine** — add more context, be more specific, break into smaller pieces
2. **Escalate** — use `model="claude-opus-4.6"` for tasks default models can't handle
3. **Decompose** — split one large task into smaller independent delegations
4. **Fallback** — after two failed attempts, use `bash`/`edit`/`view` directly

## Result Validation

Never blindly trust agent output. After every delegation:

1. `git --no-pager diff --stat` — see what files changed
2. `git --no-pager diff src/changed-dir/` — review actual changes
3. Run targeted tests — `npm test -- --grep "relevant-suite"`
4. `view` the most critical changed files to spot-check
5. Verify completeness — did the agent address ALL parts of the prompt?

If validation reveals issues, dispatch a follow-up agent with the diff and test output included in the prompt.

## Recursive Delegation

`general-purpose` agents can spawn their own sub-agents via the `task` tool. Cap nesting at two levels (you → agent → sub-agent). Prefer flat parallelism over deep recursion. If a task seems to need deep nesting, decompose it into sequential top-level steps instead.

## Token Budget Management

Use sub-agents strategically to preserve your context window:

- **Verbose output** (builds, tests) → `task` agents return brief summaries on success, full output on failure
- **Research** → `explore` agents return focused answers under 300 words
- **Large files** → send paths, not contents; let agents read files themselves
- **Iterative edit-test-fix** → delegate the whole cycle to `general-purpose` instead of doing each round yourself

## Anti-Patterns

| Anti-Pattern | Fix |
|---|---|
| Vague prompts like "fix the bug" | Include file paths, errors, constraints |
| Pasting entire files into prompts | Provide paths — let agents read files themselves |
| Sequential dispatch of independent tasks | Parallel tool calls in one response |
| No validation after delegation | Always build, test, and diff-review |
| Unbounded recursive delegation | Cap at two nesting levels |
| Parallel agents modifying same file | Serialize same-file modifications |

## Delegation Checklist

- [ ] Prompt is self-contained with all paths, context, and constraints
- [ ] Independent tasks dispatched in parallel, not sequentially
- [ ] No two parallel agents modify the same file
- [ ] Prompt specifies acceptance criteria or expected outcomes
- [ ] Validation plan ready: build, test, diff after completion
- [ ] Retry strategy defined: refine prompt → escalate model → do it yourself
- [ ] Recursive delegation capped at two nesting levels
- [ ] Verbose outputs routed through `task` agents to preserve context budget
