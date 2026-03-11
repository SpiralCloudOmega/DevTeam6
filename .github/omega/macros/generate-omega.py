#!/usr/bin/env python3
"""Omega System Generator — reads S-expression configs and generates agent
manifests, environment exports, health-check scripts, and bridge configs.

Follows the procedural design-token pattern established in
REVITHION-STUDIO/tokens/generate-tokens.py but targets the Omega RLM
agent system rather than C++20 audio headers.

Usage:
    python generate-omega.py --all
    python generate-omega.py --agents --env --verbose
    python generate-omega.py --config path/to/omega.config.sexp --dry-run
"""

from __future__ import annotations

import argparse
import json
import sys
import textwrap
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

# ---------------------------------------------------------------------------
# S-Expression Tokenizer
# ---------------------------------------------------------------------------

@dataclass
class Token:
    kind: str          # 'lparen' | 'rparen' | 'string' | 'number' | 'symbol' | 'keyword'
    value: Any
    line: int
    col: int


class TokenizerError(Exception):
    def __init__(self, msg: str, line: int, col: int) -> None:
        super().__init__(f"line {line}, col {col}: {msg}")
        self.line = line
        self.col = col


def tokenize(source: str, filename: str = "<input>") -> list[Token]:
    """Convert S-expression source text into a flat token list.

    Handles parentheses, quoted strings (with escape sequences), numbers,
    keyword arguments (:key), bare symbols, and ;-comments.
    """
    tokens: list[Token] = []
    i = 0
    line = 1
    col = 1
    length = len(source)

    while i < length:
        ch = source[i]

        # newline
        if ch == "\n":
            line += 1
            col = 1
            i += 1
            continue

        # whitespace
        if ch in " \t\r":
            col += 1
            i += 1
            continue

        # comment — skip to end of line
        if ch == ";":
            while i < length and source[i] != "\n":
                i += 1
            continue

        # parentheses
        if ch == "(":
            tokens.append(Token("lparen", "(", line, col))
            i += 1
            col += 1
            continue
        if ch == ")":
            tokens.append(Token("rparen", ")", line, col))
            i += 1
            col += 1
            continue

        # quoted string
        if ch == '"':
            start_line, start_col = line, col
            i += 1
            col += 1
            buf: list[str] = []
            while i < length and source[i] != '"':
                if source[i] == "\\":
                    i += 1
                    col += 1
                    if i >= length:
                        raise TokenizerError("unterminated escape in string", start_line, start_col)
                    esc = source[i]
                    buf.append({"n": "\n", "t": "\t", "\\": "\\", '"': '"'}.get(esc, esc))
                elif source[i] == "\n":
                    buf.append("\n")
                    line += 1
                    col = 0
                else:
                    buf.append(source[i])
                i += 1
                col += 1
            if i >= length:
                raise TokenizerError("unterminated string literal", start_line, start_col)
            i += 1  # skip closing quote
            col += 1
            tokens.append(Token("string", "".join(buf), start_line, start_col))
            continue

        # atom (number, keyword, or symbol)
        if ch not in "() \t\r\n;\"":
            start_col = col
            start = i
            while i < length and source[i] not in "() \t\r\n;\"":
                i += 1
                col += 1
            text = source[start:i]
            # keyword :foo
            if text.startswith(":"):
                tokens.append(Token("keyword", text, line, start_col))
            # integer
            elif _is_int(text):
                tokens.append(Token("number", int(text), line, start_col))
            # float
            elif _is_float(text):
                tokens.append(Token("number", float(text), line, start_col))
            # boolean-ish
            elif text in ("#t", "true"):
                tokens.append(Token("symbol", True, line, start_col))
            elif text in ("#f", "false"):
                tokens.append(Token("symbol", False, line, start_col))
            else:
                tokens.append(Token("symbol", text, line, start_col))
            continue

        raise TokenizerError(f"unexpected character {ch!r}", line, col)

    return tokens


