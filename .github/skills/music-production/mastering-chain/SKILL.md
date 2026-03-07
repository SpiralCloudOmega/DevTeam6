---
name: mastering-chain
description: Professional mastering chain for REVITHION STUDIO — EQ, multiband compression, M/S processing, limiting, dithering, and loudness targeting in C++/JUCE.
---

# Mastering Chain — REVITHION STUDIO

## 1. Signal Flow

Chain order is strict: **EQ → Multiband Compressor → Stereo Enhancer → Exciter → Limiter → Dither**.

```cpp
void MasteringChain::buildGraph() {
    auto io  = graph.addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioInputNode));
    auto out = graph.addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode));
    auto eq  = graph.addNode(std::make_unique<LinearPhaseEQProcessor>());
    auto mb  = graph.addNode(std::make_unique<MultibandCompressorProcessor>());
    auto st  = graph.addNode(std::make_unique<StereoEnhancerProcessor>());
    auto lim = graph.addNode(std::make_unique<BrickWallLimiterProcessor>());
    auto dit = graph.addNode(std::make_unique<DitheringProcessor>());
    // Wire: io→eq→mb→st→lim→dit→out (both channels)
    NodeID chain[] = { io->nodeID, eq->nodeID, mb->nodeID, st->nodeID, lim->nodeID, dit->nodeID, out->nodeID };
    for (int ch = 0; ch < 2; ++ch)
        for (int i = 0; i < 6; ++i)
            graph.addConnection({ { chain[i], ch }, { chain[i + 1], ch } });
}
```

## 2. Linear-Phase EQ

Linear-phase avoids phase distortion across the stereo image. Uses symmetric FIR via FFT convolution.

```cpp
void LinearPhaseEQProcessor::updateKernel() {
    const int fftSize = 4096;
    juce::dsp::FFT fft(12);
    std::vector<float> mag(fftSize, 1.0f);
    for (int bin = 0; bin < fftSize / 2; ++bin) {
        float freq = bin * sampleRate / fftSize;
        if (freq < 30.0f)  mag[bin] *= juce::Decibels::decibelsToGain(-12.0f); // subsonic cut
        if (freq > 10000.f) mag[bin] *= juce::Decibels::decibelsToGain(2.0f);  // air shelf
    }
    // IFFT magnitude-only → symmetric impulse, then apply Hann window
    // Load into juce::dsp::Convolution for real-time overlap-add
}
```

## 3. Multiband Compression

Four bands via Linkwitz-Riley crossovers (24 dB/oct) for phase-coherent splitting.

```cpp
// Crossover frequencies: 120 Hz, 1 kHz, 8 kHz
juce::dsp::LinkwitzRileyFilter<float> xLow, xMid, xHigh;

struct BandCompressor {
    float threshold, ratio, attackMs, releaseMs, envelope = 0.f;
    float processSample(float in, float sr) {
        float dB    = juce::Decibels::gainToDecibels(std::abs(in), -100.f);
        float over  = std::max(0.f, dB - threshold);
        float target = over * (1.f - 1.f / ratio);
        float coeff = std::exp(-1.f / ((target > envelope ? attackMs : releaseMs) * 0.001f * sr));
        envelope = coeff * envelope + (1.f - coeff) * target;
        return in * juce::Decibels::decibelsToGain(-envelope);
    }
};
// Typical mastering: Sub(-12dB,2:1,30/200ms) LowMid(-18,3:1,10/100) HiMid(-20,2.5:1,5/80) Air(-24,2:1,3/60)
```

## 4. Mid/Side Processing

M/S separates center (Mid = L+R) from spatial content (Side = L−R).

```cpp
// Encode
float mid  = (left + right) * 0.5f;
float side = (left - right) * 0.5f;
// Process: high-pass side at 200Hz (tighten bass in center), boost side above 3kHz (widen)
side = sideHPF.processSample(side) * widthGain;
// Decode
left  = mid + side;
right = mid - side;
```

## 5. Brick-Wall Limiting

True-peak limiting with lookahead anticipates transients before they arrive.

