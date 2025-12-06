"""
DevTeam6 Local AI - Agent Orchestrator

Coordinates task distribution and agent communication.
"""

from typing import Dict, Any, List, Optional
import asyncio
from datetime import datetime

from .base_agent import BaseAgent, AgentTask, AgentMessage
from .agent_registry import AgentRegistry, get_registry
from core.context7_sync import Context7Sync


class AgentOrchestrator:
    """
    Orchestrates multi-agent workflows.

    Responsibilities:
    - Task distribution
    - Message routing
    - Workflow coordination
    - State synchronization
    """

    def __init__(
        self,
        registry: Optional[AgentRegistry] = None,
        context7: Optional[Context7Sync] = None,
    ):
        """
        Initialize the orchestrator.

        Args:
            registry: Agent registry
            context7: Context7 sync engine
        """
        self.registry = registry or get_registry()
        self.context7 = context7 or Context7Sync()
        self._task_queue: List[AgentTask] = []
        self._completed_tasks: List[AgentTask] = []
        self._running = False

    async def start(self) -> None:
        """Start the orchestrator."""
        self._running = True
        await self.context7.load()

    async def stop(self) -> None:
        """Stop the orchestrator."""
        self._running = False
        await self.context7.save()

    async def submit_task(
        self,
        description: str,
        priority: str = "normal",
        target_agent: Optional[str] = None,
    ) -> AgentTask:
        """
        Submit a task for processing.

        Args:
            description: Task description
            priority: Task priority
            target_agent: Optional specific agent ID

        Returns:
            Created task
        """
        task = AgentTask(
            description=description,
            priority=priority,
            assigned_to=target_agent or "",
        )

        if target_agent:
            # Direct assignment
            agent = self.registry.get(target_agent)
            if agent and agent.is_available:
                await self._assign_task(task, agent)
            else:
                self._task_queue.append(task)
        else:
            # Add to queue for distribution
            self._task_queue.append(task)

        return task

    async def _assign_task(self, task: AgentTask, agent: BaseAgent) -> None:
        """Assign a task to an agent."""
        task.assigned_to = agent.id
        await agent.receive_task(task)

        # Sync with Context7
        await self.context7.update_agent_state(
            agent.id,
            {"current_task": task.id, "status": "working"},
        )

    async def process_queue(self) -> int:
        """
        Process pending tasks in the queue.

        Returns:
            Number of tasks assigned
        """
        assigned = 0
        remaining_tasks = []

        for task in self._task_queue:
            if task.assigned_to:
                agent = self.registry.get(task.assigned_to)
            else:
                agent = self.registry.find_best_agent(task)

            if agent and agent.is_available:
                await self._assign_task(task, agent)
                assigned += 1
            else:
                remaining_tasks.append(task)

        self._task_queue = remaining_tasks
        return assigned

    async def route_message(self, message: AgentMessage) -> Optional[AgentMessage]:
        """
        Route a message to its target agent.

        Args:
            message: Message to route

        Returns:
            Response message if any
        """
        if message.to_agent == "broadcast":
            # Broadcast to all agents
            responses = []
            for agent in self.registry.list_all():
                agent.receive_message(message)
                resp = await agent.handle_message(message)
                if resp:
                    responses.append(resp)
            return responses[0] if responses else None
        else:
            # Direct message
            agent = self.registry.get(message.to_agent)
            if agent:
                agent.receive_message(message)
                return await agent.handle_message(message)
        return None

    async def handoff(
        self,
        from_agent: str,
        to_agent: str,
        task: AgentTask,
        context: Optional[Dict[str, Any]] = None,
    ) -> bool:
        """
        Hand off a task from one agent to another.

        Args:
            from_agent: Source agent ID
            to_agent: Target agent ID
            task: Task to hand off
            context: Additional context

        Returns:
            True if handoff successful
        """
        source = self.registry.get(from_agent)
        target = self.registry.get(to_agent)

        if not source or not target:
            return False

        if not target.is_available:
            return False

        # Create handoff message
        handoff_msg = AgentMessage(
            from_agent=from_agent,
            to_agent=to_agent,
            type="handoff",
            priority="high",
            content=f"Task handoff: {task.description}",
            data={
                "task": {
                    "id": task.id,
                    "description": task.description,
                    "priority": task.priority,
                },
                "context": context or {},
            },
        )

        # Execute handoff
        source.current_task = None
        source.status = "standby"
        await target.receive_task(task)

        # Send notification
        await self.route_message(handoff_msg)

        # Sync with Context7
        await self.context7.handoff(from_agent, to_agent, {"id": task.id})

        return True

    async def run_workflow(
        self,
        steps: List[Dict[str, Any]],
    ) -> List[Dict[str, Any]]:
        """
        Run a multi-step workflow.

        Args:
            steps: List of workflow steps

        Returns:
            Results from each step
        """
        results = []

        for step in steps:
            task = await self.submit_task(
                description=step.get("description", ""),
                priority=step.get("priority", "normal"),
                target_agent=step.get("agent"),
            )

            # Wait for task completion (simplified)
            agent = self.registry.get(task.assigned_to)
            if agent:
                result = await agent.process_task(task)
                results.append({
                    "step": step,
                    "task_id": task.id,
                    "result": result,
                })

        return results

    def get_status(self) -> Dict[str, Any]:
        """Get orchestrator status."""
        return {
            "running": self._running,
            "queue_size": len(self._task_queue),
            "completed_tasks": len(self._completed_tasks),
            "registry_stats": self.registry.get_stats(),
        }
