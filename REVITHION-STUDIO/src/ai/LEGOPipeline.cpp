#include "LEGOPipeline.h"
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstring>
#include <mutex>
#include <thread>

namespace revithion::ai {

const char* layerName(LEGOLayer layer) {
    switch (layer) {
        case LEGOLayer::Drums:      return "Drums";
        case LEGOLayer::Bass:       return "Bass";
        case LEGOLayer::Guitar:     return "Guitar";
        case LEGOLayer::Synth:      return "Synth";
        case LEGOLayer::Vocals:     return "Vocals";
        case LEGOLayer::Strings:    return "Strings";
        case LEGOLayer::Brass:      return "Brass";
        case LEGOLayer::Woodwinds:  return "Woodwinds";
        case LEGOLayer::Percussion: return "Percussion";
        case LEGOLayer::FX:         return "FX";
        case LEGOLayer::Pad:        return "Pad";
        case LEGOLayer::Lead:       return "Lead";
        case LEGOLayer::Choir:      return "Choir";
        default:                    return "Unknown";
    }
}

// ─── WAV helpers ────────────────────────────────────────────────────────────────

// Standard WAV header (44 bytes, 16-bit PCM)
struct WavHeader {
    char riff[4];           // "RIFF"
    uint32_t fileSize;
    char wave[4];           // "WAVE"
    char fmt[4];            // "fmt "
    uint32_t fmtSize;
    uint16_t audioFormat;   // 1 = PCM
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char data[4];           // "data"
    uint32_t dataSize;
};

static const size_t kWavHeaderSize = 44;

static bool parseWavHeader(const std::vector<uint8_t>& wav, WavHeader& hdr) {
    if (wav.size() < kWavHeaderSize)
        return false;
    std::memcpy(&hdr, wav.data(), kWavHeaderSize);
    return (std::memcmp(hdr.riff, "RIFF", 4) == 0 &&
            std::memcmp(hdr.wave, "WAVE", 4) == 0);
}

static std::vector<uint8_t> buildWav(const std::vector<int16_t>& samples,
                                     uint16_t numChannels,
                                     uint32_t sampleRate) {
    uint32_t dataSize = static_cast<uint32_t>(samples.size() * sizeof(int16_t));
    std::vector<uint8_t> out(kWavHeaderSize + dataSize);

    WavHeader hdr{};
    std::memcpy(hdr.riff, "RIFF", 4);
    hdr.fileSize = static_cast<uint32_t>(out.size() - 8);
    std::memcpy(hdr.wave, "WAVE", 4);
    std::memcpy(hdr.fmt,  "fmt ", 4);
    hdr.fmtSize      = 16;
    hdr.audioFormat   = 1; // PCM
    hdr.numChannels   = numChannels;
    hdr.sampleRate    = sampleRate;
    hdr.bitsPerSample = 16;
    hdr.blockAlign    = numChannels * 2;
    hdr.byteRate      = sampleRate * hdr.blockAlign;
    std::memcpy(hdr.data, "data", 4);
    hdr.dataSize = dataSize;

    std::memcpy(out.data(), &hdr, kWavHeaderSize);
    std::memcpy(out.data() + kWavHeaderSize, samples.data(), dataSize);
    return out;
}

// ─── LEGOPipeline::Impl ────────────────────────────────────────────────────────

class LEGOPipeline::Impl {
public:
    ACEStepBridge& bridge;

    // Song-level settings
    std::string caption;
    std::string lyrics;
    float bpm = 0.0f;
    std::string key;
    float duration = 30.0f;
    std::string timeSignature;

    // Generation defaults
    int defaultSteps = 8;
    float defaultGuidance = 7.0f;

    // Layer configuration
    std::vector<LEGOLayer> layerOrder;
    std::map<LEGOLayer, LayerConfig> layerConfigs;

    // Runtime state
    std::atomic<bool> running{false};
    std::atomic<bool> cancelled{false};
    std::unique_ptr<std::thread> workerThread;
    std::mutex mutex;

    explicit Impl(ACEStepBridge& b) : bridge(b) {
        layerOrder = LEGOPipeline::defaultLayerOrder();
    }

    ~Impl() {
        cancelled = true;
        if (workerThread && workerThread->joinable())
            workerThread->join();
    }

    // Build prompt for a specific layer, incorporating the overall caption
    std::string buildLayerPrompt(LEGOLayer layer) const {
        auto it = layerConfigs.find(layer);
        if (it != layerConfigs.end() && !it->second.prompt.empty())
            return it->second.prompt;

        // Auto-generate a prompt from the caption + layer name
        if (!caption.empty())
            return caption + " — " + layerName(layer) + " track";
        return std::string(layerName(layer)) + " track";
    }

