#include "PluginExport.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

namespace revithion::plugins {

// ═══════════════════════════════════════════════════════════════════════════
// Impl (pImpl idiom)
// ═══════════════════════════════════════════════════════════════════════════

class PluginExportManager::Impl {
public:
    // Helpers
    static std::string formatName(PluginFormat fmt);
    static std::string exportTypeName(ExportType type);
    static std::string juceFormatsString(const std::vector<PluginFormat>& formats);
    static std::string sanitize(const std::string& name);

    static void writeFile(const fs::path& path, const std::string& content);
    static int runCommand(const std::string& cmd, std::string& output);
};

// ── Utility helpers ───────────────────────────────────────────────────────

std::string PluginExportManager::Impl::formatName(PluginFormat fmt)
{
    switch (fmt) {
        case PluginFormat::VST3:       return "VST3";
        case PluginFormat::AU:         return "AU";
        case PluginFormat::LV2:        return "LV2";
        case PluginFormat::AAX:        return "AAX";
        case PluginFormat::Standalone: return "Standalone";
        case PluginFormat::CLAP:       return "CLAP";
    }
    return "Unknown";
}

std::string PluginExportManager::Impl::exportTypeName(ExportType type)
{
    switch (type) {
        case ExportType::AmpSim:        return "AmpSim";
        case ExportType::Effect:        return "Effect";
        case ExportType::ChannelStrip:  return "ChannelStrip";
        case ExportType::MasteringChain:return "MasteringChain";
        case ExportType::Instrument:    return "Instrument";
        case ExportType::MIDIEffect:    return "MIDIEffect";
        case ExportType::Analyzer:      return "Analyzer";
    }
    return "Unknown";
}

std::string PluginExportManager::Impl::juceFormatsString(
    const std::vector<PluginFormat>& formats)
{
    std::ostringstream oss;
    for (size_t i = 0; i < formats.size(); ++i) {
        if (i > 0) oss << " ";
        oss << formatName(formats[i]);
    }
    return oss.str();
}

std::string PluginExportManager::Impl::sanitize(const std::string& name)
{
    std::string s;
    s.reserve(name.size());
    for (char c : name) {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '_')
            s += c;
    }
    return s;
}

void PluginExportManager::Impl::writeFile(const fs::path& path,
                                           const std::string& content)
{
    fs::create_directories(path.parent_path());
    std::ofstream out(path, std::ios::trunc);
    out << content;
}

int PluginExportManager::Impl::runCommand(const std::string& cmd,
                                           std::string& output)
{
    output.clear();
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return -1;

    char buf[256];
    while (fgets(buf, sizeof(buf), pipe))
        output += buf;

    return pclose(pipe);
}

// ═══════════════════════════════════════════════════════════════════════════
// PluginExportManager public interface
// ═══════════════════════════════════════════════════════════════════════════

PluginExportManager::PluginExportManager()
    : pImpl(std::make_unique<Impl>()) {}

PluginExportManager::~PluginExportManager() = default;

// ── CMakeLists.txt generation ─────────────────────────────────────────────

