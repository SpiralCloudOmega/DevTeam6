#include "LSTMAmpModel.h"
#include "NeuralAmpModel.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace revithion::neural {

//==============================================================================
// NeuralAmpModel base — default block processing
//==============================================================================

void NeuralAmpModel::processBlock(const float* input, float* output, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
        output[i] = processSample(input[i]);
}

std::unique_ptr<NeuralAmpModel> NeuralAmpModel::create(const AmpModelInfo& info)
{
    switch (info.arch)
    {
        case ModelArch::LSTM:
        {
            auto model = std::make_unique<LSTMAmpModel>(info.hiddenSize);
            model->info_ = info;
            if (!info.weightsPath.empty())
                model->loadWeights(info.weightsPath);
            return model;
        }
        case ModelArch::GRU:
        case ModelArch::WaveNet:
            // Not yet implemented — fall back to LSTM
            return std::make_unique<LSTMAmpModel>(info.hiddenSize);
    }
    return nullptr;
}

//==============================================================================
// Fallback naive LSTM implementation (replaces RTNeural when unavailable)
//
// Architecture: Input(1) -> LSTM(hiddenSize) -> Dense(1)
//
// LSTM equations (per timestep):
//   f = sigmoid(Wf * [h, x] + bf)   — forget gate
//   i = sigmoid(Wi * [h, x] + bi)   — input gate
//   g = tanh(Wg * [h, x] + bg)      — cell candidate
//   o = sigmoid(Wo * [h, x] + bo)   — output gate
//   c = f * c + i * g               — cell state
//   h = o * tanh(c)                  — hidden state
//
// All weight matrices are stored contiguously:
//   W_gates: 4*H rows x (H+1) cols  (input_size=1, so input_dim=H+1)
//   b_gates: 4*H
//   W_dense: 1 x H
//   b_dense: 1
//==============================================================================

struct LSTMAmpModel::Impl
{
    int hiddenSize = 40;

    // LSTM weights — 4 gates: forget, input, cell_candidate, output
    // W_ih: (4*H, inputSize)  weight for input
    // W_hh: (4*H, H)          weight for hidden
    // b_ih: (4*H)             bias for input
    // b_hh: (4*H)             bias for hidden
    std::vector<float> W_ih;   // (4*H) — input size is 1
    std::vector<float> W_hh;   // (4*H * H)
    std::vector<float> b_ih;   // (4*H)
    std::vector<float> b_hh;   // (4*H)

    // Dense output layer: hidden -> 1
    std::vector<float> W_dense; // (H)
    float b_dense = 0.0f;

    // Runtime state (no allocations during processing)
    std::vector<float> h;  // hidden state (H)
    std::vector<float> c;  // cell state (H)

    // Pre-allocated scratch buffers for gate computation
    std::vector<float> gates; // (4*H)

    explicit Impl(int hidden) : hiddenSize(hidden)
    {
        allocate();
        initRandomWeights();
    }

    void allocate()
    {
        const int H = hiddenSize;
        W_ih.resize(4 * H, 0.0f);
        W_hh.resize(4 * H * H, 0.0f);
        b_ih.resize(4 * H, 0.0f);
        b_hh.resize(4 * H, 0.0f);
        W_dense.resize(H, 0.0f);
        b_dense = 0.0f;
        h.resize(H, 0.0f);
        c.resize(H, 0.0f);
        gates.resize(4 * H, 0.0f);
    }

    void initRandomWeights()
    {
        // Xavier-style initialization with small values for testing
        std::mt19937 rng(42);
        const int H = hiddenSize;
        const float scale_ih = 1.0f / std::sqrt(static_cast<float>(1));
        const float scale_hh = 1.0f / std::sqrt(static_cast<float>(H));
        const float scale_dense = 1.0f / std::sqrt(static_cast<float>(H));

        std::uniform_real_distribution<float> dist_ih(-scale_ih, scale_ih);
        std::uniform_real_distribution<float> dist_hh(-scale_hh, scale_hh);
        std::uniform_real_distribution<float> dist_dense(-scale_dense, scale_dense);

        for (auto& w : W_ih)  w = dist_ih(rng);
        for (auto& w : W_hh)  w = dist_hh(rng);
        for (auto& w : W_dense) w = dist_dense(rng);

        // Bias: set forget gate bias to 1.0 (common LSTM practice)
        for (int i = 0; i < H; ++i)
            b_ih[i] = 1.0f; // forget gate bias
    }

    void resetState()
    {
        std::fill(h.begin(), h.end(), 0.0f);
        std::fill(c.begin(), c.end(), 0.0f);
    }

    //==========================================================================
    // Activations (branchless, inline)
    //==========================================================================

    static float sigmoidFast(float x)
    {
        // Clamp to avoid overflow
        x = std::max(-16.0f, std::min(16.0f, x));
        return 1.0f / (1.0f + std::exp(-x));
    }

    static float tanhFast(float x)
    {
        x = std::max(-8.0f, std::min(8.0f, x));
        return std::tanh(x);
    }

    //==========================================================================
    // Forward pass — single sample, allocation-free
    //==========================================================================

