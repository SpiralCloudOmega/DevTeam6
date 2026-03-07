#!/usr/bin/env python3
"""
REVITHION STUDIO — Audio Design Token Generator

Reads S-expression token definitions from design-tokens.sexp and macros.sexp,
expands macros, and generates C++20 constexpr headers for the native audio engine.

Pipeline:
    S-expression source → tokenize → parse → macro-expand → categorize → C++ emit

Usage:
    python generate-tokens.py
    python generate-tokens.py --tokens design-tokens.sexp --macros macros.sexp --outdir generated/
"""

import argparse
import os
import sys
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Union

# ═══════════════════════════════════════════════════════════════════════════
# Type Aliases
# ═══════════════════════════════════════════════════════════════════════════

Atom = Union[str, int, float]
SExpr = Union[Atom, list]

# ═══════════════════════════════════════════════════════════════════════════
# S-Expression Tokenizer
# ═══════════════════════════════════════════════════════════════════════════


def tokenize(text: str) -> list[str]:
    """Tokenize S-expression source text into a flat list of tokens.

    Handles: parentheses, brackets, quoted strings, ;; comments, atoms.
    """
    tokens: list[str] = []
    i = 0
    n = len(text)
    while i < n:
        c = text[i]
        if c in " \t\n\r":
            i += 1
        elif c == ";":
            while i < n and text[i] != "\n":
                i += 1
        elif c in "()[]":
            tokens.append(c)
            i += 1
        elif c == '"':
            j = i + 1
            while j < n and text[j] != '"':
                if text[j] == "\\":
                    j += 1
                j += 1
            tokens.append(text[i : j + 1])
            i = j + 1
        else:
            j = i
            while j < n and text[j] not in '()[] \t\n\r;"':
                j += 1
            tokens.append(text[i:j])
            i = j
    return tokens


# ═══════════════════════════════════════════════════════════════════════════
# S-Expression Parser
# ═══════════════════════════════════════════════════════════════════════════


def parse_atom(token: str) -> Atom:
    """Parse a single token into a Python value (str, int, or float)."""
    if token.startswith('"') and token.endswith('"'):
        return token[1:-1]
    try:
        return int(token)
    except ValueError:
        pass
    try:
        return float(token)
    except ValueError:
        pass
    return token


def parse_expr(tokens: list[str], pos: int) -> tuple[SExpr, int]:
    """Parse one S-expression from token stream. Returns (expr, new_pos)."""
    if pos >= len(tokens):
        raise SyntaxError("Unexpected end of input")

    tok = tokens[pos]
    if tok in ("(", "["):
        close = ")" if tok == "(" else "]"
        result: list[SExpr] = []
        pos += 1
        while pos < len(tokens) and tokens[pos] != close:
            expr, pos = parse_expr(tokens, pos)
            result.append(expr)
        if pos >= len(tokens):
            raise SyntaxError(f"Missing closing '{close}'")
        return result, pos + 1
    elif tok in (")", "]"):
        raise SyntaxError(f"Unexpected '{tok}'")
    else:
        return parse_atom(tok), pos + 1


def parse_all(text: str) -> list[SExpr]:
    """Parse all top-level S-expressions from source text."""
    tokens = tokenize(text)
    exprs: list[SExpr] = []
    pos = 0
    while pos < len(tokens):
        expr, pos = parse_expr(tokens, pos)
        exprs.append(expr)
    return exprs


# ═══════════════════════════════════════════════════════════════════════════
# Macro System
# ═══════════════════════════════════════════════════════════════════════════

SPLICE_TAG = "__splice__"


class Macro:
    """A macro definition: name, parameter spec, and body template."""

    def __init__(self, name: str, params: list, body: SExpr):
        self.name = name
        self.positional: list[str] = []
        self.keyword: list[str] = []
        self.rest_param: str | None = None
        self.body = body
        self._parse_params(params)

    def _parse_params(self, params: list):
        mode = "positional"
        for p in params:
            if not isinstance(p, str):
                continue
            if p == "&key":
                mode = "keyword"
            elif p == "&rest":
                mode = "rest"
            elif mode == "positional":
                self.positional.append(p)
            elif mode == "keyword":
                self.keyword.append(p)
            elif mode == "rest":
                self.rest_param = p


