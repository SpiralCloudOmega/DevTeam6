"""
DevTeam6 Local AI - Base Agent

Base class for all agents in the system.
"""

from typing import Dict, Any, Optional, List
from dataclasses import dataclass, field
from abc import ABC, abstractmethod
from datetime import datetime
import uuid


@dataclass
class AgentMessage:
    """Message between agents."""

    id: str = field(default_factory=lambda: str(uuid.uuid4()))
    from_agent: str = ""
    to_agent: str = ""
    type: str = "message"  # message, request, response, notification, handoff
    priority: str = "normal"  # low, normal, high, critical
    content: str = ""
    data: Dict[str, Any] = field(default_factory=dict)
    timestamp: str = field(default_factory=lambda: datetime.utcnow().isoformat())


@dataclass
class AgentTask:
    """Task assigned to an agent."""

    id: str = field(default_factory=lambda: str(uuid.uuid4()))
    description: str = ""
    assigned_to: str = ""
    status: str = "pending"  # pending, in_progress, completed, blocked, failed
    priority: str = "normal"
    created_at: str = field(default_factory=lambda: datetime.utcnow().isoformat())
    completed_at: Optional[str] = None
    result: Optional[Dict[str, Any]] = None


class BaseAgent(ABC):
    """
    Base class for all agents.

    Agents have:
    - Identity (id, name, role)
    - Capabilities (what they can do)
    - Rules (how they operate)
    - State (current status, tasks)
    """

    def __init__(
        self,
        agent_id: str,
        name: str,
        role: str,
        capabilities: Optional[List[str]] = None,
    ):
        """
        Initialize the agent.

        Args:
            agent_id: Unique agent ID (e.g., "@react")
            name: Display name
            role: Agent's role description
            capabilities: List of capabilities
        """
        self.id = agent_id
        self.name = name
        self.role = role
        self.capabilities = capabilities or []
        self.status = "standby"
        self.current_task: Optional[AgentTask] = None
        self._message_queue: List[AgentMessage] = []

    @property
    def is_available(self) -> bool:
        """Check if agent is available for tasks."""
        return self.status in ("standby", "online") and self.current_task is None

    @abstractmethod
    async def process_task(self, task: AgentTask) -> Dict[str, Any]:
        """
        Process an assigned task.

        Args:
            task: Task to process

        Returns:
            Task result
        """
        pass

    @abstractmethod
    async def handle_message(self, message: AgentMessage) -> Optional[AgentMessage]:
        """
        Handle an incoming message.

        Args:
            message: Incoming message

        Returns:
            Optional response message
        """
        pass

    async def receive_task(self, task: AgentTask) -> None:
        """
        Receive a task assignment.

        Args:
            task: Task to receive
        """
        self.current_task = task
        self.current_task.status = "in_progress"
        self.status = "working"

    async def complete_task(self, result: Dict[str, Any]) -> AgentTask:
        """
        Mark current task as completed.

        Args:
            result: Task result

        Returns:
            Completed task
        """
        if self.current_task:
            self.current_task.status = "completed"
            self.current_task.completed_at = datetime.utcnow().isoformat()
            self.current_task.result = result

            completed = self.current_task
            self.current_task = None
            self.status = "standby"

            return completed

        raise ValueError("No current task to complete")

    async def fail_task(self, error: str) -> AgentTask:
        """
        Mark current task as failed.

        Args:
            error: Error message

        Returns:
            Failed task
        """
        if self.current_task:
            self.current_task.status = "failed"
            self.current_task.completed_at = datetime.utcnow().isoformat()
            self.current_task.result = {"error": error}

            failed = self.current_task
            self.current_task = None
            self.status = "standby"

            return failed

        raise ValueError("No current task to fail")

    async def send_message(
        self,
        to_agent: str,
        content: str,
        msg_type: str = "message",
        data: Optional[Dict[str, Any]] = None,
    ) -> AgentMessage:
        """
        Create a message to send to another agent.

        Args:
            to_agent: Target agent ID
            content: Message content
            msg_type: Message type
            data: Additional data

        Returns:
            Created message
        """
        return AgentMessage(
            from_agent=self.id,
            to_agent=to_agent,
            type=msg_type,
            content=content,
            data=data or {},
        )

    def receive_message(self, message: AgentMessage) -> None:
        """
        Add a message to the queue.

        Args:
            message: Message to queue
        """
        self._message_queue.append(message)

    async def process_messages(self) -> List[AgentMessage]:
        """
        Process all queued messages.

        Returns:
            List of response messages
        """
        responses = []
        while self._message_queue:
            message = self._message_queue.pop(0)
            response = await self.handle_message(message)
            if response:
                responses.append(response)
        return responses

    def get_state(self) -> Dict[str, Any]:
        """Get agent's current state."""
        return {
            "id": self.id,
            "name": self.name,
            "role": self.role,
            "status": self.status,
            "capabilities": self.capabilities,
            "current_task": {
                "id": self.current_task.id,
                "description": self.current_task.description,
                "status": self.current_task.status,
            } if self.current_task else None,
            "message_queue_size": len(self._message_queue),
        }
