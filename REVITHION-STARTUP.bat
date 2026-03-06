@echo off
REM ============================================================================
REM REVITHION-STARTUP.bat — Double-click launcher for REVITHION STUDIO
REM ============================================================================
REM This batch file bypasses PowerShell execution policy and runs the
REM companion .ps1 script. Double-click this file to start your session.
REM
REM After session restores, remember to enable:
REM   /yolo  — autonomous execution (no confirmation prompts)
REM   /fleet — parallel sub-agent dispatching
REM ============================================================================

title REVITHION STUDIO — DevTeam6
powershell -ExecutionPolicy Bypass -NoProfile -File "%~dp0REVITHION-STARTUP.ps1"