class MacroRegistry:
    """Registry of macro definitions with recursive expansion."""

    def __init__(self):
        self.macros: dict[str, Macro] = {}

    def load(self, exprs: list[SExpr]):
        """Load defmacro forms from parsed S-expressions."""
        for expr in exprs:
            if not isinstance(expr, list) or len(expr) < 4:
                continue
            if expr[0] != "defmacro":
                continue
            name = expr[1]
            params = expr[2] if isinstance(expr[2], list) else []
            # Body is the last list element (skip docstrings)
            body = None
            for item in expr[3:]:
                if isinstance(item, list):
                    body = item
            if body is None and len(expr) > 3:
                body = expr[-1]
            if body is not None:
                self.macros[name] = Macro(name, params, body)

    def expand(self, expr: SExpr) -> SExpr:
        """Recursively expand macros in an expression."""
        if not isinstance(expr, list) or len(expr) == 0:
            return expr

        head = expr[0]
        if isinstance(head, str) and head in self.macros:
            macro = self.macros[head]
            bindings = self._bind_args(macro, expr[1:])
            expanded = self._substitute(macro.body, bindings)
            return self.expand(expanded)

        return [self.expand(item) if isinstance(item, list) else item for item in expr]

    def _bind_args(self, macro: Macro, args: list[SExpr]) -> dict[str, SExpr]:
        bindings: dict[str, SExpr] = {}
        arg_idx = 0

        for param in macro.positional:
            if arg_idx < len(args):
                bindings[param] = args[arg_idx]
                arg_idx += 1

        rest_args: list[SExpr] = []
        i = arg_idx
        while i < len(args):
            item = args[i]
            if isinstance(item, str) and item.startswith(":") and item[1:] in macro.keyword:
                key = item[1:]
                if i + 1 < len(args):
                    bindings[key] = args[i + 1]
                    i += 2
                else:
                    i += 1
            else:
                rest_args.append(item)
                i += 1

        if macro.rest_param:
            bindings[macro.rest_param] = rest_args

        return bindings

    def _substitute(self, template: SExpr, bindings: dict[str, SExpr]) -> SExpr:
        if isinstance(template, (int, float)):
            return template
        if isinstance(template, str):
            if not template.startswith(":") and template in bindings:
                return bindings[template]
            return template
        if not isinstance(template, list):
            return template

        # (or param default)
        if len(template) >= 3 and template[0] == "or":
            param_name = template[1]
            default_val = template[2]
            if isinstance(param_name, str) and param_name in bindings:
                return bindings[param_name]
            return self._substitute(default_val, bindings)

        # (splice list-param)
        if len(template) == 2 and template[0] == "splice":
            param_name = template[1]
            if isinstance(param_name, str) and param_name in bindings:
                val = bindings[param_name]
                if isinstance(val, list):
                    return (SPLICE_TAG, val)
            return template

        result: list[SExpr] = []
        for item in template:
            sub = self._substitute(item, bindings)
            if isinstance(sub, tuple) and len(sub) == 2 and sub[0] == SPLICE_TAG:
                result.extend(sub[1])
            else:
                result.append(sub)
        return result


# ═══════════════════════════════════════════════════════════════════════════
# Token Extraction Utilities
# ═══════════════════════════════════════════════════════════════════════════


def extract_kwargs(expr: list) -> tuple[dict[str, Any], list]:
    """Extract keyword arguments and child sub-expressions from an S-expression.

    Returns (kwargs_dict, children_list).
    """
    if not isinstance(expr, list) or len(expr) < 1:
        return {}, []

    kwargs: dict[str, Any] = {}
    children: list = []
    i = 1
    while i < len(expr):
        item = expr[i]
        if isinstance(item, str) and item.startswith(":"):
            key = item[1:]
            if i + 1 < len(expr) and not (
                isinstance(expr[i + 1], str) and expr[i + 1].startswith(":")
            ):
                kwargs[key] = expr[i + 1]
                i += 2
            else:
                kwargs[key] = True
                i += 1
        elif isinstance(item, list):
            children.append(item)
            i += 1
        else:
            i += 1
    return kwargs, children


def categorize_tokens(exprs: list[SExpr]) -> dict[str, list]:
    """Group expanded tokens by their head symbol."""
    categories: dict[str, list] = {
        "amp-model": [],
        "fx-chain": [],
        "route": [],
        "theme": [],
        "master-chain": [],
    }
    for expr in exprs:
        if isinstance(expr, list) and len(expr) > 0:
            head = expr[0]
            if isinstance(head, str) and head in categories:
                categories[head].append(expr)
    return categories


# ═══════════════════════════════════════════════════════════════════════════
# C++ Code Generation Utilities
# ═══════════════════════════════════════════════════════════════════════════

