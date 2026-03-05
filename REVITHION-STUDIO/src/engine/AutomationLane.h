#pragma once
#include <string>
#include <vector>

namespace revithion::engine {

// A single automation point
struct AutomationPoint {
    double time = 0.0;      // Position in seconds (or PPQN)
    float value = 0.0f;     // Parameter value at this point

    // Curve type between this point and the next
    enum class Curve : uint8_t { Linear, Bezier, Step, SCurve };
    Curve curve = Curve::Linear;

    // Bezier control points (only used when curve == Bezier)
    float cx1 = 0.0f, cy1 = 0.0f;  // Control point 1
    float cx2 = 0.0f, cy2 = 0.0f;  // Control point 2
};

// An automation lane for a single parameter
class AutomationLane {
public:
    AutomationLane(const std::string& paramId, float minVal = 0.0f, float maxVal = 1.0f);

    // Parameter identity
    const std::string& paramId() const { return paramId_; }
    float minValue() const { return minVal_; }
    float maxValue() const { return maxVal_; }

    // Points
    void addPoint(const AutomationPoint& point);
    void removePoint(int index);
    void clearPoints();
    int pointCount() const { return static_cast<int>(points_.size()); }
    const AutomationPoint& point(int index) const { return points_[index]; }
    AutomationPoint& point(int index) { return points_[index]; }

    // Get interpolated value at a given time
    float valueAt(double time) const;

    // Visible/active
    bool visible() const { return visible_; }
    void setVisible(bool v) { visible_ = v; }

private:
    std::string paramId_;
    float minVal_, maxVal_;
    std::vector<AutomationPoint> points_;
    bool visible_ = true;

    static float lerp(float a, float b, float t) { return a + (b - a) * t; }
};

} // namespace revithion::engine