def _is_int(s: str) -> bool:
    try:
        int(s)
        return True
    except ValueError:
        return False


def _is_float(s: str) -> bool:
    try:
        float(s)
        return "." in s or "e" in s.lower()
    except ValueError:
        return False


# ---------------------------------------------------------------------------
# S-Expression Parser (recursive descent)
# ---------------------------------------------------------------------------

class ParseError(Exception):
    def __init__(self, msg: str, token: Token | None = None) -> None:
        loc = f"line {token.line}, col {token.col}: " if token else ""
        super().__init__(f"{loc}{msg}")


SExpr = str | int | float | bool | list[Any]


def parse(tokens: list[Token]) -> list[SExpr]:
    """Parse a flat token stream into a list of S-expression trees."""
    pos = 0

    def _parse_one() -> SExpr:
        nonlocal pos
        if pos >= len(tokens):
            raise ParseError("unexpected end of input")
        tok = tokens[pos]
        if tok.kind == "lparen":
            pos += 1
            items: list[SExpr] = []
            while pos < len(tokens) and tokens[pos].kind != "rparen":
                items.append(_parse_one())
            if pos >= len(tokens):
                raise ParseError("unmatched opening parenthesis", tok)
            pos += 1  # consume rparen
            return items
        if tok.kind == "rparen":
            raise ParseError("unexpected ')'", tok)
        # atom
        pos += 1
        return tok.value

    exprs: list[SExpr] = []
    while pos < len(tokens):
        exprs.append(_parse_one())
    return exprs


# ---------------------------------------------------------------------------
# Keyword-argument extraction
# ---------------------------------------------------------------------------

def extract_kwargs(form: list[SExpr]) -> tuple[list[SExpr], dict[str, SExpr]]:
    """Split a form into positional args and :keyword args.

    Returns (positional, {key_without_colon: value}).
    """
    positional: list[SExpr] = []
    kwargs: dict[str, SExpr] = {}
    i = 0
    while i < len(form):
        item = form[i]
        if isinstance(item, str) and item.startswith(":"):
            key = item[1:]  # strip leading ':'
            if i + 1 < len(form):
                kwargs[key] = form[i + 1]
                i += 2
            else:
                kwargs[key] = True
                i += 1
        else:
            positional.append(item)
            i += 1
    return positional, kwargs


# ---------------------------------------------------------------------------
# Macro System
# ---------------------------------------------------------------------------

@dataclass
class Macro:
    name: str
    params: list[str]
    key_params: list[str]
    rest_param: str | None
    body: SExpr
    docstring: str = ""