HEADER_BANNER = """\
// ═══════════════════════════════════════════════════════════════════════════
// REVITHION STUDIO — Auto-generated by Token Compiler — DO NOT EDIT
// Source: design-tokens.sexp + macros.sexp
// Generated: {timestamp}
// ═══════════════════════════════════════════════════════════════════════════
"""


def banner() -> str:
    ts = datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")
    return HEADER_BANNER.format(timestamp=ts)


def to_identifier(name: str) -> str:
    """Convert a display name to PascalCase C++ identifier."""
    cleaned = str(name).replace("+", "Plus").replace("'", "").replace('"', "")
    parts = [p for p in cleaned.replace("-", " ").replace("_", " ").split() if p]
    return "".join(p.capitalize() for p in parts)


def to_const_name(name: str) -> str:
    """Convert a display name to kPascalCase constant name."""
    return "k" + to_identifier(name)


def to_snake(name: str) -> str:
    """Convert a display name to snake_case."""
    cleaned = str(name).replace("+", "plus").replace("'", "").replace('"', "")
    return "_".join(
        p.lower() for p in cleaned.replace("-", " ").replace("_", " ").split() if p
    )


def cpp_float(val: Any) -> str:
    """Format as C++ float literal."""
    try:
        f = float(val)
    except (TypeError, ValueError):
        return "0.0f"
    if f == int(f) and abs(f) < 1e9:
        return f"{int(f):.1f}f"
    return f"{f}f"


def cpp_string(val: Any) -> str:
    return f'"{val}"'


def cpp_uint(val: Any) -> str:
    try:
        return str(int(val))
    except (TypeError, ValueError):
        return "0"


def cpp_bool(val: Any) -> str:
    if isinstance(val, bool):
        return "true" if val else "false"
    if isinstance(val, str):
        return "true" if val.lower() in ("true", "yes", "1") else "false"
    return "true" if val else "false"


# ═══════════════════════════════════════════════════════════════════════════
# Block Type Mappings (for FxChains, Routing, Mastering)
# ═══════════════════════════════════════════════════════════════════════════

BLOCK_TYPE_MAP: dict[str, tuple[str, int]] = {
    "reverb": ("Reverb", 0),
    "comp": ("Compressor", 1),
    "compressor": ("Compressor", 1),
    "gate": ("Gate", 2),
    "eq": ("EQ", 3),
    "delay": ("Delay", 4),
    "chorus": ("Chorus", 5),
    "tremolo": ("Tremolo", 6),
    "amp": ("AmpSim", 7),
    "amp-sim": ("AmpSim", 7),
    "cab": ("CabSim", 8),
    "cab-sim": ("CabSim", 8),
    "overdrive": ("Overdrive", 9),
    "od": ("Overdrive", 9),
    "limiter": ("Limiter", 10),
    "pitch-shift": ("PitchShift", 11),
    "stereo-enhance": ("StereoEnhance", 12),
    "de-esser": ("DeEsser", 13),
    "flanger": ("Flanger", 14),
    "mixer": ("Mixer", 15),
}

SUBTYPE_MAP: dict[str, int] = {
    # Reverb types
    "plate": 0, "hall": 1, "room": 2, "spring": 3, "shimmer": 4, "chamber": 5,
    # EQ types
    "parametric": 0, "graphic": 1, "linear-phase": 2, "high-pass": 3, "low-pass": 4, "shelf": 5,
    # Compressor types
    "standard": 0, "multiband": 1, "opto": 2, "fet": 3, "vca": 4,
    # Delay types
    "stereo": 0, "ping-pong": 1, "tape": 2, "analog": 3, "digital": 4,
    # Limiter types
    "irc5": 0, "brickwall": 1,
}

