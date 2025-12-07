# DevTeam6 Workspace Guide

Welcome to the DevTeam6 multi-framework development workspace! This guide will help you navigate and use all the templates and tools available in this repository.

## 📁 Project Structure

```
DevTeam6/
├── .devcontainer/                    # DevContainer configuration
│   ├── devcontainer.json             # Container setup with all frameworks
│   └── post-create.sh                # Automated setup script
├── app/                              # 🎮 Existing 3D Demo (React + Three.js)
├── projects/                         # 📊 Existing Vue Flow project
├── local-ai/                         # 🤖 Local AI services
├── templates/                        # 🎯 Starter templates
│   ├── react-starter/                # ⚛️ React + TypeScript + Vite
│   ├── vue3-starter/                 # 🟢 Vue 3 + TypeScript + Vite
│   └── dotnet8-api/                  # 🔷 .NET 8 Web API
├── devteam6-workspace.code-workspace # VS Code multi-root workspace
├── package.json                      # Root workspace scripts
├── WORKSPACE_GUIDE.md               # This file
└── README.md                         # Main repository documentation
```

## 🚀 Quick Start

### 1. Open in GitHub Codespaces or VS Code DevContainer

This repository is pre-configured with a DevContainer that includes:
- Node.js 20
- .NET 8 SDK
- GitHub CLI
- Docker-in-Docker
- All necessary VS Code extensions

When you open in Codespaces or a DevContainer, dependencies will be automatically installed.

### 2. Open the Workspace

Open `devteam6-workspace.code-workspace` in VS Code for a multi-root workspace experience with all projects organized and ready to use.

### 3. Run Applications

From the root directory, use these commands:

```bash
# Run all applications concurrently
npm run dev

# Run individual apps
npm run dev:app          # 3D Demo on port 3000
npm run dev:projects     # Vue Flow on port 5173
npm run dev:react        # React starter on port 5173
npm run dev:vue          # Vue 3 starter on port 5174
npm run dev:dotnet       # .NET API on ports 5000/5001
```

## 🎯 Template Guides

### ⚛️ React Starter Template

**Location:** `templates/react-starter/`

A complete React 18 template with TypeScript, Vite, ESLint, and Prettier.

**Features:**
- React 18 with hooks
- TypeScript for type safety
- Vite for fast development
- Sample components
- ESLint and Prettier configured

**Getting Started:**
```bash
cd templates/react-starter
npm install
npm run dev
```

**Port:** http://localhost:5173

**Documentation:** See `templates/react-starter/README.md`

### 🟢 Vue 3 Starter Template

**Location:** `templates/vue3-starter/`

A complete Vue 3 template with TypeScript, Vue Router, Pinia, Vite, ESLint, and Prettier.

**Features:**
- Vue 3 Composition API with `<script setup>`
- TypeScript for type safety
- Vue Router for navigation
- Pinia for state management
- Vite for fast development
- Sample components and views

**Getting Started:**
```bash
cd templates/vue3-starter
npm install
npm run dev
```

**Port:** http://localhost:5174

**Documentation:** See `templates/vue3-starter/README.md`

### 🔷 .NET 8 API Template

**Location:** `templates/dotnet8-api/`

A complete .NET 8 Web API template with OpenAPI/Swagger and CORS.

**Features:**
- .NET 8 minimal APIs
- OpenAPI/Swagger documentation
- Sample controllers and models
- CORS configured
- Dependency injection ready

**Getting Started:**
```bash
cd templates/dotnet8-api
dotnet restore
dotnet run
```

**Ports:** 
- HTTP: http://localhost:5000
- HTTPS: https://localhost:5001
- Swagger: https://localhost:5001/swagger

**Documentation:** See `templates/dotnet8-api/README.md`

## 🎮 Existing Applications

### App - 3D Demo

**Location:** `app/`

Interactive 3D demonstration with React, Three.js, and various features.

**Port:** http://localhost:3000

### Projects - Vue Flow

**Location:** `projects/`

Visual workflow automation with Vue 3 and Vue Flow.

**Port:** http://localhost:5173

## 🛠️ Workspace Scripts

All scripts can be run from the root directory:

### Development
```bash
npm run dev              # Run all apps concurrently
npm run dev:app          # Run 3D Demo
npm run dev:projects     # Run Vue Flow
npm run dev:react        # Run React starter
npm run dev:vue          # Run Vue 3 starter
npm run dev:dotnet       # Run .NET API
```

### Building
```bash
npm run build            # Build all apps
npm run build:app        # Build 3D Demo
npm run build:projects   # Build Vue Flow
npm run build:react      # Build React starter
npm run build:vue        # Build Vue 3 starter
npm run build:dotnet     # Build .NET API
```

### Installation
```bash
npm run install:all      # Install all dependencies
npm run install:app      # Install app dependencies
npm run install:projects # Install projects dependencies
npm run install:react    # Install React starter dependencies
npm run install:vue      # Install Vue 3 starter dependencies
npm run restore:dotnet   # Restore .NET dependencies
```

