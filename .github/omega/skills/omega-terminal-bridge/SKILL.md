---
name: Omega Terminal Bridge
description: >
  HTTP bridge protocol for external CLI terminal integration — dispatch commands
  to Gemini, OpenCode, JULES, OpenClaw, and Ollama terminals, perform health
  checks, collect results, handle errors, and manage terminal sessions.
---

# Omega Terminal Bridge

Core principle: **One bridge, many terminals — unified dispatch protocol.**

The terminal bridge extends the Omega agent system beyond Copilot CLI by routing
commands to external CLI terminals through an HTTP server. Each terminal
(Gemini, OpenCode, JULES, OpenClaw, Ollama, GitHub CLI) has different
capabilities, protocols, and latency profiles. The bridge normalizes all of
this behind a consistent JSON API.

## 1. HTTP Bridge Protocol

### Server

The bridge runs at `http://localhost:${OMEGA_BRIDGE_PORT:-18800}` and accepts
JSON POST requests for dispatch and GET requests for health/metrics.

### Endpoints

| Method | Path | Description |
|---|---|---|
| POST | `/dispatch` | Send a command to a single terminal |
| POST | `/dispatch/batch` | Send commands to multiple terminals in parallel |
| GET | `/health` | Health check for all terminals |
| GET | `/health/{terminal}` | Health check for one terminal |
| GET | `/terminals` | List all registered terminals |
| GET | `/terminals/{name}` | Get details of one terminal |
| GET | `/metrics` | Bridge and per-terminal metrics |
| POST | `/council/vote` | Multi-terminal consensus vote |

### Dispatch Request

```json
{
  "terminal": "gemini",
  "command": "Research the ChrysaLisp macro system and its S-expression handling",
  "timeout": 300,
  "session_id": "research-chrysalisp-001"
}
```

### Dispatch Response

```json
{
  "status": "success",
  "terminal": "gemini",
  "session_id": "research-chrysalisp-001",
  "output": "ChrysaLisp uses a Lisp-like S-expression system with...",
  "duration_ms": 12450.32
}
```

### Batch Dispatch

```json
{
  "commands": [
    {"terminal": "gemini", "command": "Research topic A"},
    {"terminal": "ollama", "command": "Summarize this code"},
    {"terminal": "opencode", "command": "Fix the bug in src/main.cpp"}
  ]
}
```

Results are collected as terminals complete and returned as an ordered array.

## 2. Terminal Capabilities

| Terminal | Type | Strengths | Timeout | Max Sessions |
|---|---|---|---|---|
| **Gemini** | CLI | Deep research, multi-modal, long context, NotebookLM | 300s | 5 |
| **OpenCode** | CLI | Fast coding, LSP, multi-model, file editing | 120s | 8 |
| **JULES** | CLI | Full VM execution, dependency install, test suites | 600s | 3 |
| **OpenClaw** | HTTP | 100-agent swarm, parallel tasks, skill routing | 300s | 100 |
| **Ollama** | HTTP | Local inference, embeddings, chat, model management | 120s | 4 |
| **GitHub CLI** | CLI | PRs, issues, actions, API calls, auth | 60s | 10 |

### Routing Guide

```python
TERMINAL_ROUTING = {
    "deep_research":      "gemini",
    "paper_analysis":     "gemini",
    "notebooklm":         "gemini",
    "code_fix":           "opencode",
    "code_generation":    "opencode",
    "lsp_diagnostics":    "opencode",
    "full_build":         "jules",
    "test_suite":         "jules",
    "dependency_install": "jules",
    "swarm_task":         "openclaw",
    "parallel_100":       "openclaw",
    "local_inference":    "ollama",
    "embedding":          "ollama",
    "github_api":         "gh",
    "pr_management":      "gh",
}
```

## 3. Health Checks

### Protocol

Health checks verify terminal availability without side effects:

```python
def check_health(terminal):
    if terminal.type == "http":
        # HTTP GET to health endpoint
        resp = GET(f"{terminal.base_url}/health", timeout=5)
        return "healthy" if resp.status == 200 else "degraded"
    elif terminal.type == "cli":
        # Execute --version or --help
        result = run([terminal.binary, "--version"], timeout=10)
        return "healthy" if result.exit_code == 0 else "not_found"
```

### Status Classification

| Status | Meaning | Action |
|---|---|---|
| `healthy` | Terminal responds correctly within latency budget | Dispatch normally |
| `degraded` | Terminal responds but slowly or with intermittent errors | Dispatch with caution, set lower timeout |
| `offline` | Terminal not responding | Skip, route to fallback |
| `not_found` | Binary not in PATH or service not installed | Log, mark permanently unavailable |

