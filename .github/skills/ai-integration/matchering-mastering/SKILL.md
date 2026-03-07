---
name: matchering-mastering
version: "1.0"
description: Reference-based AI mastering with Matchering 2.0 — spectral matching, loudness normalization, Python/C++ bridge, batch processing for REVITHION STUDIO
tags: [ai, mastering, matchering, spectral, loudness, audio]
category: ai-integration
---

# Reference-Based AI Mastering with Matchering 2.0

Matchering 2.0 is an open-source automated mastering library that analyzes a reference track and applies its spectral and loudness characteristics to a target mix. REVITHION STUDIO integrates Matchering as an offline mastering stage, bridging Python processing with the C++ DAW engine. The workflow: export the mix as a 48kHz WAV, run Matchering with a user-selected reference, and import the mastered result back onto a dedicated master bus track. Batch mode processes entire albums against a single reference for tonal consistency.

## How Matchering Works

The algorithm operates in three stages: (1) **Spectral analysis** — FFT-based comparison of target and reference magnitude spectra, producing a correction EQ curve. (2) **Dynamic matching** — RMS and peak loudness alignment using multi-band compression envelopes. (3) **Limiting** — True-peak limiting to the target ceiling (typically -1 dBTP for streaming). The result preserves the target's stereo image and transient character while adopting the reference's tonal balance and perceived loudness.

## Python Mastering Engine

```python
import matchering as mg
from pathlib import Path

def master_track(target: str, reference: str, output: str,
                 pcm_format: str = "FLOAT", sample_rate: int = 48000) -> Path:
    """Master target audio to match reference characteristics."""
    result_path = Path(output)

    mg.process(
        target=target,
        reference=reference,
        results=[
            mg.pcm24(str(result_path.with_suffix(".wav"))),
        ],
        config=mg.Config(
            internal_sample_rate=sample_rate,
            max_length=3600,           # Up to 1 hour
            fft_size=8192,             # High spectral resolution
            lin_log_oversampling=4,
            peak_compensation_steps=4,
        )
    )
    return result_path.with_suffix(".wav")
```

## Batch Album Mastering

```python
import matchering as mg
from pathlib import Path
from concurrent.futures import ProcessPoolExecutor

def master_album(track_paths: list[str], reference: str, output_dir: str,
                 max_workers: int = 2) -> list[str]:
    """Master multiple tracks against the same reference for album consistency."""
    out = Path(output_dir)
    out.mkdir(parents=True, exist_ok=True)
    results = []

    def process_one(track: str) -> str:
        stem = Path(track).stem
        output = str(out / f"{stem}_mastered.wav")
        mg.process(
            target=track,
            reference=reference,
            results=[mg.pcm24(output)],
            config=mg.Config(internal_sample_rate=48000, fft_size=8192)
        )
        return output

    # Limit parallelism to avoid excessive RAM usage (each instance ~2GB)
    with ProcessPoolExecutor(max_workers=max_workers) as pool:
        for result in pool.map(process_one, track_paths):
            results.append(result)

    return results
```

## C++ Bridge — Subprocess Invocation

REVITHION STUDIO calls the Python mastering engine as a subprocess, monitoring progress via stdout line protocol. This avoids embedding a Python interpreter in the audio process.

```cpp
#include <juce_core/juce_core.h>

class MasteringBridge : public juce::Thread {
public:
    struct Job {
        juce::File target;
        juce::File reference;
        juce::File output;
    };

    using ProgressCallback = std::function<void(float pct, const juce::String& stage)>;

    MasteringBridge() : Thread("Matchering-Bridge") {}

    void startJob(const Job& job, ProgressCallback cb) {
        currentJob = job;
        progressCb = std::move(cb);
        startThread();
    }

    void run() override {
        juce::StringArray args;
        args.add("python3");
        args.add("-m");
        args.add("revithion.mastering");
        args.add("--target");    args.add(currentJob.target.getFullPathName());
        args.add("--reference"); args.add(currentJob.reference.getFullPathName());
        args.add("--output");    args.add(currentJob.output.getFullPathName());
        args.add("--progress");  // Enable line-based progress output

        juce::ChildProcess proc;
        if (!proc.start(args)) {
            if (progressCb) progressCb(-1.0f, "Failed to start Python");
            return;
        }

        while (proc.isRunning() && !threadShouldExit()) {
            auto line = proc.readAllProcessOutput();
            // Protocol: "PROGRESS:<0.0-1.0>:<stage_name>"
            if (line.startsWith("PROGRESS:")) {
                auto parts = juce::StringArray::fromTokens(line, ":", "");
                if (parts.size() >= 3 && progressCb) {
                    progressCb(parts[1].getFloatValue(), parts[2]);
                }
            }
            Thread::sleep(100);
        }

        if (progressCb)
            progressCb(proc.getExitCode() == 0 ? 1.0f : -1.0f,
                        proc.getExitCode() == 0 ? "Complete" : "Error");
    }

private:
    Job currentJob;
    ProgressCallback progressCb;
};
```

## Anti-Patterns

- ❌ Don't master clipped or heavily limited mixes — Matchering needs dynamic headroom to work with
- ❌ Don't use a reference track from a different genre — spectral mismatch produces unnatural results
- ❌ Don't run more than 2 parallel Matchering instances — each consumes ~2GB RAM
- ❌ Don't skip loudness validation after mastering — always verify LUFS and true-peak compliance
- ❌ Don't embed Python in the audio process — use subprocess isolation for stability
- ❌ Don't master at 44.1kHz if the session is 48kHz — match the internal sample rate to avoid extra resampling

## Checklist

- [ ] Matchering 2.0 installed in REVITHION Python environment (`pip install matchering`)
- [ ] Reference track curated at similar genre, tempo, and instrumentation
- [ ] Target mix exported at 48kHz/32-bit float with ≥ 3dB headroom
- [ ] Batch mode tested with ≥ 3 tracks for album consistency verification
- [ ] Subprocess bridge reports progress via line protocol (PROGRESS:0.5:spectral_match)
- [ ] Loudness report generated post-mastering: integrated LUFS, true-peak dBTP, LRA
- [ ] Mastered output A/B compared against reference in the DAW
- [ ] Streaming compliance checked: -14 LUFS ± 2dB, true-peak ≤ -1 dBTP
