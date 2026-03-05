#include "RoutingGrid.h"
#include "BlockDefs.h"

#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace revithion::blocks {

// ── Construction ────────────────────────────────────────────────

RoutingGrid::RoutingGrid() {
    // Stamp each cell with its grid coordinates
    for (int r = 0; r < kRows; ++r)
        for (int c = 0; c < kCols; ++c) {
            grid_[r][c].row = r;
            grid_[r][c].col = c;
        }
}

// ── Bounds helper ───────────────────────────────────────────────

bool RoutingGrid::inBounds(int row, int col) const {
    return row >= 0 && row < kRows && col >= 0 && col < kCols;
}

// ── Grid access ─────────────────────────────────────────────────

GridBlock& RoutingGrid::blockAt(int row, int col) {
    if (!inBounds(row, col))
        throw std::out_of_range("RoutingGrid::blockAt — position out of range");
    return grid_[row][col];
}

const GridBlock& RoutingGrid::blockAt(int row, int col) const {
    if (!inBounds(row, col))
        throw std::out_of_range("RoutingGrid::blockAt — position out of range");
    return grid_[row][col];
}

// ── Block placement ─────────────────────────────────────────────

bool RoutingGrid::placeBlock(int row, int col, BlockType type) {
    if (!inBounds(row, col))
        return false;
    auto& blk = grid_[row][col];
    blk.type = type;
    blk.bypassed = false;
    blk.activeChannel = 0;
    blk.scenes = {};
    blk.inputs = {};
    blk.outputs = {};
    initBlockDefaults(blk);
    return true;
}

void RoutingGrid::removeBlock(int row, int col) {
    if (!inBounds(row, col))
        return;

    // Disconnect any ports that reference this block
    auto& blk = grid_[row][col];
    for (uint8_t p = 0; p < 4; ++p) {
        disconnect(row, col, p, true);
        disconnect(row, col, p, false);
    }

    blk.type = BlockType::Empty;
    blk.bypassed = false;
    blk.activeChannel = 0;
    blk.params.clear();
    blk.scenes = {};
    blk.inputs = {};
    blk.outputs = {};
}

void RoutingGrid::moveBlock(int fromRow, int fromCol, int toRow, int toCol) {
    if (!inBounds(fromRow, fromCol) || !inBounds(toRow, toCol))
        return;
    if (fromRow == toRow && fromCol == toCol)
        return;

    auto& src = grid_[fromRow][fromCol];
    auto& dst = grid_[toRow][toCol];

    // Move data, clear source
    dst.type = src.type;
    dst.bypassed = src.bypassed;
    dst.activeChannel = src.activeChannel;
    dst.scenes = src.scenes;
    dst.params = std::move(src.params);
    // Connections are invalidated by the move — clear both ends
    dst.inputs = {};
    dst.outputs = {};

    src.type = BlockType::Empty;
    src.bypassed = false;
    src.activeChannel = 0;
    src.scenes = {};
    src.inputs = {};
    src.outputs = {};
    src.params.clear();
}

// ── Connections ─────────────────────────────────────────────────

bool RoutingGrid::connect(int fromRow, int fromCol, uint8_t fromPort,
                          int toRow, int toCol, uint8_t toPort) {
    if (!inBounds(fromRow, fromCol) || !inBounds(toRow, toCol))
        return false;
    // Enforce left-to-right signal flow
    if (toCol <= fromCol)
        return false;
    if (fromPort >= 4 || toPort >= 4)
        return false;

    auto& src = grid_[fromRow][fromCol];
    auto& dst = grid_[toRow][toCol];
    if (src.isEmpty() || dst.isEmpty())
        return false;

    src.outputs[fromPort] = { true, toRow, toCol, toPort };
    dst.inputs[toPort]    = { true, fromRow, fromCol, fromPort };
    return true;
}

