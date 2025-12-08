#!/bin/bash
set -e

echo "🚀 Setting up DevTeam6 Workspace..."

# Root dependencies
if [ -f "package.json" ]; then
  echo "📦 Installing root dependencies..."
  npm install
fi

# App dependencies
if [ -d "app" ] && [ -f "app/package.json" ]; then
  echo "📦 Installing app dependencies..."
  cd app && npm install && cd ..
fi

# Projects dependencies
if [ -d "projects" ] && [ -f "projects/package.json" ]; then
  echo "📦 Installing projects dependencies..."
  cd projects && npm install && cd ..
fi

# React template dependencies
if [ -d "templates/react-starter" ] && [ -f "templates/react-starter/package.json" ]; then
  echo "📦 Installing React template dependencies..."
  cd templates/react-starter && npm install && cd ../..
fi

# Vue 3 template dependencies
if [ -d "templates/vue3-starter" ] && [ -f "templates/vue3-starter/package.json" ]; then
  echo "📦 Installing Vue 3 template dependencies..."
  cd templates/vue3-starter && npm install && cd ../..
fi

# .NET template restore
if [ -d "templates/dotnet8-api" ] && [ -f "templates/dotnet8-api/dotnet8-api.csproj" ]; then
  echo "📦 Restoring .NET template dependencies..."
  cd templates/dotnet8-api && dotnet restore && cd ../..
fi

echo "✅ DevTeam6 Workspace setup complete!"
echo ""
echo "🚀 Quick Start Commands:"
echo "  - React App: cd app && npm run dev"
echo "  - Vue Projects: cd projects && npm run dev"
echo "  - React Template: cd templates/react-starter && npm run dev"
echo "  - Vue Template: cd templates/vue3-starter && npm run dev"
echo "  - .NET API: cd templates/dotnet8-api && dotnet run"
