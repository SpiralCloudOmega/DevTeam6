---
name: ace-step-inference
version: "1.0"
description: ACE-Step 1.5 music generation — GGML inference, text-to-music, covers, repainting, CUDA acceleration, 48kHz stereo output for REVITHION STUDIO
tags: [ai, music-generation, ace-step, ggml, cuda, inference]
category: ai-integration
---

# ACE-Step 1.5 Music Generation Integration

ACE-Step 1.5 is a diffusion-based music generation model that produces full stereo audio from text prompts, reference tracks, or partial audio inputs. It supports three generation modes: **text-to-music** (prompt-only), **covers** (style transfer from a reference), and **repainting** (inpainting/outpainting on existing audio). REVITHION STUDIO integrates ACE-Step through a GGML-quantized C++ backend with CUDA acceleration, outputting 48kHz/32-bit float stereo suitable for direct insertion into the DAW timeline.

## Architecture Overview

The inference pipeline consists of a text encoder (CLAP), a latent diffusion UNet, and a vocoder (BigVGAN). The GGML backend loads quantized weights (Q4_K_M or Q8_0) into GPU VRAM via CUDA, keeping the host CPU free for DAW audio processing. A dedicated inference thread communicates with the audio engine through a lock-free FIFO, ensuring zero-glitch playback during generation.

## GGML Model Loading & CUDA Context

```cpp
#include <ggml/ggml.h>
#include <ggml/ggml-cuda.h>

struct AceStepContext {
    ggml_context* ctx = nullptr;
    ggml_backend_t backend = nullptr;
    ggml_backend_buffer_t buffer = nullptr;

    bool loadModel(const std::string& modelPath, int gpuLayers) {
        backend = ggml_backend_cuda_init(0);
        if (!backend) return false;

        struct ggml_init_params params = {
            .mem_size = 512 * 1024 * 1024,
            .mem_buffer = nullptr,
            .no_alloc = true
        };
        ctx = ggml_init(params);

        // Load quantized weights onto GPU
        auto* model = ggml_model_load(modelPath.c_str(), ctx, backend, gpuLayers);
        return model != nullptr;
    }

    ~AceStepContext() {
        if (ctx) ggml_free(ctx);
        if (buffer) ggml_backend_buffer_free(buffer);
        if (backend) ggml_backend_free(backend);
    }
};
```

## Text-to-Music Generation

```cpp
struct GenerationParams {
    std::string prompt;
    float durationSec = 30.0f;
    int steps = 100;
    float cfgScale = 7.0f;
    int sampleRate = 48000;
    int seed = -1; // -1 = random
};

std::vector<float> generateFromText(AceStepContext& ace, const GenerationParams& params) {
    auto tokens = ace.encodeText(params.prompt);

    // Diffusion loop with classifier-free guidance
    auto latent = ace.initNoise(params.durationSec, params.sampleRate, params.seed);
    for (int step = 0; step < params.steps; ++step) {
        auto conditioned = ace.denoise(latent, tokens, step, params.cfgScale);
        auto unconditioned = ace.denoise(latent, {}, step, params.cfgScale);
        latent = unconditioned + params.cfgScale * (conditioned - unconditioned);
    }

    return ace.vocoder(latent); // 48kHz stereo interleaved float
}
```

## Cover & Repainting Modes

```cpp
enum class AceMode { TextToMusic, Cover, Repaint };

std::vector<float> generateWithReference(AceStepContext& ace,
                                          const GenerationParams& params,
                                          AceMode mode,
                                          const float* refAudio,
                                          int refSamples,
                                          float strength = 0.75f) {
    auto latent = ace.encodeAudio(refAudio, refSamples);

    if (mode == AceMode::Cover) {
        // Partial noise injection preserving melodic structure
        int startStep = static_cast<int>(params.steps * (1.0f - strength));
        latent = ace.addNoise(latent, startStep);
        return ace.denoiseFrom(latent, ace.encodeText(params.prompt), startStep, params);
    }

    if (mode == AceMode::Repaint) {
        auto mask = ace.buildTimeMask(params.durationSec, params.sampleRate);
        return ace.inpaint(latent, mask, ace.encodeText(params.prompt), params);
    }

    return generateFromText(ace, params);
}
```

## JUCE Integration — Async Generation Thread

```cpp
class AceStepProcessor : public juce::Thread {
    AceStepContext context;
    juce::AbstractFifo fifo { 48000 * 120 * 2 }; // 120s stereo buffer
    std::vector<float> ringBuffer;
    std::atomic<bool> generating { false };

public:
    AceStepProcessor() : Thread("ACE-Step-Inference") {
        ringBuffer.resize(static_cast<size_t>(fifo.getTotalSize()));
    }

    void startGeneration(const GenerationParams& params) {
        currentParams = params;
        generating = true;
        startThread(juce::Thread::Priority::normal);
    }

    void run() override {
        auto audio = generateFromText(context, currentParams);
        int written = 0;
        while (written < static_cast<int>(audio.size()) && !threadShouldExit()) {
            auto scope = fifo.write(static_cast<int>(audio.size()) - written);
            std::copy_n(audio.data() + written, scope.blockSize1, ringBuffer.data() + scope.startIndex1);
            written += scope.blockSize1 + scope.blockSize2;
        }
        generating = false;
    }

    void pullSamples(float* dest, int numSamples) {
        auto scope = fifo.read(numSamples);
        std::copy_n(ringBuffer.data() + scope.startIndex1, scope.blockSize1, dest);
    }

private:
    GenerationParams currentParams;
};
```

## Python API Bridge (ACE-Step HTTP)

```python
import httpx, struct

async def generate_music(prompt: str, duration: float = 30.0,
                         mode: str = "text2music",
                         reference_path: str | None = None) -> bytes:
    """Call ACE-Step API server at localhost:8001."""
    payload = {
        "prompt": prompt, "duration": duration, "mode": mode,
        "sample_rate": 48000, "cfg_scale": 7.0, "steps": 100
    }
    if reference_path:
        payload["reference_audio"] = reference_path

    async with httpx.AsyncClient(timeout=300) as client:
        resp = await client.post("http://localhost:8001/generate", json=payload)
        resp.raise_for_status()
        return resp.content  # Raw 48kHz float32 PCM
```

## Anti-Patterns

- ❌ Don't run inference on the audio thread — always use a separate thread with FIFO handoff
- ❌ Don't load full FP32 weights on a 24GB GPU — use Q4_K_M or Q8_0 quantization to fit in VRAM
- ❌ Don't generate at 44.1kHz then resample — generate natively at 48kHz to avoid aliasing artifacts
- ❌ Don't block the UI thread waiting for generation — use async callbacks or polling
- ❌ Don't skip CUDA device synchronization before reading output buffers
- ❌ Don't use cfg_scale > 15 — it causes spectral collapse and harsh artifacts

## Checklist

- [ ] GGML backend initialized with CUDA device 0 before model load
- [ ] Model weights quantized to Q4_K_M or Q8_0 and validated with checksum
- [ ] Inference thread priority set below audio thread priority
- [ ] Ring buffer sized for maximum generation duration (120s × 48kHz × 2ch)
- [ ] Output sample rate matches DAW session rate (48kHz default)
- [ ] VRAM usage monitored — abort generation if free VRAM < 2GB
- [ ] Seed stored with generated clip for reproducibility
- [ ] All three modes (text-to-music, cover, repaint) tested with reference audio