    float forward(float input)
    {
        const int H = hiddenSize;

        // Compute gates: gates = W_ih * x + b_ih + W_hh * h + b_hh
        for (int i = 0; i < 4 * H; ++i)
        {
            float val = W_ih[i] * input + b_ih[i] + b_hh[i];

            // W_hh * h: dot product of row i with h
            const float* wRow = &W_hh[i * H];
            float dot = 0.0f;
            for (int j = 0; j < H; ++j)
                dot += wRow[j] * h[j];

            gates[i] = val + dot;
        }

        // Split gates and apply activations
        // Layout: [input_gate, forget_gate, cell_candidate, output_gate]
        // (PyTorch LSTM ordering: i, f, g, o)
        for (int j = 0; j < H; ++j)
        {
            const float ig = sigmoidFast(gates[j]);            // input gate
            const float fg = sigmoidFast(gates[H + j]);        // forget gate
            const float gc = tanhFast(gates[2 * H + j]);       // cell candidate
            const float og = sigmoidFast(gates[3 * H + j]);    // output gate

            c[j] = fg * c[j] + ig * gc;
            h[j] = og * tanhFast(c[j]);
        }

        // Dense output: y = W_dense . h + b_dense
        float output = b_dense;
        for (int j = 0; j < H; ++j)
            output += W_dense[j] * h[j];

        return output;
    }

    //==========================================================================
    // Weight loading — RTNeural JSON format
    //
    // RTNeural exports LSTM weights as:
    //   { "layers": [
    //       { "type": "lstm",
    //         "weights": {
    //           "W_ih": [[...], ...],  // (4*H, input_size)
    //           "W_hh": [[...], ...],  // (4*H, H)
    //           "b_ih": [...],          // (4*H)
    //           "b_hh": [...]           // (4*H)
    //         }
    //       },
    //       { "type": "dense",
    //         "weights": {
    //           "weight": [[...]],      // (1, H)
    //           "bias": [...]            // (1)
    //         }
    //       }
    //   ]}
    //==========================================================================

    bool loadFromJson(const std::string& path)
    {
        std::ifstream file(path);
        if (!file.is_open())
            return false;

        std::string content((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());

        // Minimal JSON parsing — extract float arrays by key
        auto extractArray = [&](const std::string& key) -> std::vector<float>
        {
            std::vector<float> values;
            auto pos = content.find("\"" + key + "\"");
            if (pos == std::string::npos) return values;

            // Find the first '[' after the key
            pos = content.find('[', pos);
            if (pos == std::string::npos) return values;

            // Find matching ']' — handle nested arrays by counting brackets
            int depth = 0;
            size_t start = pos;
            size_t end = pos;
            for (size_t i = pos; i < content.size(); ++i)
            {
                if (content[i] == '[') ++depth;
                else if (content[i] == ']')
                {
                    --depth;
                    if (depth == 0) { end = i; break; }
                }
            }

            // Extract all numbers from the region
            std::string region = content.substr(start, end - start + 1);
            std::string numBuf;
            for (char ch : region)
            {
                if (ch == '-' || ch == '.' || ch == 'e' || ch == 'E' || ch == '+'
                    || (ch >= '0' && ch <= '9'))
                {
                    numBuf += ch;
                }
                else if (!numBuf.empty())
                {
                    values.push_back(std::stof(numBuf));
                    numBuf.clear();
                }
            }
            if (!numBuf.empty())
                values.push_back(std::stof(numBuf));

            return values;
        };

        // Load LSTM weights
        auto w_ih = extractArray("W_ih");
        auto w_hh = extractArray("W_hh");
        auto bi    = extractArray("b_ih");
        auto bh    = extractArray("b_hh");

        const int H = hiddenSize;
        const int expectedIH = 4 * H;
        const int expectedHH = 4 * H * H;

        if (static_cast<int>(w_ih.size()) == expectedIH)
            W_ih = std::move(w_ih);
        if (static_cast<int>(w_hh.size()) == expectedHH)
            W_hh = std::move(w_hh);
        if (static_cast<int>(bi.size()) == expectedIH)
            b_ih = std::move(bi);
        if (static_cast<int>(bh.size()) == expectedIH)
            b_hh = std::move(bh);

        // Load dense layer weights
        auto w_dense = extractArray("weight");
        auto b_d     = extractArray("bias");

        if (static_cast<int>(w_dense.size()) >= H)
        {
            W_dense.assign(w_dense.begin(), w_dense.begin() + H);
        }
        if (!b_d.empty())
            b_dense = b_d[0];

        resetState();
        return true;
    }
};

//==============================================================================
// LSTMAmpModel public API
//==============================================================================

LSTMAmpModel::LSTMAmpModel(int hiddenSize)
    : pImpl(std::make_unique<Impl>(hiddenSize))
{
    info_.arch = ModelArch::LSTM;
    info_.hiddenSize = hiddenSize;
    info_.inputSize = 1;
    info_.outputSize = 1;
}

LSTMAmpModel::~LSTMAmpModel() = default;

bool LSTMAmpModel::loadWeights(const std::string& jsonPath)
{
    loaded_ = pImpl->loadFromJson(jsonPath);
    if (loaded_)
        info_.weightsPath = jsonPath;
    return loaded_;
}

float LSTMAmpModel::processSample(float input)
{
    return pImpl->forward(input);
}

void LSTMAmpModel::reset()
{
    pImpl->resetState();
}

} // namespace revithion::neural