void RoutingGrid::disconnect(int row, int col, uint8_t port, bool isInput) {
    if (!inBounds(row, col) || port >= 4)
        return;

    auto& blk = grid_[row][col];
    auto& p = isInput ? blk.inputs[port] : blk.outputs[port];

    if (!p.connected)
        return;

    // Clear the far end of the connection
    if (inBounds(p.connectedRow, p.connectedCol)) {
        auto& remote = grid_[p.connectedRow][p.connectedCol];
        auto& remotePort = isInput ? remote.outputs[p.connectedPort]
                                   : remote.inputs[p.connectedPort];
        remotePort = {};
    }

    p = {};
}

// ── Scene management ────────────────────────────────────────────

void RoutingGrid::setScene(int sceneIndex) {
    if (sceneIndex < 0 || sceneIndex >= kMaxScenes)
        return;
    activeScene_ = sceneIndex;

    // Apply per-block scene settings
    for (auto& row : grid_)
        for (auto& blk : row) {
            if (blk.isEmpty()) continue;
            const auto& sc = blk.scenes[sceneIndex];
            blk.bypassed = sc.bypassed;
            blk.activeChannel = sc.channel;
        }
}

int RoutingGrid::activeScene() const {
    return activeScene_;
}

// ── Channel switching ───────────────────────────────────────────

void RoutingGrid::setBlockChannel(int row, int col, uint8_t channel) {
    if (!inBounds(row, col) || channel >= 4)
        return;
    grid_[row][col].activeChannel = channel;
    // Also update the current scene's stored channel
    grid_[row][col].scenes[activeScene_].channel = channel;
}

// ── Bypass ──────────────────────────────────────────────────────

void RoutingGrid::toggleBypass(int row, int col) {
    if (!inBounds(row, col))
        return;
    auto& blk = grid_[row][col];
    blk.bypassed = !blk.bypassed;
    blk.scenes[activeScene_].bypassed = blk.bypassed;
}

// ── Processing order ────────────────────────────────────────────

std::vector<GridBlock*> RoutingGrid::getProcessingOrder() {
    std::vector<GridBlock*> order;
    order.reserve(kRows * kCols);

    // Left-to-right, top-to-bottom traversal
    for (int c = 0; c < kCols; ++c)
        for (int r = 0; r < kRows; ++r)
            if (!grid_[r][c].isEmpty())
                order.push_back(&grid_[r][c]);

    return order;
}

// ── Serialization ───────────────────────────────────────────────

// Simple binary format: 4-byte magic, activeScene, then per-cell data.
// Block params are serialized as (key-length, key-chars, float-value).

static constexpr uint32_t kMagic = 0x52475244; // "RGRD"

std::vector<uint8_t> RoutingGrid::serialize() const {
    std::vector<uint8_t> buf;
    auto pushU8  = [&](uint8_t v)  { buf.push_back(v); };
    auto pushU32 = [&](uint32_t v) { buf.insert(buf.end(),
        reinterpret_cast<const uint8_t*>(&v),
        reinterpret_cast<const uint8_t*>(&v) + 4); };
    auto pushF32 = [&](float v)    { buf.insert(buf.end(),
        reinterpret_cast<const uint8_t*>(&v),
        reinterpret_cast<const uint8_t*>(&v) + 4); };

    pushU32(kMagic);
    pushU8(static_cast<uint8_t>(activeScene_));

    for (int r = 0; r < kRows; ++r) {
        for (int c = 0; c < kCols; ++c) {
            const auto& blk = grid_[r][c];
            pushU8(static_cast<uint8_t>(blk.type));
            pushU8(blk.bypassed ? 1 : 0);
            pushU8(blk.activeChannel);

            // Scenes
            for (const auto& sc : blk.scenes) {
                pushU8(sc.bypassed ? 1 : 0);
                pushU8(sc.channel);
                pushF32(sc.level);
            }

            // Ports (inputs then outputs)
            for (const auto& p : blk.inputs) {
                pushU8(p.connected ? 1 : 0);
                pushU8(static_cast<uint8_t>(p.connectedRow & 0xFF));
                pushU8(static_cast<uint8_t>(p.connectedCol & 0xFF));
                pushU8(p.connectedPort);
            }
            for (const auto& p : blk.outputs) {
                pushU8(p.connected ? 1 : 0);
                pushU8(static_cast<uint8_t>(p.connectedRow & 0xFF));
                pushU8(static_cast<uint8_t>(p.connectedCol & 0xFF));
                pushU8(p.connectedPort);
            }

            // Params: count then key-value pairs
            auto paramCount = static_cast<uint32_t>(blk.params.size());
            pushU32(paramCount);
            for (const auto& [key, val] : blk.params) {
                auto keyLen = static_cast<uint8_t>(std::min<size_t>(key.size(), 255));
                pushU8(keyLen);
                buf.insert(buf.end(), key.begin(), key.begin() + keyLen);
                pushF32(val);
            }
        }
    }
    return buf;
}

