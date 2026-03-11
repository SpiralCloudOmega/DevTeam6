#!/usr/bin/env bash
# ═══════════════════════════════════════════════════════════════════════
# REVITHION STUDIO — Omega Fleet Workspace Provisioner
# ═══════════════════════════════════════════════════════════════════════
# Creates workspace directories for all agent categories under .planning/
# Each workspace gets a README.md explaining its purpose.
#
# Usage: bash .github/omega/fleet/workspace-layout.sh
# ═══════════════════════════════════════════════════════════════════════

set -euo pipefail

REPO_ROOT="$(git rev-parse --show-toplevel 2>/dev/null || pwd)"
PLANNING_DIR="${REPO_ROOT}/.planning"

echo "🔱 Omega Fleet Workspace Provisioner"
echo "═══════════════════════════════════════"

# Create category workspaces
CATEGORIES=(
  "agent-system"
  "audio"
  "critic"
  "data-ml"
  "devops"
  "documentation"
  "game-dev"
  "heartbeat"
  "infrastructure"
  "mobile"
  "omega"
  "orchestrator"
  "personal"
  "quality"
  "revithion"
  "security"
  "testing"
  "web-backend"
  "web-frontend"
)

for cat in "${CATEGORIES[@]}"; do
  dir="${PLANNING_DIR}/${cat}"
  mkdir -p "${dir}"
  echo "  ✓ Created ${dir}"
done

# Write README.md for each workspace
cat > "${PLANNING_DIR}/agent-system/README.md" << 'HEREDOC'
# Agent System Workspace

> Meta-agent workspace: context-loader, memory-keeper, session-recorder. Manages system-level context, memory persistence, and session recording.

| Property | Value |
|---|---|
| **Category** | `agent-system` |
| **Agent Count** | 3 |
| **Heartbeat** | 5s |
| **Critic** | `omega-security` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `agent-system` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/audio/README.md" << 'HEREDOC'
# Audio Workspace

> Audio DSP workspace: amp modeling, spectral analysis, mastering, mixing, MIDI. All real-time audio processing agents coordinate here.

| Property | Value |
|---|---|
| **Category** | `audio` |
| **Agent Count** | 7 |
| **Heartbeat** | 15s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `audio` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/critic/README.md" << 'HEREDOC'
# Critic Workspace

> Critic workspace: LLM Council review artifacts, scoring rubrics, consensus records. Quality gate for all agent output.

| Property | Value |
|---|---|
| **Category** | `critic` |
| **Agent Count** | 0 |
| **Heartbeat** | 30s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `critic` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/data-ml/README.md" << 'HEREDOC'
# Data Ml Workspace

> Data science and ML workspace: pandas, PyTorch, scikit-learn, GPU computing. Model training, data analysis, and inference pipelines.

| Property | Value |
|---|---|
| **Category** | `data-ml` |
| **Agent Count** | 5 |
| **Heartbeat** | 30s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `data-ml` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/devops/README.md" << 'HEREDOC'
# Devops Workspace

> DevOps workspace: Git workflows, GitHub Actions, Kubernetes, Docker, CI/CD. Infrastructure automation and deployment agents.

| Property | Value |
|---|---|
| **Category** | `devops` |
| **Agent Count** | 7 |
| **Heartbeat** | 30s |
| **Critic** | `omega-security` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `devops` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/documentation/README.md" << 'HEREDOC'
# Documentation Workspace

> Documentation workspace: technical writing, test cases, bug reports, templates. All documentation generation and review happens here.

| Property | Value |
|---|---|
| **Category** | `documentation` |
| **Agent Count** | 7 |
| **Heartbeat** | 60s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `documentation` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/game-dev/README.md" << 'HEREDOC'
# Game Dev Workspace

> Game development workspace: Unity, Unreal Engine, DragonRuby, WebAssembly. Game physics, rendering, and design agents.

| Property | Value |
|---|---|
| **Category** | `game-dev` |
| **Agent Count** | 5 |
| **Heartbeat** | 30s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `game-dev` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/heartbeat/README.md" << 'HEREDOC'
# Heartbeat Workspace

> Heartbeat workspace: health check logs, uptime records, failure tracking. System health monitoring dashboard.

| Property | Value |
|---|---|
| **Category** | `heartbeat` |
| **Agent Count** | 0 |
| **Heartbeat** | 30s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `heartbeat` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/infrastructure/README.md" << 'HEREDOC'
# Infrastructure Workspace

