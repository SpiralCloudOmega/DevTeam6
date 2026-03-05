#include "BoxGraph.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <cstring>

namespace revithion::engine {

// ── UUID ────────────────────────────────────────────────────────
UUID UUID::generate() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<uint64_t> dist;
    
    UUID id;
    id.high = dist(gen);
    id.low = dist(gen);
    // Set version 4 (random) and variant bits
    id.high = (id.high & 0xFFFFFFFFFFFF0FFFULL) | 0x0000000000004000ULL;
    id.low = (id.low & 0x3FFFFFFFFFFFFFFFULL) | 0x8000000000000000ULL;
    return id;
}

UUID UUID::fromString(const std::string& str) {
    UUID id;
    // Parse UUID string format: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    std::string clean;
    for (char c : str)
        if (c != '-') clean += c;
    
    if (clean.size() != 32) return id;
    
    id.high = std::stoull(clean.substr(0, 16), nullptr, 16);
    id.low = std::stoull(clean.substr(16, 16), nullptr, 16);
    return id;
}

std::string UUID::toString() const {
    std::ostringstream ss;
    ss << std::hex << std::setfill('0');
    ss << std::setw(8) << (high >> 32);
    ss << '-' << std::setw(4) << ((high >> 16) & 0xFFFF);
    ss << '-' << std::setw(4) << (high & 0xFFFF);
    ss << '-' << std::setw(4) << (low >> 48);
    ss << '-' << std::setw(12) << (low & 0x0000FFFFFFFFFFFFULL);
    return ss.str();
}

// ── Field ───────────────────────────────────────────────────────
Field::Field(const std::string& key, Type type)
    : key_(key), type_(type) {}

// ── Box ─────────────────────────────────────────────────────────
Box::Box(const std::string& typeName, BoxGraph* graph, UUID id)
    : id_(id.isNull() ? UUID::generate() : id)
    , typeName_(typeName)
    , graph_(graph)
{
}

Field* Box::field(const std::string& key) {
    auto it = fieldMap_.find(key);
    return (it != fieldMap_.end()) ? it->second : nullptr;
}

const Field* Box::field(const std::string& key) const {
    auto it = fieldMap_.find(key);
    return (it != fieldMap_.end()) ? it->second : nullptr;
}

std::vector<uint8_t> Box::serialize() const {
    std::vector<uint8_t> buf;
    // Write type name
    uint32_t nameLen = static_cast<uint32_t>(typeName_.size());
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&nameLen),
               reinterpret_cast<const uint8_t*>(&nameLen) + sizeof(nameLen));
    buf.insert(buf.end(), typeName_.begin(), typeName_.end());
    
    // Write UUID
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&id_.high),
               reinterpret_cast<const uint8_t*>(&id_.high) + sizeof(id_.high));
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&id_.low),
               reinterpret_cast<const uint8_t*>(&id_.low) + sizeof(id_.low));
    
    // Write field count
    uint32_t fieldCount = static_cast<uint32_t>(fields_.size());
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&fieldCount),
               reinterpret_cast<const uint8_t*>(&fieldCount) + sizeof(fieldCount));
    
    // Fields would be serialized here based on type
    // (full implementation in later phases)
    
    return buf;
}

void Box::deserialize(const uint8_t* /*data*/, size_t /*len*/) {
    // Deserialization implementation in later phases
}

// ── BoxGraph ────────────────────────────────────────────────────
BoxGraph::BoxGraph() = default;
BoxGraph::~BoxGraph() = default;

void BoxGraph::beginTransaction() {
    ++transactionDepth_;
}

void BoxGraph::endTransaction() {
    if (transactionDepth_ > 0)
        --transactionDepth_;
}

Box* BoxGraph::addBox(std::unique_ptr<Box> box) {
    auto* ptr = box.get();
    boxes_[ptr->id()] = std::move(box);
    for (auto& obs : boxObservers_)
        obs(ptr, true);
    return ptr;
}

void BoxGraph::removeBox(const UUID& id) {
    auto it = boxes_.find(id);
    if (it != boxes_.end()) {
        auto* ptr = it->second.get();
        for (auto& obs : boxObservers_)
            obs(ptr, false);
        boxes_.erase(it);
    }
}

Box* BoxGraph::findBox(const UUID& id) {
    auto it = boxes_.find(id);
    return (it != boxes_.end()) ? it->second.get() : nullptr;
}

const Box* BoxGraph::findBox(const UUID& id) const {
    auto it = boxes_.find(id);
    return (it != boxes_.end()) ? it->second.get() : nullptr;
}

void BoxGraph::forEachBox(std::function<void(Box*)> fn) {
    for (auto& [id, box] : boxes_)
        fn(box.get());
}

void BoxGraph::forEachBox(std::function<void(const Box*)> fn) const {
    for (const auto& [id, box] : boxes_)
        fn(box.get());
}

std::vector<Box*> BoxGraph::findBoxesByType(const std::string& typeName) {
    std::vector<Box*> result;
    for (auto& [id, box] : boxes_) {
        if (box->typeName() == typeName)
            result.push_back(box.get());
    }
    return result;
}

std::vector<uint8_t> BoxGraph::toBuffer() const {
    std::vector<uint8_t> buf;
    
    // Write box count
    uint32_t count = static_cast<uint32_t>(boxes_.size());
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&count),
               reinterpret_cast<const uint8_t*>(&count) + sizeof(count));
    
    // Write each box
    for (const auto& [id, box] : boxes_) {
        auto boxData = box->serialize();
        uint32_t boxLen = static_cast<uint32_t>(boxData.size());
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&boxLen),
                   reinterpret_cast<const uint8_t*>(&boxLen) + sizeof(boxLen));
        buf.insert(buf.end(), boxData.begin(), boxData.end());
    }
    
    return buf;
}

void BoxGraph::fromBuffer(const uint8_t* /*data*/, size_t /*len*/) {
    // Full deserialization in later phases
}

} // namespace revithion::engine