void RoutingGrid::deserialize(const uint8_t* data, size_t len) {
    if (!data || len < 5)
        return;

    size_t pos = 0;
    auto readU8 = [&]() -> uint8_t {
        if (pos >= len) throw std::runtime_error("deserialize: unexpected end");
        return data[pos++];
    };
    auto readU32 = [&]() -> uint32_t {
        if (pos + 4 > len) throw std::runtime_error("deserialize: unexpected end");
        uint32_t v;
        std::memcpy(&v, data + pos, 4);
        pos += 4;
        return v;
    };
    auto readF32 = [&]() -> float {
        if (pos + 4 > len) throw std::runtime_error("deserialize: unexpected end");
        float v;
        std::memcpy(&v, data + pos, 4);
        pos += 4;
        return v;
    };

    if (readU32() != kMagic)
        throw std::runtime_error("deserialize: bad magic");

    activeScene_ = readU8();

    for (int r = 0; r < kRows; ++r) {
        for (int c = 0; c < kCols; ++c) {
            auto& blk = grid_[r][c];
            blk.type = static_cast<BlockType>(readU8());
            blk.bypassed = readU8() != 0;
            blk.activeChannel = readU8();
            blk.row = r;
            blk.col = c;

            for (auto& sc : blk.scenes) {
                sc.bypassed = readU8() != 0;
                sc.channel = readU8();
                sc.level = readF32();
            }

            for (auto& p : blk.inputs) {
                p.connected = readU8() != 0;
                p.connectedRow = static_cast<int8_t>(readU8());
                p.connectedCol = static_cast<int8_t>(readU8());
                p.connectedPort = readU8();
            }
            for (auto& p : blk.outputs) {
                p.connected = readU8() != 0;
                p.connectedRow = static_cast<int8_t>(readU8());
                p.connectedCol = static_cast<int8_t>(readU8());
                p.connectedPort = readU8();
            }

            blk.params.clear();
            auto paramCount = readU32();
            for (uint32_t i = 0; i < paramCount; ++i) {
                auto keyLen = readU8();
                if (pos + keyLen > len)
                    throw std::runtime_error("deserialize: unexpected end");
                std::string key(reinterpret_cast<const char*>(data + pos), keyLen);
                pos += keyLen;
                float val = readF32();
                blk.params[key] = val;
            }
        }
    }
}

// ── Default preset ──────────────────────────────────────────────

void RoutingGrid::loadDefaultPreset() {
    // Clear entire grid
    for (auto& row : grid_)
        for (auto& blk : row) {
            blk.type = BlockType::Empty;
            blk.bypassed = false;
            blk.activeChannel = 0;
            blk.scenes = {};
            blk.inputs = {};
            blk.outputs = {};
            blk.params.clear();
        }
    activeScene_ = 0;

    // Input(0,0) → Amp(0,4) → Cabinet(0,6) → Output(0,11)
    placeBlock(0, 0,  BlockType::Input);
    placeBlock(0, 4,  BlockType::Amp);
    placeBlock(0, 6,  BlockType::Cabinet);
    placeBlock(0, 11, BlockType::Output);

    connect(0, 0, 0,  0, 4, 0);   // Input → Amp
    connect(0, 4, 0,  0, 6, 0);   // Amp   → Cabinet
    connect(0, 6, 0,  0, 11, 0);  // Cabinet → Output
}

} // namespace revithion::blocks