### Pre-Dispatch Health Gate

```python
def dispatch_with_health_check(terminal_name, command, timeout):
    health = GET(f"{BRIDGE}/health/{terminal_name}").json()

    if health["status"] == "offline":
        fallback = find_fallback(terminal_name)
        if fallback:
            return dispatch(fallback, command, timeout)
        raise TerminalUnavailableError(terminal_name)

    if health["status"] == "degraded":
        timeout = min(timeout, 60)  # reduce timeout for degraded terminals

    return POST(f"{BRIDGE}/dispatch", {
        "terminal": terminal_name,
        "command": command,
        "timeout": timeout,
    }).json()
```

## 4. Shell Dispatch Interface

The `omega-dispatch.sh` script provides a shell-native interface to the bridge:

```bash
# Single dispatch
omega-dispatch gemini "Research the ChrysaLisp macro system"

# With timeout
omega-dispatch jules "Run the full test suite" --timeout 900

# Health check
omega-dispatch --health
omega-dispatch --health ollama

# List terminals
omega-dispatch --list

# Metrics
omega-dispatch --metrics

# Batch from file
omega-dispatch --batch commands.json

# Council vote
omega-dispatch --council "Should we use RTNeural or ONNX for inference?" --terminals gemini,ollama
```

## 5. Result Collection and Error Handling

### Timeout Handling

```python
def dispatch_with_retry(terminal, command, timeout, max_retries=2):
    for attempt in range(max_retries + 1):
        result = dispatch(terminal, command, timeout)

        if result["status"] == "success":
            return result
        elif result["status"] == "timeout":
            timeout = int(timeout * 1.5)  # increase timeout on retry
            log(f"Timeout on attempt {attempt+1}, retrying with {timeout}s")
        elif result["status"] == "error":
            if attempt < max_retries:
                log(f"Error on attempt {attempt+1}: {result['output']}")
            else:
                return result  # final failure

    return {"status": "error", "output": f"Failed after {max_retries+1} attempts"}
```

### Error Classification

| Error Type | Cause | Recovery |
|---|---|---|
| `timeout` | Terminal took too long | Retry with increased timeout |
| `error` (exit code != 0) | Command failed | Retry with refined command |
| `error` (binary not found) | CLI not installed | Skip, log, alert orchestrator |
| `error` (connection refused) | Service not running | Check if service needs starting |
| `error` (HTTP 5xx) | Server-side failure | Retry after brief delay |

## 6. Session Management

Sessions maintain context across multiple dispatches to the same terminal:

```python
# Create a session for multi-turn interaction
session_id = "research-session-001"

# First dispatch — establish context
dispatch("gemini", "Load these files as context: ...", session_id=session_id)

# Follow-up — same session, terminal has prior context
dispatch("gemini", "Now analyze the patterns in those files", session_id=session_id)
```

Sessions are tracked in the bridge's in-memory state and logged for observability.

## Anti-Patterns

| Anti-Pattern | Fix |
|---|---|
| Dispatching without health check | Always check `/health` before dispatch (§3) |
| Using the wrong terminal for a task | Consult the routing guide (§2) |
| No timeout on dispatch | Always set explicit timeouts per terminal type |
| Ignoring error responses | Classify errors and apply recovery strategy (§5) |
| Sequential dispatch to multiple terminals | Use batch dispatch for parallel execution |
| Hardcoded terminal URLs | Use environment variables (OMEGA_BRIDGE_PORT, etc.) |
| No session tracking for multi-turn tasks | Use session_id for related dispatches (§6) |
| Retrying without backoff | Increase timeout or simplify command on each retry |

## Checklist

- [ ] **Bridge running** — `curl http://localhost:${OMEGA_BRIDGE_PORT:-18800}/health` returns 200
- [ ] **Terminals registered** — `/terminals` lists all expected terminals
- [ ] **Health checks passing** — `/health` shows healthy or explains degradation
- [ ] **Dispatch tested** — each terminal responds to a test command
- [ ] **Timeouts configured** — per-terminal defaults in terminal-registry.json
- [ ] **Error handling active** — retry policy defined per error type
- [ ] **Metrics collecting** — `/metrics` shows request counts and latencies
- [ ] **Logs capturing** — dispatch.log records every command and result
- [ ] **Environment vars set** — OMEGA_BRIDGE_PORT, OMEGA_BRIDGE_HOST exported
- [ ] **Shell interface tested** — `omega-dispatch --list` returns terminal list
