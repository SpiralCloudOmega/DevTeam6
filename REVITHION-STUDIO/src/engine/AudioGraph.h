#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace revithion::engine {

// Audio processing block info (matches OpenDAW's Block type)
struct ProcessBlock {
    int index = 0;              // block index within quantum
    int64_t p0 = 0;            // start position (PPQN)
    int64_t p1 = 0;            // end position (PPQN)
    int s0 = 0;                // start sample index
    int s1 = 0;                // end sample index
    double bpm = 120.0;        // tempo for this block
    uint32_t flags = 0;        // BlockFlag bitmask
    
    static constexpr uint32_t kTransporting  = 1 << 0;
    static constexpr uint32_t kDiscontinuous = 1 << 1;
    static constexpr uint32_t kPlaying       = 1 << 2;
    static constexpr uint32_t kBpmChanged    = 1 << 3;
    
    int numSamples() const { return s1 - s0; }
    bool isPlaying() const { return (flags & kPlaying) != 0; }
};

struct ProcessInfo {
    std::vector<ProcessBlock> blocks;
    double sampleRate = 48000.0;
};

// Base class for all audio processors (effects, instruments, buses)
class Processor {
public:
    virtual ~Processor() = default;
    virtual void reset() = 0;
    virtual void process(const ProcessInfo& info) = 0;
    virtual const std::string& name() const = 0;
    
    int id() const { return id_; }
    void setId(int id) { id_ = id; }
    
private:
    int id_ = -1;
};

// Topological sort for directed acyclic audio graph
template <typename T>
class TopologicalSort {
public:
    void addVertex(T* vertex) {
        if (vertexSet_.insert(vertex).second)
            dirty_ = true;
    }
    
    void removeVertex(T* vertex) {
        if (vertexSet_.erase(vertex) > 0) {
            edges_.erase(vertex);
            for (auto& [from, targets] : edges_)
                targets.erase(vertex);
            dirty_ = true;
        }
    }
    
    void addEdge(T* from, T* to) {
        edges_[from].insert(to);
        dirty_ = true;
    }
    
    void removeEdge(T* from, T* to) {
        auto it = edges_.find(from);
        if (it != edges_.end()) {
            it->second.erase(to);
            dirty_ = true;
        }
    }
    
    // Recompute sort order (only when dirty)
    bool update() {
        if (!dirty_) return true;
        
        sorted_.clear();
        std::unordered_set<T*> visited;
        std::unordered_set<T*> inStack;
        hasLoop_ = false;
        
        for (auto* v : vertexSet_) {
            if (visited.find(v) == visited.end()) {
                if (!dfs(v, visited, inStack)) {
                    hasLoop_ = true;
                    dirty_ = false;
                    return false;
                }
            }
        }
        
        std::reverse(sorted_.begin(), sorted_.end());
        dirty_ = false;
        return true;
    }
    
    const std::vector<T*>& sorted() const { return sorted_; }
    bool hasLoop() const { return hasLoop_; }
    bool isDirty() const { return dirty_; }
    
private:
    bool dfs(T* vertex, std::unordered_set<T*>& visited, std::unordered_set<T*>& inStack) {
        visited.insert(vertex);
        inStack.insert(vertex);
        
        auto it = edges_.find(vertex);
        if (it != edges_.end()) {
            for (auto* neighbor : it->second) {
                if (inStack.find(neighbor) != inStack.end())
                    return false;  // cycle detected
                if (visited.find(neighbor) == visited.end()) {
                    if (!dfs(neighbor, visited, inStack))
                        return false;
                }
            }
        }
        
        inStack.erase(vertex);
        sorted_.push_back(vertex);
        return true;
    }
    
    std::unordered_set<T*> vertexSet_;
    std::unordered_map<T*, std::unordered_set<T*>> edges_;
    std::vector<T*> sorted_;
    bool dirty_ = true;
    bool hasLoop_ = false;
};

// AudioGraph — manages audio processors and their connections
class AudioGraph {
public:
    static constexpr int kRenderQuantum = 128;  // samples per audio callback
    
    void registerProcessor(Processor* proc);
    void unregisterProcessor(Processor* proc);
    void addConnection(Processor* from, Processor* to);
    void removeConnection(Processor* from, Processor* to);
    
    // Process all processors in topological order
    void process(const ProcessInfo& info) {
        if (sort_.isDirty())
            sort_.update();
        
        for (auto* proc : sort_.sorted())
            proc->process(info);
    }
    
    void reset() {
        for (auto* proc : sort_.sorted())
            proc->reset();
    }
    
    bool hasLoop() const { return sort_.hasLoop(); }
    const std::vector<Processor*>& processingOrder() const { return sort_.sorted(); }
    
private:
    TopologicalSort<Processor> sort_;
};

} // namespace revithion::engine
