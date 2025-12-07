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

  const xCoordinates = clusterNodes.map(node => node.position.x)
  const yCoordinates = clusterNodes.map(node => node.position.y)

  const minX = Math.min(...xCoordinates) - padding
  const maxX = Math.max(...xCoordinates) + padding
  const minY = Math.min(...yCoordinates) - padding
  const maxY = Math.max(...yCoordinates) + padding

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
 * 
 * Performance: O(V + E) where V is vertices and E is edges.
 * Uses parent tracking to reconstruct path instead of copying arrays at each step.
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
  const parentMap = new Map<string, string>()
  const queue: string[] = [startId]
  let queueReadIndex = 0 // Use index instead of shift() for O(1) dequeue

  while (queueReadIndex < queue.length) {
    const currentNode = queue[queueReadIndex++]
    
    // Check if we reached target
    if (currentNode === targetId) {
      // Reconstruct path from parent tracking
      const path: string[] = []
      let currentNodeId: string | undefined = targetId
      while (currentNodeId !== undefined) {
        path.unshift(currentNodeId)
        currentNodeId = parentMap.get(currentNodeId)
      }
      return path
    }

    const neighborNodes = adjacencyList.get(currentNode) || []
    for (const neighborId of neighborNodes) {
      if (visitedNodes.has(neighborId)) continue
      visitedNodes.add(neighborId)
      parentMap.set(neighborId, currentNode)
      queue.push(neighborId)
    }
  }

  return []
}