# Maps block type → (keyword → param slot index)
PARAM_SLOTS: dict[str, dict[str, int]] = {
    "reverb": {"decay": 0, "damping": 1, "pre-delay": 2, "mix": 3},
    "comp": {"threshold": 0, "ratio": 1, "attack": 2, "release": 3, "knee": 4, "makeup": 5, "bands": 6},
    "compressor": {"threshold": 0, "ratio": 1, "attack": 2, "release": 3, "knee": 4, "makeup": 5, "bands": 6},
    "gate": {"threshold": 0, "ratio": 1, "attack": 2, "release": 3},
    "eq": {"bands": 0, "freq": 1, "gain": 2, "q": 3},
    "delay": {"time": 0, "feedback": 1, "mix": 2, "filter-hp": 3, "filter-lp": 4, "time-l": 5, "time-r": 6},
    "chorus": {"rate": 0, "depth": 1, "mix": 2},
    "tremolo": {"rate": 0, "depth": 1, "mix": 2},
    "amp": {"gain": 0, "bass": 1, "mid": 2, "treble": 3, "presence": 4, "master": 5},
    "amp-sim": {"gain": 0, "bass": 1, "mid": 2, "treble": 3, "presence": 4, "master": 5},
    "cab": {"mic": 0, "position": 1},
    "cab-sim": {"mic": 0, "position": 1},
    "overdrive": {"drive": 0, "tone": 1, "level": 2},
    "od": {"drive": 0, "tone": 1, "level": 2},
    "limiter": {"ceiling": 0, "release": 1},
    "pitch-shift": {"semitones": 0, "mix": 1},
    "stereo-enhance": {"width": 0},
    "de-esser": {"threshold": 0, "freq": 1},
    "flanger": {"rate": 0, "depth": 1, "feedback": 2, "mix": 3},
    "mixer": {},
}

PARAM_SLOT_NAMES: dict[str, list[str]] = {}
for _btype, _slots in PARAM_SLOTS.items():
    ordered = [""] * 8
    for _name, _idx in _slots.items():
        if _idx < 8:
            ordered[_idx] = _name
    PARAM_SLOT_NAMES[_btype] = ordered


def block_to_cpp(block_expr: list) -> dict[str, Any]:
    """Convert a block S-expression to a dict of C++ field values."""
    if not isinstance(block_expr, list) or len(block_expr) == 0:
        return {"type_enum": "Reverb", "type_id": 0, "subtype": 0, "params": [0.0] * 8}

    head = str(block_expr[0])
    type_enum, type_id = BLOCK_TYPE_MAP.get(head, ("Reverb", 0))
    kwargs, _ = extract_kwargs(block_expr)

    subtype_val = 0
    if "type" in kwargs:
        subtype_val = SUBTYPE_MAP.get(str(kwargs["type"]), 0)

    params = [0.0] * 8
    slots = PARAM_SLOTS.get(head, {})
    for key, slot_idx in slots.items():
        if key in kwargs and slot_idx < 8:
            try:
                params[slot_idx] = float(kwargs[key])
            except (TypeError, ValueError):
                pass

    return {
        "type_enum": type_enum,
        "type_id": type_id,
        "subtype": subtype_val,
        "params": params,
    }


# ═══════════════════════════════════════════════════════════════════════════
# C++ Header Generators
# ═══════════════════════════════════════════════════════════════════════════


def generate_amp_models_h(amps: list, outdir: Path) -> list[str]:
    """Generate AmpModels.h — constexpr amp model definitions."""
    lines = [banner(), "#pragma once\n"]
    lines.append("#include <array>")
    lines.append("#include <cstdint>")
    lines.append("#include <string_view>")
    lines.append("")
    lines.append("namespace revithion::tokens {")
    lines.append("")

    # Collect all unique voicings
    voicings: list[str] = []
    for amp in amps:
        kwargs, _ = extract_kwargs(amp)
        v = str(kwargs.get("voicing", "unknown"))
        if v not in voicings:
            voicings.append(v)

    lines.append("enum class AmpVoicing : uint8_t {")
    for i, v in enumerate(voicings):
        lines.append(f"    {to_identifier(v)} = {i},")
    lines.append("};")
    lines.append("")

    lines.append("struct AmpModel {")
    lines.append("    std::string_view name;")
    lines.append("    AmpVoicing voicing;")
    lines.append("    uint8_t gain_stages;")
    lines.append("    uint8_t eq_bands;")
    lines.append("    float gain;")
    lines.append("    float bass;")
    lines.append("    float mid;")
    lines.append("    float treble;")
    lines.append("    float presence;")
    lines.append("    float master;")
    lines.append("};")
    lines.append("")

    const_names: list[str] = []
    for amp in amps:
        kwargs, _ = extract_kwargs(amp)
        name = str(kwargs.get("name", "Unknown"))
        cname = to_const_name(name)
        const_names.append(cname)
        voicing_id = to_identifier(str(kwargs.get("voicing", "unknown")))

        lines.append(f"inline constexpr AmpModel {cname} {{")
        lines.append(f"    {cpp_string(name)},")
        lines.append(f"    AmpVoicing::{voicing_id},")
        lines.append(f"    {cpp_uint(kwargs.get('gain-stages', 3))},")
        lines.append(f"    {cpp_uint(kwargs.get('eq-bands', 3))},")
        lines.append(f"    {cpp_float(kwargs.get('gain', 5.0))},")
        lines.append(f"    {cpp_float(kwargs.get('bass', 5.0))},")
        lines.append(f"    {cpp_float(kwargs.get('mid', 5.0))},")
        lines.append(f"    {cpp_float(kwargs.get('treble', 5.0))},")
        lines.append(f"    {cpp_float(kwargs.get('presence', 0.5))},")
        lines.append(f"    {cpp_float(kwargs.get('master', 5.0))},")
        lines.append("};")
        lines.append("")

    lines.append(
        f"inline constexpr std::array<AmpModel, {len(amps)}> kAllAmpModels = {{{{"
    )
    for cn in const_names:
        lines.append(f"    {cn},")
    lines.append("}};")
    lines.append("")
    lines.append("} // namespace revithion::tokens")
    lines.append("")

    (outdir / "AmpModels.h").write_text("\n".join(lines))
    return const_names