std::string PluginExportManager::generateCMake(const ExportConfig& config)
{
    const auto safe = Impl::sanitize(config.name);
    const auto formats = Impl::juceFormatsString(config.formats);

    std::ostringstream o;
    o << "cmake_minimum_required(VERSION 3.22)\n"
      << "project(" << safe << " VERSION " << config.version
      << " LANGUAGES CXX)\n\n"
      << "set(CMAKE_CXX_STANDARD 20)\n"
      << "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n\n"
      << "# ── JUCE ──────────────────────────────────────────────────────\n"
      << "add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../external/JUCE JUCE)\n\n"
      << "# ── Plugin target ─────────────────────────────────────────────\n"
      << "juce_add_plugin(" << safe << "\n"
      << "    PRODUCT_NAME          \"" << config.name << "\"\n"
      << "    COMPANY_NAME          \"" << config.manufacturer << "\"\n"
      << "    BUNDLE_ID             \"" << config.bundleId << "\"\n"
      << "    PLUGIN_MANUFACTURER_CODE Sprl\n"
      << "    PLUGIN_CODE           Rvth\n"
      << "    FORMATS               " << formats << "\n";

    if (config.type == ExportType::Instrument)
        o << "    IS_SYNTH              TRUE\n"
          << "    NEEDS_MIDI_INPUT      TRUE\n";
    else if (config.type == ExportType::MIDIEffect)
        o << "    IS_MIDI_EFFECT        TRUE\n"
          << "    NEEDS_MIDI_INPUT      TRUE\n"
          << "    NEEDS_MIDI_OUTPUT     TRUE\n";

    o << "    EDITOR_WANTS_KEYBOARD_FOCUS TRUE\n"
      << ")\n\n"
      << "target_sources(" << safe << " PRIVATE\n"
      << "    PluginProcessor.h\n"
      << "    PluginProcessor.cpp\n"
      << "    PluginEditor.h\n"
      << "    PluginEditor.cpp\n"
      << "    RevithionLookAndFeel.h\n"
      << "    RevithionLookAndFeel.cpp\n"
      << ")\n\n"
      << "target_compile_definitions(" << safe << " PUBLIC\n"
      << "    JUCE_WEB_BROWSER=0\n"
      << "    JUCE_USE_CURL=0\n"
      << "    JUCE_VST3_CAN_REPLACE_VST2=0\n"
      << ")\n\n"
      << "target_link_libraries(" << safe << " PRIVATE\n"
      << "    juce::juce_audio_utils\n"
      << "    juce::juce_dsp\n";

    // Link REVITHION engine libraries based on export type
    switch (config.type) {
        case ExportType::AmpSim:
            o << "    revithion_neural\n";
            break;
        case ExportType::Effect:
            o << "    revithion_blocks\n";
            break;
        case ExportType::ChannelStrip:
            o << "    revithion_engine\n";
            break;
        case ExportType::MasteringChain:
            o << "    revithion_ai\n"
              << "    revithion_engine\n";
            break;
        case ExportType::Instrument:
            o << "    revithion_ai\n";
            break;
        case ExportType::MIDIEffect:
            o << "    revithion_engine\n";
            break;
        case ExportType::Analyzer:
            o << "    revithion_engine\n";
            break;
    }

    if (config.enableGPU)
        o << "    revithion_gpu\n";

    o << "    juce::juce_recommended_config_flags\n"
      << "    juce::juce_recommended_lto_flags\n"
      << "    juce::juce_recommended_warning_flags\n"
      << ")\n";

    return o.str();
}

// ── Processor header generation ───────────────────────────────────────────

