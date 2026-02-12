# AGENTS

This file provides a concise, AI-friendly map of the DevTeam6 workspace.

## Repository Overview

DevTeam6 is a multi-project workspace that mixes web apps, templates, and
documentation. The root package.json orchestrates common workflows.

## Key Directories

- app/                React 18 + Three.js workstation UI
- projects/           Vue 3 workflow editor
- templates/          Starter templates (React, Vue, .NET, C/C++, etc.)
- local-ai/           Python AI services and utilities
- context7/           Shared agent context and registry notes

## Common Commands (root)

- npm run install:all   Install dependencies across templates
- npm run dev           Run React + Vue + .NET dev servers
- npm run dev:app       Run the workstation app
- npm run dev:projects  Run the workflow editor
- npm run build         Build React + Vue + .NET outputs
- npm run clean         Remove template node_modules and .NET artifacts

## Runtime Notes

- Node.js version: >= 20 (see package.json engines)
- Default ports:
  - 3000: React app
  - 5173: Vue app
  - 5000: .NET API

## Related References

- README.md: primary documentation hub
- context7/agents.md: shared agent registry and extra context

Last updated: 2026-02-06
