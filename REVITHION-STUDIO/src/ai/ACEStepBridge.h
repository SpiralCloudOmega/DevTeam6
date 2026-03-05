#pragma once

#include <JuceHeader.h>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace revithion::ai {

// Generation request matching ACE-STEP API parameters
struct GenerationRequest {
    std::string caption;           // Music description/prompt
    std::string lyrics;            // Optional lyrics
    float bpm = 0.0f;             // 0 = auto-detect
    std::string key;               // e.g. "C major", empty = auto
    std::string timeSignature;     // e.g. "4/4", empty = auto
    std::string language = "en";
    int inferenceSteps = 8;        // 6-15, higher = better quality
    float guidanceScale = 7.0f;
    int seed = -1;                 // -1 = random
    int batchSize = 1;
    float duration = 30.0f;        // seconds
    std::string audioFormat = "wav";
    
    // Cover/repainting
    std::string referenceAudioPath;
    std::string sourceAudioPath;
    float repaintingStart = 0.0f;
    float repaintingEnd = -1.0f;
    std::string instruction;
    float coverStrength = 1.0f;
    
    // LM features
    bool thinking = false;         // CoT reasoning
    bool enhance = true;           // Enrich tags
    float lmTemperature = 0.85f;
    float lmCfgScale = 2.0f;
};

// Generation result
struct GenerationResult {
    bool success = false;
    std::string error;
    std::vector<uint8_t> audioData;
    std::string audioFormat;
    float detectedBpm = 0.0f;
    std::string detectedKey;
    std::string generatedLyrics;
    int seed = 0;
};

// Callback types
using GenerationCallback = std::function<void(const GenerationResult&)>;
using ProgressCallback = std::function<void(float progress, const std::string& status)>;

// ACE-STEP API bridge — communicates with localhost:8001
class ACEStepBridge {
public:
    ACEStepBridge(const std::string& apiUrl = "http://localhost:8001");
    ~ACEStepBridge();
    
    // Connection
    bool checkHealth();
    bool isConnected() const;
    std::string getApiUrl() const;
    void setApiUrl(const std::string& url);
    
    // Generation (async — runs on background thread)
    void generateMusic(const GenerationRequest& request,
                       GenerationCallback onComplete,
                       ProgressCallback onProgress = nullptr);
    
    // Synchronous generation (blocks calling thread)
    GenerationResult generateMusicSync(const GenerationRequest& request);
    
    // Model management
    struct ModelInfo {
        std::string name;
        std::string type;  // "dit", "lm", "vae"
        std::string path;
    };
    std::vector<ModelInfo> getAvailableModels();
    
    // LoRA management
    bool loadLoRA(const std::string& path);
    bool unloadLoRA();
    bool setLoRAScale(float scale);
    
    // Cancel current generation
    void cancelGeneration();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace revithion::ai