class MacroRegistry:
    """Stores macro definitions and performs recursive expansion."""

    def __init__(self, verbose: bool = False) -> None:
        self.macros: dict[str, Macro] = {}
        self.verbose = verbose
        self._depth = 0
        self._max_depth = 64

    def define(self, form: list[SExpr]) -> None:
        """Register a macro from (defmacro name (params...) [docstring] body)."""
        if len(form) < 4:
            raise ParseError("defmacro requires name, params, and body")
        name = form[1]
        if not isinstance(name, str):
            raise ParseError(f"macro name must be a symbol, got {name!r}")
        raw_params = form[2]
        if not isinstance(raw_params, list):
            raise ParseError(f"macro params must be a list, got {raw_params!r}")

        positional: list[str] = []
        key_params: list[str] = []
        rest_param: str | None = None
        mode = "pos"
        pi = 0
        while pi < len(raw_params):
            p = raw_params[pi]
            if p == "&key":
                mode = "key"
                pi += 1
                continue
            if p == "&rest":
                mode = "rest"
                pi += 1
                continue
            if not isinstance(p, str):
                raise ParseError(f"parameter must be symbol, got {p!r}")
            if mode == "pos":
                positional.append(p)
            elif mode == "key":
                key_params.append(p)
            elif mode == "rest":
                rest_param = p
                mode = "done"
            pi += 1

        body_start = 3
        docstring = ""
        if isinstance(form[3], str) and len(form) > 4 and not isinstance(form[3], list):
            docstring = form[3]
            body_start = 4

        body = form[body_start] if body_start < len(form) else []
        macro = Macro(name, positional, key_params, rest_param, body, docstring)
        self.macros[name] = macro
        if self.verbose:
            print(f"  [macro] defined {name}({', '.join(positional)})", file=sys.stderr)

    def expand(self, expr: SExpr) -> SExpr:
        """Recursively expand macros in an S-expression tree."""
        if not isinstance(expr, list) or len(expr) == 0:
            return expr

        head = expr[0]

        # special form: (or value default)
        if head == "or" and len(expr) == 3:
            val = self.expand(expr[1])
            if val is None or val == [] or val == "":
                return self.expand(expr[2])
            return val

        # special form: (splice items)
        if head == "splice" and len(expr) == 2:
            inner = self.expand(expr[1])
            return inner if isinstance(inner, list) else [inner]

        # macro application
        if isinstance(head, str) and head in self.macros:
            if self._depth > self._max_depth:
                raise ParseError(f"macro expansion depth exceeded ({self._max_depth})")
            self._depth += 1
            try:
                result = self._apply(head, expr[1:])
                result = self.expand(result)
            finally:
                self._depth -= 1
            return result

        # recurse into sub-forms
        return [self.expand(item) for item in expr]

    def _apply(self, name: str, args: list[SExpr]) -> SExpr:
        macro = self.macros[name]
        positional, kwargs = extract_kwargs(args)

        bindings: dict[str, SExpr] = {}
        for i, pname in enumerate(macro.params):
            bindings[pname] = positional[i] if i < len(positional) else None
        for kp in macro.key_params:
            bindings[kp] = kwargs.get(kp)
        if macro.rest_param:
            bindings[macro.rest_param] = positional[len(macro.params):]

        if self.verbose:
            print(f"  [expand] {name} bindings={bindings}", file=sys.stderr)

        return self._substitute(macro.body, bindings)

    def _substitute(self, body: SExpr, bindings: dict[str, SExpr]) -> SExpr:
        if isinstance(body, str) and body in bindings:
            return bindings[body]
        if isinstance(body, list):
            result: list[SExpr] = []
            for item in body:
                sub = self._substitute(item, bindings)
                # handle splice — flatten into parent list
                if isinstance(item, list) and len(item) >= 1 and item[0] == "splice":
                    if isinstance(sub, list):
                        result.extend(sub)
                        continue
                result.append(sub)
            return result
        return body


# ---------------------------------------------------------------------------
# Config Model — intermediate representation after macro expansion
# ---------------------------------------------------------------------------

@dataclass
class AgentDef:
    name: str
    role: str
    description: str
    port: int | None = None
    expertise: list[str] = field(default_factory=list)
    tools: list[str] = field(default_factory=list)
    model: str = ""
    context: str = ""


@dataclass
class BridgeEntry:
    name: str
    command: str
    protocol: str = "stdio"
    port: int | None = None
    env: dict[str, str] = field(default_factory=dict)


@dataclass
class OmegaConfig:
    """Fully resolved Omega system configuration."""
    name: str = "omega"
    version: str = "1.0.0"
    agents: list[AgentDef] = field(default_factory=list)
    bridges: list[BridgeEntry] = field(default_factory=list)
    env_vars: dict[str, str] = field(default_factory=dict)
    paths: dict[str, str] = field(default_factory=dict)


def build_config(forms: list[SExpr]) -> OmegaConfig:
    """Walk expanded S-expression trees and build an OmegaConfig."""
    cfg = OmegaConfig()

    for form in forms:
        if not isinstance(form, list) or len(form) == 0:
            continue
        head = form[0]

        if head == "omega-system":
            _parse_system(cfg, form[1:])
        elif head == "defagent":
            cfg.agents.append(_parse_agent(form))
        elif head == "defbridge":
            cfg.bridges.append(_parse_bridge(form))
        elif head == "defenv":
            _parse_env(cfg, form[1:])
        elif head == "defpath":
            _parse_paths(cfg, form[1:])

    return cfg


