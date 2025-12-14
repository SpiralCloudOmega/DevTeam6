"""
DevTeam6 Local AI - Memory MCP Server

MCP server for memory operations.
"""

from typing import Dict, Any, List, Optional
from .base_server import BaseMCPServer
from .tool_schemas import (
    create_tool_schema,
    create_string_property,
    create_integer_property,
    create_object_property,
    create_array_property,
)
from core.memory_system import MemorySystem


class MemoryMCPServer(BaseMCPServer):
    """
    MCP server for AI memory operations.

    Provides tools for:
    - Storing memories
    - Querying memories
    - Memory statistics
    """

    def __init__(self, memory_system: Optional[MemorySystem] = None):
        """
        Initialize the Memory MCP server.

        Args:
            memory_system: Memory system instance
        """
        super().__init__(name="devteam6-memory", version="1.0.0")
        self.memory = memory_system or MemorySystem()

        # Register tools
        self._register_tools()
        self._register_resources()

    def _register_tools(self) -> None:
        """Register memory tools."""

        self.register_tool(
            name="store_memory",
            description="Store content in AI memory for later retrieval",
            input_schema=create_tool_schema(
                properties={
                    "content": create_string_property("Content to store"),
                    "category": create_string_property("Category for organization", default="general"),
                    "metadata": create_object_property("Additional metadata"),
                },
                required=["content"],
            ),
            handler=self._store_memory,
        )

        self.register_tool(
            name="query_memory",
            description="Search memories using semantic similarity",
            input_schema=create_tool_schema(
                properties={
                    "query": create_string_property("Search query"),
                    "top_k": create_integer_property("Number of results", default=5),
                    "category": create_string_property("Filter by category"),
                },
                required=["query"],
            ),
            handler=self._query_memory,
        )

        self.register_tool(
            name="forget_memory",
            description="Remove specific memories",
            input_schema=create_tool_schema(
                properties={
                    "doc_ids": create_array_property("Document IDs to remove", item_type="string"),
                },
                required=["doc_ids"],
            ),
            handler=self._forget_memory,
        )

        self.register_tool(
            name="memory_stats",
            description="Get memory system statistics",
            input_schema=create_tool_schema(
                properties={},
                required=[],
            ),
            handler=self._memory_stats,
        )

    def _register_resources(self) -> None:
        """Register memory resources."""

        self.register_resource(
            uri="memory://stats",
            name="Memory Statistics",
            description="Current memory system statistics",
        )

        self.register_resource(
            uri="memory://categories",
            name="Memory Categories",
            description="List of memory categories",
        )

    async def _store_memory(self, args: Dict[str, Any]) -> Dict[str, Any]:
        """Store content in memory."""
        doc_id = await self.memory.store(
            content=args["content"],
            category=args.get("category", "general"),
            metadata=args.get("metadata"),
        )
        return {"doc_id": doc_id, "status": "stored"}

    async def _query_memory(self, args: Dict[str, Any]) -> Dict[str, Any]:
        """Query memories."""
        results = await self.memory.query(
            query=args["query"],
            top_k=args.get("top_k", 5),
            category=args.get("category"),
        )
        return {
            "results": [
                {
                    "id": r.id,
                    "content": r.content,
                    "score": r.score,
                    "metadata": r.metadata,
                }
                for r in results
            ],
            "count": len(results),
        }

    async def _forget_memory(self, args: Dict[str, Any]) -> Dict[str, Any]:
        """Remove memories."""
        await self.memory.forget(args["doc_ids"])
        return {"removed": len(args["doc_ids"]), "status": "forgotten"}

    async def _memory_stats(self, args: Dict[str, Any]) -> Dict[str, Any]:
        """Get memory statistics."""
        return self.memory.get_stats()

    async def get_resource(self, uri: str) -> Dict[str, Any]:
        """Get a resource by URI."""
        if uri == "memory://stats":
            return self.memory.get_stats()
        elif uri == "memory://categories":
            stats = self.memory.get_stats()
            return {"categories": list(stats.get("categories", {}).keys())}
        return {"error": f"Resource not found: {uri}"}
