import { describe, expect, it } from 'vitest'
import { clamp, normalize } from './math'

describe('math utils', () => {
  describe('clamp', () => {
    it('clamps value below minimum', () => {
      expect(clamp(-5, 0, 10)).toBe(0)
      expect(clamp(-100, 0, 100)).toBe(0)
    })

    it('clamps value above maximum', () => {
      expect(clamp(15, 0, 10)).toBe(10)
      expect(clamp(200, 0, 100)).toBe(100)
    })

    it('returns value within range unchanged', () => {
      expect(clamp(5, 0, 10)).toBe(5)
      expect(clamp(50, 0, 100)).toBe(50)
    })

    it('handles boundary values', () => {
      expect(clamp(0, 0, 10)).toBe(0)
      expect(clamp(10, 0, 10)).toBe(10)
    })

    it('handles negative ranges', () => {
      expect(clamp(-15, -20, -10)).toBe(-15)
      expect(clamp(-5, -20, -10)).toBe(-10)
      expect(clamp(-25, -20, -10)).toBe(-20)
    })
  })

  describe('normalize', () => {
    it('normalizes value in 0-100 range', () => {
      expect(normalize(0, 0, 100)).toBe(0)
      expect(normalize(50, 0, 100)).toBe(0.5)
      expect(normalize(100, 0, 100)).toBe(1)
    })

    it('normalizes with custom range', () => {
      expect(normalize(5, 0, 10)).toBe(0.5)
      expect(normalize(25, 0, 50)).toBe(0.5)
    })

    it('clamps values outside range', () => {
      expect(normalize(-10, 0, 100)).toBe(0)
      expect(normalize(150, 0, 100)).toBe(1)
    })

    it('throws error when max <= min', () => {
      expect(() => normalize(50, 100, 100)).toThrowError('max must be greater than min')
      expect(() => normalize(50, 100, 50)).toThrowError('max must be greater than min')
    })

    it('handles negative ranges', () => {
      expect(normalize(-15, -20, -10)).toBe(0.5)
      expect(normalize(-10, -20, 0)).toBe(0.5)
    })
  })
})
