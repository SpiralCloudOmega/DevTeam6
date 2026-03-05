#pragma once
#include "ACEStepBridge.h"
#include <atomic>
#include <functional>
#include <string>
#include <vector>

namespace revithion::ai {

struct CoverRequest {
    std::string sourceAudioPath;   // Original song to cover
    std::string caption;           // Style description for the cover
    std::string lyrics;            // New lyrics (optional)
    float strength = 0.8f;         // How much to deviate (0=exact, 1=completely different)
    int inferenceSteps = 8;
    float guidanceScale = 7.0f;
};

struct CoverResult {
    bool success = false;
    std::string error;
    std::vector<uint8_t> audioData;
};

class CoverGenerator {
public:
    explicit CoverGenerator(ACEStepBridge& bridge);
    ~CoverGenerator();

    CoverResult generateSync(const CoverRequest& request);
    void generateAsync(const CoverRequest& request,
                       std::function<void(const CoverResult&)> callback);
    void cancel();

private:
    ACEStepBridge& bridge_;
    std::atomic<bool> cancelled_{false};
};

} // namespace revithion::ai
