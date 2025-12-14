import { extractYouTubeId } from './youtube'
import { clamp } from './math'

/**
 * Sanitize and normalize embed URLs for the Control Deck video hub.
 * Only allows YouTube hosts and returns a safe embed URL.
 */
export function sanitizeEmbedUrl(url: string): string | null {
  const id = extractYouTubeId(url)
  if (!id) return null
  return `https://www.youtube.com/embed/${id}?rel=0`
}

/**
 * Derive a friendly load status for the workstation HUD.
 * Clamps load between 0 and 1 to avoid invalid percentages.
 */
export function getLoadStatus(framesPerSecond: number, load: number) {
  const normalizedLoad = clamp(load, 0, 1)
  if (normalizedLoad < 0.35) {
    return {
      label: 'Calm',
      description: `${framesPerSecond} fps • ${Math.round(normalizedLoad * 100)}% glow load`,
      tone: 'cool'
    }
  }
  if (normalizedLoad < 0.7) {
    return {
      label: 'Stable',
      description: `${framesPerSecond} fps • ${Math.round(normalizedLoad * 100)}% glow load`,
      tone: 'balanced'
    }
  }
  return {
    label: 'Strained',
    description: `${framesPerSecond} fps • ${Math.round(normalizedLoad * 100)}% glow load`,
    tone: 'warm'
  }
}

