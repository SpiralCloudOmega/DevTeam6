#pragma once
#include "ACEStepBridge.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace revithion::ai {

// Instrument layer types in LEGO generation order
enum class LEGOLayer : uint8_t {
    Drums = 0,
    Bass,
    Guitar,
    Synth,
    Vocals,
    // Extended
    Strings,
    Brass,
    Woodwinds,
    Percussion,
    FX,
    Pad,
    Lead,
    Choir
};

const char* layerName(LEGOLayer layer);

// Configuration for a single layer generation
struct LayerConfig {
    LEGOLayer layer;
    std::string prompt;          // Layer-specific prompt/description
    std::string lyrics;          // Only for Vocals layer
    bool enabled = true;
    float volume = 1.0f;        // Mix level for this layer

    // Generation params (per-layer override, -1 = use pipeline defaults)
    int inferenceSteps = -1;
    float guidanceScale = -1.0f;
    int seed = -1;
};

// Result from a single layer generation
struct LayerResult {
    LEGOLayer layer;
    bool success = false;
    std::string error;
    std::vector<uint8_t> audioData;      // WAV data for this layer (cumulative mix)
    std::vector<uint8_t> isolatedAudio;  // After wave subtraction
    float detectedBpm = 0.0f;
    std::string detectedKey;
};

// Full pipeline result
struct PipelineResult {
    bool success = false;
    std::string error;
    std::vector<LayerResult> layers;
    std::vector<uint8_t> finalMix;  // Combined stereo WAV
    float bpm = 0.0f;
    std::string key;
    std::string timeSignature;
};

// Progress info
struct PipelineProgress {
    LEGOLayer currentLayer;
    int completedLayers = 0;
    int totalLayers = 0;
    float layerProgress = 0.0f;  // 0-1 within current layer
    std::string status;
};

using PipelineCallback = std::function<void(const PipelineResult&)>;
using PipelineProgressCallback = std::function<void(const PipelineProgress&)>;

// The LEGO Pipeline — generates complete songs layer by layer
class LEGOPipeline {
public:
    explicit LEGOPipeline(ACEStepBridge& bridge);
    ~LEGOPipeline();

    // Configure the pipeline
    void setCaption(const std::string& caption);  // Overall song description
    void setLyrics(const std::string& lyrics);    // For vocals layer
    void setBPM(float bpm);
    void setKey(const std::string& key);
    void setDuration(float seconds);
    void setTimeSignature(const std::string& ts);

    // Layer configuration
    void setLayerConfig(LEGOLayer layer, const LayerConfig& config);
    void enableLayer(LEGOLayer layer, bool enabled);
    void setLayerOrder(const std::vector<LEGOLayer>& order);

    // Default layer order: Drums, Bass, Guitar, Synth, Vocals
    static std::vector<LEGOLayer> defaultLayerOrder();

    // Generation defaults
    void setDefaultSteps(int steps);
    void setDefaultGuidance(float scale);

    // Execute pipeline (async)
    void generate(PipelineCallback onComplete,
                  PipelineProgressCallback onProgress = nullptr);

    // Execute pipeline (sync — blocks calling thread)
    PipelineResult generateSync();

    // Cancel running pipeline
    void cancel();
    bool isRunning() const;

    // Wave subtraction: isolate a layer by subtracting previous mix
    static std::vector<uint8_t> subtractWaves(
        const std::vector<uint8_t>& currentMix,
        const std::vector<uint8_t>& previousMix);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace revithion::ai
