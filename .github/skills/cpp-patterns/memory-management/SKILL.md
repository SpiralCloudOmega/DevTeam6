---
name: Audio Memory Management
description: Real-time-safe memory strategies for C++/JUCE audio engines — pre-allocation, pools, arenas, and RAII patterns for REVITHION STUDIO.
---

# Audio Memory Management

Memory allocation on the audio thread causes glitches, pops, and xruns. These patterns keep `processBlock()` allocation-free.

## 1. Pre-Allocation Strategies
Allocate all buffers in `prepareToPlay()` — never in `processBlock()`.
```cpp
void prepareToPlay(double sampleRate, int maxBlockSize) override {
    workBuffer.setSize(2, maxBlockSize);
    delayLine.assign(static_cast<size_t>(sampleRate * 2.0), 0.0f);
    delayLine.shrink_to_fit();
}
```

## 2. Memory Pools
Fixed-size pool for polyphonic voices — placement new, zero heap traffic at note-on.
```cpp
template <typename T, size_t N> class VoicePool {
    std::array<std::aligned_storage_t<sizeof(T), alignof(T)>, N> slots;
    std::array<bool, N> used{};
public:
    T* acquire() {
        for (size_t i = 0; i < N; ++i)
            if (!used[i]) { used[i] = true; return new (&slots[i]) T(); }
        return nullptr;
    }
    void release(T* p) {
        for (size_t i = 0; i < N; ++i)
            if (p == reinterpret_cast<T*>(&slots[i])) { p->~T(); used[i] = false; return; }
    }
};
```
For STL containers use `std::pmr::monotonic_buffer_resource` over a stack-allocated `std::array<std::byte, 65536>`.

## 3. Arena Allocators
Bump allocator for per-block scratch — O(1) reset at block boundary.
```cpp
class ArenaAllocator {
    std::vector<std::byte> buf; size_t off = 0;
public:
    explicit ArenaAllocator(size_t n) : buf(n) {}
    void* alloc(size_t sz, size_t al = alignof(std::max_align_t)) {
        size_t a = (off + al - 1) & ~(al - 1);
        if (a + sz > buf.size()) return nullptr;
        void* p = buf.data() + a; off = a + sz; return p;
    }
    void reset() { off = 0; }
};
```

## 4. No-Allocation Audio Thread
**Golden rule: zero heap allocations in processBlock().** Verify in debug builds:
```cpp
#if JUCE_DEBUG
thread_local bool tl_isAudioThread = false;
void* operator new(size_t sz) {
    if (tl_isAudioThread) jassertfalse; // allocation on audio thread!
    return std::malloc(sz);
}
#endif
```

## 5. JUCE HeapBlock & AudioBuffer Layout
`HeapBlock<T>` — raw resizable block, allocation-free after init. `AudioBuffer<float>` stores per-channel pointers; channels are **not** guaranteed contiguous.
```cpp
juce::HeapBlock<float> fftData;
void prepareToPlay(double, int maxBlock) override { fftData.allocate(maxBlock * 2, true); }
// In processBlock:
float* w = buffer.getWritePointer(ch);       // direct pointer, no copy
const float* r = buffer.getReadPointer(ch);
// Never assume getWritePointer(0) + numSamples == getWritePointer(1)
```

## 6. Smart Pointers for Audio Resources
`unique_ptr` for owned DSP processors. `shared_ptr` only for truly shared data (wavetables, IRs) — **never copy on the audio thread** (atomic ref-count can block).
```cpp
std::unique_ptr<juce::dsp::Reverb> reverb;
auto ir = std::make_shared<juce::AudioBuffer<float>>();
auto* irRaw = ir.get(); // raw ptr safe on audio thread if lifetime guaranteed
// Custom deleter: free on message thread
auto del = [](Resource* r){ juce::MessageManager::callAsync([r]{ delete r; }); };
std::unique_ptr<Resource, decltype(del)> res(new Resource, del);
```

## 7. RAII for DSP Resources
Wrap external handles so cleanup is automatic and exception-safe.
```cpp
class FFTGuard {
    void* plan;
public:
    explicit FFTGuard(int sz) : plan(fftw_plan_create(sz)) {}
    ~FFTGuard() { if (plan) fftw_plan_destroy(plan); }
    FFTGuard(const FFTGuard&) = delete;
    FFTGuard& operator=(const FFTGuard&) = delete;
};
```

## 8. Stack Allocation & ProcessorChain
`std::array` for small scratch (< 2 KB) — no heap, no `alloca`, no VLAs. `ProcessorChain` owns processors inline via `std::tuple`; call `prepare()` before first `process()`.
```cpp
std::array<float, 512> temp{};                    // stack scratch
std::copy_n(buffer.getReadPointer(0), numSamples, temp.begin());
juce::dsp::ProcessSpec spec{ sampleRate, static_cast<juce::uint32>(maxBlock), 2 };
chain.prepare(spec);                               // allocates all internal state
```

## Anti-Patterns
| Pattern | Risk |
|---|---|
| `new`/`delete` in `processBlock()` | Heap lock → priority inversion → glitch |
| `std::vector::push_back` in audio callback | Reallocation + copy |
| `std::shared_ptr` copy on audio thread | Atomic ref-count contention |
| Skipping `prepareToPlay` pre-allocation | First block triggers lazy alloc → pop |

## Checklist
- [ ] Zero heap allocations in `processBlock()` verified via debug `operator new` override
- [ ] All buffers pre-sized in `prepareToPlay()`
- [ ] Memory pools sized for worst-case polyphony (128 voices)
- [ ] Arena `reset()` called at start of every block
- [ ] RAII wrappers on all external DSP handles
- [ ] No `std::string`/`std::map` on audio thread
- [ ] Lock-free FIFO / atomics for cross-thread communication
