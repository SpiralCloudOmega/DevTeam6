#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace revithion::blocks {

// Block types matching AXE-FX III categories (36 total, from Yeks Guide)
enum class BlockType : uint8_t {
    Empty = 0,
    // Signal flow
    Input, Output, Mixer, FeedbackSend, FeedbackReturn, SendReturn, Splitter, Multiplexer,
    // Amp/cab
    Amp, Cabinet, ToneMatch,
    // Drive/dist
    Drive, Enhancer,
    // Modulation
    Chorus, Flanger, Phaser, Rotary, Tremolo, FormantShift,
    // Delay
    Delay, MultitapDelay,
    // Reverb
    Reverb,
    // EQ/dynamics
    GraphicEQ, ParametricEQ, Filter, Gate, Compressor, Multicomp,
    // Pitch/synth
    PitchShift, Harmonizer, Synth, Vocoder, Wah,
    // Utility
    Looper, Metering, VolumePan, Crossover
};

// Scene/Channel: AXE-FX has 8 scenes, each block has 4 channels (A/B/C/D)
struct SceneChannel {
    bool bypassed = false;
    uint8_t channel = 0;   // 0=A, 1=B, 2=C, 3=D
    float level = 1.0f;
};

// A single block in the routing grid
struct GridBlock {
    BlockType type = BlockType::Empty;
    int row = 0;
    int col = 0;
    bool bypassed = false;
    uint8_t activeChannel = 0;  // current channel (A/B/C/D)

    // Scene presets (8 scenes per block)
    std::array<SceneChannel, 8> scenes{};

    // Connection port: links to another block's port
    struct Port {
        bool connected = false;
        int connectedRow = -1;
        int connectedCol = -1;
        uint8_t connectedPort = 0;
    };

    // Up to 4 inputs and 4 outputs
    std::array<Port, 4> inputs{};
    std::array<Port, 4> outputs{};

    // Block-specific parameters (generic key-value store)
    std::unordered_map<std::string, float> params;

    bool isEmpty() const { return type == BlockType::Empty; }
};

// The 12×4 routing grid (AXE-FX III layout)
class RoutingGrid {
public:
    static constexpr int kRows = 4;
    static constexpr int kCols = 12;
    static constexpr int kMaxScenes = 8;

    RoutingGrid();

    // Grid access (bounds-checked)
    GridBlock& blockAt(int row, int col);
    const GridBlock& blockAt(int row, int col) const;

    // Block placement
    bool placeBlock(int row, int col, BlockType type);
    void removeBlock(int row, int col);
    void moveBlock(int fromRow, int fromCol, int toRow, int toCol);

    // Connections (left-to-right signal flow: toCol must be > fromCol)
    bool connect(int fromRow, int fromCol, uint8_t fromPort,
                 int toRow, int toCol, uint8_t toPort);
    void disconnect(int row, int col, uint8_t port, bool isInput);

    // Scene management (0–7)
    void setScene(int sceneIndex);
    int activeScene() const;

    // Channel switching per block (0–3 = A/B/C/D)
    void setBlockChannel(int row, int col, uint8_t channel);

    // Bypass toggle
    void toggleBypass(int row, int col);

    // Processing order: left-to-right, top-to-bottom (skips empty blocks)
    std::vector<GridBlock*> getProcessingOrder();

    // Binary serialization
    std::vector<uint8_t> serialize() const;
    void deserialize(const uint8_t* data, size_t len);

    // Load a basic Input → Amp → Cab → Output default preset
    void loadDefaultPreset();

private:
    bool inBounds(int row, int col) const;

    std::array<std::array<GridBlock, kCols>, kRows> grid_;
    int activeScene_ = 0;
};

} // namespace revithion::blocks
