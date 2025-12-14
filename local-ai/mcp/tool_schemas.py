"""
DevTeam6 Local AI - Common MCP Tool Schemas

Reusable JSON schemas for MCP tool registration.
"""

from typing import Dict, Any


def create_string_property(
    description: str,
    default: Any = None,
) -> Dict[str, Any]:
    """Create a string property schema."""
    schema = {
        "type": "string",
        "description": description,
    }
    if default is not None:
        schema["default"] = default
    return schema


def create_integer_property(
    description: str,
    default: Any = None,
) -> Dict[str, Any]:
    """Create an integer property schema."""
    schema = {
        "type": "integer",
        "description": description,
    }
    if default is not None:
        schema["default"] = default
    return schema


def create_object_property(
    description: str,
) -> Dict[str, Any]:
    """Create an object property schema."""
    return {
        "type": "object",
        "description": description,
    }


def create_array_property(
    description: str,
    item_type: str = "string",
) -> Dict[str, Any]:
    """Create an array property schema."""
    return {
        "type": "array",
        "items": {"type": item_type},
        "description": description,
    }


def create_tool_schema(
    properties: Dict[str, Dict[str, Any]],
    required: list[str],
) -> Dict[str, Any]:
    """
    Create a complete tool input schema.

    Args:
        properties: Dictionary of property schemas
        required: List of required property names

    Returns:
        Complete JSON schema for tool input
    """
    return {
        "type": "object",
        "properties": properties,
        "required": required,
    }


# Common schema patterns
QUERY_SCHEMA = create_tool_schema(
    properties={
        "query": create_string_property("Search query"),
        "top_k": create_integer_property("Number of results", default=5),
    },
    required=["query"],
)

STORE_CONTENT_SCHEMA = create_tool_schema(
    properties={
        "content": create_string_property("Content to store"),
        "metadata": create_object_property("Additional metadata"),
    },
    required=["content"],
)
