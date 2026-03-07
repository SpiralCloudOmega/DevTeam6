---
name: cab-modeling
description: Cabinet impulse response capture, neural cab modeling, and real-time convolution processing
---

# Cabinet & Impulse Response Modeling

Guitar cabinet modeling replaces physical speaker cabinets with digital impulse responses (IRs) or neural approximations, enabling studio-quality tone in real-time signal chains.

## Convolution Fundamentals
Time-domain convolution (`y[n] = Σ h[k]·x[n-k]`) is O(N·M) and impractical for long IRs. Frequency-domain convolution via overlap-save converts this to O(N log N) using FFT: transform both signals, multiply spectra, inverse-transform. For a 2048-sample IR at 48 kHz, frequency-domain is roughly 10× faster than brute-force.

## Impulse Response Capture Methodology
Exponential sine sweeps are preferred over linear sweeps because harmonic distortion products separate in time after deconvolution. The process: play a known stimulus through the cabinet/mic chain, record the result, deconvolve by inverse-filtering.

```python
import numpy as np
from scipy.signal import fftconvolve

def generate_sweep(duration_s, fs, f_start=20.0, f_end=20000.0):
    t = np.linspace(0, duration_s, int(fs * duration_s), endpoint=False)
    L = duration_s / np.log(f_end / f_start)
    return np.sin(2 * np.pi * f_start * L * (np.exp(t / L) - 1)).astype(np.float32)

def deconvolve_ir(recorded, sweep, fs):
    inverse_sweep = sweep[::-1]
    t = np.linspace(0, len(sweep) / fs, len(sweep), endpoint=False)
    inverse_sweep *= np.exp(-t / (len(sweep) / fs / np.log(20000 / 20)))
    ir = fftconvolve(recorded, inverse_sweep, mode='full')
    return (ir / np.max(np.abs(ir)))[:int(fs * 0.1)]
```

## Partitioned Convolution
Uniform partitioning splits the IR into equal-sized blocks matching the audio buffer, each processed via FFT multiply-accumulate. Non-uniform partitioning uses small partitions for early samples and larger ones for the tail — critical for sub-5 ms latency with long reverb IRs.

```cpp
class PartitionedConvolver {
    std::vector<std::vector<std::complex<float>>> irPartitions;
    juce::dsp::FFT fft;
    int blockSize, fftSize;
public:
    PartitionedConvolver(const float* ir, int irLen, int block)
        : fft(static_cast<int>(std::log2(block * 2))),
          blockSize(block), fftSize(block * 2) {
        int numParts = (irLen + blockSize - 1) / blockSize;
        irPartitions.resize(numParts, std::vector<std::complex<float>>(fftSize));
        for (int p = 0; p < numParts; ++p) {
            std::vector<float> buf(fftSize, 0.0f);
            std::copy_n(ir + p * blockSize, std::min(blockSize, irLen - p * blockSize), buf.begin());
            fft.performRealOnlyForwardTransform(buf.data());
            std::memcpy(irPartitions[p].data(), buf.data(), fftSize * sizeof(float));
        }
    }
};
```

## DynaCab — Neural Cabinet Modeling
DynaCab replaces static IRs with a neural network that interpolates between mic positions, distances, and speaker cone zones. A small MLP (~8K parameters) takes position coordinates as conditioning input and outputs filter coefficients, enabling smooth real-time morphing.

```cpp
// DynaCab concept: position-conditioned neural IR generator
struct DynaCabModel {
    std::array<float, 3> micPosition; // normalized [on/off-axis, cone zone, distance]
    void generateIR(float* irOutput, int irLength) {
        float hidden[32];
        denseLayerReLU(micPosition.data(), 3, hidden, 32, weightsL1, biasL1);
        denseLayerReLU(hidden, 32, hidden, 32, weightsL2, biasL2);
        denseLayerReLU(hidden, 32, hidden, 32, weightsL3, biasL3);
        denseLayerTanh(hidden, 32, irOutput, irLength, weightsL4, biasL4);
    }
};
```

## Minimum-Phase vs Mixed-Phase IRs
Minimum-phase IRs concentrate energy at the start and add zero latency — ideal for live guitar cab simulation. Mixed-phase (linear-phase) IRs preserve the original time-domain shape including pre-ring, better for mastering. Convert via cepstral method: `FFT → log magnitude → IFFT → window causal half → FFT → exp → IFFT`.

## Stereo IRs and Mic Position Modeling
Capture stereo IRs with two mics: on-axis (bright, focused) and off-axis (darker, roomy). Typical positions — on-axis at cap edge, 45° off-axis at 6 inches, room mic at 3+ feet. Crossfade between channels with a single knob for panning control.

## IR Length vs Quality Tradeoffs

| Samples | Duration @ 48 kHz | Use Case | CPU Cost |
|---------|-------------------|----------|----------|
| 256 | 5.3 ms | Tight cab, minimal color | Very low |
| 512 | 10.7 ms | Standard guitar cab | Low |
| 1024 | 21.3 ms | Full cab with room | Moderate |
| 2048 | 42.7 ms | Cab + early reflections | Higher |

Guitar cabs rarely need more than 1024 samples. Reverb IRs may need 48000+ (1 second).

## Real-Time JUCE Implementation

```cpp
class CabSimProcessor : public juce::AudioProcessor {
    juce::dsp::Convolution convolution;
    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        juce::dsp::ProcessSpec spec{sampleRate,
            static_cast<juce::uint32>(samplesPerBlock), 2};
        convolution.prepare(spec);
        convolution.loadImpulseResponse(juce::File("/path/to/cab.wav"),
            juce::dsp::Convolution::Stereo::yes,
            juce::dsp::Convolution::Trim::yes, 1024);
    }
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override {
        juce::dsp::AudioBlock<float> block(buffer);
        convolution.process(juce::dsp::ProcessContextReplacing<float>(block));
    }
};
```

## Anti-Patterns

- **Loading IRs on the audio thread** — always load asynchronously; use background thread with lock-free handoff.
- **Time-domain convolution for IRs > 128 samples** — switch to FFT-based partitioned convolution.
- **Ignoring sample rate mismatch** — resample IRs to session rate before loading; mismatched rates shift frequency response.
- **Skipping normalization** — peak-normalize deconvolved IRs to -1 dBFS before use.
- **Linear-phase IRs for live monitoring** — pre-ring causes audible artifacts; use minimum-phase for real-time paths.

## Checklist

- [ ] IR capture uses exponential sweep with proper inverse filtering
- [ ] Deconvolved IR is trimmed, normalized, and sample-rate matched
- [ ] Convolution uses partitioned FFT (not brute-force) for IRs > 128 samples
- [ ] IR loading happens off the audio thread with lock-free swap
- [ ] Minimum-phase conversion applied for zero-latency monitoring paths
- [ ] Stereo IR mic positions documented (angle, distance, axis)
- [ ] IR length chosen appropriate to use case (cab ≤1024, reverb ≤48000+)
- [ ] DynaCab model inference fits within real-time budget (< 1 ms per block)
