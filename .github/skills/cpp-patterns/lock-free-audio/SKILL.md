---
name: "Lock-Free Programming for Real-Time Audio"
description: "Patterns for thread-safe, allocation-free audio↔GUI communication using atomics, ring buffers, SeqLocks, and epoch-based reclamation — JUCE/C++ in REVITHION STUDIO."
---

# Lock-Free Programming for Real-Time Audio

Audio callbacks run under hard deadlines (~1–5 ms). Any blocking op — mutex, heap alloc, syscall — causes glitches. All shared state between the audio thread and GUI thread must be lock-free.

## 1. std::atomic Memory Orderings
Use the weakest correct ordering. `relaxed` for flags; `acquire`/`release` for parameter handoff; `seq_cst` only when multiple atomics need global ordering (rare in audio).
```cpp
std::atomic<bool> bypass{false};
void processBlock(float* buf, int n) {
    if (bypass.load(std::memory_order_relaxed)) return;    // no data dependency
}
alignas(64) std::atomic<float> gain{1.0f};
void setGain(float v) { gain.store(v, std::memory_order_release); }    // GUI thread
float getGain()       { return gain.load(std::memory_order_acquire); } // audio thread
```

## 2. SPSC Ring Buffer
Single-producer single-consumer — the workhorse for audio ↔ GUI communication. Power-of-2 capacity, acquire/release on indices.
```cpp
template <typename T, size_t N> class SPSCRing {
    static_assert((N & (N - 1)) == 0);
    alignas(64) std::atomic<size_t> wPos{0};
    alignas(64) std::atomic<size_t> rPos{0};
    std::array<T, N> buf{};
public:
    bool push(const T& v) {
        size_t w = wPos.load(std::memory_order_relaxed), nw = (w+1) & (N-1);
        if (nw == rPos.load(std::memory_order_acquire)) return false;
        buf[w] = v; wPos.store(nw, std::memory_order_release); return true;
    }
    bool pop(T& v) {
        size_t r = rPos.load(std::memory_order_relaxed);
        if (r == wPos.load(std::memory_order_acquire)) return false;
        v = buf[r]; rPos.store((r+1) & (N-1), std::memory_order_release); return true;
    }
};
SPSCRing<float, 1024> meterRing; // audio → GUI level meters
```

## 3. Lock-Free FIFO — JUCE AbstractFifo
`AbstractFifo` manages indices; you supply storage. Use for MIDI queues, command queues.
```cpp
class MidiQueue {
    juce::AbstractFifo fifo{512};
    std::array<juce::MidiMessage, 512> buf;
public:
    void push(const juce::MidiMessage& m) {                 // GUI thread
        auto s = fifo.write(1);
        if (s.blockSize1 > 0) buf[s.startIndex1] = m;
    }
    void drain(juce::MidiBuffer& out, int off) {            // audio thread
        auto s = fifo.read(fifo.getNumReady());
        for (int i = 0; i < s.blockSize1; ++i) out.addEvent(buf[s.startIndex1+i], off);
        for (int i = 0; i < s.blockSize2; ++i) out.addEvent(buf[s.startIndex2+i], off);
    }
};
```

## 4. Hazard Pointers
Readers advertise held pointers; writer defers deletion until no reader references the old pointer.
```cpp
template <typename T, int Slots = 4> struct HazardGuard {
    static inline std::atomic<T*> hp[Slots]{};
    static T* protect(int slot, const std::atomic<T*>& src) {
        T* p; do { p = src.load(std::memory_order_relaxed);
            hp[slot].store(p, std::memory_order_release);
        } while (p != src.load(std::memory_order_acquire)); return p;
    }
    static void clear(int slot) { hp[slot].store(nullptr, std::memory_order_release); }
    static bool canDelete(T* old) {
        for (auto& h : hp) if (h.load(std::memory_order_acquire) == old) return false;
        return true;
    }
};
```

