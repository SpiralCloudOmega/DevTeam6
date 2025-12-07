"""
DevTeam6 Local AI - Knowledge Graph

Operations for the knowledge graph stored in .github/agents/memory/.
"""

from typing import List, Dict, Any, Optional, Set
from dataclasses import dataclass
from pathlib import Path
import json
from datetime import datetime

from config.settings import get_settings


@dataclass
class KGNode:
    """A node in the knowledge graph."""

    id: str
    type: str
    label: str
    properties: Dict[str, Any]


@dataclass
class KGEdge:
    """An edge in the knowledge graph."""

    source: str
    target: str
    type: str
    properties: Dict[str, Any] = None


class KnowledgeGraph:
    """
    Knowledge graph for storing relationships and concepts.

    Stored in: .github/agents/memory/knowledge-graph.json
    
    Performance: Batches writes to avoid excessive I/O operations.
    Call save() explicitly when needed, or use auto_save context manager.
    """

    def __init__(self, path: Optional[str] = None, auto_save: bool = True):
        """
        Initialize the knowledge graph.

        Args:
            path: Path to knowledge-graph.json
            auto_save: Automatically save after modifications (default: True)
        """
        settings = get_settings()
        context7_path = Path(settings.context7_path)
        memory_dir = context7_path.parent / "memory"
        self.path = Path(path) if path else memory_dir / "knowledge-graph.json"
        self.auto_save = auto_save
        self._dirty = False  # Track if changes need saving

        self._nodes: Dict[str, KGNode] = {}
        self._edges: List[KGEdge] = []
        self._adjacency: Dict[str, Set[str]] = {}

        self._load()

    def _load(self) -> None:
        """Load graph from file."""
        if self.path.exists():
            data = json.loads(self.path.read_text())

            # Load nodes
            for node_data in data.get("nodes", []):
                node = KGNode(
                    id=node_data["id"],
                    type=node_data["type"],
                    label=node_data["label"],
                    properties=node_data.get("properties", {}),
                )
                self._nodes[node.id] = node

            # Load edges
            for edge_data in data.get("edges", []):
                edge = KGEdge(
                    source=edge_data["source"],
                    target=edge_data["target"],
                    type=edge_data["type"],
                    properties=edge_data.get("properties"),
                )
                self._edges.append(edge)

                # Build adjacency
                if edge.source not in self._adjacency:
                    self._adjacency[edge.source] = set()
                self._adjacency[edge.source].add(edge.target)

    def _save(self) -> None:
        """Save graph to file."""
        self.path.parent.mkdir(parents=True, exist_ok=True)

        data = {
            "version": "1.0.0",
            "description": "Knowledge graph for agent relationships and concepts",
            "created": datetime.utcnow().isoformat(),
            "updated": datetime.utcnow().isoformat(),
            "nodes": [
                {
                    "id": node.id,
                    "type": node.type,
                    "label": node.label,
                    "properties": node.properties,
                }
                for node in self._nodes.values()
            ],
            "edges": [
                {
                    "source": edge.source,
                    "target": edge.target,
                    "type": edge.type,
                    "properties": edge.properties,
                }
                for edge in self._edges
            ],
        }

        self.path.write_text(json.dumps(data, indent=2))
        self._dirty = False
    
    def save(self) -> None:
        """
        Explicitly save changes to disk.
        
        Only writes to disk if there are unsaved changes (dirty flag is set).
        No-op if graph hasn't been modified since last save.
        """
        if self._dirty:
            self._save()
    
    def _mark_dirty(self) -> None:
        """Mark graph as having unsaved changes and auto-save if enabled."""
        self._dirty = True
        if self.auto_save:
            self._save()

    def add_node(
        self,
        node_id: str,
        node_type: str,
        label: str,
        properties: Optional[Dict[str, Any]] = None,
    ) -> KGNode:
        """
        Add a node to the graph.

        Args:
            node_id: Unique node ID
            node_type: Node type (e.g., "agent", "concept", "repository")
            label: Display label
            properties: Additional properties

        Returns:
            Created node
        """
        node = KGNode(
            id=node_id,
            type=node_type,
            label=label,
            properties=properties or {},
        )
        self._nodes[node_id] = node
        self._mark_dirty()
        return node

    def add_edge(
        self,
        source: str,
        target: str,
        edge_type: str,
        properties: Optional[Dict[str, Any]] = None,
    ) -> KGEdge:
        """
        Add an edge to the graph.

        Args:
            source: Source node ID
            target: Target node ID
            edge_type: Relationship type (e.g., "manages", "develops")
            properties: Additional properties

        Returns:
            Created edge
        """
        edge = KGEdge(
            source=source,
            target=target,
            type=edge_type,
            properties=properties,
        )
        self._edges.append(edge)

        if source not in self._adjacency:
            self._adjacency[source] = set()
        self._adjacency[source].add(target)

        self._mark_dirty()
        return edge

    def get_node(self, node_id: str) -> Optional[KGNode]:
        """Get a node by ID."""
        return self._nodes.get(node_id)

    def get_nodes_by_type(self, node_type: str) -> List[KGNode]:
        """Get all nodes of a specific type."""
        return [n for n in self._nodes.values() if n.type == node_type]

    def get_edges_from(self, node_id: str) -> List[KGEdge]:
        """Get all edges originating from a node."""
        return [e for e in self._edges if e.source == node_id]

    def get_edges_to(self, node_id: str) -> List[KGEdge]:
        """Get all edges pointing to a node."""
        return [e for e in self._edges if e.target == node_id]

    def get_neighbors(self, node_id: str) -> List[KGNode]:
        """Get all nodes connected to the given node."""
        neighbor_ids = self._adjacency.get(node_id, set())
        return [self._nodes[nid] for nid in neighbor_ids if nid in self._nodes]

    def remove_node(self, node_id: str) -> None:
        """Remove a node and all its edges."""
        if node_id in self._nodes:
            del self._nodes[node_id]

            # Remove edges
            self._edges = [
                e for e in self._edges
                if e.source != node_id and e.target != node_id
            ]

            # Update adjacency
            self._adjacency.pop(node_id, None)
            for neighbors in self._adjacency.values():
                neighbors.discard(node_id)

            self._mark_dirty()

    def remove_edge(self, source: str, target: str, edge_type: str) -> None:
        """Remove a specific edge."""
        self._edges = [
            e for e in self._edges
            if not (e.source == source and e.target == target and e.type == edge_type)
        ]

        if source in self._adjacency:
            # Only remove from adjacency if no other edges exist
            remaining = [e for e in self._edges if e.source == source and e.target == target]
            if not remaining:
                self._adjacency[source].discard(target)

        self._mark_dirty()

    def find_path(self, start: str, end: str) -> List[str]:
        """
        Find shortest path between two nodes using BFS.

        Args:
            start: Starting node ID
            end: Target node ID

        Returns:
            List of node IDs in path, or empty if no path exists
        """
        if start == end:
            return [start]

        visited = {start}
        queue = [(start, [start])]

        while queue:
            current, path = queue.pop(0)

            for neighbor in self._adjacency.get(current, []):
                if neighbor == end:
                    return path + [neighbor]

                if neighbor not in visited:
                    visited.add(neighbor)
                    queue.append((neighbor, path + [neighbor]))

        return []

    def get_subgraph(
        self,
        node_ids: List[str],
        depth: int = 1,
    ) -> Dict[str, Any]:
        """
        Get a subgraph centered on given nodes.

        Args:
            node_ids: Center node IDs
            depth: How many hops to include

        Returns:
            Subgraph data
        """
        included_nodes: Set[str] = set(node_ids)

        # Expand by depth
        frontier = set(node_ids)
        for _ in range(depth):
            next_frontier: Set[str] = set()
            for node_id in frontier:
                for neighbor in self._adjacency.get(node_id, []):
                    if neighbor not in included_nodes:
                        next_frontier.add(neighbor)
                        included_nodes.add(neighbor)
            frontier = next_frontier

        # Build subgraph
        nodes = [self._nodes[nid] for nid in included_nodes if nid in self._nodes]
        edges = [
            e for e in self._edges
            if e.source in included_nodes and e.target in included_nodes
        ]

        return {
            "nodes": [
                {"id": n.id, "type": n.type, "label": n.label, "properties": n.properties}
                for n in nodes
            ],
            "edges": [
                {"source": e.source, "target": e.target, "type": e.type}
                for e in edges
            ],
        }

    def get_stats(self) -> Dict[str, Any]:
        """Get graph statistics."""
        node_types: Dict[str, int] = {}
        edge_types: Dict[str, int] = {}

        for node in self._nodes.values():
            node_types[node.type] = node_types.get(node.type, 0) + 1

        for edge in self._edges:
            edge_types[edge.type] = edge_types.get(edge.type, 0) + 1

        return {
            "total_nodes": len(self._nodes),
            "total_edges": len(self._edges),
            "node_types": node_types,
            "edge_types": edge_types,
        }
