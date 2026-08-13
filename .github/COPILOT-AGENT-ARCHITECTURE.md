# 🤖 GitHub Copilot Agent Architecture Cheat Sheet
## The Complete Reference — Coding Agent vs. Copilot Chat

> **This document is about the agents themselves — not the repo.**
> Use this to write prompts that actually work, understand what the agent can and cannot do,
> and know exactly where your tokens/characters are spent.

---

## 1 — Two Completely Different Products

Most people use "Copilot" as one thing. It isn't. These are two separate systems:

| | **Copilot Chat** | **Copilot Coding Agent** |
|---|---|---|
| **Where it runs** | In your IDE / browser tab | GitHub-hosted Ubuntu container |
| **Trigger** | You type in chat | Assign `@copilot` to an issue or PR |
| **Lifespan** | One conversation turn | Long-running autonomous task (minutes to hours) |
| **Memory** | Conversation history only | Persistent memory DB + repo memories |
| **File access** | Read-only via indexing | Full read/write to repo on disk |
| **Shell access** | No | Yes — real bash, real OS |
| **Internet access** | No (VS Code) / Limited | Yes — web search, web fetch (some domains blocked) |
| **Can commit / push** | No | Yes — via `report_progress` tool |
| **Can run tests/builds** | No | Yes — full build toolchain available |
| **Custom agents** | Limited (extensions) | `.github/agents/*.agent.md` — fully customizable |
| **Sub-agents** | No | Yes — spawns `explore`, `task`, `general-purpose`, custom |
| **Context loaded at start** | Conversation + indexed files | Issue body + system prompt + repo memories + instructions |
| **Model** | GPT-4o / Claude 3.5 Sonnet (varies) | Claude 3.7 Sonnet (Coding Agent uses Sonnet by default) |
| **MCP servers** | VS Code 1.99+ Agent mode only | Yes — GitHub MCP + any configured MCP server |

---

## 2 — Coding Agent: How It Actually Starts

When you assign `@copilot` to an issue, here's what gets injected into the agent's context **before it writes a single line**:

```
┌─────────────────────────────────────────────────────────────────────┐
│  CONTEXT SLOT            │  SOURCE                  │  SIZE LIMIT   │
├─────────────────────────────────────────────────────────────────────┤
│  System instructions     │  .github/copilot-        │  ~8K chars    │
│  (highest priority)      │  instructions.md         │  hard cut     │
├─────────────────────────────────────────────────────────────────────┤
│  Custom agent persona    │  .github/agents/         │  ~4K chars    │
│  (if invoked)            │  <name>.agent.md         │  per file     │
├─────────────────────────────────────────────────────────────────────┤
│  Repository memories     │  store_memory() calls    │  ~50 entries  │
│  (injected facts)        │  from prior sessions     │  rotated FIFO │
├─────────────────────────────────────────────────────────────────────┤
│  Problem statement       │  GitHub issue body       │  ~32K chars   │
│  (your prompt)           │                          │  effective    │
├─────────────────────────────────────────────────────────────────────┤
│  Repo clone on disk      │  Full git checkout       │  Unlimited    │
│  (tools read it live)    │                          │  via tools    │
└─────────────────────────────────────────────────────────────────────┘
```

**Critical:** The model does NOT read the entire repo into context. It uses **tools** to read individual files on demand. The repo on disk is unlimited; what's in the LLM context window is the list above.

---

## 3 — Context Window Budget (Coding Agent)

```
Total window:  ~128K tokens  ≈  500,000 characters
```

| What eats tokens | Cost | Notes |
|---|---|---|
| System prompt slot | 8K chars max | `.github/copilot-instructions.md` — silently cut at hard limit |
| Issue body | Up to 32K chars effective | First 4K chars get highest attention |
| Repo memory entries | ~200 chars each × 50 = ~10K chars | Auto-injected, oldest rotated out |
| Conversation history | Grows with each turn | Older turns compressed/dropped when full |
| Tool call results | Varies | Each `bash`/`view`/`grep` result costs tokens |
| Sub-agent responses | Returned as text | Counts against parent window |

**Practical budget for your issue body:**

```
Characters 1–2,000    → HIGH ATTENTION  ← Put your TASK and CONSTRAINTS here
Characters 2,000–8,000 → GOOD attention  ← Context, file paths, examples
Characters 8,000–32,000 → Declining attention, risk of partial read
Characters > 32,000   → May be chunked or summarized
```

---

## 4 — Tools the Coding Agent Has

This is the full toolbox. The agent picks tools autonomously based on need.

### 📁 File System
| Tool | What it does |
|------|-------------|
| `view` | Read file contents with line numbers; list directories |
| `create` | Create a new file (fails if already exists) |
| `edit` | Replace exact string in file (surgical, one location at a time) |
| `glob` | Find files by pattern (e.g., `**/*.cpp`) |
| `grep` | Search file contents by regex (ripgrep-based) |

### 💻 Shell
| Tool | What it does |
|------|-------------|
| `bash` (sync) | Run a command, wait for output (up to configurable timeout) |
| `bash` (async) | Start background process, get shell ID |
| `read_bash` | Read output from async shell |
| `write_bash` | Send input to interactive async shell |
| `stop_bash` | Kill async shell |
| `list_bash` | List all active shells |