def _parse_system(cfg: OmegaConfig, args: list[SExpr]) -> None:
    _, kw = extract_kwargs(args)
    cfg.name = str(kw.get("name", cfg.name))
    cfg.version = str(kw.get("version", cfg.version))


def _parse_agent(form: list[SExpr]) -> AgentDef:
    if len(form) < 2:
        raise ParseError("defagent requires a name")
    name = str(form[1])
    _, kw = extract_kwargs(form[2:])

    expertise_raw = kw.get("expertise", [])
    expertise = expertise_raw if isinstance(expertise_raw, list) else [str(expertise_raw)]
    tools_raw = kw.get("tools", [])
    tools = tools_raw if isinstance(tools_raw, list) else [str(tools_raw)]

    return AgentDef(
        name=name,
        role=str(kw.get("role", name)),
        description=str(kw.get("description", "")),
        port=int(kw["port"]) if kw.get("port") is not None else None,
        expertise=[str(e) for e in expertise],
        tools=[str(t) for t in tools],
        model=str(kw.get("model", "")),
        context=str(kw.get("context", "")),
    )


def _parse_bridge(form: list[SExpr]) -> BridgeEntry:
    if len(form) < 2:
        raise ParseError("defbridge requires a name")
    name = str(form[1])
    _, kw = extract_kwargs(form[2:])

    env_raw = kw.get("env", {})
    env: dict[str, str] = {}
    if isinstance(env_raw, list):
        for i in range(0, len(env_raw) - 1, 2):
            k = str(env_raw[i]).lstrip(":")
            env[k] = str(env_raw[i + 1])

    return BridgeEntry(
        name=name,
        command=str(kw.get("command", "")),
        protocol=str(kw.get("protocol", "stdio")),
        port=int(kw["port"]) if kw.get("port") is not None else None,
        env=env,
    )


def _parse_env(cfg: OmegaConfig, args: list[SExpr]) -> None:
    _, kw = extract_kwargs(args)
    for k, v in kw.items():
        cfg.env_vars[k] = str(v)


def _parse_paths(cfg: OmegaConfig, args: list[SExpr]) -> None:
    _, kw = extract_kwargs(args)
    for k, v in kw.items():
        cfg.paths[k] = str(v)


# ---------------------------------------------------------------------------
# Generator: Agent Manifests (.agent.md)
# ---------------------------------------------------------------------------

def generate_agent_manifests(cfg: OmegaConfig, output_dir: Path, dry_run: bool) -> list[str]:
    """Produce one .agent.md file per agent definition."""
    generated: list[str] = []
    agents_dir = output_dir / "agents"

    for agent in cfg.agents:
        filename = f"{agent.name}.agent.md"
        path = agents_dir / filename

        expertise_block = ""
        if agent.expertise:
            items = "\n".join(f"- {e}" for e in agent.expertise)
            expertise_block = f"\n## Key Expertise\n\n{items}\n"

        tools_block = ""
        if agent.tools:
            items = "\n".join(f"- {t}" for t in agent.tools)
            tools_block = f"\n## Tools\n\n{items}\n"

        context_block = ""
        if agent.context:
            context_block = f"\n## Omega Context\n\n{agent.context}\n"

        model_line = f"\nmodel: \"{agent.model}\"" if agent.model else ""

        content = (
            f"---\n"
            f"name: {agent.name}\n"
            f"description: \"{agent.description}\"{model_line}\n"
            f"---\n"
            f"\n"
            f"# System Prompt\n"
            f"\n"
            f"You are **{agent.role}** in the Omega Agent System ({cfg.name} v{cfg.version}).\n"
            f"{expertise_block}{tools_block}{context_block}"
        )

        if dry_run:
            print(f"[dry-run] would write {path}")
            print(textwrap.indent(content[:200] + "...\n", "  "))
        else:
            agents_dir.mkdir(parents=True, exist_ok=True)
            path.write_text(content, encoding="utf-8")

        generated.append(str(path))

    return generated


