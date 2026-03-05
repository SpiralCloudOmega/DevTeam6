#pragma once

#include <cstdint>

namespace revithion::dsp {

enum class Waveform : uint8_t
{
    Sine,
    Saw,
    Square,
    Triangle,
    WhiteNoise
};

class Oscillator
{
public:
    Oscillator() = default;

    void setSampleRate(float sampleRate) noexcept;
    void setFrequency(float frequency) noexcept;
    void setWaveform(Waveform waveform) noexcept;
    void reset() noexcept;

    float process() noexcept;

private:
    float polyBLEP(float t, float dt) const noexcept;

    Waveform waveform_  = Waveform::Sine;
    float    sampleRate_ = 44100.0f;
    float    frequency_  = 440.0f;
    float    phase_      = 0.0f;
    float    phaseInc_   = 0.0f;
    uint32_t noiseSeed_  = 123456789u;
};

} // namespace revithion::dsp