Shell is a **real Ubuntu bash shell** with full toolchain: `git`, `npm`, `pip`, `cargo`, `cmake`, `python`, `node`, `go`, etc.

### 🌐 Internet
| Tool | What it does |
|------|-------------|
| `web_search` | AI-powered web search with citations |
| `web_fetch` | Fetch a specific URL → markdown or HTML |

Some domains are blocked. If a domain is blocked, it fails visibly and you're notified.

### 🐙 GitHub (via MCP)
| Tool | What it does |
|------|-------------|
| `github-mcp-server-get_file_contents` | Read file from any ref/branch |
| `github-mcp-server-list_issues` / `issue_read` | Read issues and comments |
| `github-mcp-server-list_pull_requests` / `pull_request_read` | Read PRs, diffs, reviews |
| `github-mcp-server-search_code` | Search code across GitHub |
| `github-mcp-server-search_repositories` | Find repos |
| `github-mcp-server-list_commits` / `get_commit` | Inspect commit history |
| `github-mcp-server-actions_list` / `actions_get` | Inspect CI workflow runs |
| `github-mcp-server-get_job_logs` | Get CI log output |
| `github-mcp-server-list_releases` / `get_tag` | Release / tag info |

### 🤖 Agent Tools
| Tool | What it does |
|------|-------------|
| `store_memory` | Persist a fact to the repo memory DB (survives across sessions) |
| `search_code_subagent` | Semantic code search using natural language |
| `task` | Spawn a sub-agent (see Section 5) |
| `report_progress` | `git add . && git commit && git push` — the ONLY way to push |
| `code_review` | Automated code review of current changes |
| `codeql_checker` | Security vulnerability scan |
| `gh-advisory-database` | Check dependencies for known CVEs |

### 🌐 Browser (Playwright)
| Tool | What it does |
|------|-------------|
| `playwright-browser_navigate` | Go to URL in headless browser |
| `playwright-browser_snapshot` | Accessibility snapshot (better than screenshot for actions) |
| `playwright-browser_take_screenshot` | Visual screenshot |
| `playwright-browser_click` / `type` / `fill_form` | Interact with pages |

---

## 5 — Sub-Agent System

The Coding Agent can spawn sub-agents via the `task` tool. Each runs in its own context window.

### Built-in Sub-Agent Types
| Agent | Model | Use For |
|-------|-------|---------|
| `explore` | Claude 3 Haiku (fast) | Codebase research, multi-file questions, answering "how does X work?" |
| `task` | Claude 3 Haiku (fast) | Running builds, tests, lints — returns pass/fail + errors |
| `general-purpose` | Claude 3.7 Sonnet | Complex multi-step coding, reasoning, large rewrites |

### Custom Sub-Agents (this repo)
Defined in `.github/agents/<name>.agent.md`. The parent agent calls them by name in the `task` tool.
Examples: `audio-dsp-expert`, `amp-modeling-expert`, `juce-framework-expert`, etc.

### Sub-Agent Rules
- Sub-agents are **stateless** — they get no prior context unless you pass it in the prompt
- Sub-agents can spawn more sub-agents (recursive)
- `explore` agents can run in **parallel** — call multiple at once
- Long sub-agents (>25 min) will 503 — break them into <10 min tasks

---

## 6 — The Agent's Decision Loop

```
START
  ↓
Read: copilot-instructions.md + memories + issue body
  ↓
Plan: Build internal checklist of steps
  ↓
report_progress → commit initial plan (creates first commit)
  ↓
Loop:
  ├─ Use tools to explore / understand code
  ├─ Make changes (create / edit files)
  ├─ Run tests / builds (bash)
  ├─ Use code_review + codeql_checker before finishing
  ├─ report_progress → commit + push after each meaningful chunk
  └─ Repeat until all checklist items done
  ↓
Final: code_review → codeql_checker → report_progress
  ↓
DONE (yields back to user)
```

The agent **does not ask for permission** mid-task. It will make decisions autonomously. If you want it to stop and ask, say so explicitly in the issue body.

---

## 7 — What the Coding Agent CANNOT Do

| Cannot | Why / Workaround |
|--------|-----------------|
| `git push` directly | Must use `report_progress` tool (security) |
| Open new PRs | No tool for it — creates commits on the assigned branch only |
| Clone other repos | Sandboxed — only the assigned repo is cloned |
| Read `.github/agents/` files directly | Security restriction — agents loaded at startup only |
| Access secrets in GitHub Actions | Sandbox has no `$SECRETS` env vars unless explicitly configured |
| Use a browser with auth sessions | Playwright is unauthenticated — no cookie/session persistence |
| Access blocked internet domains | Hard block — you'll see a clear error |
| Run for >~25-30 minutes continuously | HTTP/2 GOAWAY timeout — break long tasks into sub-agents |

---

## 8 — Copilot Chat (VS Code) Architecture

This is the **non-agent** Copilot. Completely different from the Coding Agent.