# ---------------------------------------------------------------------------
# Generator: Environment Exports (shell script)
# ---------------------------------------------------------------------------

def generate_env_exports(cfg: OmegaConfig, output_dir: Path, dry_run: bool) -> list[str]:
    """Produce omega-env-generated.sh with all resolved exports."""
    path = output_dir / "omega-env-generated.sh"
    lines: list[str] = [
        "#!/usr/bin/env bash",
        f"# Auto-generated by generate-omega.py — {cfg.name} v{cfg.version}",
        "# DO NOT EDIT — regenerate with: python generate-omega.py --env",
        "",
    ]

    # System metadata
    lines.append(f'export OMEGA_SYSTEM_NAME="{cfg.name}"')
    lines.append(f'export OMEGA_SYSTEM_VERSION="{cfg.version}"')
    lines.append("")

    # Explicit env vars
    if cfg.env_vars:
        lines.append("# --- Environment variables ---")
        for k, v in sorted(cfg.env_vars.items()):
            safe_key = k.upper().replace("-", "_")
            lines.append(f'export {safe_key}="{v}"')
        lines.append("")

    # Paths
    if cfg.paths:
        lines.append("# --- Resolved paths ---")
        for k, v in sorted(cfg.paths.items()):
            safe_key = "OMEGA_PATH_" + k.upper().replace("-", "_")
            lines.append(f'export {safe_key}="{v}"')
        lines.append("")

    # Per-agent ports
    agent_ports = [(a.name, a.port) for a in cfg.agents if a.port is not None]
    if agent_ports:
        lines.append("# --- Agent ports ---")
        for name, port in agent_ports:
            safe_name = name.upper().replace("-", "_")
            lines.append(f'export OMEGA_AGENT_{safe_name}_PORT="{port}"')
        lines.append("")

    content = "\n".join(lines) + "\n"

    if dry_run:
        print(f"[dry-run] would write {path}")
        print(textwrap.indent(content[:300] + "...\n", "  "))
    else:
        output_dir.mkdir(parents=True, exist_ok=True)
        path.write_text(content, encoding="utf-8")
        path.chmod(0o755)

    return [str(path)]


# ---------------------------------------------------------------------------
# Generator: Health Check Script (bash)
# ---------------------------------------------------------------------------

def generate_health_check(cfg: OmegaConfig, output_dir: Path, dry_run: bool) -> list[str]:
    """Produce omega-health.sh that pings every agent endpoint."""
    path = output_dir / "omega-health.sh"

    header = textwrap.dedent(f"""\
        #!/usr/bin/env bash
        # Auto-generated health-check — {cfg.name} v{cfg.version}
        # DO NOT EDIT — regenerate with: python generate-omega.py --health
        set -euo pipefail

        PASS=0
        FAIL=0
        SKIP=0

        check_agent() {{
            local name="$1" port="$2"
            if [ -z "$port" ]; then
                printf "  %-25s SKIP (no port)\\n" "$name"
                ((SKIP++))
                return
            fi
            if curl -sf --max-time 3 "http://localhost:$port/health" >/dev/null 2>&1; then
                printf "  %-25s \\033[32mOK\\033[0m\\n" "$name"
                ((PASS++))
            else
                printf "  %-25s \\033[31mFAIL\\033[0m\\n" "$name"
                ((FAIL++))
            fi
        }}

        echo "=== Omega Health Check: {cfg.name} v{cfg.version} ==="
        echo ""
    """)

    checks: list[str] = []
    for agent in cfg.agents:
        port_str = str(agent.port) if agent.port else ""
        checks.append(f'check_agent "{agent.name}" "{port_str}"')

    for bridge in cfg.bridges:
        if bridge.port:
            checks.append(f'check_agent "bridge:{bridge.name}" "{bridge.port}"')

    footer = textwrap.dedent("""\

        echo ""
        echo "--- Summary ---"
        echo "  Passed: $PASS"
        echo "  Failed: $FAIL"
        echo "  Skipped: $SKIP"

        if [ "$FAIL" -gt 0 ]; then
            exit 1
        fi
    """)

    content = header + "\n".join(checks) + "\n" + footer

    if dry_run:
        print(f"[dry-run] would write {path}")
        print(textwrap.indent(content[:400] + "...\n", "  "))
    else:
        output_dir.mkdir(parents=True, exist_ok=True)
        path.write_text(content, encoding="utf-8")
        path.chmod(0o755)

    return [str(path)]


