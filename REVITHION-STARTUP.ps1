# ============================================================================
# REVITHION-STARTUP.ps1 — DevTeam6 Copilot CLI Session Launcher
# ============================================================================
# Author:  SpiralCloudOmega
# Repo:    SpiralCloudOmega/DevTeam6
# 
# PURPOSE:
#   Double-click (via .bat wrapper) to instantly restore your Copilot CLI
#   development session for REVITHION STUDIO. This script:
#     1. Displays system status (RAM, GPU, disk)
#     2. Checks WSL availability and services
#     3. Launches WSL Ubuntu with the session-startup.sh companion script
#     4. That script runs `copilot --resume` to restore your last session
#
# AFTER SESSION RESTORES:
#   - Type /yolo to enable autonomous execution mode (no confirmation prompts)
#   - Type /fleet to enable parallel sub-agent dispatching
#   These must be enabled manually each session.
#
# USAGE:
#   Option A: Double-click REVITHION-STARTUP.bat (recommended)
#   Option B: Right-click this .ps1 → "Run with PowerShell"
#   Option C: powershell -ExecutionPolicy Bypass -File REVITHION-STARTUP.ps1
# ============================================================================

$ErrorActionPreference = "Continue"
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$LogFile = Join-Path $ScriptDir ".github\scripts\startup.log"

# --- Window Title ---
$Host.UI.RawUI.WindowTitle = "REVITHION STUDIO — DevTeam6"

# --- Utility: Colored output ---
function Write-Status  { param($msg) Write-Host "  [*] " -ForegroundColor Cyan    -NoNewline; Write-Host $msg }
function Write-OK      { param($msg) Write-Host "  [+] " -ForegroundColor Green   -NoNewline; Write-Host $msg }
function Write-Warn    { param($msg) Write-Host "  [!] " -ForegroundColor Yellow  -NoNewline; Write-Host $msg }
function Write-Err     { param($msg) Write-Host "  [-] " -ForegroundColor Red     -NoNewline; Write-Host $msg }
function Write-Section { param($msg) Write-Host "`n  === $msg ===" -ForegroundColor Magenta }

# ============================================================================
# ASCII BANNER
# ============================================================================
Write-Host ""
Write-Host @"
    ╔══════════════════════════════════════════════════════════════════╗
    ║                                                                  ║
    ║   ██████╗ ███████╗██╗   ██╗██╗████████╗██╗  ██╗██╗ ██████╗ ███╗ ║
    ║   ██╔══██╗██╔════╝██║   ██║██║╚══██╔══╝██║  ██║██║██╔═══██╗████║║
    ║   ██████╔╝█████╗  ██║   ██║██║   ██║   ███████║██║██║   ██║██╔██║║
    ║   ██╔══██╗██╔══╝  ╚██╗ ██╔╝██║   ██║   ██╔══██║██║██║   ██║██║╚╝║
    ║   ██║  ██║███████╗ ╚████╔╝ ██║   ██║   ██║  ██║██║╚██████╔╝██║  ║
    ║   ╚═╝  ╚═╝╚══════╝  ╚═══╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝ ╚═════╝ ╚═╝  ║
    ║                                                                  ║
    ║          S T U D I O   —   D e v T e a m 6                       ║
    ║     ALL-IN-ONE Audio / Video / 3D Production Suite               ║
    ║                                                                  ║
    ╚══════════════════════════════════════════════════════════════════╝
"@ -ForegroundColor DarkCyan

Write-Host "    Ryzen 9 5950X  |  64GB DDR4  |  RTX 3090 24GB  |  8.4TB Storage" -ForegroundColor DarkGray
Write-Host "    $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss zzz')" -ForegroundColor DarkGray
Write-Host ""

# ============================================================================
# SYSTEM STATUS
# ============================================================================
Write-Section "System Status"

# --- RAM ---
try {
    $os = Get-CimInstance Win32_OperatingSystem
    $totalGB  = [math]::Round($os.TotalVisibleMemorySize / 1MB, 1)
    $freeGB   = [math]::Round($os.FreePhysicalMemory / 1MB, 1)
    $usedGB   = [math]::Round($totalGB - $freeGB, 1)
    $pct      = [math]::Round(($usedGB / $totalGB) * 100, 0)
    if ($pct -gt 85) {
        Write-Warn "RAM: ${usedGB}GB / ${totalGB}GB used (${pct}%) — HIGH"
    } else {
        Write-OK "RAM: ${usedGB}GB / ${totalGB}GB used (${pct}%)"
    }
} catch {
    Write-Err "RAM: Could not query — $($_.Exception.Message)"
}

# --- GPU (nvidia-smi) ---
try {
    $nvidiaSmi = Get-Command nvidia-smi -ErrorAction SilentlyContinue
    if ($nvidiaSmi) {
        $gpuInfo = & nvidia-smi --query-gpu=name,memory.used,memory.total,temperature.gpu,utilization.gpu --format=csv,noheader,nounits 2>$null
        if ($gpuInfo) {
            $parts = $gpuInfo.Trim() -split ",\s*"
            $gpuName    = $parts[0]
            $memUsed    = $parts[1]
            $memTotal   = $parts[2]
            $temp       = $parts[3]
            $utilGpu    = $parts[4]
            $memFree    = [int]$memTotal - [int]$memUsed
            Write-OK "GPU: $gpuName — ${memUsed}MB / ${memTotal}MB VRAM (${memFree}MB free)"
            Write-OK "GPU: ${temp}°C | ${utilGpu}% utilization"
        }
    } else {
        Write-Warn "GPU: nvidia-smi not found in PATH"
    }
} catch {
    Write-Err "GPU: Query failed — $($_.Exception.Message)"
}