## 5. Epoch-Based Reclamation
Simpler than hazard pointers for two-thread audio. Reader pins global epoch; writer defers deletion until reader advances past retire epoch.
```cpp
class EpochReclaimer {
    std::atomic<uint64_t> global{0};
    alignas(64) std::atomic<uint64_t> readerEpoch{UINT64_MAX};
    std::vector<std::pair<uint64_t, void*>> retired;
public:
    void enter() { readerEpoch.store(global.load(std::memory_order_acquire), std::memory_order_release); }
    void leave() { readerEpoch.store(UINT64_MAX, std::memory_order_release); }
    void retire(void* p) {
        retired.emplace_back(global.fetch_add(1, std::memory_order_acq_rel), p);
        uint64_t safe = readerEpoch.load(std::memory_order_acquire);
        std::erase_if(retired, [safe](auto& e) {
            if (e.first < safe) { operator delete(e.second); return true; } return false; });
    }
};
```

## 6. Wait-Free Parameter Smoothing
Atomic target from GUI + one-pole ramp on audio thread eliminates zipper noise.
```cpp
struct SmoothedParam {
    std::atomic<float> target{0.f};
    float current = 0.f, coeff = 0.001f; // ~10 ms @ 44.1 kHz
    void set(float v) { target.store(v, std::memory_order_release); }
    float next() { current += coeff * (target.load(std::memory_order_acquire) - current); return current; }
};
```

## 7. Double-Buffer Pattern
Audio reads one buffer, GUI writes the other, atomic index swaps them.
```cpp
struct Preset { float reverb, delay, eq[8]; };
class DoubleBuffer {
    std::array<Preset, 2> buf; std::atomic<int> idx{0};
public:
    void write(const Preset& p) { int i=1-idx.load(std::memory_order_acquire); buf[i]=p; idx.store(i, std::memory_order_release); }
    const Preset& read() const  { return buf[idx.load(std::memory_order_acquire)]; }
};
```

## 8. SeqLock for Multi-Field State
Reader retries if sequence counter is odd (write in progress) or changed mid-read.
```cpp
struct State { float gain, pan, send, feedback; };
class SeqLock {
    alignas(64) std::atomic<uint32_t> seq{0}; State st{};
public:
    void write(const State& s) { seq.fetch_add(1, std::memory_order_release); st=s; seq.fetch_add(1, std::memory_order_release); }
    State read() const {
        State snap; uint32_t s;
        do { s = seq.load(std::memory_order_acquire); if (s&1) continue; snap = st;
        } while (s != seq.load(std::memory_order_acquire)); return snap;
    }
};
```

## 9. Cache Line Padding
Separate cross-thread atomics onto different 64-byte cache lines to prevent false sharing.
```cpp
struct AudioShared {
    alignas(64) std::atomic<float> peakLevel{0.f};  // audio thread writes
    alignas(64) std::atomic<float> gainTarget{1.f};  // GUI thread writes
};
```

## Anti-Patterns
| Anti-Pattern | Why | Fix |
|---|---|---|
| `std::mutex` on audio thread | Unbounded blocking → glitch | Atomics, SPSC queues |
| `new`/`delete` on audio thread | Heap lock contention | Pre-allocate before `prepareToPlay` |
| Priority inversion | GUI holds lock, audio spins | Lock-free only |
| Naive CAS → ABA problem | Recycled pointer fools `compare_exchange` | Tagged pointers / epoch reclamation |
| Adjacent un-padded atomics | False sharing across cores | `alignas(64)` every cross-thread atomic |

## Pre-Merge Checklist
- [ ] Zero heap allocations in `processBlock` (`-fsanitize=address` + allocator hook)
- [ ] Every `std::atomic` has explicit memory ordering — no unjustified `seq_cst`
- [ ] SPSC ring tested at capacity, empty, and across wraparound boundary
- [ ] Cross-thread atomics are `alignas(64)` — false sharing eliminated
- [ ] Double-buffer / SeqLock pass ThreadSanitizer (`-fsanitize=thread`)
- [ ] Hazard pointer / epoch reclaimer stress-tested under rapid GUI updates
- [ ] No `std::string`, `vector::push_back`, or `juce::String` on the audio thread
- [ ] Parameter smoothing verified — no zipper noise on rapid automation
- [ ] Lock-free structures validated on x86 (strong model) and ARM (weak model)
