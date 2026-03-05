#include "BiquadFilter.h"
#include "DSPUtils.h"
#include <cmath>

namespace revithion::dsp {

void BiquadFilter::setSampleRate(float sampleRate) noexcept
{
    sampleRate_ = sampleRate;
}

void BiquadFilter::setCoefficients(FilterType type, float freq, float Q,
                                   float gainDb) noexcept
{
    const float w0    = kTwoPi * freq / sampleRate_;
    const float cosW0 = std::cos(w0);
    const float sinW0 = std::sin(w0);
    const float alpha = sinW0 / (2.0f * Q);

    float b0 = 0.0f, b1 = 0.0f, b2 = 0.0f;
    float a0 = 0.0f, a1 = 0.0f, a2 = 0.0f;

    // A is used only by Peak / Shelf types
    const float A = std::pow(10.0f, gainDb / 40.0f);

    switch (type)
    {
        case FilterType::LowPass:
            b0 = (1.0f - cosW0) * 0.5f;
            b1 =  1.0f - cosW0;
            b2 = (1.0f - cosW0) * 0.5f;
            a0 =  1.0f + alpha;
            a1 = -2.0f * cosW0;
            a2 =  1.0f - alpha;
            break;

        case FilterType::HighPass:
            b0 =  (1.0f + cosW0) * 0.5f;
            b1 = -(1.0f + cosW0);
            b2 =  (1.0f + cosW0) * 0.5f;
            a0 =  1.0f + alpha;
            a1 = -2.0f * cosW0;
            a2 =  1.0f - alpha;
            break;

        case FilterType::BandPass:
            b0 =  alpha;
            b1 =  0.0f;
            b2 = -alpha;
            a0 =  1.0f + alpha;
            a1 = -2.0f * cosW0;
            a2 =  1.0f - alpha;
            break;

        case FilterType::Notch:
            b0 =  1.0f;
            b1 = -2.0f * cosW0;
            b2 =  1.0f;
            a0 =  1.0f + alpha;
            a1 = -2.0f * cosW0;
            a2 =  1.0f - alpha;
            break;

        case FilterType::Peak:
            b0 =  1.0f + alpha * A;
            b1 = -2.0f * cosW0;
            b2 =  1.0f - alpha * A;
            a0 =  1.0f + alpha / A;
            a1 = -2.0f * cosW0;
            a2 =  1.0f - alpha / A;
            break;

        case FilterType::LowShelf:
        {
            const float sqrtA = std::sqrt(A);
            b0 =        A * ((A + 1.0f) - (A - 1.0f) * cosW0 + 2.0f * sqrtA * alpha);
            b1 =  2.0f * A * ((A - 1.0f) - (A + 1.0f) * cosW0);
            b2 =        A * ((A + 1.0f) - (A - 1.0f) * cosW0 - 2.0f * sqrtA * alpha);
            a0 =             (A + 1.0f) + (A - 1.0f) * cosW0 + 2.0f * sqrtA * alpha;
            a1 = -2.0f *    ((A - 1.0f) + (A + 1.0f) * cosW0);
            a2 =             (A + 1.0f) + (A - 1.0f) * cosW0 - 2.0f * sqrtA * alpha;
            break;
        }

        case FilterType::HighShelf:
        {
            const float sqrtA = std::sqrt(A);
            b0 =        A * ((A + 1.0f) + (A - 1.0f) * cosW0 + 2.0f * sqrtA * alpha);
            b1 = -2.0f * A * ((A - 1.0f) + (A + 1.0f) * cosW0);
            b2 =        A * ((A + 1.0f) + (A - 1.0f) * cosW0 - 2.0f * sqrtA * alpha);
            a0 =             (A + 1.0f) - (A - 1.0f) * cosW0 + 2.0f * sqrtA * alpha;
            a1 =  2.0f *    ((A - 1.0f) - (A + 1.0f) * cosW0);
            a2 =             (A + 1.0f) - (A - 1.0f) * cosW0 - 2.0f * sqrtA * alpha;
            break;
        }

        case FilterType::AllPass:
            b0 =  1.0f - alpha;
            b1 = -2.0f * cosW0;
            b2 =  1.0f + alpha;
            a0 =  1.0f + alpha;
            a1 = -2.0f * cosW0;
            a2 =  1.0f - alpha;
            break;
    }

    // Normalise so a0 == 1
    const float inv = 1.0f / a0;
    b0_ = b0 * inv;
    b1_ = b1 * inv;
    b2_ = b2 * inv;
    a1_ = a1 * inv;
    a2_ = a2 * inv;
}

float BiquadFilter::process(float input) noexcept
{
    // Transposed Direct Form II
    float out = b0_ * input + z1_;
    z1_ = b1_ * input - a1_ * out + z2_;
    z2_ = b2_ * input - a2_ * out;
    return out;
}

void BiquadFilter::reset() noexcept
{
    z1_ = 0.0f;
    z2_ = 0.0f;
}

} // namespace revithion::dsp
