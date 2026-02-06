# AGENTS.md

> Instructions for AI coding agents working in this repository.

## Project Overview

**DevTeam6 - The Omega Tool Kit** is a full-stack monorepo containing an interactive 3D React application, a Vue 3 workflow editor, starter templates, and Python AI services. The project uses a cyberpunk-themed design system and targets deployment via GitHub Codespaces.

## Repository Structure

```
/
├── app/                  # Main React 18 + Three.js application (port 3000)
│   └── src/
│       ├── components/   # UI components (AnimatedBackground, FloatingDock, etc.)
│       ├── pages/        # Route pages (Dashboard, CodeWorkstation, Repomind, etc.)
│       ├── utils/        # Utility modules with tests (graph, math, visuals, etc.)
│       └── config/       # Configuration (sandpackTemplates)
├── projects/             # Vue 3 Workflow Editor with Vue Flow (port 5173)
│   └── src/
│       ├── components/   # Graph editor components (NodeEditor, NodePalette)
│       ├── views/        # Editor, Home, Mindmap views
│       ├── stores/       # Pinia stores (workflow)
│       └── types/        # TypeScript type definitions
├── templates/            # Codespace starter templates
│   ├── react-starter/    # React 18 + Vite + TypeScript
│   ├── vue3-starter/     # Vue 3 + Router + Pinia
│   ├── dotnet8-api/      # .NET 8 Web API + Swagger
│   ├── c-workspace/      # C devcontainer workspace
│   ├── cpp-workspace/    # C++ devcontainer workspace
│   ├── audio-dsp-workspace/       # Audio DSP devcontainer
│   └── graphics-engine-workspace/ # Graphics engine devcontainer
├── local-ai/             # Python AI services
├── context7/             # Shared agent context files
├── .github/agents/       # Agent definitions, rules, personas, logs
├── .devcontainer/        # GitHub Codespaces devcontainer config
└── vite.config.shared.ts # Shared Vite configuration factory
```

## Tech Stack

| Area | Technology |
|------|-----------|
| Main App | React 18, TypeScript, Three.js, React Three Fiber, Framer Motion, Chart.js, GSAP |
| Workflow Editor | Vue 3 (Composition API), TypeScript, Vue Flow, Pinia, Tailwind CSS |
| Build Tool | Vite 5 |
| Testing | Vitest |
| API Templates | ASP.NET Core 8.0 (C# 12) |
| Package Manager | npm |
| Node Version | >= 20.0.0 |

## Common Commands

### Root workspace (runs all sub-projects)

```bash
npm run dev              # Start all templates (React, Vue, .NET)
npm run dev:app          # Start main React app only
npm run dev:projects     # Start Vue workflow editor only
npm run install:all      # Install all dependencies across sub-projects
npm run clean            # Remove all node_modules and build outputs
```

### Main React App (`app/`)

```bash
cd app
npm install
npm run dev              # Dev server on port 3000
npm run build            # Production build
npm run test             # Run vitest tests
```

### Vue Workflow Editor (`projects/`)

```bash
cd projects
npm install
npm run dev              # Dev server
npm run build            # Type-check (vue-tsc) then build
npm run build:fast       # Build without type-check
npm run test             # Run vitest tests
npm run lint             # ESLint with auto-fix
```

## Coding Conventions

### TypeScript

- Use TypeScript strict mode across all sub-projects.
- Prefer explicit type annotations for function signatures and exported values.
- Use `interface` for object shapes and `type` for unions/intersections.

### React (`app/`)

- Use functional components with hooks exclusively; no class components.
- Performance-sensitive 3D components use `useMemo` for geometry/data and `useRef` for imperative Three.js access.
- Configuration constants (e.g., `GRID_CONFIG`, `PARTICLE_CONFIG`) are defined at module scope outside components.
- Components are organized: `components/ui/` for shared UI, `components/ide/` for IDE embeds, `pages/` for route-level pages, `utils/` for pure logic with co-located `.test.ts` files.

### Vue (`projects/`)

- Use Vue 3 Composition API with `<script setup>` syntax.
- State management via Pinia stores in `stores/`.
- Styling with Tailwind CSS; PostCSS and Autoprefixer configured.

### Testing

- Tests use Vitest and live alongside source files as `*.test.ts`.
- Run `npm run test` in the relevant sub-project directory (`app/` or `projects/`).
- Always run existing tests after making changes to utility modules to ensure nothing is broken.

### Formatting and Linting

- Starter templates use ESLint and Prettier (configs in each template directory).
- `editor.formatOnSave` is enabled in the devcontainer VS Code settings.
- Follow existing code style: consistent indentation (2 spaces), single quotes in TypeScript, semicolons.

## Design System

The project uses a cyberpunk-themed visual language.

| Token | Value | Usage |
|-------|-------|-------|
| Primary Cyan | `#00f0ff` | Primary accent, links, highlights |
| Primary Magenta | `#ff00ff` | Secondary accent, gradients |
| Accent Green | `#00ff88` | Success states, tertiary accent |
| Purple | `#7b2fff` | Depth accents, outer elements |
| Background Dark | `#0a0a1a` | Main background |
| Background Mid | `#1a0a2e` | Card/panel backgrounds |
| Text Primary | `#ffffff` | Headings, primary text |
| Text Secondary | `#888888` | Descriptions, secondary text |

Typography: JetBrains Mono / Fira Code for headings and code; system UI sans-serif for body.

## Port Assignments

| Port | Application |
|------|-------------|
| 3000 | Main React App (`app/`) |
| 5173 | Vue 3 Starter / Projects |
| 5000 | .NET 8 API |
| 5001 | .NET 8 API (HTTPS) |

## Important Notes

- The shared Vite configuration in `vite.config.shared.ts` provides a `createBuildConfig()` factory. Sub-project Vite configs should import and extend it rather than duplicating settings.
- The main React app deploys with `base: '/DevTeam6/'` -- all route paths are prefixed accordingly.
- The devcontainer requires 8 GB memory and 4 CPUs. Node 20, .NET 8, GitHub CLI, and Docker-in-Docker are provisioned automatically.
- Never commit secrets or credentials. Use `.env.local` files (gitignored) for sensitive configuration.
- When modifying utility functions in `app/src/utils/`, always run the co-located tests (`npm run test` in `app/`) to verify correctness.
