import { describe, expect, it } from 'vitest'
import { extractYouTubeId } from './youtube'

describe('extractYouTubeId', () => {
  it('returns null for empty input', () => {
    expect(extractYouTubeId('')).toBeNull()
  })

  it('returns null for whitespace-only input', () => {
    expect(extractYouTubeId('   ')).toBeNull()
  })

  it('supports direct IDs', () => {
    expect(extractYouTubeId('dQw4w9WgXcQ')).toBe('dQw4w9WgXcQ')
  })

  it('supports direct IDs with whitespace', () => {
    expect(extractYouTubeId('  dQw4w9WgXcQ  ')).toBe('dQw4w9WgXcQ')
  })

  it('parses standard watch URLs', () => {
    expect(extractYouTubeId('https://www.youtube.com/watch?v=dQw4w9WgXcQ')).toBe('dQw4w9WgXcQ')
  })

  it('parses watch URLs without www', () => {
    expect(extractYouTubeId('https://youtube.com/watch?v=dQw4w9WgXcQ')).toBe('dQw4w9WgXcQ')
  })

  it('parses mobile YouTube URLs', () => {
    expect(extractYouTubeId('https://m.youtube.com/watch?v=dQw4w9WgXcQ')).toBe('dQw4w9WgXcQ')
  })

  it('parses short links', () => {
    expect(extractYouTubeId('https://youtu.be/dQw4w9WgXcQ')).toBe('dQw4w9WgXcQ')
  })

  it('parses shorts URLs', () => {
    expect(extractYouTubeId('https://www.youtube.com/shorts/dQw4w9WgXcQ')).toBe('dQw4w9WgXcQ')
  })

  it('parses embed URLs', () => {
    expect(extractYouTubeId('https://www.youtube.com/embed/dQw4w9WgXcQ')).toBe('dQw4w9WgXcQ')
  })

  it('parses URLs with additional parameters', () => {
    expect(extractYouTubeId('https://www.youtube.com/watch?v=dQw4w9WgXcQ&t=30s')).toBe('dQw4w9WgXcQ')
  })

  it('returns null for invalid inputs', () => {
    expect(extractYouTubeId('not a url')).toBeNull()
    expect(extractYouTubeId('https://example.com/watch?v=dQw4w9WgXcQ')).toBeNull()
  })

  it('returns null for invalid video IDs', () => {
    expect(extractYouTubeId('https://www.youtube.com/watch?v=short')).toBeNull()
    expect(extractYouTubeId('https://www.youtube.com/watch?v=toolongidvalue123')).toBeNull()
  })

  it('handles URLs without protocol', () => {
    expect(extractYouTubeId('youtube.com/watch?v=dQw4w9WgXcQ')).toBe('dQw4w9WgXcQ')
  })
})
