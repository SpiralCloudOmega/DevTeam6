#!/usr/bin/env python3
"""
Omega Inter-Terminal HTTP Bridge Server

Receives commands from the Copilot CLI master orchestrator, dispatches them
to external CLI terminals (Gemini, OpenCode, JULES, OpenClaw, Ollama),
collects results, and streams back via HTTP response.

Usage:
    http-bridge.py [--port PORT] [--config PATH] [--log-level LEVEL]

Options:
    --port       Bridge port (default: 18800, from OMEGA_BRIDGE_PORT)
    --config     Path to terminal-registry.json
    --log-level  debug|info|warning|error
"""

import argparse
import json
import logging
import os
import re
import subprocess
import sys
import threading
import time
import urllib.request
import urllib.error
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor, as_completed
from http.server import HTTPServer, BaseHTTPRequestHandler
from pathlib import Path

__version__ = "1.0.0"

logger = logging.getLogger("omega-bridge")

# ---------------------------------------------------------------------------
# Default terminal registry (used when no config file is provided)
# ---------------------------------------------------------------------------
DEFAULT_TERMINALS = {
    "gemini": {
        "type": "cli",
        "binary": "gemini",
        "args_template": ["-p", "{prompt}"],
        "description": "Google Gemini CLI",
        "timeout": 300,
    },
    "opencode": {
        "type": "cli",
        "binary": "opencode",
        "args_template": [],
        "stdin_mode": True,
        "description": "OpenCode CLI",
        "timeout": 300,
    },
    "jules": {
        "type": "cli",
        "binary": "jules",
        "args_template": ["{prompt}"],
        "description": "JULES task runner",
        "timeout": 600,
    },
    "openclaw": {
        "type": "http",
        "base_url": "http://localhost:18789",
        "description": "OpenClaw 100-agent swarm",
        "timeout": 300,
    },
    "ollama": {
        "type": "hybrid",
        "binary": "ollama",
        "args_template": ["run", "{model}", "{prompt}"],
        "base_url": "http://localhost:11434",
        "default_model": "qwen3:8b",
        "description": "Ollama local LLM server",
        "timeout": 300,
    },
}


# ---------------------------------------------------------------------------
# Session & metrics state
# ---------------------------------------------------------------------------
class BridgeState:
    """Thread-safe in-memory state for sessions and metrics."""

    def __init__(self):
        self._lock = threading.Lock()
        self.sessions: dict[str, dict] = {}
        self.terminals: dict[str, dict] = {}
        self.metrics: dict[str, dict] = defaultdict(
            lambda: {
                "total_requests": 0,
                "errors": 0,
                "total_duration_ms": 0.0,
                "last_request": None,
            }
        )
        self.start_time = time.time()

    # -- terminal registry --------------------------------------------------
    def register_terminals(self, config: dict):
        with self._lock:
            for name, cfg in config.items():
                self.terminals[name] = {
                    **cfg,
                    "name": name,
                    "status": "registered",
                    "pid": None,
                    "started_at": None,
                }

    def get_terminal(self, name: str) -> dict | None:
        with self._lock:
            return self.terminals.get(name)

    def list_terminals(self) -> dict:
        with self._lock:
            return dict(self.terminals)

    def set_terminal_status(self, name: str, status: str, pid: int | None = None):
        with self._lock:
            if name in self.terminals:
                self.terminals[name]["status"] = status
                if pid is not None:
                    self.terminals[name]["pid"] = pid
                if status == "running":
                    self.terminals[name]["started_at"] = time.time()

    # -- sessions -----------------------------------------------------------
    def get_or_create_session(self, session_id: str, terminal: str) -> dict:
        with self._lock:
            if session_id not in self.sessions:
                self.sessions[session_id] = {
                    "id": session_id,
                    "terminal": terminal,
                    "created_at": time.time(),
                    "last_active": time.time(),
                    "request_count": 0,
                }
            sess = self.sessions[session_id]
            sess["last_active"] = time.time()
            sess["request_count"] += 1
            return dict(sess)

    # -- metrics ------------------------------------------------------------
    def record_metric(self, terminal: str, duration_ms: float, error: bool = False):
        with self._lock:
            m = self.metrics[terminal]
            m["total_requests"] += 1
            m["total_duration_ms"] += duration_ms
            m["last_request"] = time.time()
            if error:
                m["errors"] += 1

    def get_metrics(self) -> dict:
        with self._lock:
            result = {}
            for name, m in self.metrics.items():
                avg = (
                    m["total_duration_ms"] / m["total_requests"]
                    if m["total_requests"]
                    else 0
                )
                error_rate = (
                    m["errors"] / m["total_requests"] if m["total_requests"] else 0
                )
                result[name] = {
                    **m,
                    "avg_duration_ms": round(avg, 2),
                    "error_rate": round(error_rate, 4),
                }
            result["_bridge"] = {
                "uptime_s": round(time.time() - self.start_time, 2),
                "version": __version__,
            }
            return result