# --- Disk Usage ---
try {
    $drives = Get-PSDrive -PSProvider FileSystem | Where-Object { $_.Used -ne $null }
    foreach ($d in $drives) {
        $usedDisk = [math]::Round($d.Used / 1GB, 1)
        $freeDisk = [math]::Round($d.Free / 1GB, 1)
        $totalDisk = [math]::Round(($d.Used + $d.Free) / 1GB, 1)
        if ($totalDisk -gt 0) {
            Write-OK "Disk $($d.Name): ${usedDisk}GB / ${totalDisk}GB used (${freeDisk}GB free)"
        }
    }
} catch {
    Write-Err "Disk: Query failed"
}

# ============================================================================
# SERVICES CHECK
# ============================================================================
Write-Section "Services"

$services = @(
    @{ Name = "Ollama";     Process = "ollama";       Port = 11434 },
    @{ Name = "PostgreSQL";  Process = "postgres";     Port = 5432  },
    @{ Name = "Docker";      Process = "com.docker";   Port = $null },
    @{ Name = "Gitea";       Process = $null;          Port = 3000  }
)

foreach ($svc in $services) {
    $running = $false
    if ($svc.Process) {
        $proc = Get-Process -Name $svc.Process -ErrorAction SilentlyContinue
        if ($proc) { $running = $true }
    }
    if (-not $running -and $svc.Port) {
        $conn = Get-NetTCPConnection -LocalPort $svc.Port -ErrorAction SilentlyContinue
        if ($conn) { $running = $true }
    }
    if ($running) {
        Write-OK "$($svc.Name): Running$(if ($svc.Port) { " (port $($svc.Port))" })"
    } else {
        Write-Status "$($svc.Name): Not detected"
    }
}

# ============================================================================
# WSL CHECK
# ============================================================================
Write-Section "WSL Environment"

$wslPath = Get-Command wsl.exe -ErrorAction SilentlyContinue
if (-not $wslPath) {
    Write-Err "WSL is not installed or not in PATH. Cannot continue."
    Write-Host ""
    Write-Host "  Press any key to exit..." -ForegroundColor Yellow
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit 1
}
Write-OK "WSL: Available"

# Check default distro
try {
    $distros = wsl.exe --list --verbose 2>$null | Out-String
    if ($distros -match "Ubuntu") {
        Write-OK "WSL: Ubuntu distro found"
    } else {
        Write-Warn "WSL: Ubuntu not found in distro list"
    }
} catch {
    Write-Warn "WSL: Could not list distros"
}

# Check copilot CLI in WSL
$copilotCheck = wsl.exe -d Ubuntu -- bash -c "command -v copilot 2>/dev/null && echo FOUND || echo MISSING" 2>$null
if ($copilotCheck -match "FOUND") {
    Write-OK "Copilot CLI: Installed in WSL"
} else {
    Write-Err "Copilot CLI: Not found in WSL Ubuntu"
    Write-Host ""
    Write-Warn "Install with: gh extension install github/gh-copilot"
    Write-Host "  Press any key to exit..." -ForegroundColor Yellow
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit 1
}

# ============================================================================
# LOG TIMESTAMP
# ============================================================================
try {
    $logDir = Split-Path $LogFile -Parent
    if (-not (Test-Path $logDir)) {
        New-Item -ItemType Directory -Path $logDir -Force | Out-Null
    }
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss zzz"
    Add-Content -Path $LogFile -Value "[$timestamp] REVITHION-STARTUP.ps1 launched"
    Write-OK "Logged startup at $timestamp"
} catch {
    Write-Warn "Could not write to log file"
}

# ============================================================================
# LAUNCH WSL SESSION
# ============================================================================
Write-Section "Launching Session"
Write-Host ""
Write-Host "  Entering WSL Ubuntu → DevTeam6 repo..." -ForegroundColor White
Write-Host "  Running session-startup.sh → copilot --resume" -ForegroundColor White
Write-Host ""
Write-Host "  ┌─────────────────────────────────────────────────────────┐" -ForegroundColor DarkYellow
Write-Host "  │  AFTER SESSION RESTORES:                                │" -ForegroundColor DarkYellow
Write-Host "  │    1. Select session #1 (most recent) from the list     │" -ForegroundColor DarkYellow
Write-Host "  │    2. Type: /yolo    — enables autonomous execution     │" -ForegroundColor DarkYellow
Write-Host "  │    3. Type: /fleet   — enables parallel sub-agents      │" -ForegroundColor DarkYellow
Write-Host "  └─────────────────────────────────────────────────────────┘" -ForegroundColor DarkYellow
Write-Host ""

# Build the WSL command: run companion script if it exists, otherwise manual fallback
$wslCommand = @"
cd '/mnt/c/Users/autom/VS Code GitHub Repos/DevTeam6' && \
if [ -x '.github/scripts/session-startup.sh' ]; then
    bash .github/scripts/session-startup.sh
else
    echo '[!] session-startup.sh not found, falling back to manual launch...'
    source ~/projects/devteam6-env.sh 2>/dev/null
    copilot --resume
fi
"@

# Launch WSL interactively — this replaces the PowerShell window with the WSL session
wsl.exe -d Ubuntu -- bash -c $wslCommand

# If WSL exits, show a message
Write-Host ""
Write-Host "  Session ended. Window will close in 5 seconds..." -ForegroundColor DarkGray
Start-Sleep -Seconds 5