> Infrastructure workspace: CMake, C++20, JUCE, Vulkan, neural inference. Build systems, native code, and low-level optimization.

| Property | Value |
|---|---|
| **Category** | `infrastructure` |
| **Agent Count** | 7 |
| **Heartbeat** | 15s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `infrastructure` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/mobile/README.md" << 'HEREDOC'
# Mobile Workspace

> Mobile development workspace: Flutter, React Native, Swift, Android, NativeScript. Cross-platform and native mobile app agents.

| Property | Value |
|---|---|
| **Category** | `mobile` |
| **Agent Count** | 11 |
| **Heartbeat** | 30s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `mobile` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/omega/README.md" << 'HEREDOC'
# Omega Workspace

> Omega system workspace: orchestrator, critic, builder, fleet-commander, heartbeat, researcher, security. Core system agents.

| Property | Value |
|---|---|
| **Category** | `omega` |
| **Agent Count** | 7 |
| **Heartbeat** | 30s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `omega` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/orchestrator/README.md" << 'HEREDOC'
# Orchestrator Workspace

> Orchestrator workspace: DAG scheduling, task decomposition, fan-out/merge, retry logic. The dispatch engine control center.

| Property | Value |
|---|---|
| **Category** | `orchestrator` |
| **Agent Count** | 0 |
| **Heartbeat** | 30s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `orchestrator` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/personal/README.md" << 'HEREDOC'
# Personal Workspace

> Personal agent workspace: custom my-* agents tailored to SpiralCloudOmega's specific workflows and projects.

| Property | Value |
|---|---|
| **Category** | `personal` |
| **Agent Count** | 22 |
| **Heartbeat** | 30s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `personal` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/quality/README.md" << 'HEREDOC'
# Quality Workspace

> Code quality workspace: clean code, style consistency, SOLID principles, code review. Quality enforcement agents.

| Property | Value |
|---|---|
| **Category** | `quality` |
| **Agent Count** | 7 |
| **Heartbeat** | 60s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `quality` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/revithion/README.md" << 'HEREDOC'
# Revithion Workspace

> REVITHION-specific workspace: LISP tokens, S-expression macros, routing grids, AXE-FX modeling. Core REVITHION STUDIO agents.

| Property | Value |
|---|---|
| **Category** | `revithion` |
| **Agent Count** | 2 |
| **Heartbeat** | 15s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `revithion` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/security/README.md" << 'HEREDOC'
# Security Workspace

> Security workspace: smart contracts, Solidity, blockchain, vulnerability scanning. Security-critical review and analysis.

| Property | Value |
|---|---|
| **Category** | `security` |
| **Agent Count** | 6 |
| **Heartbeat** | 30s |
| **Critic** | `omega-security` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `security` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/testing/README.md" << 'HEREDOC'
# Testing Workspace

> Testing workspace: Cypress, Playwright, Jest, Vitest, Gherkin. All test automation and quality assurance agents.

| Property | Value |
|---|---|
| **Category** | `testing` |
| **Agent Count** | 13 |
| **Heartbeat** | 60s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `testing` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/web-backend/README.md" << 'HEREDOC'
# Web Backend Workspace

> Web backend workspace: Python, FastAPI, Django, Node.js, Go, Rails, Rust, Java, Kotlin. Server-side development agents.

| Property | Value |
|---|---|
| **Category** | `web-backend` |
| **Agent Count** | 51 |
| **Heartbeat** | 30s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `web-backend` category use this directory as their workspace root.
HEREDOC

cat > "${PLANNING_DIR}/web-frontend/README.md" << 'HEREDOC'
# Web Frontend Workspace

> Web frontend workspace: React, Next.js, Vue, Svelte, Angular, Tailwind, TypeScript. Client-side development agents.

| Property | Value |
|---|---|
| **Category** | `web-frontend` |
| **Agent Count** | 82 |
| **Heartbeat** | 30s |
| **Critic** | `omega-critic` |

## Usage

Place planning documents, scratch files, and intermediate results here.
Agents in the `web-frontend` category use this directory as their workspace root.
HEREDOC

echo ""
echo "✅ Provisioned 19 workspaces under ${PLANNING_DIR}/"
echo "   Total agents registered: 242"