### Context Sources (what it sees)
```
1. Inline completions:  ~2,000 chars surrounding cursor
2. Chat — no context:   Conversation history only
3. Chat — @workspace:   Indexed repo (vector search, NOT full file scan)
4. Chat — #file:path:   That specific file (up to ~100K chars)
5. Chat — #selection:   Selected text + ~4K surrounding context
6. Chat — @github:      Can query GitHub API (issues, PRs, code search)
```

### Slash Commands
| Command | Does |
|---------|------|
| `/fix` | Fix error in selection |
| `/explain` | Explain selected code |
| `/tests` | Generate tests for selection |
| `/doc` | Generate documentation |
| `/new` | Scaffold new file or project |
| `/newNotebook` | Create Jupyter notebook |

### Agent Mode (VS Code 1.99+)
VS Code added an "Agent Mode" to Copilot Chat that gives it **limited** tool use:
- Can read/edit files
- Can run terminal commands (with user approval)
- Can use MCP servers
- **Still interactive** — it asks for approval on destructive actions
- **Not autonomous** — it pauses and waits for you between steps
- Model switching: GPT-4o, Claude 3.5 Sonnet, o3-mini, Gemini 1.5 Pro available

### Key Difference from Coding Agent
Copilot Chat Agent Mode still **stops and asks you** at each step.
The Coding Agent **runs autonomously** until the task is done and only reports back when complete (or when it needs a PR review).

---

## 9 — How to Write Prompts for the Coding Agent

### The Issue Body IS Your Prompt

Everything you type in the GitHub issue body is your prompt. Structure matters.

**Template:**
```markdown
## Task
[One sentence — what should be done]

## Constraints
- [Must/must not do X]
- [Tech stack / framework to use]
- [What NOT to touch]

## Context
[Only what the agent needs. File paths. Pattern names. Links.]

## Expected Output
[How to verify success]
```

### What Front-Loading Looks Like

❌ **Bad — requirements buried at the end:**
```
I've been working on this for a while and here's the background...
[500 words of history]
...so please implement X but make sure you don't change Y.
```
(The "don't change Y" may be truncated or receive low attention)

✅ **Good — constraints first:**
```
## Constraints
- DO NOT modify any files in src/engine/
- Use only existing npm dependencies

## Task
Add a dark mode toggle to the settings panel.
```

### Prompt Sizes That Work Well

| Issue Size | Works? | Notes |
|---|---|---|
| < 1,000 chars | ✅ Excellent | Entire issue in prime attention zone |
| 1,000–4,000 chars | ✅ Good | Full clear read |
| 4,000–16,000 chars | ⚠️ OK | Attention degrades toward end |
| 16,000–32,000 chars | ⚠️ Risky | Chunking may occur |
| > 32,000 chars | ❌ Poor | Use separate issues for separate tasks |

### Invoking Custom Agents in Issue Prompts

To invoke a specific custom agent (`.github/agents/<name>.agent.md`), reference the agent name naturally:

```markdown
Use the audio-dsp-expert agent to implement this biquad filter.
```

Or the agent will auto-select based on context.

---

## 10 — copilot-instructions.md Best Practices

This file is the **system prompt** — injected before every agent session.

```
Hard truncation:  ~8,000 characters
Safe zone:        ≤ 6,000 characters (content past 6K may get reduced attention)
Current state:    ~5,500 characters (healthy)
```

**What belongs here:**
- Project identity (1–2 sentences)
- Absolute rules that apply to every task
- Tech stack (compressed — key libs only)
- Critical invariants (e.g., "no heap in processBlock()")
- Links to detailed docs (not the docs themselves)

**What does NOT belong here:**
- Long context that only applies to one task
- File listings (use `glob` instead)
- Code examples (use skill files instead)
- History or background narrative

---

## 11 — Repository Memory System

The agent can persist facts across sessions using `store_memory`. These are injected into every future session.

```
Capacity:        ~50 entries active at any time
Retention:       FIFO — oldest entries rotated out
Entry size:      ~200–300 chars recommended (be concise)
Scope:           Repository-wide — all future agent sessions see them
Storage:         .github/memory/spiralcloud-memory.db (SQLite FTS5)
```

**What to store:**
- Build commands that work (`npm run dev` starts app on port 3000)
- Non-obvious architectural facts
- Tool locations discovered during a session
- Constraints that must never be broken

**What NOT to store:**
- Session-specific state (which files you edited today)
- Things obvious from the code
- Secrets or credentials

---

## 12 — Timing & Reliability

| Scenario | Expected Behavior |
|---|---|
| Simple task (1–5 file edits) | Completes in 2–8 minutes |
| Medium task (10–20 file edits, tests) | Completes in 10–20 minutes |
| Large task (architecture change) | Split into multiple issues; each <25 min |
| Sub-agent running >25 min | Will 503 (`HTTP/2 GOAWAY`) — launch backups proactively |
| Network domain blocked | Immediate clear error message |
| `report_progress` fails | Git conflict — agent will resolve or flag |

---

*This document describes the GitHub Copilot Coding Agent architecture as of 2026. The agent self-documents its own tool inventory and behavior.*
*Last updated: 2026-03-09*
