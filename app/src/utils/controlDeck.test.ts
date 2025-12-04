import { describe, expect, it } from 'vitest'
import { getLoadStatus, sanitizeEmbedUrl } from './controlDeck'

describe('sanitizeEmbedUrl', () => {
  it('converts a watch URL into embed format', () => {
    const url = sanitizeEmbedUrl('https://www.youtube.com/watch?v=abc123xyz99')
    expect(url).toBe('https://www.youtube.com/embed/abc123xyz99?rel=0')
  })

  it('handles short URLs', () => {
    const url = sanitizeEmbedUrl('https://youtu.be/abc123xyz99')
    expect(url).toBe('https://www.youtube.com/embed/abc123xyz99?rel=0')
  })

  it('returns null for invalid host', () => {
    const url = sanitizeEmbedUrl('https://example.com/video/123')
    expect(url).toBeNull()
  })
})

describe('getLoadStatus', () => {
  it('returns calm tone for low load', () => {
    const status = getLoadStatus(144, 0.2)
    expect(status.label).toBe('Calm')
    expect(status.tone).toBe('cool')
  })

  it('returns balanced tone for moderate load', () => {
    const status = getLoadStatus(120, 0.5)
    expect(status.label).toBe('Stable')
    expect(status.tone).toBe('balanced')
  })

  it('clamps high load and marks as strained', () => {
    const status = getLoadStatus(60, 1.4)
    expect(status.label).toBe('Strained')
    expect(status.description.includes('100%')).toBe(true)
    expect(status.tone).toBe('warm')
  })
})