std::string PluginExportManager::generateProcessorH(const ExportConfig& config)
{
    const auto safe = Impl::sanitize(config.name);
    const auto typeName = Impl::exportTypeName(config.type);

    std::ostringstream o;
    o << "#pragma once\n"
      << "#include <JuceHeader.h>\n\n";

    // Include the appropriate REVITHION headers
    switch (config.type) {
        case ExportType::AmpSim:
            o << "#include \"neural/LSTMAmpModel.h\"\n";
            break;
        case ExportType::Effect:
            o << "#include \"blocks/RoutingGrid.h\"\n";
            break;
        case ExportType::ChannelStrip:
            o << "#include \"engine/MixConsole.h\"\n";
            break;
        case ExportType::MasteringChain:
            o << "#include \"ai/MasteringAssistant.h\"\n";
            break;
        case ExportType::Instrument:
            o << "#include \"ai/ACEStepBridge.h\"\n";
            break;
        case ExportType::MIDIEffect:
            o << "#include \"engine/MIDISystem.h\"\n";
            break;
        case ExportType::Analyzer:
            o << "#include \"ai/MasteringAssistant.h\"\n";
            break;
    }

    o << "\nclass " << safe << "Processor : public juce::AudioProcessor {\n"
      << "public:\n"
      << "    " << safe << "Processor();\n"
      << "    ~" << safe << "Processor() override;\n\n"
      << "    void prepareToPlay(double sampleRate, int samplesPerBlock) override;\n"
      << "    void releaseResources() override;\n"
      << "    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;\n\n"
      << "    juce::AudioProcessorEditor* createEditor() override;\n"
      << "    bool hasEditor() const override { return true; }\n\n"
      << "    const juce::String getName() const override "
      << "{ return \"" << config.name << "\"; }\n"
      << "    bool acceptsMidi() const override { return "
      << (config.type == ExportType::Instrument
          || config.type == ExportType::MIDIEffect ? "true" : "false")
      << "; }\n"
      << "    bool producesMidi() const override { return "
      << (config.type == ExportType::MIDIEffect ? "true" : "false") << "; }\n"
      << "    double getTailLengthSeconds() const override { return 0.0; }\n\n"
      << "    int getNumPrograms() override { return 1; }\n"
      << "    int getCurrentProgram() override { return 0; }\n"
      << "    void setCurrentProgram(int) override {}\n"
      << "    const juce::String getProgramName(int) override "
      << "{ return {}; }\n"
      << "    void changeProgramName(int, const juce::String&) override {}\n\n"
      << "    void getStateInformation(juce::MemoryBlock& destData) override;\n"
      << "    void setStateInformation(const void* data, int sizeInBytes) override;\n\n"
      << "private:\n";

    // Private members depend on export type
    switch (config.type) {
        case ExportType::AmpSim:
            o << "    revithion::neural::LSTMAmpModel ampModel_;\n"
              << "    std::string modelPath_;\n"
              << "    float inputGain_ = 1.0f;\n"
              << "    float outputGain_ = 1.0f;\n";
            break;
        case ExportType::Effect:
            o << "    revithion::blocks::RoutingGrid grid_;\n";
            break;
        case ExportType::ChannelStrip:
            o << "    revithion::engine::MixConsole console_;\n";
            break;
        case ExportType::MasteringChain:
            o << "    revithion::ai::MasteringAssistant assistant_;\n"
              << "    revithion::ai::MasteringChain chain_;\n";
            break;
        case ExportType::Instrument:
            o << "    revithion::ai::ACEStepBridge bridge_;\n"
              << "    juce::AudioBuffer<float> generatedAudio_;\n"
              << "    int playPosition_ = 0;\n";
            break;
        case ExportType::MIDIEffect:
            o << "    std::vector<std::unique_ptr<revithion::engine::MIDIEffect>> effects_;\n";
            break;
        case ExportType::Analyzer:
            o << "    revithion::ai::MasteringAssistant analyzer_;\n"
              << "    revithion::ai::LoudnessMeter meter_;\n";
            break;
    }

    o << "\n    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR("
      << safe << "Processor)\n"
      << "};\n";

    return o.str();
}

// ── Processor implementation generation ───────────────────────────────────

