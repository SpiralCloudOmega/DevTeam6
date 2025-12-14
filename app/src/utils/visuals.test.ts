import { describe, expect, it } from 'vitest'
import { calculateGlowStrength, getPulseTone } from './visuals'

describe('visuals utils', () => {
  it('clamps glow strength within bounds', () => {
    expect(calculateGlowStrength(120, 100)).toEqual({ intensity: 1, hue: 180, alpha: 0.7 })
    expect(calculateGlowStrength(-5, 80)).toEqual({ intensity: 0, hue: 300, alpha: 0.35 })
  })

  it('rejects non-positive max values', () => {
    expect(() => calculateGlowStrength(10, 0)).toThrowError('max must be greater than min')
    expect(() => calculateGlowStrength(10, -5)).toThrowError('max must be greater than min')
  })

  it('returns pulse-friendly tones', () => {
    const pulse = getPulseTone(75, 100)
    expect(pulse.intensity).toBeCloseTo(0.75)
    expect(pulse.color).toContain('hsla(')
    expect(pulse.border).toContain('rgba(0, 255, 255')
  })

  it('calculates correct intensity for middle values', () => {
    const result = calculateGlowStrength(50, 100)
    expect(result.intensity).toBe(0.5)
    expect(result.alpha).toBeCloseTo(0.525)
  })

  it('handles exact boundary values', () => {
    expect(calculateGlowStrength(0, 100)).toEqual({ intensity: 0, hue: 300, alpha: 0.35 })
    expect(calculateGlowStrength(100, 100)).toEqual({ intensity: 1, hue: 180, alpha: 0.7 })
  })

  it('works with custom max values', () => {
    const result = calculateGlowStrength(25, 50)
    expect(result.intensity).toBe(0.5)
  })
})
