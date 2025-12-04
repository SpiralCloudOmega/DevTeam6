import { describe, expect, it } from 'vitest'
import { composePersonaPrompt, formatStatus } from './repomind'

describe('composePersonaPrompt', () => {
  it('joins non-empty segments with separator', () => {
    expect(composePersonaPrompt('Base', 'Tone', 'Focus')).toBe('Base — Tone — Focus')
  })

  it('trims whitespace and skips blanks', () => {
    expect(composePersonaPrompt('  Hello ', ' ', 'Depth ')).toBe('Hello — Depth')
  })

  it('returns empty string when all parts are blank', () => {
    expect(composePersonaPrompt(' ', '   ', '')).toBe('')
  })
})

describe('formatStatus', () => {
  it('capitalizes and lowercases correctly', () => {
    expect(formatStatus('ONLiNe')).toBe('Online')
  })

  it('handles empty strings', () => {
    expect(formatStatus('   ')).toBe('Unknown')
  })
})
