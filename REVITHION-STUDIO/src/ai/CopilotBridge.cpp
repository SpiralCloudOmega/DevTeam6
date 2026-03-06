#include "CopilotBridge.h"

#include <algorithm>
#include <atomic>
#include <sstream>

namespace revithion::ai {

// ---------------------------------------------------------------------------
// Default system prompt describing REVITHION STUDIO capabilities
// ---------------------------------------------------------------------------
static const char* kDefaultSystemPrompt = R"(You are the AI Copilot inside REVITHION STUDIO, a professional digital audio workstation.

You can control the DAW by emitting action blocks in your response. Wrap each
action in a fenced JSON block tagged "action":

```action
{
  "type": "SetFader",
  "targetId": "<track-uuid>",
  "parameterName": "fader",
  "value": -3.0,
  "stringValue": "",
  "metadata": {}
}
```

Supported action types:
  SetParameter, AddPlugin, RemovePlugin, SetFader, SetPan, Mute, Solo,
  SetBPM, Generate, ApplyMastering, SetEQ, SetCompressor, RoutingConnect,
  LoadPreset, Analyze.

Guidelines:
- Be concise and helpful.  Explain *why* you chose a setting.
- When adjusting EQ, provide band frequency, gain, and Q in metadata.
- For mastering, target loudness standards (e.g., -14 LUFS for streaming).
- For generation requests, include a descriptive caption in stringValue.
- You may emit multiple action blocks in one response for compound operations.
- If the user's request is ambiguous, ask a clarifying question instead of guessing.
)";

// ---------------------------------------------------------------------------
// Map from action type name to enum
// ---------------------------------------------------------------------------
static CopilotAction::Type actionTypeFromString(const juce::String& s) {
    static const std::unordered_map<std::string, CopilotAction::Type> kMap = {
        {"SetParameter",   CopilotAction::Type::SetParameter},
        {"AddPlugin",      CopilotAction::Type::AddPlugin},
        {"RemovePlugin",   CopilotAction::Type::RemovePlugin},
        {"SetFader",       CopilotAction::Type::SetFader},
        {"SetPan",         CopilotAction::Type::SetPan},
        {"Mute",           CopilotAction::Type::Mute},
        {"Solo",           CopilotAction::Type::Solo},
        {"SetBPM",         CopilotAction::Type::SetBPM},
        {"Generate",       CopilotAction::Type::Generate},
        {"ApplyMastering", CopilotAction::Type::ApplyMastering},
        {"SetEQ",          CopilotAction::Type::SetEQ},
        {"SetCompressor",  CopilotAction::Type::SetCompressor},
        {"RoutingConnect", CopilotAction::Type::RoutingConnect},
        {"LoadPreset",     CopilotAction::Type::LoadPreset},
        {"Analyze",        CopilotAction::Type::Analyze},
    };
    auto it = kMap.find(s.toStdString());
    return it != kMap.end() ? it->second : CopilotAction::Type::SetParameter;
}

// ---------------------------------------------------------------------------
// Implementation
// ---------------------------------------------------------------------------
class CopilotBridge::Impl {
public:
    CopilotConfig cfg;
    std::vector<CopilotMessage> chatHistory;
    ActionCallback actionHandler;
    std::unordered_map<std::string, std::string> dawContext;
    bool connected = false;
    std::atomic<bool> cancelFlag{false};
    juce::ThreadPool threadPool{1};

    static constexpr int kMaxHistoryTokenEstimate = 6000;  // rough char limit

    Impl(const CopilotConfig& c) : cfg(c) {
        if (cfg.systemPrompt.empty())
            cfg.systemPrompt = kDefaultSystemPrompt;
    }

