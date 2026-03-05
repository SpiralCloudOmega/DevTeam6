#pragma once

#include "CopilotBridge.h"

#include <memory>
#include <string>
#include <vector>

namespace revithion::ai {

// Voice command recognition result
struct VoiceCommand {
    std::string transcript;
    float confidence = 0.0f;
    double startTime = 0.0;
    double endTime = 0.0;
};

// Common voice commands and their mappings
struct VoiceCommandMapping {
    std::string pattern;       // Regex or keyword pattern
    CopilotAction::Type action;
    std::string description;
};

// VoiceCommandProcessor — processes transcribed speech into DAW actions
class VoiceCommandProcessor {
public:
    VoiceCommandProcessor();
    ~VoiceCommandProcessor();

    // Register custom command patterns
    void registerCommand(const VoiceCommandMapping& mapping);

    // Get default commands
    static std::vector<VoiceCommandMapping> defaultCommands();

    // Process a voice command transcript
    std::vector<CopilotAction> process(const VoiceCommand& cmd);

    // For complex commands, route to CopilotBridge
    bool requiresCopilot(const VoiceCommand& cmd) const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace revithion::ai
