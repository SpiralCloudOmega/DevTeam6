import { describe, expect, it } from 'vitest'
import { calculateGlowStrength, getPulseTone } from './visuals'

describe('visuals utils', () => {
  it('clamps glow strength within bounds', () => {
    expect(calculateGlowStrength(120, 100)).toEqual({ intensity: 1, hue: 180, alpha: 0.7 })
    expect(calculateGlowStrength(-5, 80)).toEqual({ intensity: 0, hue: 300, alpha: 0.35 })
  })

  it('rejects non-positive max values', () => {
    expect(() => calculateGlowStrength(10, 0)).toThrowError('max must be greater than 0')
  })

  it('returns pulse-friendly tones', () => {
    const pulse = getPulseTone(75, 100)
    expect(pulse.intensity).toBeCloseTo(0.75)
    expect(pulse.color).toContain('hsla(')
    expect(pulse.border).toContain('rgba(0, 255, 255')
  })
})