std::string PluginExportManager::generateProcessorCpp(const ExportConfig& config)
{
    const auto safe = Impl::sanitize(config.name);

    std::ostringstream o;
    o << "#include \"PluginProcessor.h\"\n"
      << "#include \"PluginEditor.h\"\n\n";

    // Constructor
    o << safe << "Processor::" << safe << "Processor()\n"
      << "    : AudioProcessor(BusesProperties()\n";
    if (config.type == ExportType::Instrument) {
        o << "          .withOutput(\"Output\", juce::AudioChannelSet::stereo(), true))\n";
    } else {
        o << "          .withInput(\"Input\", juce::AudioChannelSet::stereo(), true)\n"
          << "          .withOutput(\"Output\", juce::AudioChannelSet::stereo(), true))\n";
    }
    o << "{\n";

    // Type-specific initialisation
    switch (config.type) {
        case ExportType::AmpSim:
            if (!config.ampModelPath.empty()) {
                o << "    ampModel_.loadWeights(\""
                  << config.ampModelPath << "\");\n";
            }
            break;
        case ExportType::Effect:
            o << "    grid_.loadDefaultPreset();\n";
            break;
        case ExportType::ChannelStrip:
            o << "    console_.addChannel(\"Main\", revithion::engine::TrackType::Audio);\n";
            break;
        case ExportType::MasteringChain:
            o << "    chain_ = assistant_.suggestChain(\n"
              << "        revithion::ai::AudioAnalysis{},\n"
              << "        revithion::ai::MasterTarget::Streaming);\n";
            break;
        case ExportType::Instrument:
            break;
        case ExportType::MIDIEffect:
            o << "    effects_.push_back(std::make_unique<"
              << "revithion::engine::ArpeggiatorEffect>());\n";
            break;
        case ExportType::Analyzer:
            break;
    }

    o << "}\n\n"
      << safe << "Processor::~" << safe << "Processor() = default;\n\n";

    // prepareToPlay
    o << "void " << safe
      << "Processor::prepareToPlay(double sampleRate, int samplesPerBlock)\n"
      << "{\n"
      << "    juce::ignoreUnused(samplesPerBlock);\n";
    if (config.type == ExportType::AmpSim)
        o << "    ampModel_.reset();\n";
    else if (config.type == ExportType::ChannelStrip)
        o << "    console_.setSampleRate(sampleRate);\n"
          << "    console_.setBufferSize(samplesPerBlock);\n";
    else
        o << "    juce::ignoreUnused(sampleRate);\n";
    o << "}\n\n";

    // releaseResources
    o << "void " << safe << "Processor::releaseResources() {}\n\n";

    // processBlock
    o << "void " << safe
      << "Processor::processBlock(juce::AudioBuffer<float>& buffer, "
      << "juce::MidiBuffer& midi)\n"
      << "{\n"
      << "    juce::ScopedNoDenormals noDenormals;\n";

    switch (config.type) {
        case ExportType::AmpSim:
            o << "    auto* channelData = buffer.getWritePointer(0);\n"
              << "    for (int i = 0; i < buffer.getNumSamples(); ++i) {\n"
              << "        channelData[i] = ampModel_.processSample(\n"
              << "            channelData[i] * inputGain_) * outputGain_;\n"
              << "    }\n"
              << "    // Copy mono to stereo if needed\n"
              << "    if (buffer.getNumChannels() > 1)\n"
              << "        buffer.copyFrom(1, 0, buffer, 0, 0, "
              << "buffer.getNumSamples());\n";
            break;

        case ExportType::Effect:
            o << "    juce::ignoreUnused(midi);\n"
              << "    auto order = grid_.getProcessingOrder();\n"
              << "    // Process through grid blocks (placeholder — full DSP in engine)\n"
              << "    juce::ignoreUnused(order);\n";
            break;

        case ExportType::ChannelStrip:
            o << "    juce::ignoreUnused(midi);\n"
              << "    // Channel strip processing delegated to MixConsole engine\n";
            break;

        case ExportType::MasteringChain:
            o << "    juce::ignoreUnused(midi);\n"
              << "    assistant_.applyChain(chain_,\n"
              << "        buffer.getReadPointer(0), buffer.getWritePointer(0),\n"
              << "        buffer.getNumSamples(), buffer.getNumChannels(),\n"
              << "        getSampleRate());\n"
              << "    meter_ = assistant_.updateMeter(\n"
              << "        buffer.getReadPointer(0), buffer.getNumSamples(),\n"
              << "        buffer.getNumChannels(), getSampleRate());\n";
            break;

        case ExportType::Instrument:
            o << "    buffer.clear();\n"
              << "    // Playback of generated audio from ACE-STEP bridge\n"
              << "    if (generatedAudio_.getNumSamples() > 0) {\n"
              << "        int samplesToRead = juce::jmin(\n"
              << "            buffer.getNumSamples(),\n"
              << "            generatedAudio_.getNumSamples() - playPosition_);\n"
              << "        if (samplesToRead > 0) {\n"
              << "            for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {\n"
              << "                int srcCh = juce::jmin(ch, "
              << "generatedAudio_.getNumChannels() - 1);\n"
              << "                buffer.copyFrom(ch, 0, generatedAudio_, "
              << "srcCh, playPosition_, samplesToRead);\n"
              << "            }\n"
              << "            playPosition_ += samplesToRead;\n"
              << "        }\n"
              << "    }\n";
            break;

        case ExportType::MIDIEffect:
            o << "    // Convert MIDI buffer to REVITHION notes, process, convert back\n"
              << "    std::vector<revithion::engine::MIDINote> notes;\n"
              << "    for (const auto metadata : midi) {\n"
              << "        const auto msg = metadata.getMessage();\n"
              << "        if (msg.isNoteOn()) {\n"
              << "            revithion::engine::MIDINote n;\n"
              << "            n.pitch = msg.getNoteNumber();\n"
              << "            n.velocity = msg.getVelocity();\n"
              << "            n.channel = msg.getChannel() - 1;\n"
              << "            notes.push_back(n);\n"
              << "        }\n"
              << "    }\n"
              << "    for (auto& fx : effects_) {\n"
              << "        if (fx->active) fx->process(notes);\n"
              << "    }\n"
              << "    midi.clear();\n"
              << "    int samplePos = 0;\n"
              << "    for (const auto& n : notes) {\n"
              << "        midi.addEvent(juce::MidiMessage::noteOn(\n"
              << "            n.channel + 1, n.pitch, "
              << "static_cast<juce::uint8>(n.velocity)), samplePos++);\n"
              << "    }\n";
            break;

        case ExportType::Analyzer:
            o << "    juce::ignoreUnused(midi);\n"
              << "    meter_ = analyzer_.updateMeter(\n"
              << "        buffer.getReadPointer(0), buffer.getNumSamples(),\n"
              << "        buffer.getNumChannels(), getSampleRate());\n";
            break;
    }

    o << "}\n\n";

    // createEditor
    o << "juce::AudioProcessorEditor* " << safe
      << "Processor::createEditor()\n"
      << "{\n"
      << "    return new " << safe << "Editor(*this);\n"
      << "}\n\n";

    // State save/restore
    o << "void " << safe
      << "Processor::getStateInformation(juce::MemoryBlock& destData)\n"
      << "{\n"
      << "    juce::ignoreUnused(destData);\n"
      << "    // TODO: serialize parameters\n"
      << "}\n\n"
      << "void " << safe
      << "Processor::setStateInformation(const void* data, int sizeInBytes)\n"
      << "{\n"
      << "    juce::ignoreUnused(data, sizeInBytes);\n"
      << "    // TODO: deserialize parameters\n"
      << "}\n\n"
      << "juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()\n"
      << "{\n"
      << "    return new " << safe << "Processor();\n"
      << "}\n";

    return o.str();
}

