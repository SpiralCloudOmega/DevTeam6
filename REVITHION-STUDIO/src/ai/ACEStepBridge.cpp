#include "ACEStepBridge.h"

namespace revithion::ai {

class ACEStepBridge::Impl {
public:
    std::string apiUrl;
    bool connected = false;
    std::atomic<bool> cancelFlag{false};
    juce::ThreadPool threadPool{1};
    
    Impl(const std::string& url) : apiUrl(url) {}
    
    // Helper: make HTTP request and return response as string
    std::string httpGet(const std::string& endpoint) {
        juce::URL url(apiUrl + endpoint);
        auto options = juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inAddress)
                           .withConnectionTimeoutMs(5000);
        
        auto stream = url.createInputStream(options);
        if (!stream)
            return "";
        
        return stream->readEntireStreamAsString().toStdString();
    }
    
    std::string httpPost(const std::string& endpoint, const juce::String& jsonBody) {
        juce::URL url(apiUrl + endpoint);
        
        url = url.withPOSTData(jsonBody);
        
        auto options = juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inPostBody)
                           .withExtraHeaders("Content-Type: application/json")
                           .withConnectionTimeoutMs(120000); // 2 min for generation
        
        auto stream = url.createInputStream(options);
        if (!stream)
            return "";
        
        return stream->readEntireStreamAsString().toStdString();
    }
    
    // Build JSON payload from GenerationRequest
    juce::String buildRequestJson(const GenerationRequest& req) {
        auto obj = std::make_unique<juce::DynamicObject>();
        obj->setProperty("caption", juce::String(req.caption));
        obj->setProperty("lyrics", juce::String(req.lyrics));
        if (req.bpm > 0.0f)
            obj->setProperty("bpm", req.bpm);
        if (!req.key.empty())
            obj->setProperty("key", juce::String(req.key));
        if (!req.timeSignature.empty())
            obj->setProperty("time_signature", juce::String(req.timeSignature));
        obj->setProperty("language", juce::String(req.language));
        obj->setProperty("inference_steps", req.inferenceSteps);
        obj->setProperty("guidance_scale", req.guidanceScale);
        if (req.seed >= 0)
            obj->setProperty("seed", req.seed);
        obj->setProperty("batch_size", req.batchSize);
        obj->setProperty("duration", req.duration);
        obj->setProperty("audio_format", juce::String(req.audioFormat));
        obj->setProperty("thinking", req.thinking);
        obj->setProperty("enhance", req.enhance);
        obj->setProperty("lm_temperature", req.lmTemperature);
        obj->setProperty("lm_cfg_scale", req.lmCfgScale);
        
        if (!req.referenceAudioPath.empty())
            obj->setProperty("reference_audio", juce::String(req.referenceAudioPath));
        if (!req.sourceAudioPath.empty())
            obj->setProperty("source_audio", juce::String(req.sourceAudioPath));
        if (req.repaintingStart > 0.0f || req.repaintingEnd >= 0.0f) {
            obj->setProperty("repainting_start", req.repaintingStart);
            obj->setProperty("repainting_end", req.repaintingEnd);
        }
        if (!req.instruction.empty())
            obj->setProperty("instruction", juce::String(req.instruction));
        
        return juce::JSON::toString(juce::var(obj.release()));
    }
};

ACEStepBridge::ACEStepBridge(const std::string& apiUrl)
    : pImpl(std::make_unique<Impl>(apiUrl))
{
}

ACEStepBridge::~ACEStepBridge() = default;

bool ACEStepBridge::checkHealth() {
    auto response = pImpl->httpGet("/health");
    pImpl->connected = !response.empty();
    return pImpl->connected;
}

bool ACEStepBridge::isConnected() const {
    return pImpl->connected;
}

std::string ACEStepBridge::getApiUrl() const {
    return pImpl->apiUrl;
}

void ACEStepBridge::setApiUrl(const std::string& url) {
    pImpl->apiUrl = url;
    pImpl->connected = false;
}

void ACEStepBridge::generateMusic(const GenerationRequest& request,
                                   GenerationCallback onComplete,
                                   ProgressCallback onProgress) {
    pImpl->cancelFlag = false;
    
    pImpl->threadPool.addJob([this, request, onComplete, onProgress]() {
        auto result = generateMusicSync(request);
        
        juce::MessageManager::callAsync([onComplete, result]() {
            onComplete(result);
        });
    });
}

GenerationResult ACEStepBridge::generateMusicSync(const GenerationRequest& request) {
    GenerationResult result;
    
    auto jsonBody = pImpl->buildRequestJson(request);
    auto response = pImpl->httpPost("/v1/audio", jsonBody);
    
    if (response.empty()) {
        result.error = "No response from ACE-STEP API at " + pImpl->apiUrl;
        return result;
    }
    
    // Parse response JSON
    auto parsed = juce::JSON::parse(juce::String(response));
    if (auto* obj = parsed.getDynamicObject()) {
        if (obj->hasProperty("error")) {
            result.error = obj->getProperty("error").toString().toStdString();
            return result;
        }
        
        result.success = true;
        if (obj->hasProperty("bpm"))
            result.detectedBpm = static_cast<float>(obj->getProperty("bpm"));
        if (obj->hasProperty("key"))
            result.detectedKey = obj->getProperty("key").toString().toStdString();
        if (obj->hasProperty("lyrics"))
            result.generatedLyrics = obj->getProperty("lyrics").toString().toStdString();
        if (obj->hasProperty("seed"))
            result.seed = static_cast<int>(obj->getProperty("seed"));
        
        // Audio data handling depends on API response format
        // Could be base64-encoded audio or a file path
        result.audioFormat = request.audioFormat;
    }
    
    return result;
}

std::vector<ACEStepBridge::ModelInfo> ACEStepBridge::getAvailableModels() {
    std::vector<ModelInfo> models;
    auto response = pImpl->httpGet("/v1/models");
    
    if (response.empty())
        return models;
    
    auto parsed = juce::JSON::parse(juce::String(response));
    if (auto* arr = parsed.getArray()) {
        for (const auto& item : *arr) {
            if (auto* obj = item.getDynamicObject()) {
                ModelInfo info;
                info.name = obj->getProperty("name").toString().toStdString();
                info.type = obj->getProperty("type").toString().toStdString();
                info.path = obj->getProperty("path").toString().toStdString();
                models.push_back(info);
            }
        }
    }
    
    return models;
}

bool ACEStepBridge::loadLoRA(const std::string& path) {
    auto obj = std::make_unique<juce::DynamicObject>();
    obj->setProperty("lora_path", juce::String(path));
    auto json = juce::JSON::toString(juce::var(obj.release()));
    
    auto response = pImpl->httpPost("/v1/lora/load", json);
    return !response.empty();
}

bool ACEStepBridge::unloadLoRA() {
    auto response = pImpl->httpPost("/v1/lora/unload", "{}");
    return !response.empty();
}

bool ACEStepBridge::setLoRAScale(float scale) {
    auto obj = std::make_unique<juce::DynamicObject>();
    obj->setProperty("scale", scale);
    auto json = juce::JSON::toString(juce::var(obj.release()));
    
    auto response = pImpl->httpPost("/v1/lora/scale", json);
    return !response.empty();
}

void ACEStepBridge::cancelGeneration() {
    pImpl->cancelFlag = true;
}

} // namespace revithion::ai
