#include "ADSR.h"
#include <algorithm>
#include <cmath>

namespace revithion::dsp {

// ── Configuration ───────────────────────────────────────────────────

void ADSR::setSampleRate(float sampleRate) noexcept
{
    sampleRate_  = sampleRate;
    attackRate_  = rateForTime(attack_);
    decayRate_   = rateForTime(decay_);
    releaseRate_ = rateForTime(release_);
}

void ADSR::setAttack(float seconds) noexcept
{
    attack_     = std::max(seconds, 0.001f);
    attackRate_ = rateForTime(attack_);
}

void ADSR::setDecay(float seconds) noexcept
{
    decay_     = std::max(seconds, 0.001f);
    decayRate_ = rateForTime(decay_);
}

void ADSR::setSustain(float level) noexcept
{
    sustain_ = std::clamp(level, 0.0f, 1.0f);
}

void ADSR::setRelease(float seconds) noexcept
{
    release_     = std::max(seconds, 0.001f);
    releaseRate_ = rateForTime(release_);
}

// ── Triggers ────────────────────────────────────────────────────────

void ADSR::noteOn() noexcept
{
    state_ = State::Attack;
}

void ADSR::noteOff() noexcept
{
    if (state_ != State::Idle)
        state_ = State::Release;
}

void ADSR::reset() noexcept
{
    state_  = State::Idle;
    output_ = 0.0f;
}

bool ADSR::isActive() const noexcept
{
    return state_ != State::Idle;
}

// ── Per-sample processing ───────────────────────────────────────────

float ADSR::process() noexcept
{
    switch (state_)
    {
        case State::Idle:
            break;

        case State::Attack:
            output_ += attackRate_;
            if (output_ >= 1.0f)
            {
                output_ = 1.0f;
                state_  = State::Decay;
            }
            break;

        case State::Decay:
            output_ -= decayRate_;
            if (output_ <= sustain_)
            {
                output_ = sustain_;
                state_  = State::Sustain;
            }
            break;

        case State::Sustain:
            output_ = sustain_;
            break;

        case State::Release:
            output_ -= releaseRate_;
            if (output_ <= 0.0f)
            {
                output_ = 0.0f;
                state_  = State::Idle;
            }
            break;
    }

    return output_;
}

// ── Helpers ─────────────────────────────────────────────────────────

float ADSR::rateForTime(float seconds) const noexcept
{
    return 1.0f / (seconds * sampleRate_);
}

} // namespace revithion::dsp
