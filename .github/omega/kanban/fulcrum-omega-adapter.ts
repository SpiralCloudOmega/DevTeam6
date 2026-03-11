/**
 * Fulcrum Omega Adapter
 * Adds Omega Agent System as a first-class agent type in Fulcrum.
 *
 * Following the OpenCode integration pattern from fulcrum-main/cli/src/commands/opencode.ts:
 * - Config management: read → merge → backup → atomic write
 * - MCP server registration
 * - Agent session lifecycle
 *
 * Runtime: Bun | Framework: Hono | DB: SQLite via Drizzle
 */

import { readFileSync, writeFileSync, renameSync, existsSync, mkdirSync } from "fs";
import { join, resolve } from "path";
import { spawn, type ChildProcess } from "child_process";

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------

interface TerminalEntry {
  name: string;
  binary: string;
  type: "cli" | "http";
  protocol: "stdio" | "http";
  host?: string;
  port?: number;
  base_url?: string;
  capabilities: string[];
  models?: string[];
}

interface OmegaAgent {
  id: string;
  terminal: string;
  model?: string;
  status: "idle" | "busy" | "error" | "offline";
  sessionId: string | null;
  startedAt: number | null;
  lastHeartbeat: number | null;
}

interface OmegaSession {
  id: string;
  agentId: string;
  terminal: string;
  task: string;
  workspace: string;
  status: "pending" | "running" | "completed" | "failed" | "cancelled";
  createdAt: number;
  completedAt: number | null;
  result: unknown | null;
  process?: ChildProcess;
}

interface OmegaConfig {
  bridge_url: string;
  registry_path: string;
  workspace_base: string;
  max_parallel_sessions: number;
  health_interval_ms: number;
  fulcrum_board_id?: string;
}

interface OmegaHealthStatus {
  terminal: string;
  healthy: boolean;
  latencyMs: number;
  message: string;
  checkedAt: number;
}

interface FulcrumMcpEntry {
  type: string;
  command?: string;
  args?: string[];
  url?: string;
  env?: Record<string, string>;
}

interface FulcrumConfig {
  mcpServers?: Record<string, FulcrumMcpEntry>;
  agents?: Record<string, unknown>;
  [key: string]: unknown;
}

// ---------------------------------------------------------------------------
// Configuration management (Fulcrum pattern: read → merge → backup → write)
// ---------------------------------------------------------------------------

export class OmegaConfigManager {
  private configPath: string;

  constructor(fulcrumConfigPath: string) {
    this.configPath = resolve(fulcrumConfigPath);
  }

  read(): FulcrumConfig {
    if (!existsSync(this.configPath)) {
      return {};
    }
    const raw = readFileSync(this.configPath, "utf-8");
    return JSON.parse(raw) as FulcrumConfig;
  }

  backup(): string {
    const ts = new Date().toISOString().replace(/[:.]/g, "-");
    const backupPath = `${this.configPath}.backup-${ts}`;
    if (existsSync(this.configPath)) {
      const content = readFileSync(this.configPath, "utf-8");
      writeFileSync(backupPath, content, "utf-8");
    }
    return backupPath;
  }

  /** Atomic write: write to temp, then rename into place. */
  write(config: FulcrumConfig): void {
    const tmp = `${this.configPath}.tmp`;
    writeFileSync(tmp, JSON.stringify(config, null, 2) + "\n", "utf-8");
    renameSync(tmp, this.configPath);
  }

  /** Read → merge Omega entries → backup → atomic write. */
  merge(patch: Partial<FulcrumConfig>): FulcrumConfig {
    const current = this.read();
    this.backup();

    const merged: FulcrumConfig = {
      ...current,
      mcpServers: { ...current.mcpServers, ...patch.mcpServers },
      agents: { ...current.agents, ...patch.agents },
    };
    this.write(merged);
    return merged;
  }
}

// ---------------------------------------------------------------------------
// Session lifecycle management
// ---------------------------------------------------------------------------

export class OmegaSessionManager {
  private sessions = new Map<string, OmegaSession>();
  private workspaceBase: string;

  constructor(workspaceBase: string) {
    this.workspaceBase = workspaceBase;
    if (!existsSync(this.workspaceBase)) {
      mkdirSync(this.workspaceBase, { recursive: true });
    }
  }

  create(terminal: string, task: string, agentId: string): OmegaSession {
    const id = `omega-${Date.now()}-${Math.random().toString(36).slice(2, 8)}`;
    const workspace = join(this.workspaceBase, id);
    mkdirSync(workspace, { recursive: true });

    const session: OmegaSession = {
      id,
      agentId,
      terminal,
      task,
      workspace,
      status: "pending",
      createdAt: Date.now(),
      completedAt: null,
      result: null,
    };
    this.sessions.set(id, session);
    return session;
  }

