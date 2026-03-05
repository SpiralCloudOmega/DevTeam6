#pragma once

#include <vector>
#include <cstddef>

namespace revithion::dsp {

class DelayLine
{
public:
    DelayLine() = default;

    /// Allocate internal buffer for up to @p maxDelaySamples of delay.
    /// Call once before real-time processing.
    void setMaxDelay(std::size_t maxDelaySamples);

    /// Set the current delay length in (fractional) samples.
    void setDelay(float delaySamples) noexcept;

    /// Push one sample in and return the delayed sample.
    float process(float input) noexcept;

    /// Zero the buffer and reset the write head.
    void clear() noexcept;

private:
    std::vector<float> buffer_;
    std::size_t        writePos_  = 0;
    std::size_t        bufferLen_ = 0;
    float              delay_     = 0.0f;
};

} // namespace revithion::dsp
