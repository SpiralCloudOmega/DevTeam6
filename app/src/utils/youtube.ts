export function extractYouTubeId(input: string): string | null {
  const cleanedInput = input.trim()
  if (!cleanedInput) return null

  // Direct 11-char ID support
  const isDirectVideoId = /^[a-zA-Z0-9_-]{11}$/.test(cleanedInput)
  if (isDirectVideoId) return cleanedInput

  let parsedUrl: URL
  try {
    parsedUrl = cleanedInput.startsWith('http') ? new URL(cleanedInput) : new URL(`https://${cleanedInput}`)
  } catch (error) {
    return null
  }

  const hostname = parsedUrl.hostname.replace(/^www\./, '')
  const allowedHosts = ['youtube.com', 'm.youtube.com', 'youtu.be']
  if (!allowedHosts.includes(hostname)) {
    return null
  }

  const videoIdFromParams = parsedUrl.searchParams.get('v')
  if (videoIdFromParams && /^[a-zA-Z0-9_-]{11}$/.test(videoIdFromParams)) {
    return videoIdFromParams
  }

  const pathSegments = parsedUrl.pathname.split('/').filter(Boolean)
  const lastPathSegment = pathSegments[pathSegments.length - 1]

  if (hostname === 'youtu.be' && /^[a-zA-Z0-9_-]{11}$/.test(lastPathSegment)) {
    return lastPathSegment
  }

  const embedHosts = ['youtube.com', 'm.youtube.com']
  if (embedHosts.includes(hostname)) {
    if (parsedUrl.pathname.startsWith('/embed/') && /^[a-zA-Z0-9_-]{11}$/.test(lastPathSegment)) {
      return lastPathSegment
    }
    if (parsedUrl.pathname.startsWith('/shorts/') && /^[a-zA-Z0-9_-]{11}$/.test(lastPathSegment)) {
      return lastPathSegment
    }
  }

  return null
}
