/**
 * Mathematical utility functions for UI calculations
 */

/**
 * Clamp a value between min and max bounds.
 * Ensures the value never exceeds the specified range.
 */
export function clamp(value: number, min: number, max: number): number {
  return Math.min(Math.max(value, min), max)
}

/**
 * Normalize a value to a 0-1 range based on min and max.
 * Clamps the input to avoid values outside the range.
 */
export function normalize(value: number, min: number = 0, max: number = 100): number {
  if (max <= min) {
    throw new Error('max must be greater than min')
  }
  const clampedValue = clamp(value, min, max)
  return (clampedValue - min) / (max - min)
}
