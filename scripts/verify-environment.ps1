# DevTeam6 Development Environment Verification Script (Windows PowerShell)
# Checks all required and optional tools for development

param(
    [switch]$Verbose
)

# Colors
$script:Red = "Red"
$script:Green = "Green"
$script:Yellow = "Yellow"
$script:Blue = "Cyan"

# Counters
$script:Total = 0
$script:Passed = 0
$script:Failed = 0
$script:Missing = 0

function Test-Command {
    param(
        [string]$CommandName
    )
    $null -ne (Get-Command $CommandName -ErrorAction SilentlyContinue)
}

function Check-Tool {
    param(
        [string]$ToolName,
        [string]$CommandName,
        [string]$VersionFlag = "--version",
        [string]$Required = "optional"
    )
    
    $script:Total++
    
    if (Test-Command $CommandName) {
        try {
            $version = & $CommandName $VersionFlag 2>&1 | Select-Object -First 1
            Write-Host "✅ " -ForegroundColor $Green -NoNewline
            Write-Host "$ToolName: $version"
            $script:Passed++
            return $true
        } catch {
            if ($Required -eq "required") {
                Write-Host "❌ " -ForegroundColor $Red -NoNewline
                Write-Host "$ToolName: not found (REQUIRED)"
                $script:Failed++
            } else {
                Write-Host "⚠️  " -ForegroundColor $Yellow -NoNewline
                Write-Host "$ToolName: not found (optional)"
                $script:Missing++
            }
            return $false
        }
    } else {
        if ($Required -eq "required") {
            Write-Host "❌ " -ForegroundColor $Red -NoNewline
            Write-Host "$ToolName: not found (REQUIRED)"
            $script:Failed++
        } else {
            Write-Host "⚠️  " -ForegroundColor $Yellow -NoNewline
            Write-Host "$ToolName: not found (optional)"
            $script:Missing++
        }
        return $false
    }
}

Write-Host "========================================" -ForegroundColor $Blue
Write-Host "DevTeam6 Environment Verification" -ForegroundColor $Blue
Write-Host "========================================" -ForegroundColor $Blue
Write-Host ""

# Python Ecosystem
Write-Host "-------- Python --------" -ForegroundColor $Blue
Check-Tool "python3" "python3" "--version" "required"
Check-Tool "python" "python" "--version"
Check-Tool "pip" "pip" "--version" "required"
Check-Tool "pipx" "pipx" "--version"
Check-Tool "poetry" "poetry" "--version"
Check-Tool "uv" "uv" "--version"
Check-Tool "black" "black" "--version"
Check-Tool "mypy" "mypy" "--version"
Check-Tool "pytest" "pytest" "--version"
Check-Tool "ruff" "ruff" "--version"
Check-Tool "pyenv" "pyenv" "--version"
Write-Host ""

# Node.js Ecosystem
Write-Host "-------- NodeJS --------" -ForegroundColor $Blue
Check-Tool "node" "node" "--version" "required"
Check-Tool "nvm" "nvm" "--version"
Check-Tool "npm" "npm" "--version" "required"
Check-Tool "yarn" "yarn" "--version"
Check-Tool "pnpm" "pnpm" "--version"
Check-Tool "eslint" "eslint" "--version"
Check-Tool "prettier" "prettier" "--version"
Check-Tool "typescript" "tsc" "--version"
Check-Tool "chromedriver" "chromedriver" "--version"
Write-Host ""

# Java Ecosystem
Write-Host "-------- Java --------" -ForegroundColor $Blue
Check-Tool "java" "java" "--version"
Check-Tool "maven" "mvn" "--version"
Check-Tool "gradle" "gradle" "--version"
Write-Host ""

# .NET Ecosystem
Write-Host "-------- .NET --------" -ForegroundColor $Blue
Check-Tool "dotnet" "dotnet" "--version"
Write-Host ""

# Go Ecosystem
Write-Host "-------- Go --------" -ForegroundColor $Blue
Check-Tool "go" "go" "version"
Write-Host ""

# Rust Ecosystem
Write-Host "-------- Rust --------" -ForegroundColor $Blue
Check-Tool "rustc" "rustc" "--version"
Check-Tool "cargo" "cargo" "--version"
Write-Host ""

# C/C++ Compilers
Write-Host "-------- C/C++ Compilers --------" -ForegroundColor $Blue
Check-Tool "gcc" "gcc" "--version"
Check-Tool "g++" "g++" "--version"
Check-Tool "clang" "clang" "--version"
Check-Tool "cmake" "cmake" "--version"
Check-Tool "ninja" "ninja" "--version"
Check-Tool "make" "make" "--version"
Check-Tool "conan" "conan" "--version"
Write-Host ""

# Docker
Write-Host "-------- Docker --------" -ForegroundColor $Blue
Check-Tool "docker" "docker" "--version" "required"
Check-Tool "docker-compose" "docker-compose" "--version"
Write-Host ""

