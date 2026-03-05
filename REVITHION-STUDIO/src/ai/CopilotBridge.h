#pragma once

#include <JuceHeader.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace revithion::ai {

// A message in the copilot chat
struct CopilotMessage {
    enum class Role : uint8_t { System, User, Assistant, Tool };
    Role role;
    std::string content;
    double timestamp = 0.0;
};

// An action the copilot can take on the DAW
struct CopilotAction {
    enum class Type : uint8_t {
        SetParameter,       // Set a DSP parameter (track, param, value)
        AddPlugin,          // Add a plugin to a track
        RemovePlugin,       // Remove a plugin
        SetFader,           // Set track fader level
        SetPan,             // Set track pan
        Mute,               // Mute/unmute track
        Solo,               // Solo/unsolo track
        SetBPM,             // Set project tempo
        Generate,           // Trigger ACE-STEP generation
        ApplyMastering,     // Apply mastering chain
        SetEQ,              // Set EQ band parameters
        SetCompressor,      // Set compressor parameters
        RoutingConnect,     // Connect routing grid blocks
        LoadPreset,         // Load a preset
        Analyze,            // Analyze audio and report
    };

    Type type;
    std::string targetId;          // Track/channel UUID or block ID
    std::string parameterName;
    float value = 0.0f;
    std::string stringValue;
    std::unordered_map<std::string, std::string> metadata;
};

// Result from executing an action
struct ActionResult {
    bool success = false;
    std::string message;
    std::vector<CopilotAction> followUpActions;  // Chain of actions
};

// Copilot configuration
struct CopilotConfig {
    std::string modelName = "qwen3-coder:30b";  // Default local model
    std::string ollamaUrl = "http://localhost:11434";
    std::string systemPrompt;  // DAW context system prompt
    float temperature = 0.7f;
    int maxTokens = 2048;
    bool streamResponses = true;
};

// Callback types
using MessageCallback = std::function<void(const CopilotMessage&)>;
using ActionCallback = std::function<ActionResult(const CopilotAction&)>;
using StreamCallback = std::function<void(const std::string& chunk)>;

// CopilotBridge — conversational AI assistant for the DAW
class CopilotBridge {
public:
    CopilotBridge(const CopilotConfig& config = CopilotConfig{});
    ~CopilotBridge();

    // Configuration
    void setConfig(const CopilotConfig& config);
    const CopilotConfig& config() const;

    // Connection
    bool checkConnection();
    bool isConnected() const;

    // Chat
    void sendMessage(const std::string& userMessage,
                     MessageCallback onResponse,
                     StreamCallback onStream = nullptr);

    // Get chat history
    const std::vector<CopilotMessage>& history() const;
    void clearHistory();

    // Action execution
    void setActionHandler(ActionCallback handler);

    // DAW context (sent with each message for situational awareness)
    void updateContext(const std::string& key, const std::string& value);

    // Convenience commands
    void masterForTarget(const std::string& target,
                         MessageCallback onResponse);  // "Master this for Spotify"
    void suggestMix(MessageCallback onResponse);        // "How should I mix this?"
    void analyzeAndReport(MessageCallback onResponse);  // "Analyze the current mix"

    // Cancel current request
    void cancel();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace revithion::ai