    // Build GenerationRequest for a layer
    GenerationRequest buildRequest(LEGOLayer layer,
                                   const std::string& refAudioPath) const {
        GenerationRequest req;
        req.caption = buildLayerPrompt(layer);
        req.bpm = bpm;
        req.key = key;
        req.timeSignature = timeSignature;
        req.duration = duration;

        // Lyrics only for vocal layers
        if (layer == LEGOLayer::Vocals || layer == LEGOLayer::Choir) {
            auto it = layerConfigs.find(layer);
            if (it != layerConfigs.end() && !it->second.lyrics.empty())
                req.lyrics = it->second.lyrics;
            else
                req.lyrics = lyrics;
        }

        // Per-layer overrides
        auto it = layerConfigs.find(layer);
        int steps = defaultSteps;
        float guidance = defaultGuidance;
        int seed = -1;
        if (it != layerConfigs.end()) {
            if (it->second.inferenceSteps > 0)  steps = it->second.inferenceSteps;
            if (it->second.guidanceScale > 0.0f) guidance = it->second.guidanceScale;
            if (it->second.seed >= 0)            seed = it->second.seed;
        }
        req.inferenceSteps = steps;
        req.guidanceScale = guidance;
        req.seed = seed;

        // Context-aware: reference the previous cumulative mix
        if (!refAudioPath.empty())
            req.referenceAudioPath = refAudioPath;

        return req;
    }

    // Write WAV data to a temporary file; returns the path
    std::string writeTempWav(const std::vector<uint8_t>& data,
                             const std::string& tag) const {
        auto tempDir = juce::File::getSpecialLocation(
                           juce::File::tempDirectory);
        auto file = tempDir.getChildFile(
            "revithion_lego_" + juce::String(tag) + ".wav");
        file.replaceWithData(data.data(), data.size());
        return file.getFullPathName().toStdString();
    }

