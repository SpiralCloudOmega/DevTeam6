"""
DevTeam6 Local AI - Base MCP Server

Base class for Model Context Protocol servers.
"""

from typing import Dict, Any, List, Optional, Callable
from dataclasses import dataclass
from abc import ABC, abstractmethod
import json


@dataclass
class MCPTool:
    """Definition of an MCP tool."""

    name: str
    description: str
    input_schema: Dict[str, Any]
    handler: Callable


@dataclass
class MCPResource:
    """Definition of an MCP resource."""

    uri: str
    name: str
    description: str
    mime_type: str = "application/json"


class BaseMCPServer(ABC):
    """
    Base class for MCP servers.

    Implements the Model Context Protocol for tool integration.
    """

    def __init__(self, name: str, version: str = "1.0.0"):
        """
        Initialize the MCP server.

        Args:
            name: Server name
            version: Server version
        """
        self.name = name
        self.version = version
        self._tools: Dict[str, MCPTool] = {}
        self._resources: Dict[str, MCPResource] = {}

    def register_tool(
        self,
        name: str,
        description: str,
        input_schema: Dict[str, Any],
        handler: Callable,
    ) -> None:
        """
        Register a tool.

        Args:
            name: Tool name
            description: Tool description
            input_schema: JSON Schema for inputs
            handler: Function to call
        """
        self._tools[name] = MCPTool(
            name=name,
            description=description,
            input_schema=input_schema,
            handler=handler,
        )

    def register_resource(
        self,
        uri: str,
        name: str,
        description: str,
        mime_type: str = "application/json",
    ) -> None:
        """
        Register a resource.

        Args:
            uri: Resource URI
            name: Resource name
            description: Resource description
            mime_type: MIME type
        """
        self._resources[uri] = MCPResource(
            uri=uri,
            name=name,
            description=description,
            mime_type=mime_type,
        )

    async def call_tool(
        self,
        name: str,
        arguments: Dict[str, Any],
    ) -> Dict[str, Any]:
        """
        Call a registered tool.

        Args:
            name: Tool name
            arguments: Tool arguments

        Returns:
            Tool result
        """
        if name not in self._tools:
            return {"error": f"Tool not found: {name}"}

        tool = self._tools[name]
        try:
            result = await tool.handler(arguments)
            return {"result": result}
        except Exception as e:
            return {"error": str(e)}

    @abstractmethod
    async def get_resource(self, uri: str) -> Dict[str, Any]:
        """
        Get a resource by URI.

        Args:
            uri: Resource URI

        Returns:
            Resource content
        """
        pass

    def list_tools(self) -> List[Dict[str, Any]]:
        """List all registered tools."""
        return [
            {
                "name": tool.name,
                "description": tool.description,
                "inputSchema": tool.input_schema,
            }
            for tool in self._tools.values()
        ]

    def list_resources(self) -> List[Dict[str, Any]]:
        """List all registered resources."""
        return [
            {
                "uri": res.uri,
                "name": res.name,
                "description": res.description,
                "mimeType": res.mime_type,
            }
            for res in self._resources.values()
        ]

    def get_manifest(self) -> Dict[str, Any]:
        """Get server manifest."""
        return {
            "name": self.name,
            "version": self.version,
            "tools": self.list_tools(),
            "resources": self.list_resources(),
        }
