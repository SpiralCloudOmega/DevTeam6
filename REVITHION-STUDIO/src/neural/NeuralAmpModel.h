#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace revithion::neural {

// Supported neural network architectures for amp modeling
enum class ModelArch : uint8_t {
    LSTM,   // Long Short-Term Memory (most common for amps)
    GRU,    // Gated Recurrent Unit
    WaveNet // Simplified WaveNet (future)
};

// Metadata about a trained amp model
struct AmpModelInfo {
    std::string name;           // e.g. "MESA IIC+ Lead"
    std::string category;       // e.g. "High-Gain"
    ModelArch arch = ModelArch::LSTM;
    int hiddenSize = 40;        // LSTM hidden state size (32-64 typical)
    int numLayers = 1;          // Usually 1 for real-time
    int inputSize = 1;          // Mono input (1 sample at a time)
    int outputSize = 1;         // Mono output
    float sampleRate = 48000.0f;
    std::string weightsPath;    // Path to .json weights file
};

// Base class for neural amp models
class NeuralAmpModel {
public:
    virtual ~NeuralAmpModel() = default;

    // Load model weights from JSON file (RTNeural format)
    virtual bool loadWeights(const std::string& jsonPath) = 0;

    // Process a single sample (real-time safe — no allocations)
    virtual float processSample(float input) = 0;

    // Process a block of samples
    virtual void processBlock(const float* input, float* output, int numSamples);

    // Reset internal state (e.g. LSTM hidden/cell state)
    virtual void reset() = 0;

    // Model info
    const AmpModelInfo& info() const { return info_; }
    bool isLoaded() const { return loaded_; }

    // Factory: create model from info
    static std::unique_ptr<NeuralAmpModel> create(const AmpModelInfo& info);

protected:
    AmpModelInfo info_;
    bool loaded_ = false;
};

} // namespace revithion::neural