    // ------------------------------------------------------------------
    // HTTP helpers (mirrors ACEStepBridge pattern)
    // ------------------------------------------------------------------
    std::string httpGet(const std::string& endpoint) {
        juce::URL url(cfg.ollamaUrl + endpoint);
        auto options = juce::URL::InputStreamOptions(
                           juce::URL::ParameterHandling::inAddress)
                           .withConnectionTimeoutMs(5000);
        auto stream = url.createInputStream(options);
        if (!stream)
            return {};
        return stream->readEntireStreamAsString().toStdString();
    }

    std::string httpPost(const std::string& endpoint,
                         const juce::String& jsonBody) {
        juce::URL url(cfg.ollamaUrl + endpoint);
        url = url.withPOSTData(jsonBody);
        auto options = juce::URL::InputStreamOptions(
                           juce::URL::ParameterHandling::inPostData)
                           .withExtraHeaders("Content-Type: application/json")
                           .withConnectionTimeoutMs(120000);
        auto stream = url.createInputStream(options);
        if (!stream)
            return {};
        return stream->readEntireStreamAsString().toStdString();
    }

    // Streaming POST — reads line-delimited JSON and invokes callback per chunk
    std::string httpPostStreaming(const std::string& endpoint,
                                 const juce::String& jsonBody,
                                 StreamCallback onChunk) {
        juce::URL url(cfg.ollamaUrl + endpoint);
        url = url.withPOSTData(jsonBody);
        auto options = juce::URL::InputStreamOptions(
                           juce::URL::ParameterHandling::inPostData)
                           .withExtraHeaders("Content-Type: application/json")
                           .withConnectionTimeoutMs(120000);
        auto stream = url.createInputStream(options);
        if (!stream)
            return {};

        std::string fullResponse;
        juce::MemoryBlock lineBuffer;

        // Ollama streams newline-delimited JSON objects
        while (!stream->isExhausted() && !cancelFlag.load()) {
            char c;
            if (stream->read(&c, 1) != 1)
                break;

            if (c == '\n') {
                auto line = juce::String::fromUTF8(
                    static_cast<const char*>(lineBuffer.getData()),
                    static_cast<int>(lineBuffer.getSize()));
                lineBuffer.reset();

                auto parsed = juce::JSON::parse(line);
                if (auto* obj = parsed.getDynamicObject()) {
                    // Ollama /api/chat streams {"message":{"content":"..."}}
                    if (auto* msg = obj->getProperty("message")
                                        .getDynamicObject()) {
                        auto chunk = msg->getProperty("content")
                                        .toString()
                                        .toStdString();
                        fullResponse += chunk;
                        if (onChunk)
                            onChunk(chunk);
                    }
                }
            } else {
                lineBuffer.append(&c, 1);
            }
        }

        return fullResponse;
    }

    // ------------------------------------------------------------------
    // Build the messages array JSON for the Ollama /api/chat endpoint
    // ------------------------------------------------------------------
    juce::String buildChatJson(const std::string& userMessage) {
        auto root = std::make_unique<juce::DynamicObject>();
        root->setProperty("model", juce::String(cfg.modelName));
        root->setProperty("stream", cfg.streamResponses);

        auto optionsObj = std::make_unique<juce::DynamicObject>();
        optionsObj->setProperty("temperature",
                                static_cast<double>(cfg.temperature));
        optionsObj->setProperty("num_predict", cfg.maxTokens);
        root->setProperty("options", juce::var(optionsObj.release()));

        juce::Array<juce::var> messages;

        // System message with DAW context
        auto sysMsg = std::make_unique<juce::DynamicObject>();
        sysMsg->setProperty("role", "system");
        sysMsg->setProperty("content",
                            juce::String(buildSystemPrompt()));
        messages.add(juce::var(sysMsg.release()));

        // Conversation history (trimmed to stay within token budget)
        auto trimmedHistory = trimHistory();
        for (const auto& msg : trimmedHistory) {
            auto m = std::make_unique<juce::DynamicObject>();
            m->setProperty("role", roleToString(msg.role));
            m->setProperty("content", juce::String(msg.content));
            messages.add(juce::var(m.release()));
        }

        // Current user message
        auto userMsg = std::make_unique<juce::DynamicObject>();
        userMsg->setProperty("role", "user");
        userMsg->setProperty("content", juce::String(userMessage));
        messages.add(juce::var(userMsg.release()));

        root->setProperty("messages", messages);
        return juce::JSON::toString(juce::var(root.release()));
    }

