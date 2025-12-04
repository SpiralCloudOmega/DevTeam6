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
  const scopedNodes = nodes.filter(node => node.cluster === clusterId)
  if (scopedNodes.length === 0) return null

  const xs = scopedNodes.map(node => node.position.x)
  const ys = scopedNodes.map(node => node.position.y)

  const minX = Math.min(...xs) - padding
  const maxX = Math.max(...xs) + padding
  const minY = Math.min(...ys) - padding
  const maxY = Math.max(...ys) + padding

  const width = maxX - minX
  const height = maxY - minY
  const centerX = minX + width / 2
  const centerY = minY + height / 2

  return { clusterId, minX, maxX, minY, maxY, width, height, centerX, centerY }
}

/**
 * Filter nodes by cluster while preserving the original order for stable rendering.
 */
export const filterNodesByCluster = (nodes: GraphNode[], clusterId: string): GraphNode[] => {
  if (clusterId === 'all') return nodes
  return nodes.filter(node => node.cluster === clusterId)
}

/**
 * Compute a shortest directed path between two node ids using BFS.
 * Returns an ordered list of node ids including start and target, or [] when unreachable.
 */
export const computeShortestPath = (
  edges: GraphEdge[],
  startId: string,
  targetId: string
): string[] => {
  if (!startId || !targetId) return []
  if (startId === targetId) return [startId]

  const adjacency = new Map<string, string[]>()

  edges.forEach(({ from, to }) => {
    if (!adjacency.has(from)) adjacency.set(from, [])
    adjacency.get(from)!.push(to)
  })

  const visited = new Set<string>([startId])
  const queue: Array<{ node: string; path: string[] }> = [{ node: startId, path: [startId] }]

  while (queue.length > 0) {
    const { node, path } = queue.shift() as { node: string; path: string[] }
    const neighbors = adjacency.get(node) || []

    for (const neighbor of neighbors) {
      if (visited.has(neighbor)) continue
      const nextPath = [...path, neighbor]
      if (neighbor === targetId) return nextPath
      visited.add(neighbor)
      queue.push({ node: neighbor, path: nextPath })
    }
  }

  return []
}
