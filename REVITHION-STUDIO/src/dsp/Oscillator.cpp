#include "Oscillator.h"
#include "DSPUtils.h"
#include <cmath>

namespace revithion::dsp {

void Oscillator::setSampleRate(float sampleRate) noexcept
{
    sampleRate_ = sampleRate;
    phaseInc_   = frequency_ / sampleRate_;
}

void Oscillator::setFrequency(float frequency) noexcept
{
    frequency_ = frequency;
    phaseInc_  = frequency_ / sampleRate_;
}

void Oscillator::setWaveform(Waveform waveform) noexcept
{
    waveform_ = waveform;
}

void Oscillator::reset() noexcept
{
    phase_ = 0.0f;
}

// ── PolyBLEP residual for anti-aliasing ─────────────────────────────

float Oscillator::polyBLEP(float t, float dt) const noexcept
{
    if (t < dt)
    {
        float n = t / dt;
        return n + n - n * n - 1.0f;
    }
    if (t > 1.0f - dt)
    {
        float n = (t - 1.0f) / dt;
        return n * n + n + n + 1.0f;
    }
    return 0.0f;
}

// ── Sample generation ───────────────────────────────────────────────

float Oscillator::process() noexcept
{
    float out = 0.0f;
    const float dt = phaseInc_;

    switch (waveform_)
    {
        case Waveform::Sine:
        {
            out = std::sin(kTwoPi * phase_);
            break;
        }
        case Waveform::Saw:
        {
            out = 2.0f * phase_ - 1.0f;            // naive saw
            out -= polyBLEP(phase_, dt);             // anti-alias
            break;
        }
        case Waveform::Square:
        {
            out = (phase_ < 0.5f) ? 1.0f : -1.0f;  // naive square
            out += polyBLEP(phase_, dt);             // correct rising edge
            // correct falling edge at midpoint
            float shifted = phase_ + 0.5f;
            if (shifted >= 1.0f)
                shifted -= 1.0f;
            out -= polyBLEP(shifted, dt);
            break;
        }
        case Waveform::Triangle:
        {
            // Integrate the anti-aliased square to get triangle
            out = (phase_ < 0.5f) ? 1.0f : -1.0f;
            out += polyBLEP(phase_, dt);
            float shifted = phase_ + 0.5f;
            if (shifted >= 1.0f)
                shifted -= 1.0f;
            out -= polyBLEP(shifted, dt);
            // Leaky integration approximation
            out = 4.0f * phase_ - 2.0f;
            if (out > 1.0f)
                out = 2.0f - out;
            if (out < -1.0f)
                out = -2.0f - out;
            break;
        }
        case Waveform::WhiteNoise:
        {
            // Fast xorshift32 PRNG
            noiseSeed_ ^= noiseSeed_ << 13;
            noiseSeed_ ^= noiseSeed_ >> 17;
            noiseSeed_ ^= noiseSeed_ << 5;
            out = static_cast<float>(static_cast<int32_t>(noiseSeed_))
                / static_cast<float>(INT32_MAX);
            break;
        }
    }

    // Advance & wrap phase
    phase_ += dt;
    if (phase_ >= 1.0f)
        phase_ -= 1.0f;

    return out;
}

} // namespace revithion::dsp
