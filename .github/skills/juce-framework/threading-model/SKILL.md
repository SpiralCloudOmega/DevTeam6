---
name: JUCE Threading Model
description: Real-time audio threading in JUCE 7/8 — MessageManager, FIFOs, spin locks, thread pools. Critical for REVITHION STUDIO.
---
# JUCE Threading Model — Real-Time Audio Safety
## 1. MessageManager (UI Thread)
All Component painting and timer callbacks run here. Never touch UI from another thread.
```cpp
juce::MessageManager::callAsync([this] { levelMeter.setValue(currentLevel.load()); });
const juce::MessageManagerLock mmLock; // background threads only, NEVER audio thread
if (mmLock.lockWasGained()) progressBar.setValue(p);
```
## 2. Audio Thread — Absolute Rules
**NEVER in `processBlock`:** heap alloc (`new`/`delete`/`push_back`), locks (`std::mutex`/`CriticalSection`), system calls (file I/O, `std::cout`), virtual dispatch to unknown code.
```cpp
void processBlock(juce::AudioBuffer<float>& buf, juce::MidiBuffer&) override {
    float gain = gainParam.load(std::memory_order_relaxed);
    for (int ch = 0; ch < buf.getNumChannels(); ++ch)
        juce::FloatVectorOperations::multiply(buf.getWritePointer(ch), gain, buf.getNumSamples());
    meterFifo.push(buf.getRMSLevel(0, 0, buf.getNumSamples()));
}
```
## 3. AsyncUpdater — Audio-to-UI Signaling
Atomic flag polled by message thread. Zero allocation, zero locks.
```cpp
void processBlock(...) override {
    latestRMS.store(buf.getRMSLevel(0, 0, buf.getNumSamples()));
    triggerAsyncUpdate();
}
void handleAsyncUpdate() override { editor->updateMeter(latestRMS.load()); }
```
## 4. CallbackMessage — One-Shot Work on Message Thread
```cpp
class WaveformReady : public juce::CallbackMessage {
    std::vector<float> data; WaveformDisplay* target;
public:
    WaveformReady(std::vector<float> d, WaveformDisplay* t) : data(std::move(d)), target(t) {}
    void messageCallback() override { if (target) target->setWaveform(std::move(data)); }
}; // Usage: (new WaveformReady(std::move(data), &display))->post(); // NOT from audio thread
```
## 5. juce::Thread — Background Work
```cpp
class AudioFileLoader : public juce::Thread {
public:
    AudioFileLoader() : Thread("FileLoader") {}
    void run() override {
        while (!threadShouldExit()) {
            if (auto job = jobQueue.pop()) {
                auto buf = loadFile(job->file); if (threadShouldExit()) return;
                juce::MessageManager::callAsync([b = std::move(buf)] { /* update UI */ });
            } else wait(50);
        }
    }
};
```
## 6. ThreadPool — Parallel Background Tasks
```cpp
class AnalysisJob : public juce::ThreadPoolJob {
public:
    AnalysisJob(juce::File f) : ThreadPoolJob(f.getFileName()), file(f) {}
    JobStatus runJob() override { /* spectral analysis */ return jobHasFinished; }
    juce::File file;
};
pool.addJob(new AnalysisJob(file), true); // ThreadPool(numCpus) owns the job
```
## 7. WaitableEvent — Thread Signaling
```cpp
juce::WaitableEvent dataReady;
// Producer (audio): queue.push(chunk); dataReady.signal();
// Consumer (background): blocks up to 100ms, then drains
while (!threadShouldExit())
    if (dataReady.wait(100)) while (auto c = queue.pop()) writeToDisk(*c);
```
## 8. SpinLock — Lightweight Lock for Pointer Swaps
Busy-waits (no kernel). Sub-μs only. Prefer over `CriticalSection` — OS mutexes sleep audio thread on contention → guaranteed glitch.
```cpp
juce::SpinLock irLock;
void processBlock(...) override { // TRY-lock — never block unconditionally
    juce::SpinLock::ScopedTryLockType lock(irLock);
    if (lock.isLocked() && currentIR) currentIR->process(buffer);
}
void irLoaded(std::unique_ptr<ConvolutionIR> ir) { // loader thread — brief swap
    juce::SpinLock::ScopedLockType lock(irLock); currentIR = std::move(ir);
}
```
## 9. AbstractFifo — Lock-Free Audio↔UI Data
THE correct pattern for meters, waveforms, spectrum data flowing audio→UI.
```cpp
class MeterFifo {
    juce::AbstractFifo fifo; std::vector<float> buffer;
public:
    MeterFifo(int sz) : fifo(sz), buffer(sz) {} // pre-allocate once, never resize
    void push(float v) { auto s = fifo.write(1); if (s.blockSize1 > 0) buffer[s.startIndex1] = v; }
    float getLatest() {
        float val = 0.0f; auto s = fifo.read(fifo.getNumReady());
        if (s.blockSize1 > 0) val = buffer[s.startIndex1 + s.blockSize1 - 1]; return val;
    }
};
```
## 10. TimeSliceThread — Shared Background I/O
Services multiple `TimeSliceClient` objects round-robin — ideal for disk streaming.
```cpp
struct DiskStreamClient : juce::TimeSliceClient {
    int useTimeSlice() override { return bufferNeedsRefill() ? (fillBufferFromDisk(), 0) : 50; }
};
juce::TimeSliceThread diskThread("DiskIO");
diskThread.startThread(juce::Thread::Priority::high);
diskThread.addTimeSliceClient(&trackA, -1); // -1 = call immediately
```
## Anti-Patterns
| Anti-Pattern | Why It Fails | Fix |
|---|---|---|
| `std::mutex` in `processBlock` | Blocks → glitch | `SpinLock` try-lock / `AbstractFifo` |
| `new`/`push_back` in audio | Unbounded alloc | Pre-allocate, lock-free FIFO |
| `DBG()`/logging in audio | File I/O blocks | `triggerAsyncUpdate()`, log on UI |
| `MessageManagerLock` from audio | Deadlock risk | `AsyncUpdater` / `CallbackMessage` |
| Long `SpinLock` hold | Burns CPU | Keep < 1μs |
## Checklist
- [ ] `processBlock` has zero allocations, zero locks, zero I/O
- [ ] Audio→UI uses `AsyncUpdater` or `AbstractFifo` + timer
- [ ] Background I/O uses `juce::Thread` or `TimeSliceThread`
- [ ] `SpinLock` < 1μs; audio side uses `ScopedTryLockType`
- [ ] `AbstractFifo` buffer pre-allocated, never resized; `threadShouldExit()` checked in loops
- [ ] No `std::cout`/`printf`/logging in real-time path