def generate_fx_chains_h(chains: list, outdir: Path) -> list[str]:
    """Generate FxChains.h — constexpr effect chain definitions."""
    lines = [banner(), "#pragma once\n"]
    lines.append("#include <array>")
    lines.append("#include <cstdint>")
    lines.append("#include <string_view>")
    lines.append("")
    lines.append("namespace revithion::tokens {")
    lines.append("")

    # Block type enum
    block_types_ordered = [
        "Reverb", "Compressor", "Gate", "EQ", "Delay", "Chorus", "Tremolo",
        "AmpSim", "CabSim", "Overdrive", "Limiter", "PitchShift",
        "StereoEnhance", "DeEsser", "Flanger", "Mixer",
    ]
    lines.append("enum class FxBlockType : uint8_t {")
    for i, bt in enumerate(block_types_ordered):
        lines.append(f"    {bt} = {i},")
    lines.append("};")
    lines.append("")

    # Subtype enums
    lines.append("enum class ReverbSubtype : uint8_t { Plate, Hall, Room, Spring, Shimmer, Chamber };")
    lines.append("enum class EqSubtype : uint8_t { Parametric, Graphic, LinearPhase, HighPass, LowPass, Shelf };")
    lines.append("enum class CompSubtype : uint8_t { Standard, Multiband, Opto, FET, VCA };")
    lines.append("enum class DelaySubtype : uint8_t { Stereo, PingPong, Tape, Analog, Digital };")
    lines.append("enum class LimiterSubtype : uint8_t { IRC5, Brickwall };")
    lines.append("")

    # FxBlock struct
    lines.append("struct FxBlock {")
    lines.append("    FxBlockType type;")
    lines.append("    uint8_t subtype;")
    lines.append("    float params[8];")
    lines.append("};")
    lines.append("")

    lines.append("inline constexpr size_t kMaxChainBlocks = 16;")
    lines.append("")

    lines.append("enum class ChainTopology : uint8_t { Serial = 0, Parallel = 1 };")
    lines.append("")

    lines.append("struct FxChain {")
    lines.append("    std::string_view name;")
    lines.append("    ChainTopology topology;")
    lines.append("    size_t block_count;")
    lines.append("    FxBlock blocks[kMaxChainBlocks];")
    lines.append("};")
    lines.append("")

    const_names: list[str] = []
    for chain in chains:
        kwargs, children = extract_kwargs(chain)
        name = str(kwargs.get("name", "Unknown"))
        cname = to_const_name(name)
        const_names.append(cname)
        topology = str(kwargs.get("topology", "serial"))
        topo_enum = "Serial" if topology != "parallel" else "Parallel"

        blocks_data = [block_to_cpp(child) for child in children]
        bc = len(blocks_data)

        lines.append(f"inline constexpr FxChain {cname} {{")
        lines.append(f"    {cpp_string(name)},")
        lines.append(f"    ChainTopology::{topo_enum},")
        lines.append(f"    {bc},")
        lines.append("    {")
        for bd in blocks_data:
            pstr = ", ".join(cpp_float(p) for p in bd["params"])
            lines.append(
                f"        {{FxBlockType::{bd['type_enum']}, {bd['subtype']}, {{{pstr}}}}},"
            )
        # Pad remaining slots with empty blocks
        for _ in range(bc, 16):
            lines.append("        {FxBlockType::Reverb, 0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},")
        lines.append("    },")
        lines.append("};")
        lines.append("")

    lines.append(
        f"inline constexpr std::array<FxChain, {len(chains)}> kAllFxChains = {{{{"
    )
    for cn in const_names:
        lines.append(f"    {cn},")
    lines.append("}};")
    lines.append("")
    lines.append("} // namespace revithion::tokens")
    lines.append("")

    (outdir / "FxChains.h").write_text("\n".join(lines))
    return const_names


