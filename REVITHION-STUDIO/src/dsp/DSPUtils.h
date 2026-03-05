#pragma once

#include <cmath>
#include <algorithm>

namespace revithion::dsp {

// ── dB / Linear Conversion ──────────────────────────────────────────

inline float dBToLinear(float dB) noexcept
{
    return std::pow(10.0f, dB / 20.0f);
}

inline float linearToDb(float linear) noexcept
{
    return (linear > 0.0f) ? 20.0f * std::log10(linear) : -100.0f;
}

// ── MIDI / Frequency Conversion ─────────────────────────────────────

inline float midiNoteToFreq(float note) noexcept
{
    return 440.0f * std::pow(2.0f, (note - 69.0f) / 12.0f);
}

inline float freqToMidiNote(float freq) noexcept
{
    return (freq > 0.0f) ? 69.0f + 12.0f * std::log2(freq / 440.0f) : 0.0f;
}

// ── Interpolation & Clamping ────────────────────────────────────────

inline float clamp(float value, float lo, float hi) noexcept
{
    return std::clamp(value, lo, hi);
}

inline float lerp(float a, float b, float t) noexcept
{
    return a + t * (b - a);
}

inline float smoothStep(float edge0, float edge1, float x) noexcept
{
    float t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

// ── Saturation ──────────────────────────────────────────────────────

inline float fastTanh(float x) noexcept
{
    // Padé approximant — accurate for |x| < ~3, cheap and branchless
    float x2 = x * x;
    return x * (27.0f + x2) / (27.0f + 9.0f * x2);
}

// ── Constants ───────────────────────────────────────────────────────

inline constexpr float kPi  = 3.14159265358979323846f;
inline constexpr float kTwoPi = 2.0f * kPi;

} // namespace revithion::dsp
