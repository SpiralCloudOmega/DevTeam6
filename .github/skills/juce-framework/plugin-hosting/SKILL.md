---
name: juce-plugin-hosting
description: JUCE plugin hosting patterns for DAW/host apps — AudioPluginFormatManager, KnownPluginList, scanning, AudioProcessorGraph, instance management, parameter discovery, presets, and crash isolation for REVITHION STUDIO.
---

# JUCE Plugin Hosting

Patterns for hosting VST3/AU/LV2/CLAP plugins in a JUCE 7/8 DAW. Powers REVITHION STUDIO's plugin chain and 12×4 routing grid.

## AudioPluginFormatManager — Registering Formats
```cpp
juce::AudioPluginFormatManager formatManager;
formatManager.addDefaultFormats(); // VST3, AU (macOS), AAX
// formatManager.addFormat(new CLAPPluginFormat()); // CLAP via third-party module
```

## KnownPluginList — Persistence
```cpp
juce::KnownPluginList knownPlugins;
void savePluginList(const juce::File& f) { if (auto xml = knownPlugins.createXml()) xml->writeTo(f); }
void loadPluginList(const juce::File& f) { if (auto xml = juce::parseXML(f)) knownPlugins.recreateFromXml(*xml); }
```

## Plugin Scanning — Background Thread with Progress

Always scan on a background thread. Provide a `deadMansPedalFile` so crashing plugins are skipped on next scan.
```cpp
class PluginScanner : public juce::Thread {
    juce::AudioPluginFormat& format; juce::KnownPluginList& list; juce::File deadMansPedal;
public:
    PluginScanner(juce::AudioPluginFormat& f, juce::KnownPluginList& l, juce::File dmp)
        : Thread("PluginScan"), format(f), list(l), deadMansPedal(dmp) {}
    void run() override {
        juce::PluginDirectoryScanner scanner(list, format,
            format.getDefaultLocationsToSearch(), true, deadMansPedal);
        juce::String name;
        while (scanner.scanNextFile(true, name) && !threadShouldExit())
            DBG("Scanning (" + juce::String(int(scanner.getProgress() * 100)) + "%): " + name);
    }
};
```
Default paths: Win VST3 `C:\Program Files\Common Files\VST3` · macOS AU `/Library/Audio/Plug-Ins/Components` · macOS VST3 `/Library/Audio/Plug-Ins/VST3` · Linux VST3 `/usr/lib/vst3` · Linux LV2 `/usr/lib/lv2`

## AudioProcessorGraph — Routing Grid

Backbone of REVITHION's 12×4 grid (AXE-FX III style). Every graph needs audio+MIDI I/O nodes.
```cpp
juce::AudioProcessorGraph graph;
graph.setPlayConfigDetails(2, 2, sampleRate, blockSize);
graph.prepareToPlay(sampleRate, blockSize);
using IOProc = juce::AudioProcessorGraph::AudioGraphIOProcessor;
auto audioIn  = graph.addNode(std::make_unique<IOProc>(IOProc::audioInputNode));
auto audioOut = graph.addNode(std::make_unique<IOProc>(IOProc::audioOutputNode));
auto midiIn   = graph.addNode(std::make_unique<IOProc>(IOProc::midiInputNode));
auto midiOut  = graph.addNode(std::make_unique<IOProc>(IOProc::midiOutputNode));
for (int ch = 0; ch < 2; ++ch)
    graph.addConnection({{ audioIn->nodeID, ch }, { audioOut->nodeID, ch }});
graph.addConnection({{ midiIn->nodeID, juce::AudioProcessorGraph::midiChannelIndex },
                     { midiOut->nodeID, juce::AudioProcessorGraph::midiChannelIndex }});
```

## Plugin Instance Management
```cpp
formatManager.createPluginInstanceAsync(desc, sampleRate, blockSize,
    [this](std::unique_ptr<juce::AudioProcessor> inst, const juce::String& error) {
        if (inst) { inst->prepareToPlay(sampleRate, blockSize); graph.addNode(std::move(inst)); }
        else      { DBG("Plugin load failed: " + error); }
    });
```

## Parameter Discovery
```cpp
for (auto* param : processor.getParameters()) {
    if (auto* p = dynamic_cast<juce::AudioProcessorParameterWithID*>(param))
        DBG("[" + p->paramID + "] " + p->getName(64) + " = " + juce::String(p->getValue())
            + " (" + p->getText(0.f, 32) + ".." + p->getText(1.f, 32) + ")");
}
```

## Preset Management — State Save/Recall
```cpp
juce::MemoryBlock getState(juce::AudioProcessor& p) { juce::MemoryBlock m; p.getStateInformation(m); return m; }
void setState(juce::AudioProcessor& p, const juce::MemoryBlock& m) {
    p.setStateInformation(m.getData(), static_cast<int>(m.getSize())); }
void savePreset(juce::AudioProcessor& p, const juce::File& f) {
    auto s = getState(p); f.replaceWithData(s.getData(), s.getSize()); }
void loadPreset(juce::AudioProcessor& p, const juce::File& f) {
    juce::MemoryBlock s; if (f.loadFileAsData(s)) setState(p, s); }
```

## Sandbox / Crash Isolation

Out-of-process hosting prevents a rogue plugin from crashing the DAW.
```cpp
class PluginSandbox : public juce::ChildProcessCoordinator {
public:
    bool launchWorker(const juce::String& pluginPath) {
        return launchWorkerProcess(juce::File::getSpecialLocation(
            juce::File::currentExecutableFile), "--sandbox " + pluginPath, 5000); }
    void handleMessageFromWorker(const juce::MemoryBlock&) override { /* deserialise audio/params */ }
    void handleConnectionLost() override { DBG("Plugin crashed — bypassing slot"); }
};
```

## Anti-Patterns

- **Scanning on the message thread** — freezes UI. Always use a background `juce::Thread`.
- **Ignoring deadMansPedalFile** — a crashing plugin blocks all future scans without it.
- **Leaking AudioProcessor instances** — let `std::unique_ptr` and the graph own nodes.
- **Blocking in createPluginInstanceAsync callback** — marshal UI work to the message thread.
- **Hardcoding plugin paths** — use `format->getDefaultLocationsToSearch()` plus user paths.
- **Skipping prepareToPlay** — every plugin must be prepared with host sample rate and block size.

## Checklist

- [ ] Register formats via `addDefaultFormats()` plus CLAP if needed
- [ ] Persist `KnownPluginList` to XML between sessions
- [ ] Scan asynchronously on a background thread with progress reporting
- [ ] Provide `deadMansPedalFile` for crash-safe scanning
- [ ] Build `AudioProcessorGraph` with audio + MIDI I/O nodes
- [ ] Use `createPluginInstanceAsync` — never block the UI
- [ ] Call `prepareToPlay` on every plugin before processing
- [ ] Enumerate parameters for automation binding
- [ ] Save/restore state via `getStateInformation`/`setStateInformation`
- [ ] Implement out-of-process crash isolation for third-party plugins
