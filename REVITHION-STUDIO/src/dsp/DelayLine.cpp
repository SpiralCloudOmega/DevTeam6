#include "DelayLine.h"
#include <algorithm>
#include <cmath>

namespace revithion::dsp {

void DelayLine::setMaxDelay(std::size_t maxDelaySamples)
{
    // +1 so that a delay of exactly maxDelaySamples is representable
    bufferLen_ = maxDelaySamples + 1;
    buffer_.assign(bufferLen_, 0.0f);
    writePos_ = 0;
}

void DelayLine::setDelay(float delaySamples) noexcept
{
    delay_ = std::max(delaySamples, 0.0f);
}

float DelayLine::process(float input) noexcept
{
    if (bufferLen_ == 0)
        return input;

    // Write incoming sample
    buffer_[writePos_] = input;

    // Compute fractional read position
    const float readPosF = static_cast<float>(writePos_) - delay_;
    const float wrapped  = (readPosF < 0.0f)
                             ? readPosF + static_cast<float>(bufferLen_)
                             : readPosF;

    // Integer & fractional parts
    const auto  idx0 = static_cast<std::size_t>(wrapped) % bufferLen_;
    const auto  idx1 = (idx0 + 1) % bufferLen_;
    const float frac = wrapped - std::floor(wrapped);

    // Linear interpolation
    const float out = buffer_[idx0] + frac * (buffer_[idx1] - buffer_[idx0]);

    // Advance write head
    writePos_ = (writePos_ + 1) % bufferLen_;

    return out;
}

void DelayLine::clear() noexcept
{
    std::fill(buffer_.begin(), buffer_.end(), 0.0f);
    writePos_ = 0;
}

} // namespace revithion::dsp
