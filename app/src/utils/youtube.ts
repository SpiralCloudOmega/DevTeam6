export function extractYouTubeId(input: string): string | null {
  const trimmed = input.trim()
  if (!trimmed) return null

  // Direct 11-char ID support
  const directIdMatch = /^[a-zA-Z0-9_-]{11}$/.test(trimmed)
  if (directIdMatch) return trimmed

  let parsed: URL
  try {
    parsed = trimmed.startsWith('http') ? new URL(trimmed) : new URL(`https://${trimmed}`)
  } catch (error) {
    return null
  }

  const hostname = parsed.hostname.replace(/^www\./, '')
  const allowedHosts = ['youtube.com', 'm.youtube.com', 'youtu.be']
  if (!allowedHosts.includes(hostname)) {
    return null
  }

  const searchId = parsed.searchParams.get('v')
  if (searchId && /^[a-zA-Z0-9_-]{11}$/.test(searchId)) {
    return searchId
  }

  const pathSegments = parsed.pathname.split('/').filter(Boolean)
  const possibleId = pathSegments[pathSegments.length - 1]

  if (hostname === 'youtu.be' && /^[a-zA-Z0-9_-]{11}$/.test(possibleId)) {
    return possibleId
  }

  const embedHosts = ['youtube.com', 'm.youtube.com']
  if (embedHosts.includes(hostname)) {
    if (parsed.pathname.startsWith('/embed/') && /^[a-zA-Z0-9_-]{11}$/.test(possibleId)) {
      return possibleId
    }
    if (parsed.pathname.startsWith('/shorts/') && /^[a-zA-Z0-9_-]{11}$/.test(possibleId)) {
      return possibleId
    }
  }

  return null
}
