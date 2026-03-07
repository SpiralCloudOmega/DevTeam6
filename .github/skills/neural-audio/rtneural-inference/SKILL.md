---
name: rtneural-inference
description: Real-time neural network inference for guitar amp/effect modeling using RTNeural C++ library
---

# RTNeural Real-Time Inference for Neural Amp Modeling

RTNeural is a lightweight C++ library for real-time neural network inference optimized for audio,
powering NAM-style amp models inside REVITHION STUDIO's signal chain.

## Model Architecture

Standard amp-modeling topology: Dense→LSTM→Dense. Input dense projects one sample
into a hidden dimension, LSTM captures temporal nonlinearity, output dense collapses to one sample.

```cpp
#include <RTNeural/RTNeural.h>

// Compile-time model: Dense(1→8) → LSTM(8→16) → Dense(16→1)
using AmpModel = RTNeural::ModelT<float,
    1,  // input size
    1,  // output size
    RTNeural::DenseT<float, 1, 8>,
    RTNeural::LSTMLayerT<float, 8, 16>,
    RTNeural::DenseT<float, 16, 1>
>;
```
## Loading JSON Weights

Export from PyTorch via `torch.save(model.state_dict())`, then convert to RTNeural JSON format.

```json
{ "in_size": 1, "layers": [
    { "type": "dense", "shape": [1, 8], "weights": [[...]], "bias": [...] },
    { "type": "lstm", "shape": [8, 16], "weights": { "W": [...], "U": [...], "b": [...] } },
    { "type": "dense", "shape": [16, 1], "weights": [[...]], "bias": [...] }
] }
```
```cpp
void loadModel(AmpModel& model, const std::string& jsonPath)
{
    std::ifstream file(jsonPath);
    nlohmann::json weights; file >> weights;
    RTNeural::torch_helpers::loadDense<float>(weights, "layers.0", model.get<0>());
    RTNeural::torch_helpers::loadLSTM<float>(weights, "layers.1", model.get<1>());
    RTNeural::torch_helpers::loadDense<float>(weights, "layers.2", model.get<2>());
    model.reset();
}
```
## Per-Sample Forward Pass

RTNeural processes one sample at a time via `forward()`, fitting inside a per-sample loop.

```cpp
float processSample(AmpModel& model, float input)
{
    alignas(16) float in[1] = { input };
    return model.forward(in);
}
```
## Backend Selection

RTNeural supports multiple SIMD backends via CMake. xsimd gives ~20–30% faster
inference on AVX2; Eigen is safer for ARM/cross-platform builds.

**xsimd (recommended for x86)** — add to CMakeLists.txt:
```cpp
set(RTNEURAL_XSIMD ON CACHE BOOL "" FORCE)
add_subdirectory(RTNeural)
target_link_libraries(YourPlugin PRIVATE RTNeural)
```

**Eigen (portable fallback)** — add to CMakeLists.txt:
```cpp
set(RTNEURAL_EIGEN ON CACHE BOOL "" FORCE)
set(RTNEURAL_XSIMD OFF CACHE BOOL "" FORCE)
add_subdirectory(RTNeural)
```
## Thread Safety

Audio callbacks run on a real-time thread. The neural model **must not** allocate, lock, or
do I/O during `forward()`. RTNeural's `ModelT` is allocation-free after construction.
Use an atomic pointer swap for hot-reloading weights:

```cpp
std::atomic<AmpModel*> activeModel { nullptr };
std::unique_ptr<AmpModel> modelA, modelB;

void loadNewWeightsOnMessageThread(const std::string& path)
{
    auto* idle = (activeModel.load() == modelA.get()) ? modelB.get() : modelA.get();
    loadModel(*idle, path);
    activeModel.store(idle);
}
```
## Latency Budget

At 44.1 kHz / 128 samples the budget is ~2.9 ms; at 48 kHz / 64 samples it's ~1.3 ms.
Target < 500 µs per 128-sample block. Profile with:

```cpp
auto t0 = std::chrono::high_resolution_clock::now();
for (int i = 0; i < numSamples; ++i)
    output[i] = model.forward(&input[i]);
auto us = std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::high_resolution_clock::now() - t0).count();
```
## Oversampling + JUCE Integration

Nonlinear neural models alias badly. Wrap the model in a JUCE `Oversampling`
(2× or 4×) inside an `AudioProcessor` to push aliasing above the audible range.

```cpp
class NeuralAmpProcessor : public juce::AudioProcessor
{
    AmpModel model;
    juce::dsp::Oversampling<float> oversampler { 1, 2, // 4× oversample
        juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

public:
    void prepareToPlay(double sr, int block) override
    {
        oversampler.initProcessing(static_cast<size_t>(block));
        model.reset();
    }

    void processBlock(juce::AudioBuffer<float>& buf, juce::MidiBuffer&) override
    {
        juce::dsp::AudioBlock<float> block(buf);
        auto os = oversampler.processSamplesUp(block);
        float* ch = os.getChannelPointer(0);
        for (size_t i = 0; i < os.getNumSamples(); ++i)
            ch[i] = model.forward(&ch[i]);
        oversampler.processSamplesDown(block);
    }

    void releaseResources() override { oversampler.reset(); }
};
```
## Anti-Patterns

| Mistake | Fix |
|---|---|
| Using `RTNeural::Model` (runtime API) in audio loop | Use `RTNeural::ModelT` compile-time API |
| Calling `model.reset()` on audio thread | Reset only when suspended or via double-buffer swap |
| Skipping oversampling with LSTM models | Always oversample 2×–4× before the neural model |
| Allocating inside `processBlock` | Pre-allocate all buffers in `prepareToPlay` |
| Building without xsimd or Eigen | Enable one SIMD backend in CMake |

## Checklist

- [ ] Model compiles with `RTNeural::ModelT` (no runtime polymorphism)
- [ ] Weights load successfully — output matches PyTorch reference within ±1e-5
- [ ] xsimd or Eigen backend enabled in CMake
- [ ] `forward()` < 500 µs per 128-sample block
- [ ] Oversampling active (2× minimum) before the neural model
- [ ] No allocations, locks, or I/O inside `processBlock`
- [ ] `model.reset()` called in `prepareToPlay`, never mid-stream
- [ ] Hot-reload uses atomic pointer swap
- [ ] Tested at 44.1 kHz and 48 kHz with 64/128/256 buffer sizes
