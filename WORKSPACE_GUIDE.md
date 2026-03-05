# 🚀 DevTeam6 Workspace Guide

A complete full-stack development environment with React, Vue 3, and .NET 8 templates.

## 📁 Project Structure

```
DevTeam6/
├── .devcontainer/              # Codespaces configuration
│   ├── devcontainer.json       # Container settings
│   └── post-create.sh          # Auto-setup script
├── app/                        # 🔷 Existing main React app
├── projects/                   # 📊 Existing projects folder
├── templates/                  # 📦 Starter templates
│   ├── react-starter/          # 🔵 React 18 + Vite + TypeScript
│   ├── vue3-starter/           # 🟢 Vue 3 + Router + Pinia
│   └── dotnet8-api/            # 🟣 .NET 8 Web API + Swagger
├── devteam6-workspace.code-workspace  # VS Code workspace
├── package.json                # Root workspace scripts
└── WORKSPACE_GUIDE.md          # This file
```

## 🏃 Quick Start

### GitHub Codespaces (Recommended)

1. Click **Code** → **Codespaces** → **Create codespace on main**
2. Wait for setup to complete (~2-3 minutes)
3. Open `devteam6-workspace.code-workspace` when prompted
4. Run `npm run dev` to start all applications

### Local Development

```bash
# Clone the repository
git clone https://github.com/SpiralCloudOmega/DevTeam6.git
cd DevTeam6

# Install all dependencies
npm run install:all

# Open workspace in VS Code
code devteam6-workspace.code-workspace

# Start all applications
npm run dev
```

## 🛠️ Available Commands

### Running Applications

| Command | Description | Ports |
|---------|-------------|-------|
| `npm run dev` | Run ALL templates simultaneously | 3000, 5173, 5000 |
| `npm run dev:react` | React template only | 3000 |
| `npm run dev:vue` | Vue 3 template only | 5173 |
| `npm run dev:dotnet` | .NET API only | 5000 |
| `npm run dev:frontend` | React + Vue (no API) | 3000, 5173 |
| `npm run dev:app` | Main app folder | varies |
| `npm run dev:projects` | Projects folder | varies |

### Building Applications

| Command | Description |
|---------|-------------|
| `npm run build` | Build all templates |
| `npm run build:react` | Build React template only |
| `npm run build:vue` | Build Vue 3 template only |
| `npm run build:dotnet` | Build .NET API only |

### Utilities

| Command | Description |
|---------|-------------|
| `npm run install:all` | Install all dependencies |
| `npm run clean` | Clean all node_modules and build outputs |

## 📦 Template Details

### 🔵 React Starter (Port 3000)

**Location:** `templates/react-starter/`

**Stack:**
- React 18.2
- TypeScript 5.2
- Vite 5.0
- ESLint + Prettier

**Features:**
- Hot Module Replacement
- Sample Button component
- Production build configuration
- TypeScript strict mode

**Commands:**
```bash
cd templates/react-starter
npm install
npm run dev      # Start dev server
npm run build    # Production build
npm run lint     # Run linter
```

**Build Output:** 143KB (gzipped: 46KB)

### 🟢 Vue 3 Starter (Port 5173)

**Location:** `templates/vue3-starter/`

**Stack:**
- Vue 3.4 (Composition API)
- TypeScript 5.3
- Vue Router 4.2
- Pinia 2.1 (State Management)
- Vite 5.0

**Features:**
- Client-side routing
- Centralized state management
- Example views and components
- Counter store demo

**Commands:**
```bash
cd templates/vue3-starter
npm install
npm run dev      # Start dev server
npm run build    # Production build
npm run lint     # Run linter
```

**Build Output:** 94KB (gzipped: 37KB)

### 🟣 .NET 8 API (Port 5000)

**Location:** `templates/dotnet8-api/`

**Stack:**
- ASP.NET Core 8.0
- C# 12
- Swagger/OpenAPI
- CORS enabled

**Features:**
- RESTful API structure
- Automatic API documentation
- Example WeatherForecast endpoint
- Development & production settings

**Commands:**
```bash
cd templates/dotnet8-api
dotnet restore
dotnet run           # Start API
dotnet build         # Build project
dotnet watch run     # Hot reload
```

**Swagger URL:** http://localhost:5000/swagger

## 🐛 Debugging

### VS Code Debug Configurations

The workspace includes pre-configured debug setups:

**Single App Debugging:**
- 🔵 **React App** - Chrome debugger on port 3000
- 🟢 **Vue 3 App** - Chrome debugger on port 5173
- 🟣 **.NET API** - CoreCLR debugger on port 5000
- 🚀 **Full Stack** - Runs all apps with npm

