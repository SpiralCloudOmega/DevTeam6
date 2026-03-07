---
name: "Constexpr Code Generation for Audio DSP"
description: "Compile-time computation patterns for audio DSP in C++20 — lookup tables, filter coefficients, parameter validation, and REVITHION design token expansion with zero runtime overhead."
---

# Constexpr Code Generation for Audio DSP

Compile-time code generation for REVITHION STUDIO's audio engine.

## 1. Compile-Time Computation

```cpp
#include <cstdint>
#include <numbers>

constexpr double kSampleRate     = 48000.0;
constexpr std::size_t kBlockSize = 256;
constexpr double kNyquist        = kSampleRate / 2.0;
constexpr double kBlockLatencyMs = (static_cast<double>(kBlockSize) / kSampleRate) * 1000.0;

static_assert(kBlockSize > 0 && (kBlockSize & (kBlockSize - 1)) == 0,
              "Block size must be a power of two");
```

## 2. constexpr DSP Utility Functions

```cpp
constexpr double dbToLinear(double dB)     { return __builtin_pow(10.0, dB / 20.0); }
constexpr double linearToDb(double linear) { return 20.0 * __builtin_log10(linear); }
constexpr double freqToMidi(double freqHz) { return 69.0 + 12.0 * __builtin_log2(freqHz / 440.0); }
constexpr double midiToFreq(int note)      { return 440.0 * __builtin_pow(2.0, (note - 69) / 12.0); }

static_assert(midiToFreq(69) == 440.0, "A4 must be 440 Hz");
```

## 3. consteval — Immediate Functions (C++20)

`consteval` guarantees compile-time execution — use for table generators where runtime fallback is a performance bug.

```cpp
consteval auto generateSineTable() {
    constexpr std::size_t N = 2048;
    std::array<float, N> table{};
    for (std::size_t i = 0; i < N; ++i)
        table[i] = static_cast<float>(__builtin_sin(
            (static_cast<double>(i) / N) * 2.0 * std::numbers::pi));
    return table;
}

inline constexpr auto kSineTable = generateSineTable();  // lives in .rodata
```

## 4. constexpr Containers — Coefficient Arrays

```cpp
struct BiquadCoeffs { double b0, b1, b2, a1, a2; };

constexpr BiquadCoeffs makeLowpass(double freq, double Q, double sr) {
    double w0 = 2.0 * std::numbers::pi * freq / sr;
    double cosW0 = __builtin_cos(w0);
    double alpha = __builtin_sin(w0) / (2.0 * Q);
    double a0    = 1.0 + alpha;
    return { ((1.0-cosW0)/2.0)/a0, (1.0-cosW0)/a0,
             ((1.0-cosW0)/2.0)/a0, (-2.0*cosW0)/a0, (1.0-alpha)/a0 };
}

constexpr auto kLowBand = makeLowpass(200.0, 0.707, 48000.0);
constexpr auto kMidBand = makeLowpass(2000.0, 0.707, 48000.0);
```

## 5. Build-Time Token Expansion — REVITHION Design Tokens

S-expression tokens like `(filter :type biquad :freq 1000 :Q 0.707)` expand into constexpr structs during the CMake build.

```cpp
// Generated from: (filter :type biquad :freq 1000 :Q 0.707)
struct FilterToken {
    enum class Type { Biquad, StateVariable, OnePole };
    Type type; double frequency; double Q;
};
constexpr FilterToken kDefaultFilter{ FilterToken::Type::Biquad, 1000.0, 0.707 };

// Token feeds directly into the coefficient generator:
constexpr auto kDefaultCoeffs = makeLowpass(
    kDefaultFilter.frequency, kDefaultFilter.Q, kSampleRate);

// JUCE interop (non-constexpr) happens at prepare() time:
// auto* c = juce::dsp::IIR::Coefficients<float>::makeLowPass(
//     kSampleRate, kDefaultFilter.frequency, kDefaultFilter.Q);
```

## 6. Template Metaprogramming for DSP Chains

Variadic templates compose processor chains at compile time — no virtual dispatch, no heap.

```cpp
template <typename... Processors>
class ProcessorChain {
    std::tuple<Processors...> stages;
    template <std::size_t I = 0>
    void processBlock(float* buf, std::size_t n) {
        if constexpr (I < sizeof...(Processors)) {
            std::get<I>(stages).process(buf, n);
            processBlock<I + 1>(buf, n);
        }
    }
public:
    void prepare(double sr, std::size_t bs) {
        std::apply([&](auto&... p) { (p.prepare(sr, bs), ...); }, stages);
    }
    void process(float* buf, std::size_t n) { processBlock<0>(buf, n); }
};
// Usage: ProcessorChain<NoiseGate, Compressor, BiquadFilter, Saturator> channel;
```

## 7. Compile-Time Lookup Tables — Window Functions

```cpp
template <std::size_t N, typename F>
consteval auto generateWindow(F windowFn) {
    std::array<float, N> w{};
    for (std::size_t i = 0; i < N; ++i)
        w[i] = static_cast<float>(windowFn(static_cast<double>(i) / (N - 1)));
    return w;
}

inline constexpr auto kHanning1024 = generateWindow<1024>([](double t) {
    return 0.5 * (1.0 - __builtin_cos(2.0 * std::numbers::pi * t));
});
inline constexpr auto kBlackman2048 = generateWindow<2048>([](double t) {
    return 0.42 - 0.50 * __builtin_cos(2.0 * std::numbers::pi * t)
                + 0.08 * __builtin_cos(4.0 * std::numbers::pi * t);
});
```

## 8. static_assert Validation

```cpp
template <std::size_t BlockSize, unsigned SampleRate, int Channels>
struct AudioConfig {
    static_assert(BlockSize > 0 && (BlockSize & (BlockSize - 1)) == 0, "BlockSize must be power of two");
    static_assert(SampleRate == 44100 || SampleRate == 48000 || SampleRate == 88200 || SampleRate == 96000,
                  "SampleRate must be a standard audio rate");
    static_assert(Channels >= 1 && Channels <= 128, "Channel count out of range [1, 128]");
    static constexpr double nyquist = SampleRate / 2.0;
};
using StereoConfig = AudioConfig<256, 48000, 2>;
```

---

## Anti-Patterns

| ❌ Anti-Pattern | ✅ Correct Approach |
|---|---|
| `double nyquist = sr / 2.0;` at runtime | `constexpr double kNyquist = kSampleRate / 2.0;` |
| `#define BLOCK_SIZE 256` | `constexpr std::size_t kBlockSize = 256;` + `static_assert` |
| `sin(2 * 3.14159 * i / N)` magic numbers | `consteval` generator with `std::numbers::pi` |
| Lookup tables built in a constructor | `consteval` generator → `inline constexpr` in `.rodata` |
| No validation on buffer size / sample rate | `static_assert` on every config template parameter |
| `std::vector` for fixed-size coefficients | `std::array<double, N>` with constexpr init |

## Checklist

- [ ] All lookup tables generated with `consteval`, stored as `inline constexpr`
- [ ] `static_assert` on every DSP constraint (sample rate, block size, channels)
- [ ] Zero runtime overhead — verify tables in `.rodata` via `-O2` disassembly
- [ ] `consteval` for mandatory compile-time paths; `constexpr` only where runtime fallback is acceptable
- [ ] No `#define` for numeric constants — named `constexpr` variables only
- [ ] Design token expansion produces `constexpr` structs feeding coefficient generators
- [ ] Template processor chains fully inlined — no virtual dispatch or heap allocation
- [ ] JUCE interop separated: constexpr math feeds non-constexpr JUCE types at `prepare()` time
