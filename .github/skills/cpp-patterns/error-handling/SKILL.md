---
name: "Error Handling in Audio C++"
description: "Real-time-safe error handling for REVITHION STUDIO — no exceptions on the audio thread, monadic error types, lock-free logging, crash-safe persistence."
---
# Error Handling in Audio C++
## No Exceptions on Audio Thread
Real-time audio callbacks have hard deadlines (~5.8 ms at 44.1 kHz / 256 samples). Exception unwinding is non-deterministic and heap-allocates. Compile DSP modules accordingly:
```cmake
target_compile_options(revithion_dsp PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-fno-exceptions -fno-rtti>)
```
## Error Codes for Real-Time Paths
Return scoped enumerations from every function reachable by `processBlock()`:
```cpp
enum class [[nodiscard]] AudioError : uint8_t {
    Ok = 0, BufferOverrun, InvalidChannelCount, ParameterOutOfRange
};
AudioError processDSP(float* buf, int frames, int ch) noexcept {
    if (!buf)               return AudioError::BufferOverrun;
    if (ch < 1 || ch > 64)  return AudioError::InvalidChannelCount;
    for (int i = 0; i < frames * ch; ++i) buf[i] *= gain;
    return AudioError::Ok;
}
```
## std::expected (C++23) for Non-Real-Time Paths
Use `std::expected` or `tl::expected` on UI/file threads for monadic error chaining:
```cpp
std::expected<PresetData, LoadError> loadPreset(const fs::path& p) {
    auto bytes = readFile(p);
    if (!bytes) return std::unexpected(LoadError{"Not found: " + p.string(), 404});
    return parsePresetBytes(*bytes);
}
auto result = loadPreset(path).and_then(validateVersion).transform(applyDefaults);
```
## JUCE Result Class
Carry error messages through file I/O and preset management with `juce::Result`:
```cpp
juce::Result saveProject(const juce::File& file, const ProjectState& state) {
    auto tmp = file.getSiblingFile(file.getFileName() + ".tmp");
    auto s = tmp.createOutputStream();
    if (!s) return juce::Result::fail("Cannot write " + file.getFullPathName());
    if (!state.serialiseTo(*s)) return juce::Result::fail("Serialisation failed");
    s.reset();
    return tmp.moveFileTo(file) ? juce::Result::ok() : juce::Result::fail("Rename failed");
}
```
## Assertion Macros
`jassert()` and `DBG()` for debug builds; custom macro routes failures to the lock-free logger:
```cpp
#define REVITHION_ASSERT(cond) do {                               \
    if (!(cond)) {                                                \
        LockFreeLogger::push("ASSERT FAIL: " #cond               \
            " @ " __FILE__ ":" JUCE_STRINGIFY(__LINE__));         \
        jassertfalse;                                             \
    } } while (0)
```
Never use `assert()` for runtime errors — it compiles away in release builds.
## Graceful Degradation
Bypass a failed block instead of crashing; check an error flag each audio callback:
```cpp
void EffectChain::processBlock(AudioBuffer<float>& buf, MidiBuffer& m) noexcept {
    for (auto& blk : blocks) {
        if (blk.hasError()) { blk.logErrorOnce(logger); continue; }
        if (auto e = blk.process(buf, m); e != AudioError::Ok) {
            blk.setError(e);           // flagged for next callback
            blk.softBypass(buf);       // crossfade to dry over ~10 ms
        }
    } // Fallback: bypass block → bypass chain → output silence. Sanitize NaN/inf last.
}
```
## Logging from the Audio Thread
Never call `printf` or any allocating/locking function from the audio thread. Use a lock-free ring buffer:
```cpp
class LockFreeLogger {
    struct Entry { char text[256]; };
    juce::AbstractFifo fifo{512};
    std::array<Entry, 512> buf;
public:
    void push(const char* msg) noexcept { // audio thread — wait-free
        if (auto s = fifo.write(1); s.blockSize1 > 0)
            std::strncpy(buf[s.startIndex1].text, msg, 255);
    }
    void flush() { // timer thread — drains to JUCE Logger
        auto s = fifo.read(fifo.getNumReady());
        for (int i = 0; i < s.blockSize1; ++i)
            juce::Logger::writeToLog(buf[s.startIndex1 + i].text);
    }
};
```
## Crash-Safe Preset Saving
Write to temp file then atomic-rename; keep N snapshots in a circular buffer for undo/redo:
```cpp
bool atomicSave(const juce::File& target, const juce::MemoryBlock& data) {
    auto tmp = target.getSiblingFile(target.getFileName() + ".tmp");
    return tmp.replaceWithData(data.getData(), data.getSize()) && tmp.moveFileTo(target);
}
```
## Anti-Patterns
| Anti-Pattern | Risk |
|---|---|
| `try/catch` in `processBlock()` | Breaks real-time guarantees |
| Throwing from DSP helpers | Undefined with `-fno-exceptions` |
| Ignoring `AudioError` returns | NaN/inf reaches audio output |
| `assert()` guarding runtime errors | Disappears in release builds |
| Blocking I/O on audio thread | Glitches in the hundreds of ms |
## Checklist
- [ ] DSP modules compiled with `-fno-exceptions -fno-rtti`
- [ ] Every `processBlock()`-reachable function is `noexcept`
- [ ] All `AudioError` return values checked at call sites
- [ ] Graceful degradation tested per effect block
- [ ] NaN/inf sanitisation at end of audio callback
- [ ] Lock-free logger used for audio-thread diagnostics
- [ ] Preset saving uses atomic temp-file-then-rename
- [ ] Undo/redo snapshots survive unexpected termination
