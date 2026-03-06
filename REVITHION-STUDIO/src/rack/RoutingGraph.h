#pragma once

#include <algorithm>
#include <functional>
#include <optional>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>

namespace revithion::rack {

// Hash specialization so juce::Uuid can be used as unordered_map key.
struct UuidHash {
    std::size_t operator()(const juce::Uuid& uuid) const noexcept
    {
        const auto* raw = uuid.getRawData();
        std::size_t h = 0;
        for (int i = 0; i < 16; ++i)
            h ^= std::hash<uint8_t>{}(raw[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

struct UuidEqual {
    bool operator()(const juce::Uuid& a, const juce::Uuid& b) const noexcept
    {
        return a == b;
    }
};

template <typename V>
using UuidMap = std::unordered_map<juce::Uuid, V, UuidHash, UuidEqual>;

using UuidSet = std::unordered_set<juce::Uuid, UuidHash, UuidEqual>;

// ---------------------------------------------------------------------------
// GraphNode — a single module slot in the routing DAG
// ---------------------------------------------------------------------------
struct GraphNode {
    juce::Uuid moduleId;
    std::vector<juce::Uuid> inputConnections;   // modules feeding into this node
    std::vector<juce::Uuid> outputConnections;   // modules this node feeds

    int latencySamples    = 0;  // reported latency of this module
    int compensationDelay = 0;  // additional delay for path alignment
    int processingGroup   = 0;  // parallel execution group index

    bool isFeedbackSource = false;  // marks feedback loop break points

    // Transient DFS state — not serialized
    mutable bool visited = false;
    mutable bool onStack = false;
};

// ---------------------------------------------------------------------------
// Connection — a single cable between two module ports
// ---------------------------------------------------------------------------
struct Connection {
    juce::Uuid sourceModule;
    juce::Uuid destModule;
    uint32_t   sourcePort = 0;
    uint32_t   destPort   = 0;
    juce::Uuid cableId;       // reference to the PatchCable
    bool       isFeedback = false;  // true → 1-block delay compensation
};

// ---------------------------------------------------------------------------
// ProcessingOrder — result of topological sort
// ---------------------------------------------------------------------------
struct ProcessingOrder {
    std::vector<std::vector<juce::Uuid>> parallelGroups;  // concurrent execution groups
    std::vector<juce::Uuid>              serialOrder;      // flat linear fallback
    UuidMap<int>                         compensationDelays;

    bool         isValid      = true;
    juce::String errorMessage;
};

// ---------------------------------------------------------------------------
// RoutingGraph — DAG-based modular rack routing engine
// ---------------------------------------------------------------------------
class RoutingGraph {
public:
    // -----------------------------------------------------------------------
    // Listener
    // -----------------------------------------------------------------------
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void graphChanged(RoutingGraph*) = 0;
        virtual void processingOrderChanged(RoutingGraph*) {}
    };

    RoutingGraph()  = default;
    ~RoutingGraph() = default;

    // -- Node management ---------------------------------------------------

    void addNode(const juce::Uuid& moduleId, int latencySamples = 0)
    {
        const juce::ScopedLock sl(graphLock_);
        if (nodes_.count(moduleId))
            return;

        GraphNode node;
        node.moduleId       = moduleId;
        node.latencySamples = latencySamples;
        nodes_[moduleId]    = std::move(node);
        orderDirty_ = true;
        notifyListeners();
    }

    void removeNode(const juce::Uuid& moduleId)
    {
        const juce::ScopedLock sl(graphLock_);
        if (!nodes_.count(moduleId))
            return;

        removeAllConnectionsInternal(moduleId);
        nodes_.erase(moduleId);
        orderDirty_ = true;
        notifyListeners();
    }

    void updateNodeLatency(const juce::Uuid& moduleId, int samples)
    {
        const juce::ScopedLock sl(graphLock_);
        auto it = nodes_.find(moduleId);
        if (it == nodes_.end())
            return;

        it->second.latencySamples = samples;
        orderDirty_ = true;
    }

    bool hasNode(const juce::Uuid& moduleId) const
    {
        const juce::ScopedLock sl(graphLock_);
        return nodes_.count(moduleId) > 0;
    }

    int getNodeCount() const
    {
        const juce::ScopedLock sl(graphLock_);
        return static_cast<int>(nodes_.size());
    }

    // -- Connection management ---------------------------------------------

    bool addConnection(const Connection& conn)
    {
        const juce::ScopedLock sl(graphLock_);
        if (!nodes_.count(conn.sourceModule) || !nodes_.count(conn.destModule))
            return false;

        if (!conn.isFeedback && !canConnectInternal(conn.sourceModule, conn.destModule))
            return false;

        connections_.push_back(conn);

        nodes_[conn.sourceModule].outputConnections.push_back(conn.destModule);
        nodes_[conn.destModule].inputConnections.push_back(conn.sourceModule);

        if (conn.isFeedback)
            nodes_[conn.sourceModule].isFeedbackSource = true;

        orderDirty_ = true;
        notifyListeners();
        return true;
    }

    void removeConnection(const juce::Uuid& cableId)
    {
        const juce::ScopedLock sl(graphLock_);
        auto it = std::find_if(connections_.begin(), connections_.end(),
                               [&](const Connection& c) { return c.cableId == cableId; });
        if (it == connections_.end())
            return;

        removeEdge(it->sourceModule, it->destModule);
        connections_.erase(it);
        orderDirty_ = true;
        notifyListeners();
    }

    void removeAllConnections(const juce::Uuid& moduleId)
    {
        const juce::ScopedLock sl(graphLock_);
        removeAllConnectionsInternal(moduleId);
        orderDirty_ = true;
        notifyListeners();
    }

    bool canConnect(const juce::Uuid& source, const juce::Uuid& dest) const
    {
        const juce::ScopedLock sl(graphLock_);
        return canConnectInternal(source, dest);
    }

    std::vector<Connection> getConnectionsFrom(const juce::Uuid& moduleId) const
    {
        const juce::ScopedLock sl(graphLock_);
        std::vector<Connection> result;
        for (auto& c : connections_)
            if (c.sourceModule == moduleId)
                result.push_back(c);
        return result;
    }

    std::vector<Connection> getConnectionsTo(const juce::Uuid& moduleId) const
    {
        const juce::ScopedLock sl(graphLock_);
        std::vector<Connection> result;
        for (auto& c : connections_)
            if (c.destModule == moduleId)
                result.push_back(c);
        return result;
    }

    // -- Graph analysis ----------------------------------------------------

    const ProcessingOrder& getProcessingOrder()
    {
        const juce::ScopedLock sl(graphLock_);
        if (orderDirty_)
        {
            recalculateOrder();
            orderDirty_ = false;
        }
        return cachedOrder_;
    }

    bool hasCycle() const
    {
        const juce::ScopedLock sl(graphLock_);
        return detectCycle();
    }

    std::vector<juce::Uuid> getModulesInOrder()
    {
        const juce::ScopedLock sl(graphLock_);
        if (orderDirty_)
        {
            recalculateOrder();
            orderDirty_ = false;
        }
        return cachedOrder_.serialOrder;
    }

    int getMaxParallelism() const
    {
        const juce::ScopedLock sl(graphLock_);
        return static_cast<int>(cachedOrder_.parallelGroups.size());
    }

    int getTotalLatency() const;

    // -- Path queries ------------------------------------------------------

    std::vector<juce::Uuid> findPath(const juce::Uuid& from,
                                     const juce::Uuid& to) const;

    std::vector<std::vector<juce::Uuid>> findAllPaths(const juce::Uuid& from,
                                                      const juce::Uuid& to) const;

    bool isConnected(const juce::Uuid& from, const juce::Uuid& to) const;

    std::vector<juce::Uuid> getUpstream(const juce::Uuid& moduleId) const;
    std::vector<juce::Uuid> getDownstream(const juce::Uuid& moduleId) const;

    // -- Serialization -----------------------------------------------------

    juce::ValueTree toValueTree() const;
    void fromValueTree(const juce::ValueTree& tree);

    void clear()
    {
        const juce::ScopedLock sl(graphLock_);
        nodes_.clear();
        connections_.clear();
        cachedOrder_ = {};
        orderDirty_  = false;
        notifyListeners();
    }

    // -- Listener management -----------------------------------------------

    void addListener(Listener* l)    { listeners_.add(l); }
    void removeListener(Listener* l) { listeners_.remove(l); }

private:
    // -- Data --------------------------------------------------------------
    UuidMap<GraphNode>     nodes_;
    std::vector<Connection> connections_;
    ProcessingOrder        cachedOrder_;
    bool                   orderDirty_ = false;
    mutable juce::CriticalSection graphLock_;
    juce::ListenerList<Listener>  listeners_;

    // -- Internal helpers --------------------------------------------------

    void removeEdge(const juce::Uuid& src, const juce::Uuid& dst)
    {
        auto eraseFrom = [](std::vector<juce::Uuid>& vec, const juce::Uuid& id) {
            vec.erase(std::remove(vec.begin(), vec.end(), id), vec.end());
        };

        if (auto it = nodes_.find(src); it != nodes_.end())
            eraseFrom(it->second.outputConnections, dst);
        if (auto it = nodes_.find(dst); it != nodes_.end())
            eraseFrom(it->second.inputConnections, src);
    }

    void removeAllConnectionsInternal(const juce::Uuid& moduleId)
    {
        for (auto it = connections_.begin(); it != connections_.end();)
        {
            if (it->sourceModule == moduleId || it->destModule == moduleId)
            {
                removeEdge(it->sourceModule, it->destModule);
                it = connections_.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    // Returns true when adding src→dst would NOT create a cycle
    // (i.e. dest cannot already reach src via forward edges).
    bool canConnectInternal(const juce::Uuid& src, const juce::Uuid& dst) const
    {
        if (src == dst)
            return false;

        // BFS from dst along forward (non-feedback) edges.
        // If we reach src, a cycle would form.
        UuidSet visited;
        std::queue<juce::Uuid> q;
        q.push(dst);
        visited.insert(dst);

        while (!q.empty())
        {
            auto cur = q.front();
            q.pop();

            auto nit = nodes_.find(cur);
            if (nit == nodes_.end())
                continue;

            for (auto& next : nit->second.outputConnections)
            {
                // Skip feedback edges — they have 1-block delay and break cycles.
                bool isFeedbackEdge = false;
                for (auto& c : connections_)
                    if (c.sourceModule == cur && c.destModule == next && c.isFeedback)
                    { isFeedbackEdge = true; break; }

                if (isFeedbackEdge)
                    continue;

                if (next == src)
                    return false;  // would create a cycle

                if (visited.insert(next).second)
                    q.push(next);
            }
        }
        return true;
    }

    // -- Cycle detection (DFS with 3-color marking) ------------------------
    // Returns true if the non-feedback subgraph contains a cycle.
    bool detectCycle() const
    {
        enum class Color { White, Gray, Black };
        UuidMap<Color> color;

        for (auto& [id, _] : nodes_)
            color[id] = Color::White;

        std::function<bool(const juce::Uuid&)> dfs = [&](const juce::Uuid& u) -> bool {
            color[u] = Color::Gray;

            auto nit = nodes_.find(u);
            if (nit != nodes_.end())
            {
                for (auto& v : nit->second.outputConnections)
                {
                    // Skip feedback edges
                    bool feedback = false;
                    for (auto& c : connections_)
                        if (c.sourceModule == u && c.destModule == v && c.isFeedback)
                        { feedback = true; break; }
                    if (feedback)
                        continue;

                    if (color[v] == Color::Gray)
                        return true;   // back edge → cycle
                    if (color[v] == Color::White && dfs(v))
                        return true;
                }
            }

            color[u] = Color::Black;
            return false;
        };

        for (auto& [id, _] : nodes_)
            if (color[id] == Color::White && dfs(id))
                return true;

        return false;
    }

    // -- Topological sort (Kahn's algorithm) -------------------------------
    // Populates cachedOrder_ with serial + parallel group information.
    void recalculateOrder()
    {
        cachedOrder_ = {};

        // Build in-degree map over non-feedback edges only
        UuidMap<int> inDegree;
        for (auto& [id, _] : nodes_)
            inDegree[id] = 0;

        for (auto& c : connections_)
        {
            if (c.isFeedback)
                continue;
            inDegree[c.destModule]++;
        }

        // Seed queue with zero in-degree nodes (sources)
        std::queue<juce::Uuid> ready;
        for (auto& [id, deg] : inDegree)
            if (deg == 0)
                ready.push(id);

        std::vector<juce::Uuid> serial;
        std::vector<std::vector<juce::Uuid>> parallel;

        while (!ready.empty())
        {
            // All nodes currently in `ready` can execute in parallel
            std::vector<juce::Uuid> group;
            auto count = ready.size();

            for (std::size_t i = 0; i < count; ++i)
            {
                auto id = ready.front();
                ready.pop();
                group.push_back(id);
                serial.push_back(id);
            }

            // Decrease in-degree for successors
            for (auto& id : group)
            {
                auto nit = nodes_.find(id);
                if (nit == nodes_.end())
                    continue;

                for (auto& succ : nit->second.outputConnections)
                {
                    // Only count non-feedback edges
                    bool feedback = false;
                    for (auto& c : connections_)
                        if (c.sourceModule == id && c.destModule == succ && c.isFeedback)
                        { feedback = true; break; }
                    if (feedback)
                        continue;

                    if (--inDegree[succ] == 0)
                        ready.push(succ);
                }
            }

            parallel.push_back(std::move(group));
        }

        // Check if all nodes were visited (otherwise an unresolvable cycle exists)
        if (serial.size() != nodes_.size())
        {
            cachedOrder_.isValid      = false;
            cachedOrder_.errorMessage = "Unresolvable cycle detected in routing graph";
            return;
        }

        cachedOrder_.serialOrder   = std::move(serial);
        cachedOrder_.parallelGroups = std::move(parallel);
        cachedOrder_.isValid       = true;

        // Assign group indices back to nodes
        assignParallelGroups();
        calculateCompensationDelays();
        markFeedbackEdges();

        listeners_.call([this](Listener& l) { l.processingOrderChanged(this); });
    }

    void calculateCompensationDelays();
    void assignParallelGroups();
    void markFeedbackEdges();

    void notifyListeners()
    {
        listeners_.call([this](Listener& l) { l.graphChanged(this); });
    }
};

} // namespace revithion::rack
