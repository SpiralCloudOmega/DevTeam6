#include "VoiceCommand.h"

#include <algorithm>
#include <cctype>
#include <regex>

namespace revithion::ai {

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
static std::string toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return out;
}

static bool contains(const std::string& haystack, const std::string& needle) {
    return haystack.find(needle) != std::string::npos;
}

// Try to extract a track name from phrases like "mute track vocals"
static std::string extractTrackName(const std::string& text,
                                    const std::string& keyword) {
    auto pos = text.find(keyword);
    if (pos == std::string::npos)
        return {};

    auto after = text.substr(pos + keyword.size());
    // Trim leading spaces
    auto start = after.find_first_not_of(' ');
    if (start == std::string::npos)
        return {};
    return after.substr(start);
}

// Try to extract a number from the transcript
static float extractNumber(const std::string& text) {
    std::regex numRegex(R"((\d+\.?\d*))");
    std::smatch match;
    if (std::regex_search(text, match, numRegex))
        return std::stof(match[1].str());
    return 0.0f;
}

// ---------------------------------------------------------------------------
// Keywords that indicate the command is too complex for pattern matching
// ---------------------------------------------------------------------------
static const std::vector<std::string> kCopilotKeywords = {
    "suggest", "recommend", "how should", "what if",
    "improve", "fix", "analyze", "explain", "help",
    "master for", "mastering", "mix advice",
};

// ---------------------------------------------------------------------------
// Implementation
// ---------------------------------------------------------------------------
class VoiceCommandProcessor::Impl {
public:
    std::vector<VoiceCommandMapping> customMappings;
};

VoiceCommandProcessor::VoiceCommandProcessor()
    : pImpl(std::make_unique<Impl>())
{
}

VoiceCommandProcessor::~VoiceCommandProcessor() = default;

void VoiceCommandProcessor::registerCommand(
    const VoiceCommandMapping& mapping) {
    pImpl->customMappings.push_back(mapping);
}

std::vector<VoiceCommandMapping> VoiceCommandProcessor::defaultCommands() {
    return {
        {"play",                   CopilotAction::Type::SetParameter,   "Transport play"},
        {"stop",                   CopilotAction::Type::SetParameter,   "Transport stop"},
        {"record",                 CopilotAction::Type::SetParameter,   "Transport record"},
        {"mute track",             CopilotAction::Type::Mute,           "Mute a track"},
        {"unmute track",           CopilotAction::Type::Mute,           "Unmute a track"},
        {"solo track",             CopilotAction::Type::Solo,           "Solo a track"},
        {"unsolo track",           CopilotAction::Type::Solo,           "Unsolo a track"},
        {"set tempo to",           CopilotAction::Type::SetBPM,         "Set project tempo"},
        {"set bpm to",             CopilotAction::Type::SetBPM,         "Set project BPM"},
        {"generate",               CopilotAction::Type::Generate,       "ACE-STEP generate music"},
        {"master for streaming",   CopilotAction::Type::ApplyMastering, "Apply streaming mastering"},
        {"master for cd",          CopilotAction::Type::ApplyMastering, "Apply CD mastering"},
        {"louder",                 CopilotAction::Type::SetFader,       "Increase selected fader"},
        {"quieter",                CopilotAction::Type::SetFader,       "Decrease selected fader"},
        {"softer",                 CopilotAction::Type::SetFader,       "Decrease selected fader"},
        {"more bass",              CopilotAction::Type::SetEQ,          "Boost low frequencies"},
        {"less bass",              CopilotAction::Type::SetEQ,          "Cut low frequencies"},
        {"more treble",            CopilotAction::Type::SetEQ,          "Boost high frequencies"},
        {"less treble",            CopilotAction::Type::SetEQ,          "Cut high frequencies"},
        {"add reverb",             CopilotAction::Type::AddPlugin,      "Add reverb plugin"},
        {"add delay",              CopilotAction::Type::AddPlugin,      "Add delay plugin"},
        {"add compressor",         CopilotAction::Type::AddPlugin,      "Add compressor plugin"},
        {"remove plugin",          CopilotAction::Type::RemovePlugin,   "Remove a plugin"},
        {"load preset",            CopilotAction::Type::LoadPreset,     "Load a preset"},
        {"analyze",                CopilotAction::Type::Analyze,        "Analyze current mix"},
    };
}

