import { normalize } from './math'

/**
 * Calculates a normalized glow strength for UI meters and cards.
 * Values are clamped to avoid negative intensities and to keep gradients stable.
 */
export function calculateGlowStrength(value: number, max: number = 100) {
  const intensity = normalize(value, 0, max)
  const hue = 180 + (1 - intensity) * 120 // Cyan (high) → Magenta (low)
  const alpha = 0.35 + intensity * 0.35

  return { intensity, hue, alpha }
}

/**
 * Builds CSS-friendly color tokens for pulse-driven UI elements.
 */
export function getPulseTone(value: number, max: number = 100) {
  const { intensity, hue, alpha } = calculateGlowStrength(value, max)
  const color = `hsla(${hue}, 90%, ${30 + intensity * 40}%, ${alpha})`
  const border = `0 0 ${12 + intensity * 18}px rgba(0, 255, 255, ${0.35 + intensity * 0.3})`

  return { color, border, alpha, intensity }
}
