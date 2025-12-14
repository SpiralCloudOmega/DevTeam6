/**
 * Compose a persona-aware prompt for Repomind and Repomind Jr.
 * Ensures clean spacing and trims blank segments while preserving tone/focus.
 */
export function composePersonaPrompt(basePrompt: string, toneModifier: string, focusArea: string): string {
  const promptParts = [basePrompt, toneModifier, focusArea].map((part) => part.trim()).filter(Boolean)
  if (promptParts.length === 0) {
    return ''
  }
  return promptParts.join(' — ')
}

/**
 * Normalize a status label for display chips.
 */
export function formatStatus(statusLabel: string): string {
  const cleanedStatus = statusLabel.trim()
  if (!cleanedStatus) return 'Unknown'
  return cleanedStatus.charAt(0).toUpperCase() + cleanedStatus.slice(1).toLowerCase()
}
