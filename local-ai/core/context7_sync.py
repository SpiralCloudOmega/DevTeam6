"""
DevTeam6 Local AI - Context7 Sync Engine

Synchronizes state with the Context7 multi-agent system in .github/agents/.
"""

from typing import Dict, Any, List, Optional
from dataclasses import dataclass
from pathlib import Path
import json
import asyncio
from datetime import datetime
import re

from config.settings import get_settings


@dataclass
class AgentState:
    """State of an agent in the Context7 system."""

    id: str
    name: str
    role: str
    status: str  # online, standby, offline
    current_task: Optional[str]
    last_sync: str
    metadata: Dict[str, Any]


@dataclass
class ProjectState:
    """Current project state from Context7."""

    active_tasks: List[Dict[str, Any]]
    recent_completions: List[str]
    pending_work: List[str]
    last_updated: str


class Context7Sync:
    """
    Sync engine for Context7 multi-agent system.

    Reads and writes to:
    - .github/agents/context7.agents.md
    - .github/agents/logs/*.log.md
    - .github/agents/memory/*.json
    """

    def __init__(self, context7_path: Optional[str] = None):
        """
        Initialize the Context7 sync engine.

        Args:
            context7_path: Path to context7.agents.md
        """
        settings = get_settings()
        self.context7_path = Path(context7_path or settings.context7_path)
        self.agents_dir = self.context7_path.parent
        self.logs_dir = self.agents_dir / "logs"
        self.memory_dir = self.agents_dir / "memory"

        self._agents: Dict[str, AgentState] = {}
        self._project_state: Optional[ProjectState] = None
        self._last_sync: Optional[datetime] = None

    def _ensure_dirs(self) -> None:
        """Ensure required directories exist."""
        self.logs_dir.mkdir(parents=True, exist_ok=True)
        self.memory_dir.mkdir(parents=True, exist_ok=True)

    async def load(self) -> None:
        """Load state from Context7 files."""
        self._ensure_dirs()

        # Parse context7.agents.md
        if self.context7_path.exists():
            content = self.context7_path.read_text()
            self._parse_context7(content)

        # Load memory files
        await self._load_memory()

        self._last_sync = datetime.utcnow()

    def _parse_context7(self, content: str) -> None:
        """Parse the context7.agents.md file."""
        # Extract agent registry table
        agent_pattern = r"\| `@(\w+)` \| ([^|]+) \| ([^|]+) \| ([^|]+) \|"
        for match in re.finditer(agent_pattern, content):
            agent_id = f"@{match.group(1)}"
            self._agents[agent_id] = AgentState(
                id=agent_id,
                name=match.group(2).strip(),
                role=match.group(3).strip(),
                status="online" if "🟢" in match.group(4) else "standby",
                current_task=None,
                last_sync=datetime.utcnow().isoformat(),
                metadata={},
            )

        # Extract project state
        tasks_pattern = r"\| `([^`]+)` \| ([^|]+) \| ([^|]+) \| ([^|]+) \| ([^|]+) \|"
        active_tasks = []
        for match in re.finditer(tasks_pattern, content):
            if match.group(1).startswith("TASK-"):
                active_tasks.append({
                    "id": match.group(1),
                    "description": match.group(2).strip(),
                    "assigned_to": match.group(3).strip(),
                    "status": match.group(4).strip(),
                    "priority": match.group(5).strip(),
                })

        self._project_state = ProjectState(
            active_tasks=active_tasks,
            recent_completions=[],
            pending_work=[],
            last_updated=datetime.utcnow().isoformat(),
        )

    async def _load_memory(self) -> None:
        """Load memory files."""
        embeddings_path = self.memory_dir / "embeddings.json"
        kg_path = self.memory_dir / "knowledge-graph.json"

        # Load embeddings metadata
        if embeddings_path.exists():
            data = json.loads(embeddings_path.read_text())
            # Process embeddings metadata if needed

        # Load knowledge graph
        if kg_path.exists():
            data = json.loads(kg_path.read_text())
            # Process knowledge graph if needed

    async def save(self) -> None:
        """Save state back to Context7 files."""
        self._ensure_dirs()

        # Update knowledge graph with agent states
        kg_path = self.memory_dir / "knowledge-graph.json"
        if kg_path.exists():
            kg_data = json.loads(kg_path.read_text())
        else:
            kg_data = {"version": "1.0.0", "nodes": [], "edges": []}

        # Update timestamp
        kg_data["updated"] = datetime.utcnow().isoformat()

        kg_path.write_text(json.dumps(kg_data, indent=2))

    def get_agent(self, agent_id: str) -> Optional[AgentState]:
        """
        Get agent state by ID.

        Args:
            agent_id: Agent ID (e.g., "@react")

        Returns:
            AgentState or None
        """
        return self._agents.get(agent_id)

    def list_agents(self) -> List[AgentState]:
        """Get all agent states."""
        return list(self._agents.values())

    async def update_agent_state(
        self,
        agent_id: str,
        updates: Dict[str, Any],
    ) -> None:
        """
        Update an agent's state.

        Args:
            agent_id: Agent ID
            updates: State updates
        """
        if agent_id in self._agents:
            agent = self._agents[agent_id]
            for key, value in updates.items():
                if hasattr(agent, key):
                    setattr(agent, key, value)
            agent.last_sync = datetime.utcnow().isoformat()

            # Log the update
            await self._log_agent_activity(agent_id, "state_update", updates)

    async def _log_agent_activity(
        self,
        agent_id: str,
        activity_type: str,
        data: Dict[str, Any],
    ) -> None:
        """Log agent activity to log files."""
        agent_name = agent_id.replace("@", "")
        log_path = self.logs_dir / f"{agent_name}-agent.log.md"

        timestamp = datetime.utcnow().isoformat()
        log_entry = f"\n[{timestamp}] [{activity_type.upper()}] {json.dumps(data)}\n"

        if log_path.exists():
            content = log_path.read_text()
            # Insert after "## Activity Log" section
            if "## Activity Log" in content:
                parts = content.split("## Activity Log", 1)
                if len(parts) == 2:
                    # Find the code block and append
                    new_content = parts[0] + "## Activity Log" + parts[1].replace(
                        "```\n", f"```\n{log_entry}", 1
                    )
                    log_path.write_text(new_content)

    def get_project_state(self) -> Optional[ProjectState]:
        """Get current project state."""
        return self._project_state

    async def sync(self) -> Dict[str, Any]:
        """
        Perform full sync with Context7.

        Returns:
            Sync status
        """
        await self.load()
        await self.save()

        return {
            "status": "success",
            "agents_synced": len(self._agents),
            "last_sync": self._last_sync.isoformat() if self._last_sync else None,
        }

    async def broadcast(self, message: str, sender: str) -> None:
        """
        Broadcast message to all agents.

        Args:
            message: Message content
            sender: Sender agent ID
        """
        for agent_id in self._agents:
            await self._log_agent_activity(
                agent_id,
                "broadcast",
                {"from": sender, "message": message},
            )

    async def handoff(
        self,
        from_agent: str,
        to_agent: str,
        task: Dict[str, Any],
    ) -> None:
        """
        Hand off a task between agents.

        Args:
            from_agent: Source agent ID
            to_agent: Target agent ID
            task: Task data
        """
        # Update source agent
        await self.update_agent_state(from_agent, {"current_task": None})

        # Update target agent
        await self.update_agent_state(to_agent, {"current_task": task.get("id")})

        # Log handoff
        await self._log_agent_activity(
            from_agent,
            "handoff_out",
            {"to": to_agent, "task": task},
        )
        await self._log_agent_activity(
            to_agent,
            "handoff_in",
            {"from": from_agent, "task": task},
        )
