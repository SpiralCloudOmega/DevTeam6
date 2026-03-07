---
name: fft-spectral
description: FFT/STFT for audio — window functions, overlap-add, spectral processing, real-time constraints
---

# FFT & Spectral Processing for Audio

The Fast Fourier Transform is the backbone of modern spectral audio processing. It enables spectral analysis and visualization, convolution-based reverb, pitch shifting, noise reduction, vocoding, and cross-synthesis. The DFT converts a finite block of time-domain samples into a complex frequency-domain representation; the FFT is its O(N log N) implementation (vs. O(N²) for the naïve DFT), making real-time spectral processing feasible.

## The Discrete Fourier Transform

The DFT of N real samples produces N complex bins. Each bin k represents frequency `k * Fs / N` Hz. For real-valued input the spectrum is conjugate-symmetric, so only the first `N/2 + 1` bins (DC through Nyquist) carry unique information. Each complex bin encodes magnitude (`std::abs`) and phase (`std::atan2`). Frequency resolution is `Δf = Fs / N` — a 2048-point FFT at 48 kHz gives ~23.4 Hz per bin.

## Window Functions

Applying an FFT to a finite block implies a rectangular window, which causes severe spectral leakage — energy from one frequency smears into adjacent bins. Smooth window functions taper the signal edges to suppress sidelobes at the cost of a wider main lobe.

| Window | Peak Sidelobe | Main Lobe Width | Use Case |
|---|---|---|---|
| **Hann** | −31.5 dB | 1.5 bins | General-purpose analysis/synthesis |
| **Blackman** | −58 dB | 2.5 bins | Better sidelobe rejection |
| **Blackman-Harris** | −92 dB | 3.0 bins | High dynamic-range analysis |
| **Kaiser** (β tunable) | varies | varies | Adjustable tradeoff via β parameter |
| **Flat-top** | −44 dB | 3.8 bins | Accurate amplitude measurement |

```cpp
// Generate Hann and Blackman windows (C++17)
#include <cmath>
#include <vector>

inline std::vector<float> makeHannWindow(int N)
{
    std::vector<float> w(N);
    for (int n = 0; n < N; ++n)
        w[n] = 0.5f * (1.0f - std::cos(2.0f * M_PI * n / (N - 1)));
    return w;
}

inline std::vector<float> makeBlackmanWindow(int N)
{
    std::vector<float> w(N);
    constexpr float a0 = 0.42f, a1 = 0.5f, a2 = 0.08f;
    for (int n = 0; n < N; ++n)
        w[n] = a0 - a1 * std::cos(2.0f * M_PI * n / (N - 1))
                   + a2 * std::cos(4.0f * M_PI * n / (N - 1));
    return w;
}
```

## STFT: Short-Time Fourier Transform

The STFT segments a continuous signal into overlapping frames, windows each frame, then computes the FFT. The **hop size** H controls overlap — for a Hann window, H = N/4 (75% overlap) provides excellent time resolution while satisfying the COLA constraint. Larger N improves frequency resolution (narrower bins) but degrades time resolution (longer frames) — this is the fundamental time-frequency tradeoff.

```cpp
// STFT analysis: fill spectral frames from a mono input buffer
void stftAnalysis(const float* input, int numSamples, int fftSize, int hopSize,
                  const std::vector<float>& window, juce::dsp::FFT& fft,
                  std::vector<std::vector<float>>& spectralFrames)
{
    const int numBins = fftSize + 2; // juce FFT real-only format: fftSize+2 floats
    std::vector<float> frame(numBins, 0.0f);

    for (int pos = 0; pos + fftSize <= numSamples; pos += hopSize)
    {
        std::fill(frame.begin(), frame.end(), 0.0f);
        for (int n = 0; n < fftSize; ++n)
            frame[n] = input[pos + n] * window[n];

        fft.performRealOnlyForwardTransform(frame.data());
        spectralFrames.push_back(frame);
    }
}
```

## Overlap-Add Reconstruction

OLA synthesis inverts each spectral frame, applies a synthesis window, and accumulates overlapping frames into the output. The **COLA constraint** requires that the analysis and synthesis windows, when overlapped at the chosen hop size, sum to a constant. A Hann window with 50% overlap (H = N/2) or 75% overlap (H = N/4) satisfies COLA. With 75% overlap, the Hann window sums to exactly 1.5 — normalize by dividing by this constant.

```cpp
// OLA synthesis: reconstruct time-domain signal from spectral frames
void olaSynthesis(const std::vector<std::vector<float>>& spectralFrames,
                  int fftSize, int hopSize, const std::vector<float>& window,
                  juce::dsp::FFT& fft, std::vector<float>& output)
{
    std::vector<float> frame(fftSize + 2, 0.0f);
    output.assign(spectralFrames.size() * hopSize + fftSize, 0.0f);

    for (size_t i = 0; i < spectralFrames.size(); ++i)
    {
        std::copy(spectralFrames[i].begin(), spectralFrames[i].end(), frame.begin());
        fft.performRealOnlyInverseTransform(frame.data());

        const int outPos = static_cast<int>(i) * hopSize;
        for (int n = 0; n < fftSize; ++n)
            output[outPos + n] += frame[n] * window[n];
    }
}
```

