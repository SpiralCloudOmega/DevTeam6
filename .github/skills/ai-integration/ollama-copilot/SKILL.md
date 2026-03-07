---
name: ollama-copilot
version: "1.0"
description: Ollama LLM integration for in-DAW AI assistant — qwen3-coder, HTTP streaming, context management, tool calling for REVITHION STUDIO
tags: [ai, llm, ollama, assistant, streaming, tool-calling]
category: ai-integration
---

# Ollama LLM Integration for In-DAW AI Assistant

REVITHION STUDIO embeds an AI assistant powered by locally-hosted Ollama models (qwen3-coder:30b for code tasks, qwen3:8b for fast chat). The assistant lives inside the DAW as a dockable panel, providing context-aware help: explaining DSP chains, suggesting effect parameters, generating automation curves, and writing JUCE code snippets. All inference runs locally via Ollama's HTTP API on port 11434, ensuring zero-latency network hops and full privacy.

## Architecture Overview

The integration uses a three-layer design: (1) a **C++ HTTP client** in the DAW that streams tokens from Ollama, (2) a **context manager** that maintains conversation history within token limits, and (3) a **tool-calling layer** that lets the LLM invoke DAW functions (set parameter, add track, analyze spectrum). The assistant runs on a background thread, feeding tokens into a UI text component via `juce::MessageManager::callAsync`.

## HTTP Streaming Client

```cpp
#include <juce_core/juce_core.h>

class OllamaClient {
    juce::URL baseUrl { "http://localhost:11434" };

public:
    using TokenCallback = std::function<void(const juce::String&, bool done)>;

    void streamCompletion(const juce::String& model,
                          const juce::var& messages,
                          TokenCallback onToken) {
        juce::DynamicObject::Ptr body = new juce::DynamicObject();
        body->setProperty("model", model);
        body->setProperty("messages", messages);
        body->setProperty("stream", true);

        auto url = baseUrl.getChildURL("/api/chat");
        auto request = url.withPOSTData(juce::JSON::toString(body.get()));

        auto stream = request.createInputStream(juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inPostBody)
            .withConnectionTimeoutMs(5000));

        if (!stream) { onToken("Error: cannot reach Ollama", true); return; }

        juce::MemoryBlock lineBuffer;
        while (!stream->isExhausted()) {
            char c;
            if (stream->read(&c, 1) == 1) {
                if (c == '\n') {
                    auto json = juce::JSON::parse(lineBuffer.toString());
                    auto content = json["message"]["content"].toString();
                    bool done = json["done"].operator bool();
                    if (content.isNotEmpty()) onToken(content, done);
                    lineBuffer.reset();
                    if (done) break;
                } else {
                    lineBuffer.append(&c, 1);
                }
            }
        }
    }
};
```

## Context Window Management

```cpp
struct ChatMessage {
    juce::String role;   // "system", "user", "assistant", "tool"
    juce::String content;
    int estimatedTokens = 0;
};

class ContextManager {
    std::vector<ChatMessage> history;
    ChatMessage systemPrompt;
    int maxContextTokens = 32768;

public:
    ContextManager() {
        systemPrompt = { "system",
            "You are REVITHION Assistant, an expert audio engineer and DSP programmer. "
            "You help users with mixing, mastering, effect chains, JUCE code, and "
            "REVITHION STUDIO features. Be concise and precise.", 64 };
    }

    juce::var buildMessages(const juce::String& userMsg) {
        history.push_back({ "user", userMsg, estimateTokens(userMsg) });
        trimToFit();

        juce::Array<juce::var> msgs;
        msgs.add(messageToVar(systemPrompt));
        for (auto& m : history) msgs.add(messageToVar(m));
        return msgs;
    }

    void addAssistantReply(const juce::String& reply) {
        history.push_back({ "assistant", reply, estimateTokens(reply) });
    }

private:
    int estimateTokens(const juce::String& s) const {
        return s.length() / 4 + 1; // rough byte/4 heuristic
    }

    void trimToFit() {
        int total = systemPrompt.estimatedTokens;
        for (auto& m : history) total += m.estimatedTokens;
        while (total > maxContextTokens && history.size() > 1) {
            total -= history.front().estimatedTokens;
            history.erase(history.begin());
        }
    }

    static juce::var messageToVar(const ChatMessage& m) {
        auto* obj = new juce::DynamicObject();
        obj->setProperty("role", m.role);
        obj->setProperty("content", m.content);
        return juce::var(obj);
    }
};
```

## Tool Calling — DAW Function Invocation

```cpp
struct ToolDefinition {
    juce::String name;
    juce::String description;
    std::function<juce::var(const juce::var& args)> execute;
};

class ToolRegistry {
    std::map<juce::String, ToolDefinition> tools;

public:
    void registerTool(ToolDefinition def) { tools[def.name] = std::move(def); }

    juce::var executeTool(const juce::String& name, const juce::var& args) {
        auto it = tools.find(name);
        if (it == tools.end()) return "Unknown tool: " + name;
        return it->second.execute(args);
    }

    void registerDefaults(/* DAW engine reference */) {
        registerTool({ "set_parameter", "Set a plugin parameter value",
            [](const juce::var& args) -> juce::var {
                auto track = args["track"].toString();
                auto param = args["parameter"].toString();
                float value = static_cast<float>(args["value"]);
                // Engine::setParameter(track, param, value);
                return "Set " + param + " to " + juce::String(value);
            }});

        registerTool({ "analyze_spectrum", "Get spectral analysis of current selection",
            [](const juce::var& /*args*/) -> juce::var {
                // auto peaks = Engine::analyzeSpectrum();
                return "Peak at 2.4kHz (-3dB), low-end buildup at 120Hz";
            }});
    }
};
```

## Anti-Patterns

- ❌ Don't stream LLM tokens on the message thread — use a background thread with `callAsync` for UI updates
- ❌ Don't send the entire project state as context — summarize relevant tracks and parameters only
- ❌ Don't hardcode model names — allow user selection between qwen3:8b (fast) and qwen3-coder:30b (precise)
- ❌ Don't ignore Ollama connection failures — show a clear status indicator and retry with backoff
- ❌ Don't let context grow unbounded — trim oldest messages when approaching the token limit
- ❌ Don't execute tool calls without user confirmation for destructive actions (delete track, reset session)

## Checklist

- [ ] Ollama server reachable on localhost:11434 at DAW startup
- [ ] Streaming client handles partial JSON lines and connection drops gracefully
- [ ] Context manager trims to 32K tokens (configurable per model)
- [ ] System prompt includes REVITHION-specific DSP vocabulary
- [ ] Tool registry validates argument types before execution
- [ ] Destructive tool calls require explicit user confirmation
- [ ] Model warm-up request sent on first panel open to reduce initial latency
- [ ] Conversation history persisted to project file for session continuity