    // Run the full pipeline (blocking)
    PipelineResult run(PipelineProgressCallback onProgress) {
        PipelineResult result;

        // Filter to enabled layers only
        std::vector<LEGOLayer> activeLayers;
        for (auto layer : layerOrder) {
            auto it = layerConfigs.find(layer);
            bool enabled = (it == layerConfigs.end()) || it->second.enabled;
            if (enabled)
                activeLayers.push_back(layer);
        }

        if (activeLayers.empty()) {
            result.error = "No layers enabled";
            return result;
        }

        int totalLayers = static_cast<int>(activeLayers.size());
        std::vector<uint8_t> previousMix;  // Cumulative mix from prior layers
        std::string refAudioPath;

        for (int i = 0; i < totalLayers; ++i) {
            if (cancelled.load()) {
                result.error = "Pipeline cancelled";
                return result;
            }

            LEGOLayer layer = activeLayers[static_cast<size_t>(i)];

            // Report progress
            if (onProgress) {
                PipelineProgress prog;
                prog.currentLayer = layer;
                prog.completedLayers = i;
                prog.totalLayers = totalLayers;
                prog.layerProgress = 0.0f;
                prog.status = std::string("Generating ") + layerName(layer) + "...";
                onProgress(prog);
            }

            // Build request with reference to previous mix
            auto req = buildRequest(layer, refAudioPath);

            // Generate this layer
            auto genResult = bridge.generateMusicSync(req);

            LayerResult lr;
            lr.layer = layer;

            if (!genResult.success || genResult.audioData.empty()) {
                lr.success = false;
                lr.error = genResult.error.empty()
                               ? "Generation failed for " + std::string(layerName(layer))
                               : genResult.error;
                result.layers.push_back(lr);
                // Continue to next layer — don't break the whole pipeline
                continue;
            }

            lr.success = true;
            lr.audioData = genResult.audioData;
            lr.detectedBpm = genResult.detectedBpm;
            lr.detectedKey = genResult.detectedKey;

            // Capture BPM/key from first successful layer
            if (result.bpm == 0.0f && genResult.detectedBpm > 0.0f)
                result.bpm = genResult.detectedBpm;
            if (result.key.empty() && !genResult.detectedKey.empty())
                result.key = genResult.detectedKey;

            // Wave subtraction: isolate this layer from the cumulative mix
            if (!previousMix.empty()) {
                lr.isolatedAudio = subtractWaves(lr.audioData, previousMix);
            } else {
                lr.isolatedAudio = lr.audioData;  // First layer IS the isolated audio
            }

            // The current audio becomes the new cumulative mix
            previousMix = lr.audioData;

            // Write cumulative mix to temp file for next layer's context
            refAudioPath = writeTempWav(previousMix,
                                        layerName(layer) + std::string("_mix"));

            result.layers.push_back(lr);

            // Progress: layer complete
            if (onProgress) {
                PipelineProgress prog;
                prog.currentLayer = layer;
                prog.completedLayers = i + 1;
                prog.totalLayers = totalLayers;
                prog.layerProgress = 1.0f;
                prog.status = std::string(layerName(layer)) + " complete";
                onProgress(prog);
            }
        }

        // Final mix is the last cumulative output
        result.finalMix = previousMix;
        result.timeSignature = timeSignature;
        result.success = !result.layers.empty() &&
                         std::any_of(result.layers.begin(), result.layers.end(),
                                     [](const LayerResult& lr) { return lr.success; });

        // Clean up temp files
        auto tempDir = juce::File::getSpecialLocation(juce::File::tempDirectory);
        for (auto layer : activeLayers) {
            auto f = tempDir.getChildFile(
                "revithion_lego_" + juce::String(layerName(layer)) + "_mix.wav");
            f.deleteFile();
        }

        return result;
    }
};

// ─── LEGOPipeline public interface ──────────────────────────────────────────────

LEGOPipeline::LEGOPipeline(ACEStepBridge& bridge)
    : pImpl(std::make_unique<Impl>(bridge))
{
}

LEGOPipeline::~LEGOPipeline() = default;

void LEGOPipeline::setCaption(const std::string& caption) { pImpl->caption = caption; }
void LEGOPipeline::setLyrics(const std::string& lyrics)   { pImpl->lyrics = lyrics; }
void LEGOPipeline::setBPM(float bpm)                      { pImpl->bpm = bpm; }
void LEGOPipeline::setKey(const std::string& key)         { pImpl->key = key; }
void LEGOPipeline::setDuration(float seconds)             { pImpl->duration = seconds; }
void LEGOPipeline::setTimeSignature(const std::string& ts){ pImpl->timeSignature = ts; }

void LEGOPipeline::setLayerConfig(LEGOLayer layer, const LayerConfig& config) {
    pImpl->layerConfigs[layer] = config;
}

void LEGOPipeline::enableLayer(LEGOLayer layer, bool enabled) {
    pImpl->layerConfigs[layer].layer = layer;
    pImpl->layerConfigs[layer].enabled = enabled;
}

void LEGOPipeline::setLayerOrder(const std::vector<LEGOLayer>& order) {
    pImpl->layerOrder = order;
}

std::vector<LEGOLayer> LEGOPipeline::defaultLayerOrder() {
    return {
        LEGOLayer::Drums,
        LEGOLayer::Bass,
        LEGOLayer::Guitar,
        LEGOLayer::Synth,
        LEGOLayer::Vocals
    };
}

void LEGOPipeline::setDefaultSteps(int steps)       { pImpl->defaultSteps = steps; }
void LEGOPipeline::setDefaultGuidance(float scale)   { pImpl->defaultGuidance = scale; }

void LEGOPipeline::generate(PipelineCallback onComplete,
                             PipelineProgressCallback onProgress) {
    if (pImpl->running.load())
        return;

    pImpl->cancelled = false;
    pImpl->running = true;

    // Join any previous thread
    if (pImpl->workerThread && pImpl->workerThread->joinable())
        pImpl->workerThread->join();

    pImpl->workerThread = std::make_unique<std::thread>(
        [this, onComplete, onProgress]() {
            auto result = pImpl->run(onProgress);
            pImpl->running = false;

            // Deliver result on the JUCE message thread
            if (onComplete) {
                juce::MessageManager::callAsync([onComplete, result]() {
                    onComplete(result);
                });
            }
        });
}

PipelineResult LEGOPipeline::generateSync() {
    pImpl->cancelled = false;
    pImpl->running = true;
    auto result = pImpl->run(nullptr);
    pImpl->running = false;
    return result;
}

void LEGOPipeline::cancel() {
    pImpl->cancelled = true;
}

bool LEGOPipeline::isRunning() const {
    return pImpl->running.load();
}

std::vector<uint8_t> LEGOPipeline::subtractWaves(
    const std::vector<uint8_t>& currentMix,
    const std::vector<uint8_t>& previousMix)
{
    WavHeader curHdr{}, prevHdr{};
    if (!parseWavHeader(currentMix, curHdr) || !parseWavHeader(previousMix, prevHdr))
        return currentMix;  // Can't parse — return unchanged

    // Sample counts (16-bit PCM)
    size_t curSamples  = (currentMix.size()  - kWavHeaderSize) / sizeof(int16_t);
    size_t prevSamples = (previousMix.size() - kWavHeaderSize) / sizeof(int16_t);
    size_t numSamples  = std::min(curSamples, prevSamples);

    const auto* curData  = reinterpret_cast<const int16_t*>(
                               currentMix.data() + kWavHeaderSize);
    const auto* prevData = reinterpret_cast<const int16_t*>(
                               previousMix.data() + kWavHeaderSize);

    std::vector<int16_t> isolated(curSamples, 0);

    // Subtract sample-by-sample, clipping to int16 range
    for (size_t i = 0; i < numSamples; ++i) {
        int32_t diff = static_cast<int32_t>(curData[i]) -
                       static_cast<int32_t>(prevData[i]);
        diff = std::clamp(diff, static_cast<int32_t>(-32768),
                                static_cast<int32_t>(32767));
        isolated[i] = static_cast<int16_t>(diff);
    }

    // Samples beyond previous mix length: keep as-is from current
    for (size_t i = numSamples; i < curSamples; ++i)
        isolated[i] = curData[i];

    return buildWav(isolated, curHdr.numChannels, curHdr.sampleRate);
}

} // namespace revithion::ai