**Compound Debugging:**
- 🔷 **React + .NET** - Frontend + Backend combo
- 🟢 **Vue + .NET** - Vue frontend + API
- 🎨 **All Frontends** - Both React and Vue

### How to Debug

1. Open `devteam6-workspace.code-workspace` in VS Code
2. Go to Run & Debug panel (Ctrl+Shift+D)
3. Select a configuration from the dropdown
4. Press F5 or click the green play button
5. Set breakpoints in your code

## 🌐 Port Reference

| Port | Application | URL |
|------|-------------|-----|
| 3000 | React Starter | http://localhost:3000 |
| 5173 | Vue 3 Starter | http://localhost:5173 |
| 5000 | .NET 8 API | http://localhost:5000 |
| 5001 | .NET 8 API (HTTPS) | https://localhost:5001 |

## 🔧 Workspace Features

### Multi-Root Workspace

The VS Code workspace is organized into 6 folders:
- 📁 DevTeam6 Root (workspace scripts)
- 🔷 Main App (existing React app)
- 📊 Projects (existing Vue 3 projects)
- 🔵 React Starter (template)
- 🟢 Vue 3 Starter (template)
- 🟣 .NET 8 API (template)

### Language-Specific Formatters

- **C#**: ms-dotnettools.csharp
- **Vue**: Vue.volar
- **TypeScript/React**: esbenp.prettier-vscode

### Recommended Extensions

The workspace automatically recommends:
- C# DevKit
- Vue - Official (Volar)
- ES7+ React/Redux snippets
- ESLint
- Prettier
- GitHub Copilot
- GitLens
- REST Client
- Tailwind CSS IntelliSense

## 📚 Learning Resources

### React
- [React Documentation](https://react.dev)
- [Vite Guide](https://vitejs.dev/guide/)
- [TypeScript Handbook](https://www.typescriptlang.org/docs/)

### Vue 3
- [Vue 3 Documentation](https://vuejs.org)
- [Vue Router](https://router.vuejs.org)
- [Pinia (State Management)](https://pinia.vuejs.org)

### .NET 8
- [ASP.NET Core Documentation](https://docs.microsoft.com/aspnet/core)
- [C# Documentation](https://docs.microsoft.com/dotnet/csharp/)
- [Swagger/OpenAPI](https://swagger.io/docs/)

## 🆘 Troubleshooting

### Port Already in Use

If you see "port already in use" errors:

```bash
# Find process using the port (replace 3000 with your port)
lsof -i :3000

# Kill the process
kill -9 <PID>
```

### Dependencies Not Installing

```bash
# Clear npm cache
npm cache clean --force

# Remove all node_modules
npm run clean

# Reinstall everything
npm run install:all
```

### TypeScript Errors

```bash
# React template
cd templates/react-starter
npm run build  # Check for type errors

# Vue template
cd templates/vue3-starter
npm run build  # Includes type checking
```

### .NET Build Fails

```bash
cd templates/dotnet8-api

# Clean build artifacts
dotnet clean

# Restore dependencies
dotnet restore

# Rebuild
dotnet build
```

## 🔒 Security

### Environment Variables

Never commit sensitive data. Use environment files:

```bash
# Create .env.local files (already gitignored)
echo "API_KEY=your-key-here" > .env.local
```

### CORS Configuration

The .NET API has CORS enabled for development. For production:

1. Edit `templates/dotnet8-api/Program.cs`
2. Modify the CORS policy to allow only your domains
3. Remove `AllowAnyOrigin()` and use `WithOrigins("https://yourdomain.com")`

## 🚀 Deployment

### React Template

```bash
cd templates/react-starter
npm run build
# Deploy the dist/ folder to your hosting provider
```

### Vue 3 Template

```bash
cd templates/vue3-starter
npm run build
# Deploy the dist/ folder to your hosting provider
```

### .NET API

```bash
cd templates/dotnet8-api
dotnet publish -c Release
# Deploy the bin/Release/net8.0/publish/ folder
```

## 📝 Best Practices

1. **Use the workspace file**: Open `devteam6-workspace.code-workspace` instead of individual folders
2. **Run parallel dev servers**: Use `npm run dev` to start all templates at once
3. **Commit often**: Small, focused commits are easier to review
4. **Follow TypeScript**: Leverage type safety in React and Vue templates
5. **Use Swagger**: Test .NET API endpoints via Swagger UI
6. **Debug properly**: Use VS Code debugger instead of console.log
7. **Clean builds**: Run `npm run clean` if you encounter issues

## 🤝 Contributing

1. Create a feature branch
2. Make your changes
3. Test all affected applications
4. Submit a pull request

## 📄 License

See LICENSE file in the root directory.

---

**Questions?** Check the README.md files in each template folder for more specific documentation.

**Need help?** Create an issue in the GitHub repository.

**Happy coding! 🎉**
