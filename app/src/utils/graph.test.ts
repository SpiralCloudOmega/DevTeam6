import { describe, expect, it } from 'vitest'
import { computeClusterBounds, computeShortestPath, filterNodesByCluster, GraphEdge, GraphNode } from './graph'

describe('graph utilities', () => {
  const sampleNodes: GraphNode[] = [
    { id: 'a', position: { x: 100, y: 200 }, cluster: 'core' },
    { id: 'b', position: { x: 150, y: 250 }, cluster: 'core' },
    { id: 'c', position: { x: 300, y: 400 }, cluster: 'automation' },
  ]

  it('computes padded bounds for a cluster', () => {
    const bounds = computeClusterBounds(sampleNodes, 'core', 20)
    expect(bounds).toMatchObject({
      minX: 80,
      maxX: 170,
      minY: 180,
      maxY: 270,
      clusterId: 'core'
    })
    expect(bounds?.width).toBeCloseTo(90)
    expect(bounds?.centerX).toBeCloseTo(125)
  })

  it('returns null when no nodes exist for cluster', () => {
    expect(computeClusterBounds(sampleNodes, 'ghost')).toBeNull()
  })

  it('filters nodes by cluster while preserving order', () => {
    const filtered = filterNodesByCluster(sampleNodes, 'core')
    expect(filtered.map(n => n.id)).toEqual(['a', 'b'])
  })

  it('passes through all nodes when cluster is all', () => {
    expect(filterNodesByCluster(sampleNodes, 'all')).toHaveLength(sampleNodes.length)
  })

  it('returns empty array for empty nodes array', () => {
    expect(filterNodesByCluster([], 'core')).toEqual([])
  })

  it('returns all nodes for empty cluster id', () => {
    expect(filterNodesByCluster(sampleNodes, '')).toHaveLength(sampleNodes.length)
  })

  it('finds shortest directed path when reachable', () => {
    const edges: GraphEdge[] = [
      { from: 'a', to: 'b' },
      { from: 'b', to: 'c' },
      { from: 'a', to: 'c' }
    ]

    expect(computeShortestPath(edges, 'a', 'c')).toEqual(['a', 'c'])
  })

  it('returns empty path when unreachable', () => {
    const edges: GraphEdge[] = [
      { from: 'a', to: 'b' },
      { from: 'c', to: 'd' }
    ]

    expect(computeShortestPath(edges, 'a', 'd')).toEqual([])
  })

  it('returns singleton path when start equals target', () => {
    expect(computeShortestPath([], 'solo', 'solo')).toEqual(['solo'])
  })

  it('returns empty path when edges array is empty and start differs from target', () => {
    expect(computeShortestPath([], 'a', 'b')).toEqual([])
  })

  it('returns empty path when start node is not in graph', () => {
    const edges: GraphEdge[] = [
      { from: 'a', to: 'b' },
      { from: 'b', to: 'c' }
    ]
    expect(computeShortestPath(edges, 'x', 'c')).toEqual([])
  })

  it('returns empty path for empty startId', () => {
    const edges: GraphEdge[] = [{ from: 'a', to: 'b' }]
    expect(computeShortestPath(edges, '', 'b')).toEqual([])
  })

  it('returns empty path for empty targetId', () => {
    const edges: GraphEdge[] = [{ from: 'a', to: 'b' }]
    expect(computeShortestPath(edges, 'a', '')).toEqual([])
  })
})