  get(sessionId: string): OmegaSession | undefined {
    return this.sessions.get(sessionId);
  }

  complete(sessionId: string, result: unknown, failed = false): void {
    const session = this.sessions.get(sessionId);
    if (!session) return;
    session.status = failed ? "failed" : "completed";
    session.completedAt = Date.now();
    session.result = result;
  }

  cancel(sessionId: string): void {
    const session = this.sessions.get(sessionId);
    if (!session) return;
    if (session.process) {
      session.process.kill("SIGTERM");
    }
    session.status = "cancelled";
    session.completedAt = Date.now();
  }

  listActive(): OmegaSession[] {
    return [...this.sessions.values()].filter(
      (s) => s.status === "pending" || s.status === "running",
    );
  }
}

// ---------------------------------------------------------------------------
// Bridge client (communicates with http-bridge.py on port 18800)
// ---------------------------------------------------------------------------

export class OmegaBridgeClient {
  private baseUrl: string;

  constructor(bridgeUrl = "http://127.0.0.1:18800") {
    this.baseUrl = bridgeUrl;
  }

  /** Dispatch a task to a specific terminal via the bridge. */
  async dispatch(
    terminal: string,
    task: string,
    options: { model?: string; timeout?: number } = {},
  ): Promise<{ sessionId: string; result: unknown }> {
    const resp = await fetch(`${this.baseUrl}/api/dispatch`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        terminal,
        task,
        model: options.model,
        timeout: options.timeout ?? 120,
      }),
    });

    if (!resp.ok) {
      const body = await resp.text();
      throw new Error(`Bridge dispatch failed (${resp.status}): ${body}`);
    }
    return resp.json();
  }

  /** Poll for session result. */
  async poll(sessionId: string): Promise<{ status: string; result: unknown }> {
    const resp = await fetch(`${this.baseUrl}/api/sessions/${sessionId}`);
    if (!resp.ok) {
      throw new Error(`Bridge poll failed (${resp.status})`);
    }
    return resp.json();
  }

  /** List available terminals from the bridge's loaded registry. */
  async listTerminals(): Promise<Record<string, TerminalEntry>> {
    const resp = await fetch(`${this.baseUrl}/api/terminals`);
    if (!resp.ok) {
      throw new Error(`Bridge list-terminals failed (${resp.status})`);
    }
    return resp.json();
  }
}

// ---------------------------------------------------------------------------
// Health monitoring (SkillKit pattern)
// ---------------------------------------------------------------------------

export class OmegaHealthMonitor {
  private bridge: OmegaBridgeClient;
  private intervalHandle: ReturnType<typeof setInterval> | null = null;
  private lastStatus = new Map<string, OmegaHealthStatus>();

  constructor(bridge: OmegaBridgeClient) {
    this.bridge = bridge;
  }

  async checkTerminal(terminal: string): Promise<OmegaHealthStatus> {
    const start = Date.now();
    try {
      const resp = await fetch(
        `http://127.0.0.1:18800/api/health/${terminal}`,
      );
      const ok = resp.ok;
      const status: OmegaHealthStatus = {
        terminal,
        healthy: ok,
        latencyMs: Date.now() - start,
        message: ok ? "ok" : `HTTP ${resp.status}`,
        checkedAt: Date.now(),
      };
      this.lastStatus.set(terminal, status);
      return status;
    } catch (err) {
      const status: OmegaHealthStatus = {
        terminal,
        healthy: false,
        latencyMs: Date.now() - start,
        message: err instanceof Error ? err.message : String(err),
        checkedAt: Date.now(),
      };
      this.lastStatus.set(terminal, status);
      return status;
    }
  }

  async checkAll(): Promise<OmegaHealthStatus[]> {
    const terminals = await this.bridge.listTerminals().catch(() => ({}));
    const checks = Object.keys(terminals).map((t) => this.checkTerminal(t));
    return Promise.all(checks);
  }

  start(intervalMs = 30_000): void {
    this.stop();
    this.intervalHandle = setInterval(() => void this.checkAll(), intervalMs);
    void this.checkAll();
  }

  stop(): void {
    if (this.intervalHandle) {
      clearInterval(this.intervalHandle);
      this.intervalHandle = null;
    }
  }

  getStatus(): Map<string, OmegaHealthStatus> {
    return new Map(this.lastStatus);
  }
}

// ---------------------------------------------------------------------------
// Main adapter — registers Omega with Fulcrum
// ---------------------------------------------------------------------------

export class FulcrumOmegaAdapter {
  private config: OmegaConfigManager;
  private sessions: OmegaSessionManager;
  private bridge: OmegaBridgeClient;
  private health: OmegaHealthMonitor;

