#include "CoverGenerator.h"
#include <thread>

namespace revithion::ai {

CoverGenerator::CoverGenerator(ACEStepBridge& bridge)
    : bridge_(bridge)
{
}

CoverGenerator::~CoverGenerator() = default;

CoverResult CoverGenerator::generateSync(const CoverRequest& request) {
    CoverResult result;
    cancelled_ = false;

    if (request.sourceAudioPath.empty()) {
        result.error = "Source audio path is required for cover generation";
        return result;
    }

    // Map CoverRequest → GenerationRequest
    GenerationRequest genReq;
    genReq.caption = request.caption;
    genReq.lyrics = request.lyrics;
    genReq.sourceAudioPath = request.sourceAudioPath;
    genReq.referenceAudioPath = request.sourceAudioPath;
    genReq.coverStrength = request.strength;
    genReq.inferenceSteps = request.inferenceSteps;
    genReq.guidanceScale = request.guidanceScale;
    genReq.instruction = "cover";

    if (cancelled_.load()) {
        result.error = "Cover generation cancelled";
        return result;
    }

    auto genResult = bridge_.generateMusicSync(genReq);

    result.success = genResult.success;
    result.error = genResult.error;
    result.audioData = std::move(genResult.audioData);
    return result;
}

void CoverGenerator::generateAsync(const CoverRequest& request,
                                    std::function<void(const CoverResult&)> callback) {
    cancelled_ = false;

    std::thread([this, request, callback]() {
        auto result = generateSync(request);

        if (callback) {
            juce::MessageManager::callAsync([callback, result]() {
                callback(result);
            });
        }
    }).detach();
}

void CoverGenerator::cancel() {
    cancelled_ = true;
    bridge_.cancelGeneration();
}

} // namespace revithion::ai