def generate_routing_presets_h(routes: list, outdir: Path) -> list[str]:
    """Generate RoutingPresets.h — constexpr routing grid presets."""
    lines = [banner(), "#pragma once\n"]
    lines.append('#include "FxChains.h"')
    lines.append("")
    lines.append("#include <array>")
    lines.append("#include <cstdint>")
    lines.append("#include <string_view>")
    lines.append("")
    lines.append("namespace revithion::tokens {")
    lines.append("")

    lines.append("inline constexpr size_t kMaxRouteBlocks = 16;")
    lines.append("")

    lines.append("struct RoutingPreset {")
    lines.append("    std::string_view name;")
    lines.append("    uint8_t columns;")
    lines.append("    uint8_t rows;")
    lines.append("    uint8_t input_row;")
    lines.append("    uint8_t output_row;")
    lines.append("    size_t block_count;")
    lines.append("    FxBlock blocks[kMaxRouteBlocks];")
    lines.append("};")
    lines.append("")

    const_names: list[str] = []
    for route in routes:
        kwargs, _ = extract_kwargs(route)
        name = str(kwargs.get("name", "Unknown"))
        cname = to_const_name(name)
        const_names.append(cname)

        grid = str(kwargs.get("grid", "12x4"))
        if "x" in grid:
            parts = grid.split("x")
            cols, rows = int(parts[0]), int(parts[1])
        else:
            cols, rows = 12, 4

        input_row = int(kwargs.get("input", 1))
        output_row = int(kwargs.get("output", 1))

        block_list = kwargs.get("blocks", [])
        if not isinstance(block_list, list):
            block_list = []
        blocks_data = [block_to_cpp(b) for b in block_list if isinstance(b, list)]
        bc = len(blocks_data)

        lines.append(f"inline constexpr RoutingPreset {cname} {{")
        lines.append(f"    {cpp_string(name)},")
        lines.append(f"    {cols},")
        lines.append(f"    {rows},")
        lines.append(f"    {input_row},")
        lines.append(f"    {output_row},")
        lines.append(f"    {bc},")
        lines.append("    {")
        for bd in blocks_data:
            pstr = ", ".join(cpp_float(p) for p in bd["params"])
            lines.append(
                f"        {{FxBlockType::{bd['type_enum']}, {bd['subtype']}, {{{pstr}}}}},"
            )
        for _ in range(bc, 16):
            lines.append("        {FxBlockType::Reverb, 0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},")
        lines.append("    },")
        lines.append("};")
        lines.append("")

    lines.append(
        f"inline constexpr std::array<RoutingPreset, {len(routes)}> kAllRoutingPresets = {{{{"
    )
    for cn in const_names:
        lines.append(f"    {cn},")
    lines.append("}};")
    lines.append("")
    lines.append("} // namespace revithion::tokens")
    lines.append("")

    (outdir / "RoutingPresets.h").write_text("\n".join(lines))
    return const_names


def hex_to_rgb(hex_str: str) -> tuple[int, int, int]:
    """Parse '#rrggbb' hex color string to (r, g, b) tuple."""
    h = str(hex_str).lstrip("#")
    if len(h) != 6:
        return (0, 0, 0)
    return (int(h[0:2], 16), int(h[2:4], 16), int(h[4:6], 16))