  constructor(opts: OmegaConfig) {
    this.config = new OmegaConfigManager(
      opts.registry_path.replace(/terminal-registry\.json$/, ""),
    );
    this.sessions = new OmegaSessionManager(opts.workspace_base);
    this.bridge = new OmegaBridgeClient(opts.bridge_url);
    this.health = new OmegaHealthMonitor(this.bridge);
  }

  /** Register Omega as an MCP server + agent type in Fulcrum config. */
  registerWithFulcrum(fulcrumConfigPath: string): FulcrumConfig {
    const cfgMgr = new OmegaConfigManager(fulcrumConfigPath);
    return cfgMgr.merge({
      mcpServers: {
        omega: {
          type: "stdio",
          command: "python3",
          args: [
            resolve(
              join(__dirname, "..", "bridge", "http-bridge.py"),
            ),
            "--registry",
            resolve(
              join(__dirname, "..", "bridge", "terminal-registry.json"),
            ),
          ],
          env: { OMEGA_MODE: "mcp" },
        },
      },
      agents: {
        omega: {
          type: "omega",
          display_name: "Omega Agent System",
          description:
            "Multi-terminal orchestrator — routes tasks to Gemini, OpenCode, Jules, OpenClaw, Ollama, gh, and KimiCode",
          capabilities: [
            "code-generation",
            "deep-research",
            "multi-agent",
            "local-inference",
            "github-operations",
          ],
        },
      },
    });
  }

  /** Create and dispatch a session to a terminal through the bridge. */
  async createSession(
    terminal: string,
    task: string,
    options: { model?: string; timeout?: number } = {},
  ): Promise<OmegaSession> {
    const agentId = `omega-agent-${terminal}`;
    const session = this.sessions.create(terminal, task, agentId);

    try {
      session.status = "running";
      const { result } = await this.bridge.dispatch(terminal, task, options);
      this.sessions.complete(session.id, result);
    } catch (err) {
      this.sessions.complete(session.id, { error: String(err) }, true);
    }
    return this.sessions.get(session.id)!;
  }

  /** Dispatch task and return result directly. */
  async dispatch(
    terminal: string,
    task: string,
    options: { model?: string; timeout?: number } = {},
  ): Promise<unknown> {
    const session = await this.createSession(terminal, task, options);
    if (session.status === "failed") {
      throw new Error(
        `Omega dispatch failed: ${JSON.stringify(session.result)}`,
      );
    }
    return session.result;
  }

  /** Get health of all Omega components. */
  async getHealth(): Promise<OmegaHealthStatus[]> {
    return this.health.checkAll();
  }

  /** Sync Omega task statuses with a Fulcrum kanban board. */
  async syncKanban(boardId: string): Promise<{ synced: number }> {
    const active = this.sessions.listActive();
    let synced = 0;

    for (const session of active) {
      try {
        const { status } = await this.bridge.poll(session.id);
        const kanbanStatus =
          status === "completed"
            ? "done"
            : status === "failed"
              ? "blocked"
              : status === "running"
                ? "in_progress"
                : "pending";

        await fetch(`http://127.0.0.1:18800/api/kanban/${boardId}/tasks`, {
          method: "PUT",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({
            session_id: session.id,
            terminal: session.terminal,
            task: session.task,
            status: kanbanStatus,
          }),
        });
        synced++;
      } catch {
        // Best-effort sync — skip failures silently
      }
    }
    return { synced };
  }

  /** Start background health monitoring. */
  startHealthMonitor(intervalMs = 30_000): void {
    this.health.start(intervalMs);
  }

  /** Stop health monitoring. */
  stopHealthMonitor(): void {
    this.health.stop();
  }
}

// ---------------------------------------------------------------------------
// CLI entry point
// ---------------------------------------------------------------------------

export async function registerOmegaAgent(
  fulcrumConfigPath: string,
): Promise<void> {
  const adapter = new FulcrumOmegaAdapter({
    bridge_url: "http://127.0.0.1:18800",
    registry_path: resolve(
      join(__dirname, "..", "bridge", "terminal-registry.json"),
    ),
    workspace_base: "/tmp/omega-workspaces",
    max_parallel_sessions: 20,
    health_interval_ms: 30_000,
  });

  console.log("[omega] Registering Omega agent with Fulcrum...");
  const config = adapter.registerWithFulcrum(fulcrumConfigPath);
  console.log("[omega] Fulcrum config updated. MCP server: omega");
  console.log(
    `[omega] Registered agents: ${Object.keys(config.agents ?? {}).join(", ")}`,
  );

  console.log("[omega] Checking terminal health...");
  const health = await adapter.getHealth();
  for (const h of health) {
    const icon = h.healthy ? "✓" : "✗";
    console.log(`  ${icon} ${h.terminal}: ${h.message} (${h.latencyMs}ms)`);
  }

  console.log("[omega] Omega agent registration complete.");
}

export default FulcrumOmegaAdapter;
