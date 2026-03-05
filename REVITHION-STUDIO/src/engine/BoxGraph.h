#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace revithion::engine {

// UUID type (128-bit)
struct UUID {
    uint64_t high = 0;
    uint64_t low = 0;
    
    bool operator==(const UUID& other) const { return high == other.high && low == other.low; }
    bool operator!=(const UUID& other) const { return !(*this == other); }
    bool isNull() const { return high == 0 && low == 0; }
    
    static UUID generate();
    static UUID fromString(const std::string& str);
    std::string toString() const;
};

struct UUIDHash {
    size_t operator()(const UUID& id) const {
        return std::hash<uint64_t>{}(id.high) ^ (std::hash<uint64_t>{}(id.low) << 1);
    }
};

// Field value types
using FieldValue = std::variant<
    bool,
    int32_t,
    int64_t,
    float,
    double,
    std::string,
    std::vector<uint8_t>  // raw bytes
>;

// Forward declarations
class Box;
class BoxGraph;

// Field — Component in ECS terms
// Every parameter, connection, and data point is a Field
class Field {
public:
    enum class Type {
        Primitive,
        Pointer,
        Array
    };
    
    Field(const std::string& key, Type type);
    virtual ~Field() = default;
    
    const std::string& key() const { return key_; }
    Type type() const { return type_; }
    Box* owner() const { return owner_; }
    
protected:
    friend class Box;
    std::string key_;
    Type type_;
    Box* owner_ = nullptr;
};

// PrimitiveField<T> — Observable value field
template <typename T>
class PrimitiveField : public Field {
public:
    using Observer = std::function<void(const T& oldVal, const T& newVal)>;
    
    PrimitiveField(const std::string& key, T defaultValue = T{})
        : Field(key, Type::Primitive), value_(defaultValue), default_(defaultValue) {}
    
    const T& get() const { return value_; }
    
    void set(const T& newValue) {
        if (value_ != newValue) {
            T oldValue = value_;
            value_ = newValue;
            for (auto& obs : observers_)
                obs(oldValue, newValue);
        }
    }
    
    const T& defaultValue() const { return default_; }
    
    void addObserver(Observer obs) { observers_.push_back(std::move(obs)); }
    
private:
    T value_;
    T default_;
    std::vector<Observer> observers_;
};

// PointerField — Reference to another Box
class PointerField : public Field {
public:
    PointerField(const std::string& key)
        : Field(key, Type::Pointer) {}
    
    UUID target() const { return target_; }
    void setTarget(const UUID& id) { target_ = id; }
    bool hasTarget() const { return !target_.isNull(); }
    
private:
    UUID target_;
};

// ArrayField — Ordered collection of child values
class ArrayField : public Field {
public:
    ArrayField(const std::string& key)
        : Field(key, Type::Array) {}
    
    size_t size() const { return items_.size(); }
    const FieldValue& at(size_t index) const { return items_.at(index); }
    void push(const FieldValue& val) { items_.push_back(val); }
    void removeAt(size_t index) { items_.erase(items_.begin() + static_cast<ptrdiff_t>(index)); }
    void clear() { items_.clear(); }
    
private:
    std::vector<FieldValue> items_;
};

// Box — Entity in ECS terms
// Each Box has a type name, UUID, and a set of Fields (components)
class Box {
public:
    Box(const std::string& typeName, BoxGraph* graph, UUID id = UUID{});
    virtual ~Box() = default;
    
    const UUID& id() const { return id_; }
    const std::string& typeName() const { return typeName_; }
    BoxGraph* graph() const { return graph_; }
    
    // Field access
    Field* field(const std::string& key);
    const Field* field(const std::string& key) const;
    
    template <typename T>
    PrimitiveField<T>* primitiveField(const std::string& key) {
        return dynamic_cast<PrimitiveField<T>*>(field(key));
    }
    
    PointerField* pointerField(const std::string& key) {
        return dynamic_cast<PointerField*>(field(key));
    }
    
    ArrayField* arrayField(const std::string& key) {
        return dynamic_cast<ArrayField*>(field(key));
    }
    
    const std::vector<std::unique_ptr<Field>>& fields() const { return fields_; }
    
    // Serialization
    std::vector<uint8_t> serialize() const;
    void deserialize(const uint8_t* data, size_t len);
    
protected:
    // Called by derived classes to register fields
    template <typename FieldT, typename... Args>
    FieldT* addField(Args&&... args) {
        auto f = std::make_unique<FieldT>(std::forward<Args>(args)...);
        f->owner_ = this;
        auto* ptr = f.get();
        fieldMap_[f->key()] = ptr;
        fields_.push_back(std::move(f));
        return ptr;
    }
    
private:
    UUID id_;
    std::string typeName_;
    BoxGraph* graph_;
    std::vector<std::unique_ptr<Field>> fields_;
    std::unordered_map<std::string, Field*> fieldMap_;
};

// BoxGraph — World in ECS terms
// Manages all Boxes, handles transactions, dispatches updates
class BoxGraph {
public:
    using BoxObserver = std::function<void(Box* box, bool added)>;
    
    BoxGraph();
    ~BoxGraph();
    
    // Transaction system
    void beginTransaction();
    void endTransaction();
    bool inTransaction() const { return transactionDepth_ > 0; }
    
    // Box management
    template <typename BoxT, typename... Args>
    BoxT* createBox(Args&&... args) {
        auto box = std::make_unique<BoxT>(this, std::forward<Args>(args)...);
        auto* ptr = box.get();
        boxes_[ptr->id()] = std::move(box);
        for (auto& obs : boxObservers_)
            obs(ptr, true);
        return ptr;
    }
    
    Box* addBox(std::unique_ptr<Box> box);
    void removeBox(const UUID& id);
    
    Box* findBox(const UUID& id);
    const Box* findBox(const UUID& id) const;
    
    template <typename BoxT>
    BoxT* findBoxAs(const UUID& id) {
        return dynamic_cast<BoxT*>(findBox(id));
    }
    
    // Iterate all boxes
    size_t boxCount() const { return boxes_.size(); }
    void forEachBox(std::function<void(Box*)> fn);
    void forEachBox(std::function<void(const Box*)> fn) const;
    
    // Find boxes by type
    std::vector<Box*> findBoxesByType(const std::string& typeName);
    
    // Serialization
    std::vector<uint8_t> toBuffer() const;
    void fromBuffer(const uint8_t* data, size_t len);
    
    // Observers
    void addBoxObserver(BoxObserver obs) { boxObservers_.push_back(std::move(obs)); }
    
private:
    std::unordered_map<UUID, std::unique_ptr<Box>, UUIDHash> boxes_;
    std::vector<BoxObserver> boxObservers_;
    int transactionDepth_ = 0;
};

} // namespace revithion::engine