# ---------------------------------------------------------------------------
# Generator: Bridge Config (JSON)
# ---------------------------------------------------------------------------

def generate_bridge_config(cfg: OmegaConfig, output_dir: Path, dry_run: bool) -> list[str]:
    """Produce terminal-registry.json with external CLI mappings."""
    path = output_dir / "terminal-registry.json"

    registry: dict[str, Any] = {
        "$schema": "omega-bridge-v1",
        "system": cfg.name,
        "version": cfg.version,
        "bridges": [],
    }

    for bridge in cfg.bridges:
        entry: dict[str, Any] = {
            "name": bridge.name,
            "command": bridge.command,
            "protocol": bridge.protocol,
        }
        if bridge.port is not None:
            entry["port"] = bridge.port
        if bridge.env:
            entry["env"] = bridge.env
        registry["bridges"].append(entry)

    # Also register agents that expose HTTP ports
    agent_endpoints: list[dict[str, Any]] = []
    for agent in cfg.agents:
        if agent.port is not None:
            agent_endpoints.append({
                "name": agent.name,
                "endpoint": f"http://localhost:{agent.port}",
                "model": agent.model or "default",
            })
    registry["agent_endpoints"] = agent_endpoints

    content = json.dumps(registry, indent=2, ensure_ascii=False) + "\n"

    if dry_run:
        print(f"[dry-run] would write {path}")
        print(textwrap.indent(content[:300] + "...\n", "  "))
    else:
        output_dir.mkdir(parents=True, exist_ok=True)
        path.write_text(content, encoding="utf-8")

    return [str(path)]


# ---------------------------------------------------------------------------
# Pipeline orchestrator
# ---------------------------------------------------------------------------

def load_and_parse(filepath: Path, verbose: bool) -> list[SExpr]:
    """Read a .sexp file, tokenize, and parse."""
    if not filepath.exists():
        print(f"warning: {filepath} not found, skipping", file=sys.stderr)
        return []
    if verbose:
        print(f"[parse] reading {filepath}", file=sys.stderr)
    source = filepath.read_text(encoding="utf-8")
    tokens = tokenize(source, str(filepath))
    return parse(tokens)