// ── Editor header generation ──────────────────────────────────────────────

std::string PluginExportManager::generateEditorH(const ExportConfig& config)
{
    const auto safe = Impl::sanitize(config.name);

    std::ostringstream o;
    o << "#pragma once\n"
      << "#include \"PluginProcessor.h\"\n"
      << "#include \"RevithionLookAndFeel.h\"\n\n"
      << "class " << safe
      << "Editor : public juce::AudioProcessorEditor {\n"
      << "public:\n"
      << "    explicit " << safe << "Editor(" << safe << "Processor&);\n"
      << "    ~" << safe << "Editor() override;\n\n"
      << "    void paint(juce::Graphics&) override;\n"
      << "    void resized() override;\n\n"
      << "private:\n"
      << "    " << safe << "Processor& processor_;\n"
      << "    revithion::plugins::RevithionLookAndFeel lookAndFeel_;\n\n"
      << "    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR("
      << safe << "Editor)\n"
      << "};\n";

    return o.str();
}

// ── Editor implementation generation ──────────────────────────────────────

std::string PluginExportManager::generateEditorCpp(const ExportConfig& config)
{
    const auto safe = Impl::sanitize(config.name);

    std::ostringstream o;
    o << "#include \"PluginEditor.h\"\n\n"
      << safe << "Editor::" << safe << "Editor(" << safe << "Processor& p)\n"
      << "    : AudioProcessorEditor(p), processor_(p)\n"
      << "{\n"
      << "    setLookAndFeel(&lookAndFeel_);\n"
      << "    setSize(" << config.defaultWidth << ", "
      << config.defaultHeight << ");\n"
      << "}\n\n"
      << safe << "Editor::~" << safe << "Editor()\n"
      << "{\n"
      << "    setLookAndFeel(nullptr);\n"
      << "}\n\n"
      << "void " << safe << "Editor::paint(juce::Graphics& g)\n"
      << "{\n"
      << "    g.fillAll(juce::Colour(revithion::plugins::RevithionLookAndFeel::kBackground));\n"
      << "    g.setColour(juce::Colour(revithion::plugins::RevithionLookAndFeel::kText));\n"
      << "    g.setFont(20.0f);\n"
      << "    g.drawFittedText(\"" << config.name << "\",\n"
      << "        getLocalBounds().removeFromTop(40),\n"
      << "        juce::Justification::centred, 1);\n"
      << "}\n\n"
      << "void " << safe << "Editor::resized()\n"
      << "{\n"
      << "    // Layout child components here\n"
      << "}\n";

    return o.str();
}