### Maintenance
```bash
npm run clean            # Clean all build artifacts
npm run lint             # Lint all projects
npm run format           # Format all projects
```

## 🌐 Port Mappings

| Port | Application | Description |
|------|------------|-------------|
| 3000 | App | 3D Demo (React + Three.js) |
| 5173 | Projects / React | Vue Flow or React Starter |
| 5174 | Vue 3 | Vue 3 Starter Template |
| 5000 | .NET API | .NET 8 API (HTTP) |
| 5001 | .NET API | .NET 8 API (HTTPS + Swagger) |

## 🚀 VS Code Launch Configurations

The workspace includes pre-configured launch configurations:

### Single App Launch
- **🎮 Launch App (3D Demo)** - Launch the 3D demo
- **📊 Launch Projects (Vue Flow)** - Launch Vue Flow project
- **⚛️ Launch React Starter** - Launch React template
- **🟢 Launch Vue 3 Starter** - Launch Vue 3 template
- **🔷 Launch .NET 8 API** - Launch .NET API with debugger

### Compound Launch Configurations
- **🚀 Full Stack (All Apps)** - Launch all applications
- **⚛️ React + .NET** - Launch React starter + .NET API
- **🟢 Vue + .NET** - Launch Vue 3 starter + .NET API
- **🌐 All Frontends** - Launch all frontend apps

To use: Press `F5` or go to **Run and Debug** in VS Code and select a configuration.

## 🎨 Customization

### Adding New Projects

1. Create your project folder in the root or under `templates/`
2. Add folder to `devteam6-workspace.code-workspace`
3. Add scripts to root `package.json`
4. Update `.devcontainer/post-create.sh` for auto-install

### Modifying Templates

Each template is a standalone project. You can:
- Modify files directly in the template folder
- Add new dependencies with `npm install` or `dotnet add package`
- Customize configurations (Vite, TypeScript, ESLint, etc.)
- Add new components, views, controllers, etc.

### Port Conflicts

If ports are already in use, modify the port in:
- Template's `vite.config.ts` or `.NET launchSettings.json`
- Root workspace's `.devcontainer/devcontainer.json`
- VS Code workspace launch configurations

## 📚 Learning Resources

### React
- [React Documentation](https://react.dev/)
- [TypeScript Documentation](https://www.typescriptlang.org/)
- [Vite Documentation](https://vitejs.dev/)

### Vue 3
- [Vue 3 Documentation](https://vuejs.org/)
- [Vue Router Documentation](https://router.vuejs.org/)
- [Pinia Documentation](https://pinia.vuejs.org/)

### .NET 8
- [.NET Documentation](https://docs.microsoft.com/dotnet/)
- [ASP.NET Core Documentation](https://docs.microsoft.com/aspnet/core/)
- [Minimal APIs](https://docs.microsoft.com/aspnet/core/fundamentals/minimal-apis)

## 🤝 Contributing

When contributing to this repository:

1. Create a feature branch
2. Make your changes in the appropriate folder
3. Test locally using the workspace scripts
4. Run linting and formatting
5. Submit a pull request

## 📝 Best Practices

### Code Organization
- Keep templates independent and self-contained
- Use TypeScript for type safety
- Follow ESLint and Prettier configurations
- Add meaningful comments and documentation

### Development Workflow
1. Use the VS Code workspace for organized development
2. Run individual apps during development
3. Use compound launch configs for full-stack testing
4. Build all projects before committing

### Version Control
- Templates should remain starter-friendly
- Don't commit `node_modules/`, `bin/`, `obj/`, or `dist/`
- Use `.gitignore` appropriately
- Keep templates updated with latest stable versions

## 🐛 Troubleshooting

### Dependencies Not Installing
```bash
# Clean and reinstall
npm run clean
npm run install:all
```

### Port Already in Use
```bash
# Find and kill process on port (e.g., 5173)
lsof -ti:5173 | xargs kill -9
```

### .NET Build Errors
```bash
cd templates/dotnet8-api
dotnet clean
dotnet restore
dotnet build
```

### VS Code Extensions Not Working
1. Open Command Palette (`Ctrl+Shift+P` / `Cmd+Shift+P`)
2. Run "Developer: Reload Window"
3. Check "Extensions" view for installation status

## 💡 Tips

- Use `Ctrl+Shift+P` (or `Cmd+Shift+P`) to access VS Code command palette
- Use workspace search (`Ctrl+Shift+F`) to search across all projects
- Use the integrated terminal to run commands from any folder
- Leverage GitHub Copilot for AI-assisted coding
- Use the multi-root workspace to keep projects organized

## 🔗 Useful Links

- [Main Repository](https://github.com/SpiralCloudOmega/DevTeam6)
- [3D Demo Live](https://spiralcloudomega.github.io/DevTeam6/)
- [Issues](https://github.com/SpiralCloudOmega/DevTeam6/issues)
- [Pull Requests](https://github.com/SpiralCloudOmega/DevTeam6/pulls)

---

**Happy Coding! 🚀**

For more information, see the main [README.md](README.md) or individual template READMEs.