def generate_theme_tokens_h(themes: list, outdir: Path) -> list[str]:
    """Generate ThemeTokens.h — constexpr UI theme definitions."""
    lines = [banner(), "#pragma once\n"]
    lines.append("#include <array>")
    lines.append("#include <cstdint>")
    lines.append("#include <string_view>")
    lines.append("")
    lines.append("namespace revithion::tokens {")
    lines.append("")

    lines.append("struct Color {")
    lines.append("    uint8_t r;")
    lines.append("    uint8_t g;")
    lines.append("    uint8_t b;")
    lines.append("    uint8_t a;")
    lines.append("};")
    lines.append("")

    lines.append("inline constexpr Color make_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {")
    lines.append("    return {r, g, b, a};")
    lines.append("}")
    lines.append("")

    lines.append("struct Theme {")
    lines.append("    std::string_view name;")
    lines.append("    Color bg;")
    lines.append("    Color fg;")
    lines.append("    Color accent;")
    lines.append("    Color border;")
    lines.append("    Color surface;")
    lines.append("    std::string_view font;")
    lines.append("};")
    lines.append("")

    const_names: list[str] = []
    for theme in themes:
        kwargs, _ = extract_kwargs(theme)
        name = str(kwargs.get("name", "Unknown"))
        cname = to_const_name(name)
        const_names.append(cname)

        def color_literal(hex_val: str) -> str:
            r, g, b = hex_to_rgb(hex_val)
            return f"make_color({r}, {g}, {b})"

        lines.append(f"inline constexpr Theme {cname} {{")
        lines.append(f"    {cpp_string(name)},")
        lines.append(f"    {color_literal(str(kwargs.get('bg', '#000000')))},")
        lines.append(f"    {color_literal(str(kwargs.get('fg', '#ffffff')))},")
        lines.append(f"    {color_literal(str(kwargs.get('accent', '#ff0000')))},")
        lines.append(f"    {color_literal(str(kwargs.get('border', '#333333')))},")
        lines.append(f"    {color_literal(str(kwargs.get('surface', '#222222')))},")
        lines.append(f"    {cpp_string(kwargs.get('font', 'Inter'))},")
        lines.append("};")
        lines.append("")

    lines.append(
        f"inline constexpr std::array<Theme, {len(themes)}> kAllThemes = {{{{"
    )
    for cn in const_names:
        lines.append(f"    {cn},")
    lines.append("}};")
    lines.append("")
    lines.append("} // namespace revithion::tokens")
    lines.append("")

    (outdir / "ThemeTokens.h").write_text("\n".join(lines))
    return const_names


def generate_mastering_chains_h(masters: list, outdir: Path) -> list[str]:
    """Generate MasteringChains.h — constexpr mastering chain presets."""
    lines = [banner(), "#pragma once\n"]
    lines.append('#include "FxChains.h"')
    lines.append("")
    lines.append("#include <array>")
    lines.append("#include <cstdint>")
    lines.append("#include <string_view>")
    lines.append("")
    lines.append("namespace revithion::tokens {")
    lines.append("")

    lines.append("inline constexpr size_t kMaxMasterBlocks = 16;")
    lines.append("")

    lines.append("struct MasteringChain {")
    lines.append("    std::string_view name;")
    lines.append("    float target_lufs;")
    lines.append("    bool dithering;")
    lines.append("    uint32_t sample_rate;")
    lines.append("    uint8_t bit_depth;")
    lines.append("    size_t block_count;")
    lines.append("    FxBlock blocks[kMaxMasterBlocks];")
    lines.append("};")
    lines.append("")

    const_names: list[str] = []
    for master in masters:
        kwargs, children = extract_kwargs(master)
        name = str(kwargs.get("name", "Unknown"))
        cname = to_const_name(name)
        const_names.append(cname)

        blocks_data = [block_to_cpp(child) for child in children]
        bc = len(blocks_data)

        lines.append(f"inline constexpr MasteringChain {cname} {{")
        lines.append(f"    {cpp_string(name)},")
        lines.append(f"    {cpp_float(kwargs.get('target-lufs', -14.0))},")
        lines.append(f"    {cpp_bool(kwargs.get('dithering', True))},")
        lines.append(f"    {cpp_uint(kwargs.get('sample-rate', 96000))},")
        lines.append(f"    {cpp_uint(kwargs.get('bit-depth', 32))},")
        lines.append(f"    {bc},")
        lines.append("    {")
        for bd in blocks_data:
            pstr = ", ".join(cpp_float(p) for p in bd["params"])
            lines.append(
                f"        {{FxBlockType::{bd['type_enum']}, {bd['subtype']}, {{{pstr}}}}},"
            )
        for _ in range(bc, 16):
            lines.append("        {FxBlockType::Reverb, 0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},")
        lines.append("    },")
        lines.append("};")
        lines.append("")

    lines.append(
        f"inline constexpr std::array<MasteringChain, {len(masters)}> kAllMasteringChains = {{{{"
    )
    for cn in const_names:
        lines.append(f"    {cn},")
    lines.append("}};")
    lines.append("")
    lines.append("} // namespace revithion::tokens")
    lines.append("")

    (outdir / "MasteringChains.h").write_text("\n".join(lines))
    return const_names