// ── Build a plugin ────────────────────────────────────────────────────────

void PluginExportManager::buildPlugin(const ExportConfig& config,
                                       BuildCallback onProgress)
{
    // Validate first
    std::string err;
    if (!validateConfig(config, err)) {
        onProgress({0.0f, "Validation", err, true, false, err, ""});
        return;
    }

    const fs::path outDir = config.outputDir;
    const auto safe = Impl::sanitize(config.name);

    // Write generated source files
    onProgress({0.1f, "Generating", "Writing source files...", false, false, "", ""});
    Impl::writeFile(outDir / "CMakeLists.txt",      generateCMake(config));
    Impl::writeFile(outDir / "PluginProcessor.h",   generateProcessorH(config));
    Impl::writeFile(outDir / "PluginProcessor.cpp", generateProcessorCpp(config));
    Impl::writeFile(outDir / "PluginEditor.h",      generateEditorH(config));
    Impl::writeFile(outDir / "PluginEditor.cpp",    generateEditorCpp(config));

    // Copy LookAndFeel files
    onProgress({0.2f, "Generating", "Copying look-and-feel files...",
                false, false, "", ""});
    Impl::writeFile(outDir / "RevithionLookAndFeel.h",
                    "// Copy RevithionLookAndFeel.h from REVITHION-STUDIO\n"
                    "#include \"../../src/plugins/RevithionLookAndFeel.h\"\n");
    Impl::writeFile(outDir / "RevithionLookAndFeel.cpp",
                    "#include \"../../src/plugins/RevithionLookAndFeel.cpp\"\n");

    // CMake configure
    onProgress({0.3f, "Configuring", "Running cmake configure...",
                false, false, "", ""});
    const fs::path buildDir = outDir / "build";
    fs::create_directories(buildDir);

    std::string output;
    std::string cmakeCmd = "cmake -S " + outDir.string()
                         + " -B " + buildDir.string()
                         + " -DCMAKE_BUILD_TYPE=Release 2>&1";
    int ret = Impl::runCommand(cmakeCmd, output);
    if (ret != 0) {
        onProgress({0.4f, "Configure", output, true, false,
                    "CMake configure failed", ""});
        return;
    }

    // Build
    onProgress({0.5f, "Building", "Compiling plugin...", false, false, "", ""});
    std::string buildCmd = "cmake --build " + buildDir.string()
                         + " --config Release 2>&1";
    ret = Impl::runCommand(buildCmd, output);
    if (ret != 0) {
        onProgress({0.8f, "Build", output, true, false,
                    "Build failed", ""});
        return;
    }

    onProgress({1.0f, "Complete", "Plugin built successfully.",
                true, true, "", outDir.string()});
}

// ── Default presets ───────────────────────────────────────────────────────

