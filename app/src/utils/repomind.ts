/**
 * Compose a persona-aware prompt for Repomind and Repomind Jr.
 * Ensures clean spacing and trims blank segments while preserving tone/focus.
 */
export function composePersonaPrompt(base: string, tone: string, focus: string): string {
  const parts = [base, tone, focus].map((part) => part.trim()).filter(Boolean)
  if (parts.length === 0) {
    return ''
  }
  return parts.join(' — ')
}

/**
 * Normalize a status label for display chips.
 */
export function formatStatus(status: string): string {
  const cleaned = status.trim()
  if (!cleaned) return 'Unknown'
  return cleaned.charAt(0).toUpperCase() + cleaned.slice(1).toLowerCase()
}
