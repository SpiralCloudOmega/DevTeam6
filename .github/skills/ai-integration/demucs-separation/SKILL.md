---
name: demucs-separation
version: "1.0"
description: AI stem separation with Demucs/HTDemucs — 4-stem and 6-stem modes, GPU inference via LibTorch, real-time preview for REVITHION STUDIO
tags: [ai, stem-separation, demucs, libtorch, cuda, audio]
category: ai-integration
---

# AI Stem Separation with Demucs/HTDemucs

Demucs (Hybrid Transformer Demucs) is Meta's state-of-the-art source separation model that splits mixed audio into individual stems. REVITHION STUDIO integrates HTDemucs v4 via LibTorch C++ for native GPU-accelerated inference. The 4-stem model separates drums, bass, vocals, and other; the 6-stem model adds guitar and piano. Output is 44.1kHz float32 per stem, resampled to the session rate (typically 48kHz). A real-time preview mode processes small overlapping chunks for interactive auditioning before committing to full offline separation.

## Model Configuration

HTDemucs uses a hybrid architecture combining a temporal convolutional network with a spectral transformer. The C++ integration loads TorchScript-traced models, keeping the audio engine independent of Python at runtime.

```cpp
struct DemucsConfig {
    int numStems = 4;           // 4 = drums/bass/vocals/other, 6 adds guitar/piano
    int sampleRate = 44100;     // Native model rate
    int segmentLength = 7;      // Seconds per chunk (overlap-add)
    float overlap = 0.25f;      // 25% overlap between segments
    bool useCuda = true;
    int cudaDevice = 0;
    juce::String modelPath;     // Path to .pt TorchScript model
};
```

## LibTorch C++ Integration

```cpp
#include <torch/script.h>
#include <torch/cuda.h>

class DemucsEngine {
    torch::jit::script::Module model;
    torch::Device device;
    DemucsConfig config;

public:
    DemucsEngine(const DemucsConfig& cfg)
        : device(cfg.useCuda && torch::cuda::is_available()
                 ? torch::Device(torch::kCUDA, cfg.cudaDevice)
                 : torch::Device(torch::kCPU)),
          config(cfg) {}

    bool loadModel() {
        try {
            model = torch::jit::load(config.modelPath.toStdString(), device);
            model.eval();
            torch::NoGradGuard noGrad;
            // Warm up with a short tensor to trigger CUDA kernel compilation
            auto dummy = torch::randn({1, 2, config.sampleRate * 2}).to(device);
            model.forward({dummy});
            return true;
        } catch (const c10::Error& e) {
            DBG("Demucs load failed: " << e.what());
            return false;
        }
    }

    // Returns [numStems, 2, numSamples] tensor
    torch::Tensor separate(const float* interleavedStereo, int numFrames) {
        torch::NoGradGuard noGrad;
        auto input = torch::from_blob(
            const_cast<float*>(interleavedStereo),
            {1, 2, numFrames}, torch::kFloat32
        ).to(device);

        auto output = model.forward({input}).toTensor(); // [1, stems, 2, samples]
        return output.squeeze(0).cpu();
    }
};
```

## Overlap-Add Segment Processing

Full tracks are too long for a single forward pass. Segment the input with overlap and crossfade the outputs to eliminate boundary artifacts.

```cpp
struct StemResult {
    std::vector<std::vector<float>> stems; // [stemIdx][interleavedSamples]
};

StemResult separateFullTrack(DemucsEngine& engine, const float* audio,
                              int totalFrames, const DemucsConfig& cfg) {
    int segSamples = cfg.segmentLength * cfg.sampleRate;
    int hopSamples = static_cast<int>(segSamples * (1.0f - cfg.overlap));
    int numStems = cfg.numStems;

    StemResult result;
    result.stems.resize(static_cast<size_t>(numStems),
                        std::vector<float>(static_cast<size_t>(totalFrames * 2), 0.0f));
    std::vector<float> weightSum(static_cast<size_t>(totalFrames), 0.0f);

    // Build Hann crossfade window
    std::vector<float> window(static_cast<size_t>(segSamples));
    for (int i = 0; i < segSamples; ++i)
        window[static_cast<size_t>(i)] = 0.5f * (1.0f - std::cos(2.0f * M_PI * i / segSamples));

    for (int offset = 0; offset < totalFrames; offset += hopSamples) {
        int chunkLen = std::min(segSamples, totalFrames - offset);
        auto output = engine.separate(audio + offset * 2, chunkLen);

        for (int s = 0; s < numStems; ++s) {
            auto stemData = output[s].contiguous().data_ptr<float>();
            for (int i = 0; i < chunkLen; ++i) {
                float w = window[static_cast<size_t>(i)];
                size_t outIdx = static_cast<size_t>((offset + i) * 2);
                result.stems[s][outIdx]     += stemData[i * 2]     * w;
                result.stems[s][outIdx + 1] += stemData[i * 2 + 1] * w;
                weightSum[static_cast<size_t>(offset + i)] += w;
            }
        }
    }

    // Normalize by accumulated window weight
    for (int s = 0; s < numStems; ++s)
        for (size_t i = 0; i < static_cast<size_t>(totalFrames); ++i) {
            if (weightSum[i] > 0.0f) {
                result.stems[s][i * 2]     /= weightSum[i];
                result.stems[s][i * 2 + 1] /= weightSum[i];
            }
        }

    return result;
}
```

## Python — Batch Separation Script

```python
import torch, torchaudio
from demucs.pretrained import get_model
from demucs.apply import apply_model

def separate_file(input_path: str, output_dir: str, model_name: str = "htdemucs") -> list[str]:
    model = get_model(model_name)
    model.cuda()
    wav, sr = torchaudio.load(input_path)
    wav = wav.unsqueeze(0).cuda()  # [1, channels, samples]

    with torch.no_grad():
        stems = apply_model(model, wav, shifts=1, overlap=0.25)

    paths = []
    for i, name in enumerate(model.sources):
        out = stems[0, i].cpu()
        path = f"{output_dir}/{name}.wav"
        torchaudio.save(path, out, sr)
        paths.append(path)
    return paths
```

## Anti-Patterns

- ❌ Don't run separation on the audio thread — even GPU inference takes 2–10× real-time
- ❌ Don't skip overlap-add windowing — hard segment boundaries cause audible clicks
- ❌ Don't assume 48kHz input — Demucs expects 44.1kHz; resample before inference
- ❌ Don't allocate CUDA tensors in a loop — pre-allocate and reuse buffers
- ❌ Don't mix LibTorch CUDA contexts with GGML CUDA contexts without stream synchronization
- ❌ Don't forget `torch::NoGradGuard` — gradient tracking wastes VRAM during inference

## Checklist

- [ ] LibTorch linked with CUDA 12+ and matching cuDNN version
- [ ] TorchScript model traced and validated against Python reference outputs
- [ ] Overlap-add window function tested for perfect reconstruction (sum-to-one)
- [ ] Sample rate conversion (48kHz ↔ 44.1kHz) uses high-quality sinc resampler
- [ ] GPU memory monitored — fallback to CPU if VRAM < 4GB free
- [ ] Preview mode processes ≤ 8 seconds to maintain interactive latency
- [ ] Stem output gain-matched to original mix (sum of stems ≈ input)
- [ ] All 4/6 stem names mapped to correct mixer channels
