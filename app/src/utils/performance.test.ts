import { describe, it, expect } from 'vitest'
import { computeShortestPath } from './graph'

describe('Performance Benchmarks', () => {
  describe('computeShortestPath', () => {
    it('should handle large graphs efficiently', () => {
      // Create a large graph with 100 nodes
      const edges = []
      for (let i = 0; i < 100; i++) {
        // Each node connects to the next 3 nodes
        for (let j = 1; j <= 3; j++) {
          if (i + j < 100) {
            edges.push({ from: `node${i}`, to: `node${i + j}` })
          }
        }
      }

      const startTime = performance.now()
      const path = computeShortestPath(edges, 'node0', 'node99')
      const endTime = performance.now()
      const duration = endTime - startTime

      // Path should be found
      expect(path.length).toBeGreaterThan(0)
      expect(path[0]).toBe('node0')
      expect(path[path.length - 1]).toBe('node99')

      // Should complete in reasonable time (< 10ms for 100 nodes)
      expect(duration).toBeLessThan(10)
    })

    it('should handle disconnected graphs quickly', () => {
      // Create a disconnected graph
      const edges = [
        { from: 'a', to: 'b' },
        { from: 'b', to: 'c' },
        { from: 'd', to: 'e' },
        { from: 'e', to: 'f' },
      ]

      const startTime = performance.now()
      const path = computeShortestPath(edges, 'a', 'f')
      const endTime = performance.now()
      const duration = endTime - startTime

      // No path should be found
      expect(path).toEqual([])

      // Should complete very quickly (< 1ms)
      expect(duration).toBeLessThan(1)
    })

    it('should handle deep paths efficiently', () => {
      // Create a linear chain of 50 nodes
      const edges = []
      for (let i = 0; i < 49; i++) {
        edges.push({ from: `node${i}`, to: `node${i + 1}` })
      }

      const startTime = performance.now()
      const path = computeShortestPath(edges, 'node0', 'node49')
      const endTime = performance.now()
      const duration = endTime - startTime

      // Path should have exactly 50 nodes
      expect(path.length).toBe(50)

      // Should complete quickly (< 5ms)
      expect(duration).toBeLessThan(5)
    })
  })

  describe('Array operations benchmark', () => {
    it('concat vs spread operator performance', () => {
      const baseArray = Array.from({ length: 10 }, (_, i) => `item${i}`)

      // Test concat (our optimization)
      const concatStart = performance.now()
      for (let i = 0; i < 1000; i++) {
        const result = baseArray.concat('newItem')
      }
      const concatDuration = performance.now() - concatStart

      // Test spread (old approach)
      const spreadStart = performance.now()
      for (let i = 0; i < 1000; i++) {
        const result = [...baseArray, 'newItem']
      }
      const spreadDuration = performance.now() - spreadStart

      // concat should be at least as fast or faster
      // This is informational - actual performance may vary
      console.log(`concat: ${concatDuration.toFixed(3)}ms, spread: ${spreadDuration.toFixed(3)}ms`)

      // Both should complete in reasonable time
      expect(concatDuration).toBeLessThan(100)
      expect(spreadDuration).toBeLessThan(100)
    })

    it('for loop vs forEach performance', () => {
      const array = Array.from({ length: 1000 }, (_, i) => i)

      // Test for loop
      const forStart = performance.now()
      let sum1 = 0
      for (let i = 0; i < array.length; i++) {
        sum1 += array[i]
      }
      const forDuration = performance.now() - forStart

      // Test forEach
      const forEachStart = performance.now()
      let sum2 = 0
      array.forEach(n => {
        sum2 += n
      })
      const forEachDuration = performance.now() - forEachStart

      // Results should be the same
      expect(sum1).toBe(sum2)

      // for loop is typically faster
      console.log(`for loop: ${forDuration.toFixed(3)}ms, forEach: ${forEachDuration.toFixed(3)}ms`)

      // Both should complete quickly
      expect(forDuration).toBeLessThan(10)
      expect(forEachDuration).toBeLessThan(10)
    })
  })
})
