---
name: realtime-audio
description: Real-time audio programming — lock-free design, audio thread constraints, JUCE processBlock patterns
---

# Real-Time Audio Programming

The audio callback (`processBlock` in JUCE) runs on a real-time thread with a hard deadline. At 44.1 kHz / 128-sample buffer you have ~2.9 ms per block. Missing the deadline causes audible glitches. This means **NO allocations, NO locks, NO system calls, NO blocking** on the audio thread. Ever.

## The Audio Thread Contract

1. **No memory allocation or deallocation** — `malloc`/`new`/`delete`/`free` block on OS mutexes
2. **No locks or mutexes** — causes priority inversion if another thread holds the lock
3. **No system calls** — file I/O, network, console output can block indefinitely
4. **No exceptions** — stack unwinding has unpredictable timing
5. **No STL containers that allocate** — no `push_back`, no `std::string` concatenation
6. **Deterministic execution time** — no data-dependent worst-case algorithms

```cpp
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) override
{
    // ✅ OK: math, pre-allocated buffers, atomics
    // ❌ NEVER: new, delete, malloc, free, lock, mutex, file I/O, logging
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
            data[i] = processSample(data[i]);
    }
}
```

## Lock-Free Communication

### std::atomic for Simple Values
```cpp
std::atomic<float> gain{1.0f};
// GUI thread:  gain.store(newGain, std::memory_order_relaxed);
// Audio thread: float g = gain.load(std::memory_order_relaxed);
```

### SPSC Lock-Free Queue
For streaming data between threads (MIDI events, parameter changes, waveform snapshots):
```cpp
template <typename T, size_t Size>
class SPSCQueue {
    std::array<T, Size> buffer;
    std::atomic<size_t> readPos{0}, writePos{0};
public:
    bool push(const T& item) {
        const size_t w = writePos.load(std::memory_order_relaxed);
        const size_t next = (w + 1) % Size;
        if (next == readPos.load(std::memory_order_acquire)) return false;
        buffer[w] = item;
        writePos.store(next, std::memory_order_release);
        return true;
    }
    bool pop(T& item) {
        const size_t r = readPos.load(std::memory_order_relaxed);
        if (r == writePos.load(std::memory_order_acquire)) return false;
        item = buffer[r];
        readPos.store((r + 1) % Size, std::memory_order_release);
        return true;
    }
};
```

### JUCE Lock-Free Utilities
- `juce::AbstractFifo` — index management for lock-free ring buffers
- `juce::AudioProcessorValueTreeState` — thread-safe parameter binding backed by atomics

## Wait-Free vs Lock-Free
- **Lock-free**: at least one thread always makes progress (no deadlock)
- **Wait-free**: every thread completes in bounded steps (stronger)
- Audio thread path must be at minimum lock-free, ideally wait-free

## Ring Buffer for Visualization
Pass audio from audio thread to GUI for waveform/spectrum display:
```cpp
class AudioRingBuffer {
    std::vector<float> buffer; // pre-allocated in constructor
    std::atomic<int> writePos{0};
    int size;
public:
    explicit AudioRingBuffer(int sz) : buffer(static_cast<size_t>(sz), 0.0f), size(sz) {}
    void write(const float* data, int numSamples) {
        int pos = writePos.load(std::memory_order_relaxed);
        for (int i = 0; i < numSamples; ++i)
            buffer[static_cast<size_t>((pos + i) % size)] = data[i];
        writePos.store((pos + numSamples) % size, std::memory_order_release);
    }
};
```

## Pre-Allocation Strategy
Allocate everything in `prepareToPlay()` — never in `processBlock()`:
```cpp
void prepareToPlay(double sampleRate, int maxBlockSize) override
{
    tempBuffer.setSize(2, maxBlockSize);
    delayBuffer.resize(static_cast<size_t>(sampleRate * maxDelaySeconds));
    fftData.resize(static_cast<size_t>(fftSize * 2));
    oversampling.initProcessing(static_cast<size_t>(maxBlockSize));
}
```

## Buffer Size and Latency
Buffer size can vary between calls (up to `maxBlockSize`). Always use `buffer.getNumSamples()` — never assume a fixed size. Report latency accurately so the host aligns tracks:
```cpp
int getLatencySamples() const override {
    return lookaheadSamples + oversamplingLatency + fftLatency;
}
```
Call `setLatencySamples()` in `prepareToPlay()` and whenever latency changes.

## JUCE processBlock — Production Pattern
```cpp
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
{
    juce::ScopedNoDenormals noDenormals;
    const auto totalIn  = getTotalNumInputChannels();
    const auto totalOut = getTotalNumOutputChannels();
    for (int ch = totalIn; ch < totalOut; ++ch)
        buffer.clear(ch, 0, buffer.getNumSamples());
    for (int ch = 0; ch < totalIn; ++ch) {
        auto* channelData = buffer.getWritePointer(ch);
        for (int s = 0; s < buffer.getNumSamples(); ++s)
            channelData[s] = processOneSample(channelData[s]);
    }
}
```

## Denormals
Denormalized floats (very small values near zero) trigger slow microcode on x86, causing CPU spikes. `juce::ScopedNoDenormals` sets FTZ/DAZ flags — always place at top of `processBlock`. For feedback loops add a tiny DC offset (`1e-15f`) as extra protection.

## SIMD Optimization
- Process samples in blocks to enable auto-vectorization
- Use `juce::FloatVectorOperations` for bulk add, multiply, copy, clear
- Use `juce::dsp::SIMDRegister` for explicit SIMD (SSE2/AVX2/NEON)
- Align buffers to 16-byte (SSE) or 32-byte (AVX) boundaries

## Anti-Patterns
- ❌ Allocating on the audio thread — pre-allocate in `prepareToPlay()`
- ❌ Using `std::mutex` or any lock on the audio thread
- ❌ Calling functions that may allocate (`push_back`, `std::string`, `new`, `make_shared`)
- ❌ Logging, printing, or file I/O on the audio thread
- ❌ Virtual calls in tight inner loops — use CRTP or direct calls
- ❌ Assuming buffer size is constant — always use `buffer.getNumSamples()`
- ❌ Forgetting `ScopedNoDenormals` — causes random CPU spikes
- ❌ Using `try`/`catch` on the audio thread — not real-time safe

## Checklist
- [ ] No allocations in `processBlock` (verify with allocator hooks or ASan)
- [ ] No locks or mutexes in the audio thread code path
- [ ] `ScopedNoDenormals` at the start of `processBlock`
- [ ] All parameters accessed via atomics or lock-free queue
- [ ] Latency accurately reported and updated when it changes
- [ ] Buffer sizes never assumed — `buffer.getNumSamples()` everywhere
- [ ] Unused output channels cleared
- [ ] Pre-allocated buffers sized for `maxBlockSize` from `prepareToPlay`
- [ ] Meter/visualization data passed to GUI via lock-free mechanism
- [ ] Plugin tested at multiple buffer sizes (64, 128, 256, 512, 1024, 2048)
