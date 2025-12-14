export interface GraphPoint {
  x: number
  y: number
}

export interface GraphNode {
  id: string
  position: GraphPoint
  cluster?: string
}

export interface GraphEdge {
  from: string
  to: string
}

export interface ClusterBounds {
  clusterId: string
  minX: number
  maxX: number
  minY: number
  maxY: number
  width: number
  height: number
  centerX: number
  centerY: number
}

/**
 * Compute padded bounding boxes for nodes in a cluster. Returns null when no nodes are present.
 */
export const computeClusterBounds = (
  nodes: GraphNode[],
  clusterId: string,
  padding = 48
): ClusterBounds | null => {
  const clusterNodes = nodes.filter(node => node.cluster === clusterId)
  if (clusterNodes.length === 0) return null

  const xPositions = clusterNodes.map(node => node.position.x)
  const yPositions = clusterNodes.map(node => node.position.y)

  const minX = Math.min(...xPositions) - padding
  const maxX = Math.max(...xPositions) + padding
  const minY = Math.min(...yPositions) - padding
  const maxY = Math.max(...yPositions) + padding

  const width = maxX - minX
  const height = maxY - minY
  const centerX = minX + width / 2
  const centerY = minY + height / 2

  return { clusterId, minX, maxX, minY, maxY, width, height, centerX, centerY }
}

/**
 * Filter nodes by cluster while preserving the original order for stable rendering.
 * Returns empty array for null/undefined nodes input.
 */
export const filterNodesByCluster = (nodes: GraphNode[], clusterId: string): GraphNode[] => {
  if (!nodes || nodes.length === 0) return []
  if (!clusterId || clusterId === 'all') return nodes
  return nodes.filter(node => node.cluster === clusterId)
}

/**
 * Compute a shortest directed path between two node ids using BFS.
 * Returns an ordered list of node ids including start and target, or [] when unreachable.
 * Returns empty array if start node does not exist in the graph.
 */
export const computeShortestPath = (
  edges: GraphEdge[],
  startId: string,
  targetId: string
): string[] => {
  if (!startId || !targetId) return []
  if (startId === targetId) return [startId]
  if (!edges || edges.length === 0) return []

  const adjacencyList = new Map<string, string[]>()

  edges.forEach(({ from, to }) => {
    if (!adjacencyList.has(from)) adjacencyList.set(from, [])
    adjacencyList.get(from)!.push(to)
  })

  // Return empty if start node is not in the graph
  if (!adjacencyList.has(startId)) return []

  const visitedNodes = new Set<string>([startId])
  const breadthFirstQueue: Array<{ currentNode: string; currentPath: string[] }> = [{ currentNode: startId, currentPath: [startId] }]

  while (breadthFirstQueue.length > 0) {
    const { currentNode, currentPath } = breadthFirstQueue.shift() as { currentNode: string; currentPath: string[] }
    const neighborNodes = adjacencyList.get(currentNode) || []

    for (const neighborNode of neighborNodes) {
      if (visitedNodes.has(neighborNode)) continue
      // Optimize: Build new path with concat to avoid mutation issues
      const nextPath = currentPath.concat(neighborNode)
      if (neighborNode === targetId) return nextPath
      visitedNodes.add(neighborNode)
      breadthFirstQueue.push({ currentNode: neighborNode, currentPath: nextPath })
    }
  }

  return []
}
