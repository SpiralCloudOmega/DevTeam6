#!/bin/bash
set -e

echo "🚀 DevTeam6 Development Environment Setup"
echo "=========================================="

# Function to check if directory exists and has package.json
install_npm_dependencies() {
    local dir=$1
    local name=$2
    
    if [ -d "$dir" ] && [ -f "$dir/package.json" ]; then
        echo ""
        echo "📦 Installing dependencies for $name..."
        cd "$dir"
        npm install
        cd - > /dev/null
        echo "✅ $name dependencies installed"
    fi
}

# Install root dependencies if package.json exists
if [ -f "package.json" ]; then
    echo ""
    echo "📦 Installing root workspace dependencies..."
    npm install
    echo "✅ Root dependencies installed"
fi

# Install dependencies for existing app
install_npm_dependencies "app" "App (3D Demo)"

# Install dependencies for existing projects
install_npm_dependencies "projects" "Projects (Vue Flow)"

# Install dependencies for React starter template
install_npm_dependencies "templates/react-starter" "React Starter Template"

# Install dependencies for Vue 3 starter template
install_npm_dependencies "templates/vue3-starter" "Vue 3 Starter Template"

# Check if .NET SDK is available
if command -v dotnet &> /dev/null; then
    echo ""
    echo "🔧 Restoring .NET dependencies..."
    if [ -d "templates/dotnet8-api" ]; then
        cd templates/dotnet8-api
        dotnet restore
        cd - > /dev/null
        echo "✅ .NET 8 API dependencies restored"
    fi
else
    echo ""
    echo "⚠️  .NET SDK not found, skipping .NET restore"
fi

echo ""
echo "=========================================="
echo "✨ Setup Complete!"
echo ""
echo "Available Commands:"
echo "  npm run dev              - Run all apps concurrently"
echo "  npm run dev:react        - Run React starter template"
echo "  npm run dev:vue          - Run Vue 3 starter template"
echo "  npm run dev:dotnet       - Run .NET 8 API"
echo "  npm run dev:app          - Run existing app"
echo "  npm run dev:projects     - Run existing projects"
echo ""
echo "Port Mappings:"
echo "  3000  - App (3D Demo)"
echo "  5173  - React Starter Template"
echo "  5174  - Vue 3 Starter Template"
echo "  5000  - .NET 8 API (HTTP)"
echo "  5001  - .NET 8 API (HTTPS)"
echo ""
echo "Open devteam6-workspace.code-workspace for multi-root workspace!"
echo "=========================================="