std::vector<CopilotAction> VoiceCommandProcessor::process(
    const VoiceCommand& cmd) {
    std::vector<CopilotAction> actions;
    auto text = toLower(cmd.transcript);

    // Check custom mappings first (regex-based)
    for (const auto& mapping : pImpl->customMappings) {
        try {
            std::regex re(mapping.pattern, std::regex::icase);
            if (std::regex_search(text, re)) {
                CopilotAction action;
                action.type = mapping.action;
                action.stringValue = cmd.transcript;
                actions.push_back(std::move(action));
                return actions;
            }
        } catch (...) {
            // Invalid regex — skip
        }
    }

    // --- Transport commands ---
    if (text == "play" || text == "start playback") {
        CopilotAction a;
        a.type = CopilotAction::Type::SetParameter;
        a.parameterName = "transport";
        a.stringValue = "play";
        actions.push_back(std::move(a));
        return actions;
    }
    if (text == "stop" || text == "stop playback") {
        CopilotAction a;
        a.type = CopilotAction::Type::SetParameter;
        a.parameterName = "transport";
        a.stringValue = "stop";
        actions.push_back(std::move(a));
        return actions;
    }
    if (text == "record" || text == "start recording") {
        CopilotAction a;
        a.type = CopilotAction::Type::SetParameter;
        a.parameterName = "transport";
        a.stringValue = "record";
        actions.push_back(std::move(a));
        return actions;
    }

    // --- Mute / Solo ---
    if (contains(text, "mute track") || contains(text, "unmute track")) {
        CopilotAction a;
        a.type = CopilotAction::Type::Mute;
        a.targetId = extractTrackName(text, "track");
        a.value = contains(text, "unmute") ? 0.0f : 1.0f;
        actions.push_back(std::move(a));
        return actions;
    }
    if (contains(text, "solo track") || contains(text, "unsolo track")) {
        CopilotAction a;
        a.type = CopilotAction::Type::Solo;
        a.targetId = extractTrackName(text, "track");
        a.value = contains(text, "unsolo") ? 0.0f : 1.0f;
        actions.push_back(std::move(a));
        return actions;
    }

    // --- Tempo ---
    if (contains(text, "set tempo") || contains(text, "set bpm")) {
        CopilotAction a;
        a.type = CopilotAction::Type::SetBPM;
        a.value = extractNumber(text);
        actions.push_back(std::move(a));
        return actions;
    }

    // --- Fader (louder / quieter) ---
    if (contains(text, "louder")) {
        CopilotAction a;
        a.type = CopilotAction::Type::SetFader;
        a.value = 3.0f;  // +3 dB relative
        a.metadata["relative"] = "true";
        actions.push_back(std::move(a));
        return actions;
    }
    if (contains(text, "quieter") || contains(text, "softer")) {
        CopilotAction a;
        a.type = CopilotAction::Type::SetFader;
        a.value = -3.0f;  // -3 dB relative
        a.metadata["relative"] = "true";
        actions.push_back(std::move(a));
        return actions;
    }

    // --- EQ adjustments ---
    if (contains(text, "more bass")) {
        CopilotAction a;
        a.type = CopilotAction::Type::SetEQ;
        a.metadata["band"] = "low";
        a.metadata["frequency"] = "100";
        a.value = 3.0f;
        actions.push_back(std::move(a));
        return actions;
    }
    if (contains(text, "less bass")) {
        CopilotAction a;
        a.type = CopilotAction::Type::SetEQ;
        a.metadata["band"] = "low";
        a.metadata["frequency"] = "100";
        a.value = -3.0f;
        actions.push_back(std::move(a));
        return actions;
    }
    if (contains(text, "more treble")) {
        CopilotAction a;
        a.type = CopilotAction::Type::SetEQ;
        a.metadata["band"] = "high";
        a.metadata["frequency"] = "8000";
        a.value = 3.0f;
        actions.push_back(std::move(a));
        return actions;
    }
    if (contains(text, "less treble")) {
        CopilotAction a;
        a.type = CopilotAction::Type::SetEQ;
        a.metadata["band"] = "high";
        a.metadata["frequency"] = "8000";
        a.value = -3.0f;
        actions.push_back(std::move(a));
        return actions;
    }

    // --- Plugin management ---
    if (contains(text, "add reverb") || contains(text, "add delay") ||
        contains(text, "add compressor")) {
        CopilotAction a;
        a.type = CopilotAction::Type::AddPlugin;
        if (contains(text, "reverb"))
            a.stringValue = "reverb";
        else if (contains(text, "delay"))
            a.stringValue = "delay";
        else
            a.stringValue = "compressor";
        actions.push_back(std::move(a));
        return actions;
    }
    if (contains(text, "remove plugin")) {
        CopilotAction a;
        a.type = CopilotAction::Type::RemovePlugin;
        a.stringValue = extractTrackName(text, "plugin");
        actions.push_back(std::move(a));
        return actions;
    }

    // --- Generation ---
    if (contains(text, "generate")) {
        CopilotAction a;
        a.type = CopilotAction::Type::Generate;
        // Everything after "generate" is the description
        auto desc = extractTrackName(text, "generate");
        a.stringValue = desc.empty() ? cmd.transcript : desc;
        actions.push_back(std::move(a));
        return actions;
    }

    // --- Mastering ---
    if (contains(text, "master for")) {
        CopilotAction a;
        a.type = CopilotAction::Type::ApplyMastering;
        a.stringValue = extractTrackName(text, "master for");
        actions.push_back(std::move(a));
        return actions;
    }

    // --- Preset ---
    if (contains(text, "load preset")) {
        CopilotAction a;
        a.type = CopilotAction::Type::LoadPreset;
        a.stringValue = extractTrackName(text, "load preset");
        actions.push_back(std::move(a));
        return actions;
    }

    // --- Analyze ---
    if (contains(text, "analyze")) {
        CopilotAction a;
        a.type = CopilotAction::Type::Analyze;
        actions.push_back(std::move(a));
        return actions;
    }

    return actions;  // Empty — caller should route to CopilotBridge
}

bool VoiceCommandProcessor::requiresCopilot(const VoiceCommand& cmd) const {
    auto text = toLower(cmd.transcript);

    // If simple pattern matching would produce results, no copilot needed
    // (We do a lightweight check without fully running process())
    for (const auto& kw : kCopilotKeywords) {
        if (contains(text, kw))
            return true;
    }

    // If process() would return empty, the command is unrecognized — route
    // to copilot for natural-language interpretation
    VoiceCommand copy = cmd;
    auto actions = const_cast<VoiceCommandProcessor*>(this)->process(copy);
    return actions.empty();
}

} // namespace revithion::ai
