#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace revithion::plugins {

// Plugin format types
enum class PluginFormat : uint8_t {
    VST3,
    AU,         // Audio Units (macOS)
    LV2,        // Linux
    AAX,        // Pro Tools
    Standalone,
    CLAP        // Modern plugin format
};

// Plugin type (what gets exported)
enum class ExportType : uint8_t {
    AmpSim,          // Neural amp + cab simulation
    Effect,          // Single effect (delay, reverb, chorus, etc.)
    ChannelStrip,    // Full channel strip
    MasteringChain,  // Complete mastering suite
    Instrument,      // ACE-STEP based instrument
    MIDIEffect,      // MIDI processor (arp, chord, etc.)
    Analyzer         // Metering/analysis tool
};

// Export configuration
struct ExportConfig {
    std::string name;                               // Plugin name
    std::string version;                            // Plugin version
    std::string manufacturer = "SpiralCloudOmega";
    std::string description;
    std::string bundleId;  // e.g. "com.spiralcloudomega.revithion.ampsim"

    ExportType type;
    std::vector<PluginFormat> formats;  // Which formats to build

    // For AmpSim exports
    std::string ampModelPath;  // Path to RTNeural .json weights
    std::string cabIRPath;     // Path to cabinet IR

    // For Effect exports
    std::string faustDSPPath;  // Path to .dsp file (if Faust-based)

    // Build options
    bool embedModels = true;   // Embed neural models in binary
    bool enableGPU = false;    // CUDA/Metal acceleration
    std::string outputDir = "build/plugins";

    // UI options
    bool customUI = true;  // Use custom REVITHION look-and-feel
    int defaultWidth = 800;
    int defaultHeight = 600;
};

// Build progress
struct BuildProgress {
    float progress = 0.0f;  // 0-1
    std::string stage;       // Current build stage
    std::string message;
    bool complete = false;
    bool success = false;
    std::string error;
    std::string outputPath;  // Path to built plugin
};

using BuildCallback = std::function<void(const BuildProgress&)>;

// Plugin export manager
class PluginExportManager {
public:
    PluginExportManager();
    ~PluginExportManager();

    // Generate CMakeLists.txt for a plugin export
    std::string generateCMake(const ExportConfig& config);

    // Generate plugin processor source code
    std::string generateProcessorH(const ExportConfig& config);
    std::string generateProcessorCpp(const ExportConfig& config);

    // Generate plugin editor (UI) source code
    std::string generateEditorH(const ExportConfig& config);
    std::string generateEditorCpp(const ExportConfig& config);

    // Build a plugin (runs CMake + compile)
    void buildPlugin(const ExportConfig& config, BuildCallback onProgress);

    // Get list of available export presets
    // Presets: "REVITHION Amp Sim", "REVITHION Channel Strip",
    //          "REVITHION Mastering", "REVITHION ACE-STEP", etc.
    static std::vector<ExportConfig> defaultPresets();

    // Validate export config
    bool validateConfig(const ExportConfig& config, std::string& error);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

}  // namespace revithion::plugins
