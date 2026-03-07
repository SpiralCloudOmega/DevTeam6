---
name: SIMD Optimization for Audio Processing
description: >
  Portable SIMD patterns for real-time audio DSP using xsimd, Eigen, and
  compiler hints. Covers vectorized filters, gain ramps, alignment, SoA
  layouts, and benchmarking for SSE4.2/AVX2/NEON targets.
---

# SIMD Optimization for Audio Processing

SIMD lets you process 4 (SSE) or 8 (AVX2) float samples per instruction. This
skill covers portable abstractions and real DSP patterns for REVITHION STUDIO.

## 1 — xsimd Portable Abstraction + Batch Processing

```cpp
#include <xsimd/xsimd.hpp>
using Batch = xsimd::batch<float, xsimd::best_arch>; // 8 on AVX2, 4 on SSE/NEON

void applyGain(float* buffer, std::size_t N, float gain) {
    constexpr auto S = Batch::size;
    std::size_t i = 0;
    for (; i + S <= N; i += S)
        (Batch::load_aligned(buffer + i) * Batch(gain)).store_aligned(buffer + i);
    for (; i < N; ++i) buffer[i] *= gain; // scalar remainder
}
```

Force a specific arch: `xsimd::batch<float, xsimd::avx2>` (always 8 lanes).
For JUCE `AudioBuffer`, use `load_unaligned` — `getWritePointer()` doesn't
guarantee 32-byte alignment. Process each channel with the same pattern above.

## 2 — Vectorized Biquad Filter (Multi-Channel)

Pack one sample per channel into a SIMD register — each lane is an independent
biquad, processing up to `Batch::size` channels simultaneously.

```cpp
template <std::size_t NumCh>
struct SIMDBiquad {
    using B = xsimd::batch<float, xsimd::best_arch>;
    B b0, b1, b2, a1, a2, z1{0.f}, z2{0.f};
    void process(std::array<float*, NumCh>& ch, int N) {
        for (int n = 0; n < N; ++n) {
            alignas(32) float f[B::size] = {};
            for (std::size_t c = 0; c < NumCh; ++c) f[c] = ch[c][n];
            B x = B::load_aligned(f), y = b0*x + z1;
            z1 = b1*x - a1*y + z2;  z2 = b2*x - a2*y;
            y.store_aligned(f);
            for (std::size_t c = 0; c < NumCh; ++c) ch[c][n] = f[c];
    }   };
};
```

## 3 — Vectorized Gain Ramp

```cpp
void gainRamp(float* buf, std::size_t N, float start, float end) {
    using B = xsimd::batch<float, xsimd::best_arch>;
    constexpr auto S = B::size;
    const float delta = (end - start) / float(N);
    alignas(32) float off[S];
    for (std::size_t k = 0; k < S; ++k) off[k] = float(k) * delta;
    B vGain = B(start) + B::load_aligned(off);
    const B vStep(float(S) * delta);
    std::size_t i = 0;
    for (; i + S <= N; i += S, vGain += vStep)
        (B::load_aligned(buf+i) * vGain).store_aligned(buf+i);
    float g = start + float(i) * delta;
    for (; i < N; ++i, g += delta) buf[i] *= g;
}
```

## 4 — Memory Alignment

AVX2 needs 32-byte; AVX-512 needs 64-byte. Misaligned → crash or silent slowdown.

```cpp
alignas(32) float scratch[1024]; // stack — AVX2 ready
auto makeBuffer(std::size_t n) {  // heap
    void* p = nullptr;
    if (posix_memalign(&p, 32, n * sizeof(float))) throw std::bad_alloc();
    return std::unique_ptr<float[],decltype(&std::free)>((float*)p, &std::free);
}
inline bool isAligned(const void* p, std::size_t a) {
    return (reinterpret_cast<std::uintptr_t>(p) & (a-1)) == 0;
}
```

## 5 — Auto-Vectorization Hints
```cpp
// __restrict__ tells the compiler pointers don't alias
void mix(float* __restrict__ dst, const float* __restrict__ a,
         const float* __restrict__ b, int N) {
    #pragma omp simd
    for (int i = 0; i < N; ++i) dst[i] = a[i] + b[i];
}
```

CMake flags — gate to x86_64; ARM gets separate NEON flags:
```cmake
target_compile_options(audio_engine PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-mavx2 -mfma -O3 -ffast-math>
    $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2 /fp:fast /O2>)
```

C++23 `[[assume(N % 8 == 0)]];` lets the compiler skip remainder handling.

## 6 — SoA vs AoS Data Layout

AoS (bad): `struct Sample { float L,R,C,LFE; }; vector<Sample> audio;` —
scatters across cache lines, blocks vectorization. SoA (good):
`struct Audio { vector<float> L,R,C,LFE; };` — contiguous per-channel.
JUCE `AudioBuffer<float>` is already SoA (each channel = contiguous `float*`).

## 7 — Benchmarking SIMD vs Scalar

```cpp
#include <chrono>
void bench(const char* tag, void(*fn)(float*,size_t,float), float* buf, size_t N, float g) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < 1000; ++r) fn(buf, N, g);
    double us = std::chrono::duration<double,std::micro>(
        std::chrono::high_resolution_clock::now() - t0).count();
    printf("%-20s %.1f µs/block  %.2f cy/sample\n", tag, us/1000, (us*4.9e3)/(N*1000));
}
```

Target: **>4× speedup** (SSE), **>7×** (AVX2). Below 2× → check alignment/flags.

## 8 — Eigen for Matrix Audio Operations
```cpp
#include <Eigen/Dense>
void matrixMix(const Eigen::Matrix8f& M, const float* const* in,
               float* const* out, int N) {
    Eigen::Map<const Eigen::Matrix<float,8,Eigen::Dynamic>> I(in[0],8,N);
    Eigen::Map<Eigen::Matrix<float,8,Eigen::Dynamic>> O(out[0],8,N);
    O.noalias() = M * I;  // Eigen uses SIMD internally
}
```

Use `Eigen::aligned_allocator` for heap matrices; `-DEIGEN_DONT_ALIGN_STATICALLY` if conflicts.

## Anti-Patterns

| Anti-Pattern | Fix |
|---|---|
| `_mm256_load_ps` on unaligned ptr | Use `load_unaligned` or `alignas(32)` |
| Branching inside SIMD loop | `xsimd::select(mask, a, b)` for branchless paths |
| AoS layout for per-sample DSP | Convert to SoA / per-channel arrays |
| Skipping remainder samples | Always add scalar tail loop |
| Assuming AVX2 is available | Use `best_arch` or `xsimd::dispatch` |
| Blanket `-ffast-math` | Isolate to DSP translation units only |

## Checklist

- [ ] All SIMD buffers use `alignas(32)` (AVX2) or `alignas(64)` (AVX-512)
- [ ] Every SIMD loop has a scalar remainder tail
- [ ] Debug builds assert alignment via `isAligned()`
- [ ] Scalar fallback compiles and passes on non-SIMD targets (WASM, etc.)
- [ ] `__restrict__` on all non-aliasing pointer params
- [ ] Benchmarks confirm >2× speedup over scalar baseline per kernel
- [ ] CMake gates `-mavx2` to x86_64; ARM targets get NEON flags
- [ ] No hardcoded `__m256` — xsimd `best_arch` or `dispatch()` everywhere
- [ ] Eigen heap matrices use `aligned_allocator`
- [ ] CI tests both SSE-only and AVX2 configurations