# Kubernetes
Write-Host "-------- Kubernetes --------" -ForegroundColor $Blue
Check-Tool "kubectl" "kubectl" "version --client"
Check-Tool "helm" "helm" "version"
Check-Tool "k9s" "k9s" "version"
Check-Tool "minikube" "minikube" "version"
Write-Host ""

# Databases
Write-Host "-------- Databases --------" -ForegroundColor $Blue
Check-Tool "psql" "psql" "--version"
Check-Tool "mysql" "mysql" "--version"
Check-Tool "sqlite3" "sqlite3" "--version"
Check-Tool "redis-cli" "redis-cli" "--version"

# Check Qdrant
$script:Total++
try {
    $response = Invoke-WebRequest -Uri "http://localhost:6333/" -TimeoutSec 2 -ErrorAction SilentlyContinue
    if ($response.StatusCode -eq 200) {
        Write-Host "✅ " -ForegroundColor $Green -NoNewline
        Write-Host "Qdrant: running on http://localhost:6333"
        $script:Passed++
    }
} catch {
    Write-Host "⚠️  " -ForegroundColor $Yellow -NoNewline
    Write-Host "Qdrant: not running (optional)"
    $script:Missing++
}

# Check Ollama
$script:Total++
try {
    $response = Invoke-WebRequest -Uri "http://localhost:11434/" -TimeoutSec 2 -ErrorAction SilentlyContinue
    if ($response.StatusCode -eq 200) {
        Write-Host "✅ " -ForegroundColor $Green -NoNewline
        Write-Host "Ollama: running on http://localhost:11434"
        $script:Passed++
    }
} catch {
    Write-Host "⚠️  " -ForegroundColor $Yellow -NoNewline
    Write-Host "Ollama: not running (optional)"
    $script:Missing++
}
Write-Host ""

# Cloud CLIs
Write-Host "-------- Cloud CLIs --------" -ForegroundColor $Blue
Check-Tool "aws" "aws" "--version"
Check-Tool "az" "az" "--version"
Check-Tool "gcloud" "gcloud" "--version"
Write-Host ""

# Infrastructure as Code
Write-Host "-------- Infrastructure as Code --------" -ForegroundColor $Blue
Check-Tool "terraform" "terraform" "--version"
Check-Tool "ansible" "ansible" "--version"
Check-Tool "pulumi" "pulumi" "version"
Write-Host ""

# PowerShell
Write-Host "-------- PowerShell --------" -ForegroundColor $Blue
Check-Tool "PowerShell 7+" "pwsh" "--version"
Write-Host ""

# Command-Line Utilities
Write-Host "-------- Command-Line Utilities --------" -ForegroundColor $Blue
Check-Tool "git" "git" "--version" "required"
Check-Tool "gh" "gh" "--version"
Check-Tool "curl" "curl" "--version"
Check-Tool "wget" "wget" "--version"
Check-Tool "jq" "jq" "--version" "required"
Check-Tool "yq" "yq" "--version"
Write-Host ""

# Windows-specific Package Managers
Write-Host "-------- Windows Package Managers --------" -ForegroundColor $Blue
Check-Tool "winget" "winget" "--version"
Check-Tool "choco (Chocolatey)" "choco" "--version"
Check-Tool "scoop" "scoop" "--version"
Write-Host ""

# Development Tools
Write-Host "-------- Development Tools --------" -ForegroundColor $Blue
Check-Tool "code (VS Code)" "code" "--version"
Write-Host ""

# Security Tools
Write-Host "-------- Security Tools --------" -ForegroundColor $Blue
Check-Tool "openssl" "openssl" "version"
Check-Tool "gpg" "gpg" "--version"
Write-Host ""

# Summary
Write-Host "========================================" -ForegroundColor $Blue
Write-Host "Summary" -ForegroundColor $Blue
Write-Host "========================================" -ForegroundColor $Blue
Write-Host "Total checks:   $Total"
Write-Host "Passed:         $Passed" -ForegroundColor $Green
Write-Host "Missing:        $Missing (optional)" -ForegroundColor $Yellow
Write-Host "Failed:         $Failed (required)" -ForegroundColor $Red
Write-Host ""

if ($Failed -gt 0) {
    Write-Host "⚠️  Some required tools are missing!" -ForegroundColor $Red
    Write-Host "Please install the missing required tools and run this script again."
    Write-Host "See DEVELOPMENT_ENVIRONMENT.md for installation instructions."
    exit 1
} else {
    Write-Host "✅ All required tools are installed!" -ForegroundColor $Green
    if ($Missing -gt 0) {
        Write-Host "ℹ️  Some optional tools are missing. Consider installing them for full functionality." -ForegroundColor $Yellow
    }
    exit 0
}
