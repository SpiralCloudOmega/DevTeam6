#pragma once

#include <cstdint>

namespace revithion::dsp {

class ADSR
{
public:
    ADSR() = default;

    void setSampleRate(float sampleRate) noexcept;

    void setAttack(float seconds) noexcept;
    void setDecay(float seconds) noexcept;
    void setSustain(float level) noexcept;   // 0.0 – 1.0
    void setRelease(float seconds) noexcept;

    void noteOn() noexcept;
    void noteOff() noexcept;
    void reset() noexcept;

    bool isActive() const noexcept;

    float process() noexcept;

private:
    enum class State : uint8_t { Idle, Attack, Decay, Sustain, Release };

    float rateForTime(float seconds) const noexcept;

    State state_      = State::Idle;
    float sampleRate_ = 44100.0f;
    float output_     = 0.0f;

    float attack_     = 0.01f;   // seconds
    float decay_      = 0.1f;
    float sustain_    = 0.7f;    // level
    float release_    = 0.3f;

    float attackRate_  = 0.0f;
    float decayRate_   = 0.0f;
    float releaseRate_ = 0.0f;
};

} // namespace revithion::dsp