    // ------------------------------------------------------------------
    // System prompt with live DAW context
    // ------------------------------------------------------------------
    std::string buildSystemPrompt() const {
        std::ostringstream ss;
        ss << cfg.systemPrompt << "\n\n--- Current DAW State ---\n";
        for (const auto& [key, val] : dawContext)
            ss << key << ": " << val << "\n";
        return ss.str();
    }

    // ------------------------------------------------------------------
    // Trim history to fit approximate token window
    // ------------------------------------------------------------------
    std::vector<CopilotMessage> trimHistory() const {
        std::vector<CopilotMessage> result;
        int charCount = 0;

        // Walk backwards, keeping most-recent messages first
        for (auto it = chatHistory.rbegin(); it != chatHistory.rend(); ++it) {
            charCount += static_cast<int>(it->content.size());
            if (charCount > kMaxHistoryTokenEstimate)
                break;
            result.push_back(*it);
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

    // ------------------------------------------------------------------
    // Parse action blocks from assistant response
    // ------------------------------------------------------------------
    std::vector<CopilotAction> parseActions(const std::string& response) {
        std::vector<CopilotAction> actions;
        const std::string openTag = "```action";
        const std::string closeTag = "```";

        size_t pos = 0;
        while (pos < response.size()) {
            auto start = response.find(openTag, pos);
            if (start == std::string::npos)
                break;
            start += openTag.size();

            auto end = response.find(closeTag, start);
            if (end == std::string::npos)
                break;

            auto jsonStr = response.substr(start, end - start);
            auto parsed = juce::JSON::parse(juce::String(jsonStr));
            if (auto* obj = parsed.getDynamicObject()) {
                CopilotAction action;
                action.type = actionTypeFromString(
                    obj->getProperty("type").toString());
                action.targetId =
                    obj->getProperty("targetId").toString().toStdString();
                action.parameterName =
                    obj->getProperty("parameterName").toString().toStdString();
                action.value =
                    static_cast<float>(obj->getProperty("value"));
                action.stringValue =
                    obj->getProperty("stringValue").toString().toStdString();

                if (auto* meta =
                        obj->getProperty("metadata").getDynamicObject()) {
                    for (const auto& prop : meta->getProperties())
                        action.metadata[prop.name.toString().toStdString()] =
                            prop.value.toString().toStdString();
                }
                actions.push_back(std::move(action));
            }

            pos = end + closeTag.size();
        }
        return actions;
    }

    // ------------------------------------------------------------------
    // Execute parsed actions via the registered handler
    // ------------------------------------------------------------------
    void executeActions(const std::vector<CopilotAction>& actions) {
        if (!actionHandler)
            return;

        for (const auto& action : actions) {
            auto result = actionHandler(action);
            // Process any follow-up actions returned by the handler
            if (result.success && !result.followUpActions.empty())
                executeActions(result.followUpActions);
        }
    }

    // ------------------------------------------------------------------
    // Role helpers
    // ------------------------------------------------------------------
    static juce::String roleToString(CopilotMessage::Role role) {
        switch (role) {
            case CopilotMessage::Role::System:    return "system";
            case CopilotMessage::Role::User:      return "user";
            case CopilotMessage::Role::Assistant:  return "assistant";
            case CopilotMessage::Role::Tool:       return "tool";
        }
        return "user";
    }
};

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------
CopilotBridge::CopilotBridge(const CopilotConfig& config)
    : pImpl(std::make_unique<Impl>(config))
{
}

CopilotBridge::~CopilotBridge() = default;

void CopilotBridge::setConfig(const CopilotConfig& config) {
    pImpl->cfg = config;
    if (pImpl->cfg.systemPrompt.empty())
        pImpl->cfg.systemPrompt = kDefaultSystemPrompt;
    pImpl->connected = false;
}

const CopilotConfig& CopilotBridge::config() const {
    return pImpl->cfg;
}

bool CopilotBridge::checkConnection() {
    auto response = pImpl->httpGet("/api/tags");
    pImpl->connected = !response.empty();
    return pImpl->connected;
}

bool CopilotBridge::isConnected() const {
    return pImpl->connected;
}

void CopilotBridge::sendMessage(const std::string& userMessage,
                                MessageCallback onResponse,
                                StreamCallback onStream) {
    pImpl->cancelFlag = false;

    // Record user message in history
    CopilotMessage userMsg;
    userMsg.role = CopilotMessage::Role::User;
    userMsg.content = userMessage;
    userMsg.timestamp = juce::Time::getMillisecondCounterHiRes() / 1000.0;
    pImpl->chatHistory.push_back(userMsg);

    auto jsonBody = pImpl->buildChatJson(userMessage);

    pImpl->threadPool.addJob([this, jsonBody, onResponse, onStream]() {
        std::string responseText;

        if (pImpl->cfg.streamResponses && onStream) {
            responseText = pImpl->httpPostStreaming("/api/chat", jsonBody,
                                                   onStream);
        } else {
            auto raw = pImpl->httpPost("/api/chat", jsonBody);
            auto parsed = juce::JSON::parse(juce::String(raw));
            if (auto* obj = parsed.getDynamicObject()) {
                if (auto* msg = obj->getProperty("message")
                                    .getDynamicObject()) {
                    responseText =
                        msg->getProperty("content").toString().toStdString();
                }
            }
        }

        if (pImpl->cancelFlag.load())
            return;

        // Record assistant message
        CopilotMessage assistantMsg;
        assistantMsg.role = CopilotMessage::Role::Assistant;
        assistantMsg.content = responseText;
        assistantMsg.timestamp =
            juce::Time::getMillisecondCounterHiRes() / 1000.0;
        pImpl->chatHistory.push_back(assistantMsg);

        // Parse and execute any embedded action blocks
        auto actions = pImpl->parseActions(responseText);
        pImpl->executeActions(actions);

        // Deliver response on the message thread
        juce::MessageManager::callAsync(
            [onResponse, assistantMsg]() { onResponse(assistantMsg); });
    });
}

const std::vector<CopilotMessage>& CopilotBridge::history() const {
    return pImpl->chatHistory;
}

void CopilotBridge::clearHistory() {
    pImpl->chatHistory.clear();
}

void CopilotBridge::setActionHandler(ActionCallback handler) {
    pImpl->actionHandler = std::move(handler);
}

void CopilotBridge::updateContext(const std::string& key,
                                  const std::string& value) {
    pImpl->dawContext[key] = value;
}

void CopilotBridge::masterForTarget(const std::string& target,
                                    MessageCallback onResponse) {
    sendMessage("Master the current mix for " + target +
                    ". Apply appropriate loudness normalization, EQ, limiting, "
                    "and stereo imaging. Emit the action blocks to set up the "
                    "mastering chain.",
                onResponse);
}

void CopilotBridge::suggestMix(MessageCallback onResponse) {
    sendMessage("Analyze the current mix state and suggest improvements. "
                "Consider levels, panning, EQ balance, dynamics, and spatial "
                "placement. Emit action blocks for any recommended changes.",
                onResponse);
}

void CopilotBridge::analyzeAndReport(MessageCallback onResponse) {
    sendMessage("Analyze the current mix and provide a detailed report. "
                "Include observations about frequency balance, dynamic range, "
                "stereo width, and any potential issues. Use the Analyze action "
                "to gather data first.",
                onResponse);
}

void CopilotBridge::cancel() {
    pImpl->cancelFlag = true;
}

} // namespace revithion::ai