std::vector<ExportConfig> PluginExportManager::defaultPresets()
{
    std::vector<ExportConfig> presets;

    // REVITHION Amp Sim
    {
        ExportConfig c;
        c.name = "REVITHION Amp Sim";
        c.version = "1.0.0";
        c.description = "Neural amp modeling and cabinet simulation";
        c.bundleId = "com.spiralcloudomega.revithion.ampsim";
        c.type = ExportType::AmpSim;
        c.formats = {PluginFormat::VST3, PluginFormat::AU, PluginFormat::Standalone};
        c.defaultWidth = 900;
        c.defaultHeight = 500;
        presets.push_back(std::move(c));
    }

    // REVITHION Channel Strip
    {
        ExportConfig c;
        c.name = "REVITHION Channel Strip";
        c.version = "1.0.0";
        c.description = "Full channel strip with gate, compressor, EQ, saturation, limiter";
        c.bundleId = "com.spiralcloudomega.revithion.channelstrip";
        c.type = ExportType::ChannelStrip;
        c.formats = {PluginFormat::VST3, PluginFormat::AU, PluginFormat::AAX};
        c.defaultWidth = 1000;
        c.defaultHeight = 600;
        presets.push_back(std::move(c));
    }

    // REVITHION Mastering
    {
        ExportConfig c;
        c.name = "REVITHION Mastering";
        c.version = "1.0.0";
        c.description = "AI-assisted mastering suite with loudness metering";
        c.bundleId = "com.spiralcloudomega.revithion.mastering";
        c.type = ExportType::MasteringChain;
        c.formats = {PluginFormat::VST3, PluginFormat::AU, PluginFormat::AAX};
        c.defaultWidth = 1100;
        c.defaultHeight = 700;
        presets.push_back(std::move(c));
    }

    // REVITHION ACE-STEP
    {
        ExportConfig c;
        c.name = "REVITHION ACE-STEP";
        c.version = "1.0.0";
        c.description = "AI music generation instrument powered by ACE-STEP";
        c.bundleId = "com.spiralcloudomega.revithion.acestep";
        c.type = ExportType::Instrument;
        c.formats = {PluginFormat::VST3, PluginFormat::AU, PluginFormat::Standalone};
        c.defaultWidth = 900;
        c.defaultHeight = 600;
        presets.push_back(std::move(c));
    }

    // REVITHION MIDI FX
    {
        ExportConfig c;
        c.name = "REVITHION MIDI FX";
        c.version = "1.0.0";
        c.description = "MIDI arpeggiator, chord generator, velocity tools";
        c.bundleId = "com.spiralcloudomega.revithion.midifx";
        c.type = ExportType::MIDIEffect;
        c.formats = {PluginFormat::VST3, PluginFormat::AU};
        c.defaultWidth = 700;
        c.defaultHeight = 400;
        presets.push_back(std::move(c));
    }

    // REVITHION Analyzer
    {
        ExportConfig c;
        c.name = "REVITHION Analyzer";
        c.version = "1.0.0";
        c.description = "Loudness metering and spectral analysis (EBU R128)";
        c.bundleId = "com.spiralcloudomega.revithion.analyzer";
        c.type = ExportType::Analyzer;
        c.formats = {PluginFormat::VST3, PluginFormat::AU, PluginFormat::Standalone};
        c.defaultWidth = 800;
        c.defaultHeight = 500;
        presets.push_back(std::move(c));
    }

    return presets;
}

// ── Validation ────────────────────────────────────────────────────────────

bool PluginExportManager::validateConfig(const ExportConfig& config,
                                          std::string& error)
{
    if (config.name.empty()) {
        error = "Plugin name must not be empty";
        return false;
    }
    if (config.version.empty()) {
        error = "Plugin version must not be empty";
        return false;
    }
    if (config.bundleId.empty()) {
        error = "Bundle ID must not be empty";
        return false;
    }
    if (config.formats.empty()) {
        error = "At least one plugin format must be selected";
        return false;
    }

    // Type-specific validation
    switch (config.type) {
        case ExportType::AmpSim:
            if (!config.ampModelPath.empty()
                && !fs::exists(config.ampModelPath)) {
                error = "Amp model file not found: " + config.ampModelPath;
                return false;
            }
            if (!config.cabIRPath.empty()
                && !fs::exists(config.cabIRPath)) {
                error = "Cabinet IR file not found: " + config.cabIRPath;
                return false;
            }
            break;

        case ExportType::Effect:
            if (!config.faustDSPPath.empty()
                && !fs::exists(config.faustDSPPath)) {
                error = "Faust DSP file not found: " + config.faustDSPPath;
                return false;
            }
            break;

        default:
            break;
    }

    if (config.defaultWidth < 100 || config.defaultHeight < 100) {
        error = "Plugin window size must be at least 100x100";
        return false;
    }

    return true;
}

}  // namespace revithion::plugins
