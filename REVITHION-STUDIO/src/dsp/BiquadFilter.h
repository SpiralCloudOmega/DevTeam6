#pragma once

#include <cstdint>

namespace revithion::dsp {

enum class FilterType : uint8_t
{
    LowPass,
    HighPass,
    BandPass,
    Notch,
    Peak,
    LowShelf,
    HighShelf,
    AllPass
};

class BiquadFilter
{
public:
    BiquadFilter() = default;

    void setSampleRate(float sampleRate) noexcept;

    /// Recalculate coefficients for the given filter parameters.
    /// @param type   Filter type
    /// @param freq   Centre / corner frequency in Hz
    /// @param Q      Quality factor (> 0)
    /// @param gainDb Gain in dB (used only by Peak / Shelf types)
    void setCoefficients(FilterType type, float freq, float Q,
                         float gainDb = 0.0f) noexcept;

    float process(float input) noexcept;
    void  reset() noexcept;

private:
    float sampleRate_ = 44100.0f;

    // Transfer-function coefficients (normalised so a0 == 1)
    float b0_ = 1.0f, b1_ = 0.0f, b2_ = 0.0f;
    float a1_ = 0.0f, a2_ = 0.0f;

    // State (Transposed Direct Form II)
    float z1_ = 0.0f, z2_ = 0.0f;
};

} // namespace revithion::dsp
