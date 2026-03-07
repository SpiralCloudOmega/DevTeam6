---
name: daw-automation-system
description: DAW automation engine for REVITHION STUDIO — modes, curves, parameter binding, thinning, and punch logic in C++/JUCE.
---

# DAW Automation System

## Automation Modes
```cpp
enum class AutoMode { Read, Touch, AutoLatch, CrossOver, Write, Trim };
struct ModeState { AutoMode mode; bool touching; float lastVal; };

float evalMode(ModeState& s, float input, float existing, bool active) {
    switch (s.mode) {
    case AutoMode::Read:      return existing;
    case AutoMode::Touch:     return active ? (s.lastVal = input) : existing;
    case AutoMode::AutoLatch: if (active) { s.touching = true; s.lastVal = input; }
                              return s.touching ? s.lastVal : existing;
    case AutoMode::CrossOver: if (active && std::abs(input - existing) > 0.001f) return input;
                              s.touching = false; return existing;
    case AutoMode::Write:     return input;
    case AutoMode::Trim:      return juce::jlimit(0.f, 1.f, existing + (input - 0.5f));
    }
    return existing;
}
```

## Automation Curves and Point Structure
```cpp
enum class CurveType { Linear, Bezier, Step };
struct AutoPoint { double time; float value; CurveType curve; float tension = 0.5f; };

float interpolate(const AutoPoint& a, const AutoPoint& b, double t) {
    float n = juce::jlimit(0.f, 1.f, float((t - a.time) / (b.time - a.time)));
    switch (a.curve) {
    case CurveType::Linear: return a.value + (b.value - a.value) * n;
    case CurveType::Bezier: {
        float p1 = a.value + (b.value - a.value) * a.tension;
        float p2 = b.value - (b.value - a.value) * a.tension;
        float i = 1.f - n;
        return i*i*i*a.value + 3*i*i*n*p1 + 3*i*n*n*p2 + n*n*n*b.value;
    }
    case CurveType::Step: return a.value;
    }
    return a.value;
}
```

## Parameter Targets and Lane Binding
```cpp
struct AutoBinding { juce::String paramID, displayName; juce::NormalisableRange<float> range; };

class AutoLane {
public:
    AutoBinding binding;
    std::vector<AutoPoint> points;

    void attachToAPVTS(juce::AudioProcessorValueTreeState& apvts, double pos) {
        if (auto* p = apvts.getParameter(binding.paramID))
            p->setValue(getValueAt(pos));
    }
    float getValueAt(double t) const {
        if (points.empty()) return binding.range.start;
        if (t <= points.front().time) return points.front().value;
        if (t >= points.back().time) return points.back().value;
        for (size_t i = 0; i + 1 < points.size(); ++i)
            if (t >= points[i].time && t < points[i+1].time)
                return interpolate(points[i], points[i+1], t);
        return points.back().value;
    }
};

struct TrackAutomation {
    juce::String trackID;
    std::vector<AutoLane> lanes; // one lane per automated parameter
};
```

## Thinning (Douglas-Peucker)
```cpp
float perpDist(const AutoPoint& pt, const AutoPoint& a, const AutoPoint& b) {
    double dx = b.time - a.time; float dy = b.value - a.value;
    return std::abs(dy * float(a.time - pt.time) - (a.value - pt.value) * float(dx))
         / std::sqrt(dy * dy + float(dx * dx));
}
void dpReduce(std::vector<AutoPoint>& pts, int s, int e, float tol, std::vector<bool>& keep) {
    if (e - s < 2) return;
    float mx = 0; int mi = s;
    for (int i = s+1; i < e; ++i) { float d = perpDist(pts[i], pts[s], pts[e]); if (d > mx) { mx = d; mi = i; } }
    if (mx > tol) { keep[mi] = true; dpReduce(pts, s, mi, tol, keep); dpReduce(pts, mi, e, tol, keep); }
}
std::vector<AutoPoint> thin(std::vector<AutoPoint> pts, float tol = 0.01f) {
    if (pts.size() < 3) return pts;
    std::vector<bool> k(pts.size(), false); k.front() = k.back() = true;
    dpReduce(pts, 0, int(pts.size()) - 1, tol, k);
    std::vector<AutoPoint> r; for (size_t i = 0; i < pts.size(); ++i) if (k[i]) r.push_back(pts[i]);
    return r;
}
```

## Punch In/Out
```cpp
struct PunchRange { double in = 0, out = 0; bool enabled = false; };

void recordAuto(AutoLane& lane, ModeState& m, double t, float input, const PunchRange& p) {
    if (m.mode == AutoMode::Read) return;
    if (p.enabled && (t < p.in || t > p.out)) return;
    float val = evalMode(m, input, lane.getValueAt(t), true);
    lane.points.push_back({ t, val, CurveType::Linear });
}
```

## Relative vs Absolute
```cpp
float applyAbsolute(float input) { return input; }
float applyTrim(float existing, float delta) { return juce::jlimit(0.f, 1.f, existing + delta); }
```

## Anti-Patterns
- **Writing automation on the audio thread** — use a lock-free FIFO from the GUI thread.
- **Sample-per-sample storage** — use sparse points with interpolation; thin aggressively.
- **Assuming linear interpolation** — always dispatch through the curve interpolator.
- **Hardcoded parameter ranges** — read from `NormalisableRange` on the APVTS parameter.
- **Skipping post-record thinning** — project files grow unbounded without it.

## Checklist
- [ ] Six automation modes implemented and tested
- [ ] Linear, bezier, and step interpolation verified
- [ ] Lanes bound to APVTS parameters via `AutoBinding`
- [ ] Douglas-Peucker thinning with configurable tolerance
- [ ] Punch in/out gating respects transport position
- [ ] Trim mode clamps to 0–1 normalized range
- [ ] Lock-free FIFO for audio-thread safety