# ---------------------------------------------------------------------------
# Terminal dispatchers
# ---------------------------------------------------------------------------
def _dispatch_cli(terminal_cfg: dict, prompt: str, timeout: int) -> dict:
    """Execute a CLI terminal via subprocess."""
    binary = terminal_cfg["binary"]
    args_template = list(terminal_cfg.get("args_template", []))
    model = terminal_cfg.get("default_model", "")

    args = [binary] + [
        a.format(prompt=prompt, model=model) for a in args_template
    ]

    stdin_mode = terminal_cfg.get("stdin_mode", False)
    t0 = time.monotonic()

    try:
        proc = subprocess.Popen(
            args,
            stdin=subprocess.PIPE if stdin_mode else None,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        stdout, stderr = proc.communicate(
            input=prompt if stdin_mode else None,
            timeout=timeout,
        )
        duration_ms = (time.monotonic() - t0) * 1000

        if proc.returncode != 0:
            return {
                "status": "error",
                "output": stderr.strip() or stdout.strip(),
                "return_code": proc.returncode,
                "duration_ms": round(duration_ms, 2),
            }

        return {
            "status": "success",
            "output": stdout.strip(),
            "duration_ms": round(duration_ms, 2),
        }

    except subprocess.TimeoutExpired:
        proc.kill()
        proc.wait()
        return {
            "status": "timeout",
            "output": f"Command timed out after {timeout}s",
            "duration_ms": round((time.monotonic() - t0) * 1000, 2),
        }
    except FileNotFoundError:
        return {
            "status": "error",
            "output": f"Binary not found: {binary}",
            "duration_ms": 0,
        }
    except Exception as exc:
        return {
            "status": "error",
            "output": str(exc),
            "duration_ms": round((time.monotonic() - t0) * 1000, 2),
        }


def _dispatch_http(terminal_cfg: dict, prompt: str, timeout: int) -> dict:
    """Dispatch to an HTTP-based terminal (OpenClaw, Ollama API)."""
    base_url = terminal_cfg["base_url"].rstrip("/")
    t0 = time.monotonic()

    # Build the request payload based on terminal type
    name = terminal_cfg.get("name", "")
    if "ollama" in name:
        url = f"{base_url}/api/generate"
        payload = {
            "model": terminal_cfg.get("default_model", "qwen3:8b"),
            "prompt": prompt,
            "stream": False,
        }
    else:
        url = f"{base_url}/dispatch"
        payload = {"prompt": prompt}

    data = json.dumps(payload).encode("utf-8")
    req = urllib.request.Request(
        url,
        data=data,
        headers={"Content-Type": "application/json"},
        method="POST",
    )

    try:
        with urllib.request.urlopen(req, timeout=timeout) as resp:
            body = json.loads(resp.read().decode("utf-8"))
            duration_ms = (time.monotonic() - t0) * 1000

            # Normalize response
            output = body.get("response") or body.get("output") or json.dumps(body)
            return {
                "status": "success",
                "output": output if isinstance(output, str) else json.dumps(output),
                "duration_ms": round(duration_ms, 2),
            }

    except urllib.error.URLError as exc:
        return {
            "status": "error",
            "output": f"HTTP error: {exc.reason}",
            "duration_ms": round((time.monotonic() - t0) * 1000, 2),
        }
    except Exception as exc:
        return {
            "status": "error",
            "output": str(exc),
            "duration_ms": round((time.monotonic() - t0) * 1000, 2),
        }


def dispatch(terminal_cfg: dict, prompt: str, timeout: int | None = None) -> dict:
    """Route dispatch to the correct handler based on terminal type."""
    t_type = terminal_cfg.get("type", "cli")
    timeout = timeout or terminal_cfg.get("timeout", 300)

    if t_type == "http":
        return _dispatch_http(terminal_cfg, prompt, timeout)
    elif t_type == "hybrid":
        # Prefer HTTP API if base_url is set, fall back to CLI
        base_url = terminal_cfg.get("base_url")
        if base_url:
            result = _dispatch_http(terminal_cfg, prompt, timeout)
            if result["status"] == "success":
                return result
            logger.warning("HTTP dispatch failed for %s, falling back to CLI", terminal_cfg.get("name"))
        return _dispatch_cli(terminal_cfg, prompt, timeout)
    else:
        return _dispatch_cli(terminal_cfg, prompt, timeout)


# ---------------------------------------------------------------------------
# Health checks
# ---------------------------------------------------------------------------
def check_terminal_health(terminal_cfg: dict) -> dict:
    """Check whether a terminal is reachable."""
    name = terminal_cfg.get("name", "unknown")
    t_type = terminal_cfg.get("type", "cli")
    t0 = time.monotonic()

    if t_type in ("http", "hybrid") and terminal_cfg.get("base_url"):
        base_url = terminal_cfg["base_url"].rstrip("/")
        # Ollama exposes GET / as health; OpenClaw may use /health
        for path in ("/", "/health"):
            try:
                req = urllib.request.Request(f"{base_url}{path}", method="GET")
                with urllib.request.urlopen(req, timeout=5) as resp:
                    latency_ms = (time.monotonic() - t0) * 1000
                    return {
                        "terminal": name,
                        "status": "healthy",
                        "latency_ms": round(latency_ms, 2),
                        "last_check": time.time(),
                    }
            except Exception:
                continue

    if t_type in ("cli", "hybrid"):
        binary = terminal_cfg.get("binary")
        if binary:
            for flag in ("--version", "--help"):
                try:
                    proc = subprocess.run(
                        [binary, flag],
                        capture_output=True,
                        text=True,
                        timeout=10,
                    )
                    latency_ms = (time.monotonic() - t0) * 1000
                    if proc.returncode == 0:
                        return {
                            "terminal": name,
                            "status": "healthy",
                            "latency_ms": round(latency_ms, 2),
                            "last_check": time.time(),
                            "version_info": proc.stdout.strip()[:200],
                        }
                except FileNotFoundError:
                    return {
                        "terminal": name,
                        "status": "not_found",
                        "latency_ms": 0,
                        "last_check": time.time(),
                        "error": f"Binary '{binary}' not found in PATH",
                    }
                except subprocess.TimeoutExpired:
                    pass
                except Exception:
                    pass

    latency_ms = (time.monotonic() - t0) * 1000
    return {
        "terminal": name,
        "status": "unreachable",
        "latency_ms": round(latency_ms, 2),
        "last_check": time.time(),
    }


# ---------------------------------------------------------------------------
# HTTP Request Handler
# ---------------------------------------------------------------------------
class BridgeHandler(BaseHTTPRequestHandler):
    """HTTP handler for the Omega bridge."""

    state: BridgeState  # set on class before server starts
    executor: ThreadPoolExecutor

    # Silence default stderr logging — we use the logger
    def log_message(self, fmt, *args):
        logger.debug("HTTP %s", fmt % args)

    # -- helpers ------------------------------------------------------------
    def _read_body(self) -> dict:
        length = int(self.headers.get("Content-Length", 0))
        if length == 0:
            return {}
        raw = self.rfile.read(length)
        return json.loads(raw.decode("utf-8"))

    def _send_json(self, data: dict, code: int = 200):
        body = json.dumps(data, indent=2).encode("utf-8")
        self.send_response(code)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def _send_error_json(self, code: int, message: str):
        self._send_json({"status": "error", "message": message}, code)

    def _route_path(self) -> tuple[str, list[str]]:
        """Return (base_route, path_segments)."""
        path = self.path.split("?")[0].rstrip("/") or "/"
        parts = [p for p in path.split("/") if p]
        base = f"/{parts[0]}" if parts else "/"
        return base, parts

    # -- GET routes ---------------------------------------------------------
    def do_GET(self):
        base, parts = self._route_path()

        if base == "/health":
            if len(parts) == 1:
                self._handle_health_all()
            else:
                self._handle_health_terminal(parts[1])
        elif base == "/terminals":
            if len(parts) == 1:
                self._handle_list_terminals()
            else:
                self._handle_get_terminal(parts[1])
        elif base == "/metrics":
            self._handle_metrics()
        else:
            self._send_error_json(404, f"Unknown route: {self.path}")

    # -- POST routes --------------------------------------------------------
    def do_POST(self):
        base, parts = self._route_path()

        try:
            body = self._read_body()
        except json.JSONDecodeError as exc:
            self._send_error_json(400, f"Invalid JSON: {exc}")
            return

        if base == "/dispatch":
            if len(parts) >= 2 and parts[1] == "batch":
                self._handle_dispatch_batch(body)
            else:
                self._handle_dispatch(body)
        elif base == "/terminals" and len(parts) >= 3:
            action = parts[2]
            name = parts[1]
            if action == "start":
                self._handle_terminal_start(name, body)
            elif action == "stop":
                self._handle_terminal_stop(name, body)
            else:
                self._send_error_json(404, f"Unknown terminal action: {action}")
        elif base == "/council" and len(parts) >= 2 and parts[1] == "vote":
            self._handle_council_vote(body)
        else:
            self._send_error_json(404, f"Unknown route: {self.path}")

    # -- handler implementations --------------------------------------------
    def _handle_dispatch(self, body: dict):
        terminal_name = body.get("terminal")
        command = body.get("command")
        if not terminal_name or not command:
            self._send_error_json(400, "Missing 'terminal' or 'command' field")
            return

        cfg = self.state.get_terminal(terminal_name)
        if cfg is None:
            self._send_error_json(404, f"Unknown terminal: {terminal_name}")
            return

        timeout = body.get("timeout", cfg.get("timeout", 300))
        session_id = body.get("session_id", f"auto-{int(time.time())}")
        self.state.get_or_create_session(session_id, terminal_name)

        logger.info("Dispatch → %s  session=%s  cmd=%s", terminal_name, session_id, command[:80])
        result = dispatch(cfg, command, timeout)

        is_error = result["status"] != "success"
        self.state.record_metric(terminal_name, result.get("duration_ms", 0), error=is_error)

        result["terminal"] = terminal_name
        result["session_id"] = session_id
        code = 200 if not is_error else 502
        self._send_json(result, code)

    def _handle_dispatch_batch(self, body: dict):
        commands = body.get("commands")
        if not isinstance(commands, list) or not commands:
            self._send_error_json(400, "Missing or empty 'commands' array")
            return

        futures = {}
        for idx, cmd in enumerate(commands):
            terminal_name = cmd.get("terminal")
            command = cmd.get("command")
            if not terminal_name or not command:
                continue
            cfg = self.state.get_terminal(terminal_name)
            if cfg is None:
                continue
            timeout = cmd.get("timeout", cfg.get("timeout", 300))
            session_id = cmd.get("session_id", f"batch-{int(time.time())}-{idx}")
            self.state.get_or_create_session(session_id, terminal_name)

            future = self.executor.submit(dispatch, cfg, command, timeout)
            futures[future] = {"terminal": terminal_name, "session_id": session_id, "index": idx}

        results = []
        for future in as_completed(futures):
            meta = futures[future]
            try:
                result = future.result()
            except Exception as exc:
                result = {"status": "error", "output": str(exc), "duration_ms": 0}

            is_error = result["status"] != "success"
            self.state.record_metric(meta["terminal"], result.get("duration_ms", 0), error=is_error)

            result["terminal"] = meta["terminal"]
            result["session_id"] = meta["session_id"]
            result["index"] = meta["index"]
            results.append(result)

        results.sort(key=lambda r: r.get("index", 0))
        self._send_json({"status": "complete", "results": results})

    def _handle_list_terminals(self):
        terminals = self.state.list_terminals()
        summary = {}
        for name, cfg in terminals.items():
            summary[name] = {
                "type": cfg.get("type"),
                "status": cfg.get("status"),
                "description": cfg.get("description", ""),
                "pid": cfg.get("pid"),
                "started_at": cfg.get("started_at"),
            }
        self._send_json({"terminals": summary})

    def _handle_get_terminal(self, name: str):
        cfg = self.state.get_terminal(name)
        if cfg is None:
            self._send_error_json(404, f"Unknown terminal: {name}")
            return
        safe = {k: v for k, v in cfg.items() if k != "args_template"}
        self._send_json(safe)

    def _handle_terminal_start(self, name: str, body: dict):
        cfg = self.state.get_terminal(name)
        if cfg is None:
            self._send_error_json(404, f"Unknown terminal: {name}")
            return

        self.state.set_terminal_status(name, "running")
        logger.info("Terminal started: %s", name)
        self._send_json({"status": "started", "terminal": name})

    def _handle_terminal_stop(self, name: str, body: dict):
        cfg = self.state.get_terminal(name)
        if cfg is None:
            self._send_error_json(404, f"Unknown terminal: {name}")
            return

        pid = cfg.get("pid")
        if pid:
            try:
                os.kill(pid, 15)  # SIGTERM
            except ProcessLookupError:
                pass

        self.state.set_terminal_status(name, "stopped")
        logger.info("Terminal stopped: %s", name)
        self._send_json({"status": "stopped", "terminal": name})

    def _handle_health_all(self):
        terminals = self.state.list_terminals()
        health = {}
        futures = {}

        for name, cfg in terminals.items():
            future = self.executor.submit(check_terminal_health, cfg)
            futures[future] = name

        for future in as_completed(futures):
            name = futures[future]
            try:
                health[name] = future.result()
            except Exception as exc:
                health[name] = {"terminal": name, "status": "error", "error": str(exc)}

        all_healthy = all(h.get("status") == "healthy" for h in health.values())
        self._send_json({
            "bridge": "healthy",
            "version": __version__,
            "uptime_s": round(time.time() - self.state.start_time, 2),
            "terminals": health,
            "all_healthy": all_healthy,
        })

    def _handle_health_terminal(self, name: str):
        cfg = self.state.get_terminal(name)
        if cfg is None:
            self._send_error_json(404, f"Unknown terminal: {name}")
            return
        result = check_terminal_health(cfg)
        self._send_json(result)

    def _handle_metrics(self):
        self._send_json(self.state.get_metrics())

    def _handle_council_vote(self, body: dict):
        """Submit a prompt to multiple terminals and collect consensus votes."""
        prompt = body.get("prompt")
        terminals = body.get("terminals")
        if not prompt:
            self._send_error_json(400, "Missing 'prompt' field")
            return

        available = self.state.list_terminals()
        if not terminals:
            terminals = list(available.keys())
        else:
            terminals = [t for t in terminals if t in available]

        if not terminals:
            self._send_error_json(400, "No valid terminals for council vote")
            return

        timeout = body.get("timeout", 120)
        futures = {}
        for name in terminals:
            cfg = available[name]
            council_prompt = (
                f"[COUNCIL VOTE] Evaluate and respond concisely:\n\n{prompt}\n\n"
                f"End your response with VOTE: APPROVE or VOTE: REJECT with a one-line reason."
            )
            future = self.executor.submit(dispatch, cfg, council_prompt, timeout)
            futures[future] = name

        votes = []
        for future in as_completed(futures):
            name = futures[future]
            try:
                result = future.result()
            except Exception as exc:
                result = {"status": "error", "output": str(exc), "duration_ms": 0}

            self.state.record_metric(name, result.get("duration_ms", 0), error=result["status"] != "success")

            output = result.get("output", "")
            vote = "ABSTAIN"
            if "VOTE: APPROVE" in output.upper():
                vote = "APPROVE"
            elif "VOTE: REJECT" in output.upper():
                vote = "REJECT"

            votes.append({
                "terminal": name,
                "vote": vote,
                "output": output,
                "status": result["status"],
                "duration_ms": result.get("duration_ms", 0),
            })

        approve = sum(1 for v in votes if v["vote"] == "APPROVE")
        reject = sum(1 for v in votes if v["vote"] == "REJECT")
        abstain = sum(1 for v in votes if v["vote"] == "ABSTAIN")

        consensus = "APPROVE" if approve > reject else "REJECT" if reject > approve else "TIE"
        self._send_json({
            "consensus": consensus,
            "tally": {"approve": approve, "reject": reject, "abstain": abstain},
            "votes": votes,
        })


# ---------------------------------------------------------------------------
# Configuration loading
# ---------------------------------------------------------------------------
def load_terminal_config(config_path: str | None) -> dict:
    """Load terminal registry from JSON file, falling back to defaults."""
    raw: dict | None = None

    if config_path and os.path.isfile(config_path):
        logger.info("Loading terminal config from %s", config_path)
        with open(config_path, "r") as f:
            raw = json.load(f)
    else:
        # Try adjacent terminal-registry.json
        here = Path(__file__).resolve().parent
        auto_path = here / "terminal-registry.json"
        if auto_path.is_file():
            logger.info("Loading terminal config from %s", auto_path)
            with open(auto_path, "r") as f:
                raw = json.load(f)

    if raw is None:
        logger.info("Using default terminal registry")
        return dict(DEFAULT_TERMINALS)

    # Support both flat {name: cfg, ...} and wrapped {"terminals": {name: cfg}}
    if "terminals" in raw and isinstance(raw["terminals"], dict):
        return raw["terminals"]
    return raw


def load_omega_env():
    """Load OMEGA_CLI_* environment variables (from omega-env.sh pattern)."""
    env_vars = {k: v for k, v in os.environ.items() if k.startswith("OMEGA_")}
    if env_vars:
        logger.info("Loaded %d OMEGA_* env vars", len(env_vars))
    return env_vars


# ---------------------------------------------------------------------------
# Server entry point
# ---------------------------------------------------------------------------
def create_server(port: int, state: BridgeState) -> HTTPServer:
    """Create and configure the HTTP server."""
    executor = ThreadPoolExecutor(max_workers=16, thread_name_prefix="omega-dispatch")

    BridgeHandler.state = state
    BridgeHandler.executor = executor

    server = HTTPServer(("0.0.0.0", port), BridgeHandler)
    server.executor = executor
    return server


def main():
    parser = argparse.ArgumentParser(
        description="Omega Inter-Terminal HTTP Bridge Server",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )
    parser.add_argument(
        "--port",
        type=int,
        default=int(os.environ.get("OMEGA_BRIDGE_PORT", "18800")),
        help="Bridge port (default: 18800, from OMEGA_BRIDGE_PORT)",
    )
    parser.add_argument(
        "--config",
        type=str,
        default=os.environ.get("OMEGA_BRIDGE_CONFIG"),
        help="Path to terminal-registry.json",
    )
    parser.add_argument(
        "--log-level",
        type=str,
        default=os.environ.get("OMEGA_LOG_LEVEL", "info"),
        choices=["debug", "info", "warning", "error"],
        help="Logging level (default: info)",
    )
    args = parser.parse_args()

    logging.basicConfig(
        level=getattr(logging, args.log_level.upper()),
        format="%(asctime)s [%(levelname)s] %(name)s: %(message)s",
        datefmt="%Y-%m-%dT%H:%M:%S",
    )

    load_omega_env()
    config = load_terminal_config(args.config)

    state = BridgeState()
    state.register_terminals(config)

    server = create_server(args.port, state)

    logger.info("=" * 60)
    logger.info("  Omega HTTP Bridge v%s", __version__)
    logger.info("  Listening on 0.0.0.0:%d", args.port)
    logger.info("  Terminals: %s", ", ".join(config.keys()))
    logger.info("=" * 60)

    try:
        server.serve_forever()
    except KeyboardInterrupt:
        logger.info("Shutting down bridge...")
    finally:
        server.shutdown()
        server.executor.shutdown(wait=False)
        logger.info("Bridge stopped.")


if __name__ == "__main__":
    main()
