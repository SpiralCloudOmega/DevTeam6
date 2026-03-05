#pragma once

#include "NeuralAmpModel.h"
#include <array>
#include <cmath>

namespace revithion::neural {

// LSTM amp model using RTNeural (when available) or fallback implementation
// Architecture: Input(1) -> LSTM(hidden_size) -> Dense(1)
// Based on: "Build AI-Enhanced Audio Plugins with C++" Ch.42
class LSTMAmpModel : public NeuralAmpModel {
public:
    explicit LSTMAmpModel(int hiddenSize = 40);
    ~LSTMAmpModel() override;

    bool loadWeights(const std::string& jsonPath) override;
    float processSample(float input) override;
    void reset() override;

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace revithion::neural
