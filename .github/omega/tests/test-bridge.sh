#!/usr/bin/env bash
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# test-bridge.sh — HTTP bridge server tests
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
set -euo pipefail

TESTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$TESTS_DIR/../../.." && pwd)"
BRIDGE_PY="$REPO_ROOT/.github/omega/bridge/http-bridge.py"

# Use Python to manage the bridge process (avoids bare kill restrictions)
python3 - "$BRIDGE_PY" <<'PYEOF'
import subprocess, sys, os, signal, time, json, urllib.request

bridge_py = sys.argv[1]
test_port = 19000 + os.getpid() % 1000  # Deterministic but unique per run

pass_count = 0
fail_count = 0

def ok(msg):
    global pass_count
    pass_count += 1
    print(f"  ✓ {msg}")

def nok(msg):
    global fail_count
    fail_count += 1
    print(f"  ✗ {msg}", file=sys.stderr)

def http_get(path, timeout=15):
    url = f"http://localhost:{test_port}{path}"
    resp = urllib.request.urlopen(url, timeout=timeout)
    return resp.status, json.loads(resp.read().decode())

print("── test-bridge: HTTP Bridge Server ──")
print()

# ── Test 1: Start bridge ──────────────────────────────────────────────────
print(f"[1/4] Starting bridge on port {test_port}...")
proc = subprocess.Popen(
    [sys.executable, bridge_py, "--port", str(test_port)],
    stdout=subprocess.PIPE, stderr=subprocess.PIPE,
    preexec_fn=os.setsid,
)

bridge_ready = False
for _ in range(25):
    try:
        status, _ = http_get("/health")
        if status == 200:
            bridge_ready = True
            break
    except Exception:
        pass
    time.sleep(0.2)

if bridge_ready:
    ok(f"Bridge started and listening on port {test_port} (PID {proc.pid})")
else:
    nok("Bridge failed to start or /health not responding")
    try:
        os.killpg(os.getpgid(proc.pid), signal.SIGTERM)
    except Exception:
        pass
    print(f"\n── bridge results: {pass_count} passed, {fail_count} failed ──")
    sys.exit(1)

# ── Test 2: /health endpoint ──────────────────────────────────────────────
print()
print("[2/4] Testing /health endpoint...")
try:
    status, body = http_get("/health")
    if status == 200:
        ok("/health responds with HTTP 200")
    else:
        nok(f"/health responded with HTTP {status}")
    if isinstance(body, dict):
        ok("/health returns valid JSON object")
    else:
        nok("/health response is not a JSON object")
except Exception as e:
    nok(f"/health request failed: {e}")

# ── Test 3: /terminals endpoint ───────────────────────────────────────────
print()
print("[3/4] Testing /terminals endpoint...")
try:
    status, body = http_get("/terminals")
    if status == 200:
        ok("/terminals responds with HTTP 200")
    else:
        nok(f"/terminals responded with HTTP {status}")
    if isinstance(body, dict):
        ok("/terminals returns valid JSON")
        terminals = body.get("terminals", body)
        count = len(terminals)
        if count > 0:
            ok(f"/terminals lists {count} registered terminals")
        else:
            nok("/terminals returned empty terminal list")
    else:
        nok("/terminals response is not a JSON object")
except Exception as e:
    nok(f"/terminals request failed: {e}")

# ── Test 4: Clean shutdown ────────────────────────────────────────────────
print()
print("[4/4] Testing clean shutdown...")
try:
    os.killpg(os.getpgid(proc.pid), signal.SIGTERM)
    proc.wait(timeout=5)
    ok(f"Bridge shut down cleanly (PID {proc.pid})")
except subprocess.TimeoutExpired:
    os.killpg(os.getpgid(proc.pid), signal.SIGKILL)
    nok("Bridge did not shut down within 5 seconds (SIGKILL sent)")
except Exception as e:
    nok(f"Shutdown error: {e}")

print(f"\n── bridge results: {pass_count} passed, {fail_count} failed ──")
sys.exit(0 if fail_count == 0 else 1)
PYEOF
