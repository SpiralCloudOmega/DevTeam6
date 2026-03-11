#!/usr/bin/env bash
# Auto-generated health-check — REVITHION Omega v1.0.0
# DO NOT EDIT — regenerate with: python generate-omega.py --health
set -euo pipefail

PASS=0
FAIL=0
SKIP=0

check_agent() {
    local name="$1" port="$2"
    if [ -z "$port" ]; then
        printf "  %-25s SKIP (no port)\n" "$name"
        ((SKIP++))
        return
    fi
    if curl -sf --max-time 3 "http://localhost:$port/health" >/dev/null 2>&1; then
        printf "  %-25s \033[32mOK\033[0m\n" "$name"
        ((PASS++))
    else
        printf "  %-25s \033[31mFAIL\033[0m\n" "$name"
        ((FAIL++))
    fi
}

echo "=== Omega Health Check: REVITHION Omega v1.0.0 ==="
echo ""


echo ""
echo "--- Summary ---"
echo "  Passed: $PASS"
echo "  Failed: $FAIL"
echo "  Skipped: $SKIP"

if [ "$FAIL" -gt 0 ]; then
    exit 1
fi
