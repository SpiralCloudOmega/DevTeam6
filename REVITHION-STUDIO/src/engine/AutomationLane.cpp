#include "AutomationLane.h"
#include <algorithm>
#include <cmath>

namespace revithion::engine {

AutomationLane::AutomationLane(const std::string& paramId, float minVal, float maxVal)
    : paramId_(paramId)
    , minVal_(minVal)
    , maxVal_(maxVal)
{
}

void AutomationLane::addPoint(const AutomationPoint& point) {
    // Insert in sorted order by time
    auto it = std::lower_bound(points_.begin(), points_.end(), point,
        [](const AutomationPoint& a, const AutomationPoint& b) {
            return a.time < b.time;
        });
    points_.insert(it, point);
}

void AutomationLane::removePoint(int index) {
    if (index >= 0 && index < static_cast<int>(points_.size())) {
        points_.erase(points_.begin() + index);
    }
}

void AutomationLane::clearPoints() {
    points_.clear();
}

float AutomationLane::valueAt(double time) const {
    if (points_.empty()) return minVal_;

    // Before first point — hold first value
    if (time <= points_.front().time) return points_.front().value;

    // After last point — hold last value
    if (time >= points_.back().time) return points_.back().value;

    // Binary search for the segment containing time
    auto it = std::upper_bound(points_.begin(), points_.end(), time,
        [](double t, const AutomationPoint& p) { return t < p.time; });

    const auto& p1 = *(it - 1);
    const auto& p2 = *it;

    double duration = p2.time - p1.time;
    if (duration <= 0.0) return p1.value;

    float t = static_cast<float>((time - p1.time) / duration);

    switch (p1.curve) {
        case AutomationPoint::Curve::Linear:
            return lerp(p1.value, p2.value, t);

        case AutomationPoint::Curve::Step:
            return p1.value;

        case AutomationPoint::Curve::SCurve: {
            // Hermite smoothstep: 3t² - 2t³
            float s = t * t * (3.0f - 2.0f * t);
            return lerp(p1.value, p2.value, s);
        }

        case AutomationPoint::Curve::Bezier: {
            // Cubic bezier evaluation on the value axis
            // P0 = p1.value, P1 = cy1, P2 = cy2, P3 = p2.value
            float u = 1.0f - t;
            float tt = t * t;
            float uu = u * u;
            float uuu = uu * u;
            float ttt = tt * t;
            return uuu * p1.value
                 + 3.0f * uu * t * p1.cy1
                 + 3.0f * u * tt * p1.cy2
                 + ttt * p2.value;
        }
    }

    return p1.value;
}

} // namespace revithion::engine