## Zero-Padding

Zero-padding a frame to a larger FFT size interpolates between frequency bins — it increases the apparent frequency resolution of the DFT output without adding new spectral information. This is useful for smoother spectral displays and more precise peak detection. A common pattern: zero-pad to the next power of 2 for FFT efficiency (e.g., pad a 1000-sample frame to 1024).

## Spectral Processing Techniques

Once in the frequency domain, bins can be manipulated directly:

- **Spectral noise gate** — zero out bins whose magnitude falls below a threshold, suppressing low-level noise while preserving signal peaks.
- **Spectral EQ** — multiply each bin's magnitude by a frequency-dependent gain curve; this is equivalent to convolution with a very long FIR filter.
- **Spectral freeze** — capture a single STFT frame and continuously resynthesize it, creating an infinite sustain effect.
- **Cross-synthesis** — combine the magnitude spectrum of one signal with the phase spectrum of another, producing hybrid timbres.

```cpp
// Spectral noise gate — applied to one STFT frame in JUCE real-only format
// fftData layout: [re0, im0, re1, im1, ...] with fftSize+2 floats
void spectralNoiseGate(float* fftData, int fftSize, float thresholdDB)
{
    const float thresholdLinear = std::pow(10.0f, thresholdDB / 20.0f);
    const int numBins = fftSize / 2 + 1;

    for (int bin = 0; bin < numBins; ++bin)
    {
        const float re = fftData[bin * 2];
        const float im = fftData[bin * 2 + 1];
        const float magnitude = std::sqrt(re * re + im * im);

        if (magnitude < thresholdLinear)
        {
            fftData[bin * 2]     = 0.0f;
            fftData[bin * 2 + 1] = 0.0f;
        }
    }
}
```

## Real-Time FFT in JUCE

JUCE provides `juce::dsp::FFT` which wraps a platform-optimal FFT implementation. It takes an **order** (log₂ of FFT size), not the size directly.

```cpp
constexpr int fftOrder = 11;                         // 2^11 = 2048
constexpr int fftSize  = 1 << fftOrder;              // 2048
juce::dsp::FFT forwardFFT { fftOrder };

std::array<float, fftSize * 2> fftData {};           // interleaved re/im
forwardFFT.performRealOnlyForwardTransform(fftData.data());
// ... process spectral bins ...
forwardFFT.performRealOnlyInverseTransform(fftData.data());
```

The critical real-time pattern: `processBlock()` delivers buffers of arbitrary size (32–2048 samples typically) while the FFT requires exactly `fftSize` samples. Solve this with a **FIFO pair**:

1. Push incoming samples into an **input FIFO** ring buffer.
2. When `fftSize` samples are available, copy them out, apply window, run forward FFT.
3. Process in frequency domain.
4. Inverse FFT, apply synthesis window, overlap-add into an **output FIFO**.
5. Pull output samples from the output FIFO to fill `processBlock()`'s output buffer.

This introduces a minimum latency of one FFT frame (`fftSize / sampleRate` seconds). Report this via `getLatencySamples()`.

## Library Patterns

| Library | License | Notes |
|---|---|---|
| **FFTW** | GPL / commercial | Fastest general-purpose; uses wisdom-based planning |
| **KissFFT** | BSD | Simple, portable, good for embedded targets |
| **pffft** | BSD | Very fast, SIMD-optimized (SSE/NEON), header-friendly |
| **Intel IPP** | proprietary | Fastest on Intel CPUs, integrated with many DAW SDKs |
| **JUCE dsp::FFT** | GPLv3 / commercial | Wraps platform-optimal backend; easiest for JUCE projects |

## Anti-Patterns

- ❌ Don't skip windowing — spectral leakage will corrupt analysis and produce audible artifacts in resynthesis
- ❌ Don't use a rectangular window unless the signal is perfectly periodic within the frame
- ❌ Don't forget to normalize the inverse FFT — some libraries (including JUCE) scale by N, others don't
- ❌ Don't modify phase bins carelessly — discontinuities cause clicks; always consider phase continuity across frames
- ❌ Don't assume `processBlock()` buffer size equals FFT size — always use FIFO buffering
- ❌ Don't allocate memory on the audio thread — pre-allocate all FFT buffers in `prepareToPlay()`

## Checklist

- [ ] Window function applied before forward FFT
- [ ] COLA constraint verified for chosen window + hop size combination
- [ ] FFT size is a power of 2 for algorithmic efficiency
- [ ] Phase handling is correct for any spectral modification
- [ ] Input/output FIFOs decouple `processBlock()` buffer size from FFT size
- [ ] Zero heap allocations on the audio thread — all buffers pre-allocated
- [ ] Latency correctly reported via `getLatencySamples()` (≥ 1 FFT frame)
- [ ] Inverse FFT normalization matches the library's convention