def run_pipeline(
    config_path: Path,
    macros_path: Path,
    output_dir: Path,
    *,
    gen_agents: bool = False,
    gen_env: bool = False,
    gen_health: bool = False,
    gen_bridge: bool = False,
    dry_run: bool = False,
    verbose: bool = False,
) -> None:
    """Full generation pipeline: parse → expand → build model → generate."""

    # Phase 1: Parse macro definitions
    if verbose:
        print("=== Phase 1: Parse macros ===", file=sys.stderr)
    registry = MacroRegistry(verbose=verbose)
    macro_forms = load_and_parse(macros_path, verbose)
    for form in macro_forms:
        if isinstance(form, list) and len(form) > 0 and form[0] == "defmacro":
            registry.define(form)
    if verbose:
        print(f"  loaded {len(registry.macros)} macros", file=sys.stderr)

    # Phase 2: Parse config
    if verbose:
        print("=== Phase 2: Parse config ===", file=sys.stderr)
    config_forms = load_and_parse(config_path, verbose)
    if not config_forms:
        print("error: no config forms found — nothing to generate", file=sys.stderr)
        sys.exit(1)

    # Phase 3: Expand macros
    if verbose:
        print("=== Phase 3: Expand macros ===", file=sys.stderr)
    expanded = [registry.expand(form) for form in config_forms]

    # Phase 4: Build configuration model
    if verbose:
        print("=== Phase 4: Build config model ===", file=sys.stderr)
    cfg = build_config(expanded)
    if verbose:
        print(f"  system: {cfg.name} v{cfg.version}", file=sys.stderr)
        print(f"  agents: {len(cfg.agents)}", file=sys.stderr)
        print(f"  bridges: {len(cfg.bridges)}", file=sys.stderr)
        print(f"  env_vars: {len(cfg.env_vars)}", file=sys.stderr)
        print(f"  paths: {len(cfg.paths)}", file=sys.stderr)

    # Phase 5: Generate outputs
    if verbose:
        print("=== Phase 5: Generate outputs ===", file=sys.stderr)
    all_generated: list[str] = []

    if gen_agents:
        all_generated.extend(generate_agent_manifests(cfg, output_dir, dry_run))
    if gen_env:
        all_generated.extend(generate_env_exports(cfg, output_dir, dry_run))
    if gen_health:
        all_generated.extend(generate_health_check(cfg, output_dir, dry_run))
    if gen_bridge:
        all_generated.extend(generate_bridge_config(cfg, output_dir, dry_run))

    # Summary
    verb = "would generate" if dry_run else "generated"
    print(f"\n{verb} {len(all_generated)} file(s):")
    for f in all_generated:
        print(f"  {f}")


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main() -> None:
    parser = argparse.ArgumentParser(
        prog="generate-omega.py",
        description="Omega System Generator — S-expression configs → agent manifests, "
                    "env exports, health checks, bridge config.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=textwrap.dedent("""\
            examples:
              %(prog)s --all
              %(prog)s --agents --env --verbose
              %(prog)s --config ../omega.config.sexp --dry-run --all
        """),
    )

    script_dir = Path(__file__).resolve().parent

    parser.add_argument(
        "--config", type=Path,
        default=script_dir.parent / "omega.config.sexp",
        help="Path to omega.config.sexp (default: ../omega.config.sexp)",
    )
    parser.add_argument(
        "--macros", type=Path,
        default=script_dir / "agent-macros.sexp",
        help="Path to agent-macros.sexp (default: agent-macros.sexp in script dir)",
    )
    parser.add_argument(
        "--output-dir", type=Path,
        default=script_dir.parent / "generated",
        help="Output directory (default: ../generated/)",
    )

    parser.add_argument("--agents", action="store_true", help="Generate agent manifests")
    parser.add_argument("--env", action="store_true", help="Generate environment exports")
    parser.add_argument("--health", action="store_true", help="Generate health check scripts")
    parser.add_argument("--bridge", action="store_true", help="Generate bridge config")
    parser.add_argument("--all", action="store_true", help="Generate everything")

    parser.add_argument("--dry-run", action="store_true",
                        help="Show what would be generated without writing")
    parser.add_argument("--verbose", action="store_true",
                        help="Show macro expansion details")

    args = parser.parse_args()

    gen_agents = args.agents or args.all
    gen_env = args.env or args.all
    gen_health = args.health or args.all
    gen_bridge = args.bridge or args.all

    if not (gen_agents or gen_env or gen_health or gen_bridge):
        parser.error("specify at least one of --agents, --env, --health, --bridge, or --all")

    run_pipeline(
        config_path=args.config,
        macros_path=args.macros,
        output_dir=args.output_dir,
        gen_agents=gen_agents,
        gen_env=gen_env,
        gen_health=gen_health,
        gen_bridge=gen_bridge,
        dry_run=args.dry_run,
        verbose=args.verbose,
    )


if __name__ == "__main__":
    main()