def generate_audio_tokens_h(categories: dict, outdir: Path):
    """Generate the master AudioTokens.h header that includes all others."""
    lines = [banner(), "#pragma once\n"]
    lines.append("// Master header — includes all generated token headers")
    lines.append('#include "AmpModels.h"')
    lines.append('#include "FxChains.h"')
    lines.append('#include "RoutingPresets.h"')
    lines.append('#include "ThemeTokens.h"')
    lines.append('#include "MasteringChains.h"')
    lines.append("")
    lines.append("namespace revithion::tokens {")
    lines.append("")
    lines.append(f"inline constexpr size_t kTotalAmpModels      = {len(categories['amp-model'])};")
    lines.append(f"inline constexpr size_t kTotalFxChains        = {len(categories['fx-chain'])};")
    lines.append(f"inline constexpr size_t kTotalRoutingPresets  = {len(categories['route'])};")
    lines.append(f"inline constexpr size_t kTotalThemes          = {len(categories['theme'])};")
    lines.append(f"inline constexpr size_t kTotalMasteringChains = {len(categories['master-chain'])};")
    lines.append("")
    lines.append("} // namespace revithion::tokens")
    lines.append("")

    (outdir / "AudioTokens.h").write_text("\n".join(lines))


# ═══════════════════════════════════════════════════════════════════════════
# Main Entry Point
# ═══════════════════════════════════════════════════════════════════════════


def main():
    parser = argparse.ArgumentParser(
        description="REVITHION STUDIO — Audio Design Token Generator"
    )
    parser.add_argument(
        "--tokens", default="design-tokens.sexp", help="Path to token definitions"
    )
    parser.add_argument(
        "--macros", default="macros.sexp", help="Path to macro definitions"
    )
    parser.add_argument(
        "--outdir", default="generated", help="Output directory for C++ headers"
    )
    args = parser.parse_args()

    script_dir = Path(__file__).parent
    tokens_path = script_dir / args.tokens
    macros_path = script_dir / args.macros
    outdir = script_dir / args.outdir

    # Validate inputs
    if not tokens_path.exists():
        print(f"ERROR: Token file not found: {tokens_path}", file=sys.stderr)
        sys.exit(1)
    if not macros_path.exists():
        print(f"ERROR: Macro file not found: {macros_path}", file=sys.stderr)
        sys.exit(1)

    outdir.mkdir(parents=True, exist_ok=True)

    print("═══ REVITHION Token Compiler ═══")
    print(f"  Macros:  {macros_path}")
    print(f"  Tokens:  {tokens_path}")
    print(f"  Output:  {outdir}")
    print()

    # Phase 1: Parse macros
    print("Phase 1: Parsing macros...")
    macros_text = macros_path.read_text()
    macros_exprs = parse_all(macros_text)
    registry = MacroRegistry()
    registry.load(macros_exprs)
    print(f"  Loaded {len(registry.macros)} macros: {', '.join(registry.macros.keys())}")

    # Phase 2: Parse tokens
    print("Phase 2: Parsing tokens...")
    tokens_text = tokens_path.read_text()
    token_exprs = parse_all(tokens_text)
    print(f"  Parsed {len(token_exprs)} top-level expressions")

    # Phase 3: Expand macros
    print("Phase 3: Expanding macros...")
    expanded = [registry.expand(expr) for expr in token_exprs]
    print(f"  Expanded {len(expanded)} expressions")

    # Phase 4: Categorize
    print("Phase 4: Categorizing tokens...")
    categories = categorize_tokens(expanded)
    for cat, items in categories.items():
        print(f"  {cat}: {len(items)}")

    # Phase 5: Generate C++ headers
    print("Phase 5: Generating C++ headers...")

    amp_names = generate_amp_models_h(categories["amp-model"], outdir)
    print(f"  ✓ AmpModels.h         ({len(amp_names)} models)")

    fx_names = generate_fx_chains_h(categories["fx-chain"], outdir)
    print(f"  ✓ FxChains.h          ({len(fx_names)} chains)")

    route_names = generate_routing_presets_h(categories["route"], outdir)
    print(f"  ✓ RoutingPresets.h    ({len(route_names)} presets)")

    theme_names = generate_theme_tokens_h(categories["theme"], outdir)
    print(f"  ✓ ThemeTokens.h       ({len(theme_names)} themes)")

    master_names = generate_mastering_chains_h(categories["master-chain"], outdir)
    print(f"  ✓ MasteringChains.h   ({len(master_names)} chains)")

    generate_audio_tokens_h(categories, outdir)
    print("  ✓ AudioTokens.h       (master header)")

    print()
    print("═══ Token compilation complete ═══")
    total = sum(len(v) for v in categories.values())
    print(f"  Total tokens: {total}")
    print(f"  Generated files: {len(list(outdir.glob('*.h')))}")


if __name__ == "__main__":
    main()
