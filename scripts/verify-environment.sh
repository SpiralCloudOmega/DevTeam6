#!/bin/bash

# DevTeam6 Development Environment Verification Script
# Checks all required and optional tools for development

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
TOTAL=0
PASSED=0
FAILED=0
MISSING=0

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to check version
check_tool() {
    local tool_name=$1
    local command_name=$2
    local version_flag=${3:-"--version"}
    local required=${4:-"optional"}
    
    TOTAL=$((TOTAL + 1))
    
    if command_exists "$command_name"; then
        version=$($command_name $version_flag 2>&1 | head -n 1)
        echo -e "${GREEN}✅${NC} $tool_name: $version"
        PASSED=$((PASSED + 1))
        return 0
    else
        if [ "$required" = "required" ]; then
            echo -e "${RED}❌${NC} $tool_name: not found (REQUIRED)"
            FAILED=$((FAILED + 1))
        else
            echo -e "${YELLOW}⚠️${NC}  $tool_name: not found (optional)"
            MISSING=$((MISSING + 1))
        fi
        return 1
    fi
}

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}DevTeam6 Environment Verification${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# Python Ecosystem
echo -e "${BLUE}-------- Python --------${NC}"
check_tool "python3" "python3" "--version" "required"
check_tool "python" "python" "--version"
check_tool "pip" "pip" "--version" "required"
check_tool "pipx" "pipx" "--version"
check_tool "poetry" "poetry" "--version"
check_tool "uv" "uv" "--version"
check_tool "black" "black" "--version"
check_tool "mypy" "mypy" "--version"
check_tool "pytest" "pytest" "--version"
check_tool "ruff" "ruff" "--version"
check_tool "pyenv" "pyenv" "--version"
echo ""

# Node.js Ecosystem
echo -e "${BLUE}-------- NodeJS --------${NC}"
check_tool "node" "node" "--version" "required"
check_tool "nvm" "nvm" "--version"
check_tool "npm" "npm" "--version" "required"
check_tool "yarn" "yarn" "--version"
check_tool "pnpm" "pnpm" "--version"
check_tool "eslint" "eslint" "--version"
check_tool "prettier" "prettier" "--version"
check_tool "typescript" "tsc" "--version"
check_tool "chromedriver" "chromedriver" "--version"
echo ""

# Java Ecosystem
echo -e "${BLUE}-------- Java --------${NC}"
check_tool "java" "java" "--version"
check_tool "maven" "mvn" "--version"
check_tool "gradle" "gradle" "--version"
echo ""

# .NET Ecosystem
echo -e "${BLUE}-------- .NET --------${NC}"
check_tool "dotnet" "dotnet" "--version"
echo ""

# Go Ecosystem
echo -e "${BLUE}-------- Go --------${NC}"
check_tool "go" "go" "version"
echo ""

# Rust Ecosystem
echo -e "${BLUE}-------- Rust --------${NC}"
check_tool "rustc" "rustc" "--version"
check_tool "cargo" "cargo" "--version"
echo ""

# C/C++ Compilers
echo -e "${BLUE}-------- C/C++ Compilers --------${NC}"
check_tool "gcc" "gcc" "--version"
check_tool "g++" "g++" "--version"
check_tool "clang" "clang" "--version"
check_tool "cmake" "cmake" "--version"
check_tool "ninja" "ninja" "--version"
check_tool "make" "make" "--version"
check_tool "conan" "conan" "--version"
echo ""

# Docker
echo -e "${BLUE}-------- Docker --------${NC}"
check_tool "docker" "docker" "--version" "required"
check_tool "docker-compose" "docker-compose" "--version"
echo ""

# Kubernetes
echo -e "${BLUE}-------- Kubernetes --------${NC}"
check_tool "kubectl" "kubectl" "version --client"
check_tool "helm" "helm" "version"
check_tool "k9s" "k9s" "version"
check_tool "minikube" "minikube" "version"
echo ""

# Databases
echo -e "${BLUE}-------- Databases --------${NC}"
check_tool "psql" "psql" "--version"
check_tool "mysql" "mysql" "--version"
check_tool "sqlite3" "sqlite3" "--version"
check_tool "redis-cli" "redis-cli" "--version"

# Check Qdrant
TOTAL=$((TOTAL + 1))
if curl -s http://localhost:6333/ >/dev/null 2>&1; then
    echo -e "${GREEN}✅${NC} Qdrant: running on http://localhost:6333"
    PASSED=$((PASSED + 1))
else
    echo -e "${YELLOW}⚠️${NC}  Qdrant: not running (optional)"
    MISSING=$((MISSING + 1))
fi

# Check Ollama
TOTAL=$((TOTAL + 1))
if curl -s http://localhost:11434/ >/dev/null 2>&1; then
    echo -e "${GREEN}✅${NC} Ollama: running on http://localhost:11434"
    PASSED=$((PASSED + 1))
else
    echo -e "${YELLOW}⚠️${NC}  Ollama: not running (optional)"
    MISSING=$((MISSING + 1))
fi
echo ""

# Cloud CLIs
echo -e "${BLUE}-------- Cloud CLIs --------${NC}"
check_tool "aws" "aws" "--version"
check_tool "az" "az" "--version"
check_tool "gcloud" "gcloud" "--version"
echo ""

# Infrastructure as Code
echo -e "${BLUE}-------- Infrastructure as Code --------${NC}"
check_tool "terraform" "terraform" "--version"
check_tool "ansible" "ansible" "--version"
check_tool "pulumi" "pulumi" "version"
echo ""

# PowerShell
echo -e "${BLUE}-------- PowerShell --------${NC}"
check_tool "PowerShell 7+" "pwsh" "--version"
echo ""

# Command-Line Utilities
echo -e "${BLUE}-------- Command-Line Utilities --------${NC}"
check_tool "git" "git" "--version" "required"
check_tool "gh" "gh" "--version"
check_tool "curl" "curl" "--version" "required"
check_tool "wget" "wget" "--version"
check_tool "awk" "awk" "--version"
check_tool "sed" "sed" "--version"
check_tool "grep" "grep" "--version"
check_tool "rg (ripgrep)" "rg" "--version"
check_tool "tar" "tar" "--version"
check_tool "gzip" "gzip" "--version"
check_tool "unzip" "unzip" "-v"
check_tool "jq" "jq" "--version" "required"
check_tool "yq" "yq" "--version"
check_tool "tmux" "tmux" "-V"
echo ""

# Development Tools
echo -e "${BLUE}-------- Development Tools --------${NC}"
check_tool "code (VS Code)" "code" "--version"
check_tool "htop" "htop" "--version"
check_tool "btop" "btop" "--version"
check_tool "ncdu" "ncdu" "--version"
echo ""

# Security Tools
echo -e "${BLUE}-------- Security Tools --------${NC}"
check_tool "openssl" "openssl" "version"
check_tool "gpg" "gpg" "--version"
echo ""

# Summary
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}Summary${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "Total checks:   $TOTAL"
echo -e "${GREEN}Passed:         $PASSED${NC}"
echo -e "${YELLOW}Missing:        $MISSING (optional)${NC}"
echo -e "${RED}Failed:         $FAILED (required)${NC}"
echo ""

if [ $FAILED -gt 0 ]; then
    echo -e "${RED}⚠️  Some required tools are missing!${NC}"
    echo -e "Please install the missing required tools and run this script again."
    echo -e "See DEVELOPMENT_ENVIRONMENT.md for installation instructions."
    exit 1
else
    echo -e "${GREEN}✅ All required tools are installed!${NC}"
    if [ $MISSING -gt 0 ]; then
        echo -e "${YELLOW}ℹ️  Some optional tools are missing. Consider installing them for full functionality.${NC}"
    fi
    exit 0
fi
