"""
DevTeam6 Local AI - Agent Registry

Registration and lookup for agents in the system.
"""

from typing import Dict, List, Optional, Type
from .base_agent import BaseAgent, AgentTask


class AgentRegistry:
    """
    Registry for all agents in the system.

    Provides:
    - Agent registration
    - Lookup by ID, role, or capabilities
    - Task routing
    """

    def __init__(self):
        """Initialize the agent registry."""
        self._agents: Dict[str, BaseAgent] = {}
        self._by_role: Dict[str, List[str]] = {}
        self._by_capability: Dict[str, List[str]] = {}

    def register(self, agent: BaseAgent) -> None:
        """
        Register an agent.

        Args:
            agent: Agent to register
        """
        self._agents[agent.id] = agent

        # Index by role
        if agent.role not in self._by_role:
            self._by_role[agent.role] = []
        self._by_role[agent.role].append(agent.id)

        # Index by capabilities
        for capability in agent.capabilities:
            if capability not in self._by_capability:
                self._by_capability[capability] = []
            self._by_capability[capability].append(agent.id)

    def unregister(self, agent_id: str) -> Optional[BaseAgent]:
        """
        Unregister an agent.

        Args:
            agent_id: Agent ID to unregister

        Returns:
            Removed agent or None
        """
        agent = self._agents.pop(agent_id, None)
        if agent:
            # Remove from role index
            if agent.role in self._by_role:
                self._by_role[agent.role] = [
                    aid for aid in self._by_role[agent.role] if aid != agent_id
                ]

            # Remove from capability index
            for capability in agent.capabilities:
                if capability in self._by_capability:
                    self._by_capability[capability] = [
                        aid for aid in self._by_capability[capability] if aid != agent_id
                    ]

        return agent

    def get(self, agent_id: str) -> Optional[BaseAgent]:
        """
        Get an agent by ID.

        Args:
            agent_id: Agent ID

        Returns:
            Agent or None
        """
        return self._agents.get(agent_id)

    def get_by_role(self, role: str) -> List[BaseAgent]:
        """
        Get agents by role.

        Args:
            role: Role to filter by

        Returns:
            List of matching agents
        """
        agent_ids = self._by_role.get(role, [])
        return [self._agents[aid] for aid in agent_ids if aid in self._agents]

    def get_by_capability(self, capability: str) -> List[BaseAgent]:
        """
        Get agents by capability.

        Args:
            capability: Capability to filter by

        Returns:
            List of matching agents
        """
        agent_ids = self._by_capability.get(capability, [])
        return [self._agents[aid] for aid in agent_ids if aid in self._agents]

    def get_available(self) -> List[BaseAgent]:
        """
        Get all available agents.

        Returns:
            List of available agents
        """
        return [a for a in self._agents.values() if a.is_available]

    def find_best_agent(self, task: AgentTask) -> Optional[BaseAgent]:
        """
        Find the best agent for a task.

        Uses a simple scoring system based on:
        - Availability
        - Capability match
        - Current workload

        Args:
            task: Task to assign

        Returns:
            Best matching agent or None
        """
        available = self.get_available()
        if not available:
            return None

        # Simple selection: first available
        # TODO: Add capability matching and load balancing
        return available[0]

    def list_all(self) -> List[BaseAgent]:
        """Get all registered agents."""
        return list(self._agents.values())

    def get_stats(self) -> Dict[str, any]:
        """Get registry statistics."""
        total = len(self._agents)
        available = len(self.get_available())
        working = sum(1 for a in self._agents.values() if a.status == "working")

        return {
            "total_agents": total,
            "available": available,
            "working": working,
            "roles": list(self._by_role.keys()),
            "capabilities": list(self._by_capability.keys()),
        }


# Global registry instance
_registry: Optional[AgentRegistry] = None


def get_registry() -> AgentRegistry:
    """Get the global agent registry."""
    global _registry
    if _registry is None:
        _registry = AgentRegistry()
    return _registry