```cpp
class BrickWallLimiter {
    static constexpr int lookahead = 256; // ~5ms @ 48kHz
    juce::AudioBuffer<float> delayBuf;
    std::vector<float> gr;
    int writePos = 0;
    float ceiling = juce::Decibels::decibelsToGain(-0.3f); // -0.3 dBTP

    void processBlock(juce::AudioBuffer<float>& buf) {
        for (int i = 0; i < buf.getNumSamples(); ++i) {
            float peak = 0.f;
            for (int ch = 0; ch < 2; ++ch)
                peak = std::max(peak, std::abs(buf.getSample(ch, i)));
            float g = (peak > ceiling) ? ceiling / peak : 1.f;
            for (int j = 0; j < lookahead; ++j)
                gr[(writePos + j) % lookahead] = std::min(gr[(writePos + j) % lookahead], g);
            int rp = (writePos + 1) % lookahead;
            for (int ch = 0; ch < 2; ++ch) {
                buf.setSample(ch, i, delayBuf.getSample(ch, rp) * gr[rp]);
                delayBuf.setSample(ch, writePos, buf.getSample(ch, i));
            }
            gr[rp] = 1.f;
            writePos = (writePos + 1) % lookahead;
        }
    }
    // IRC I: single exponential release. IRC II: fast + slow two-stage. IRC III: per-band release.
};
```

## 6. Dithering

TPDF dither before truncation eliminates quantization distortion. Always the **last** process.

```cpp
void DitheringProcessor::processBlock(juce::AudioBuffer<float>& buf, juce::MidiBuffer&) {
    const float maxVal = std::pow(2.f, 15.f); // 16-bit target
    const float lsb = 1.f / maxVal;
    for (int ch = 0; ch < buf.getNumChannels(); ++ch) {
        auto* d = buf.getWritePointer(ch);
        for (int i = 0; i < buf.getNumSamples(); ++i) {
            float tpdf = (rng.nextFloat() - 0.5f) + (rng.nextFloat() - 0.5f);
            float shaped = d[i] - prevError * 0.5f; // first-order noise shaping
            float q = std::round((shaped + tpdf * lsb) * maxVal) / maxVal;
            prevError = q - shaped;
            d[i] = q;
        }
    }
}
```

## 7. Target Loudness — EBU R128

| Platform     | Target    | Normalization    |
|-------------|-----------|------------------|
| Spotify     | -14 LUFS  | Gain down only   |
| Apple Music | -16 LUFS  | Gain up/down     |
| YouTube     | -14 LUFS  | Gain down only   |
| CD Master   | -9 LUFS   | None             |

```cpp
float autoGainForTarget(float measuredLUFS, float targetLUFS) {
    float delta = juce::jlimit(-6.f, 6.f, targetLUFS - measuredLUFS);
    return juce::Decibels::decibelsToGain(delta);
}
// Measurement uses K-weighted filter (pre-filter shelf +4dB@1681Hz, RLB HP@38Hz)
// then gated mean-square per EBU R128: LUFS = -0.691 + 10*log10(meanSquare)
```

## Anti-Patterns

- **EQ after limiting** — any boost post-limiter causes clipping
- **Double dithering** — dither only on the final bounce, never twice
- **Butterworth crossovers in multiband** — use Linkwitz-Riley for flat recombination
- **Ignoring inter-sample peaks** — use true-peak metering per ITU-R BS.1770
- **M/S without gain compensation** — side boost raises overall level; re-normalize after decode
- **Single LUFS target for all platforms** — master to loudest target, create per-platform versions

## Checklist

- [ ] Chain order: EQ → Compressor → Stereo → Exciter → Limiter → Dither
- [ ] Linear-phase EQ kernel symmetric and windowed
- [ ] Crossovers use Linkwitz-Riley (phase-coherent summation)
- [ ] M/S encode/decode gain-matched (0.5 factor)
- [ ] Limiter lookahead ≥ 5 ms, ceiling ≤ -0.3 dBTP
- [ ] TPDF dither amplitude = 1 LSB of target bit depth
- [ ] Loudness measured per EBU R128 with gating
- [ ] All processors report latency for PDC in the graph
