---
name: whisper-transcription
version: "1.0"
description: Faster-Whisper speech-to-text for lyrics and voice notes — CTranslate2, VAD, word-level timestamps, real-time streaming for REVITHION STUDIO
tags: [ai, transcription, whisper, ctranslate2, vad, speech-to-text]
category: ai-integration
---

# Faster-Whisper Speech-to-Text for Lyrics & Voice Notes

REVITHION STUDIO integrates Faster-Whisper (OpenAI Whisper re-implemented on CTranslate2) for automatic transcription of vocal tracks, voice memos, and spoken annotations. Use cases include lyric extraction from recorded vocals, transcribing producer voice notes into text markers on the timeline, and generating subtitle tracks for video projects. The CTranslate2 backend provides 4× faster inference than vanilla Whisper with INT8 quantization on CUDA, while Silero VAD pre-filters silence to eliminate hallucinated text in quiet sections.

## Architecture Overview

The transcription pipeline runs as a Python service callable from the C++ DAW via HTTP or subprocess. Input audio is resampled to 16kHz mono (Whisper's native format), passed through VAD to identify speech segments, then each segment is transcribed with word-level timestamps. Results are returned as timed text events that map directly to DAW timeline markers or subtitle tracks.

## Python Transcription Engine

```python
from faster_whisper import WhisperModel
from dataclasses import dataclass

@dataclass
class TranscriptWord:
    start: float
    end: float
    text: str
    confidence: float

@dataclass
class TranscriptSegment:
    start: float
    end: float
    text: str
    words: list[TranscriptWord]

class WhisperEngine:
    def __init__(self, model_size: str = "large-v3",
                 device: str = "cuda", compute_type: str = "int8_float16"):
        self.model = WhisperModel(model_size, device=device, compute_type=compute_type)

    def transcribe(self, audio_path: str,
                   language: str | None = None,
                   vad_filter: bool = True) -> list[TranscriptSegment]:
        segments, info = self.model.transcribe(
            audio_path,
            language=language,
            beam_size=5,
            word_timestamps=True,
            vad_filter=vad_filter,
            vad_parameters=dict(
                min_silence_duration_ms=500,
                speech_pad_ms=200,
                threshold=0.5
            )
        )

        results = []
        for seg in segments:
            words = [
                TranscriptWord(w.start, w.end, w.word.strip(), w.probability)
                for w in (seg.words or [])
            ]
            results.append(TranscriptSegment(seg.start, seg.end, seg.text.strip(), words))
        return results
```

## Real-Time Streaming Transcription

For live voice notes during a session, process audio in sliding windows with partial result updates.

```python
import numpy as np
from collections import deque

class StreamingTranscriber:
    def __init__(self, engine: WhisperEngine, chunk_sec: float = 5.0,
                 overlap_sec: float = 1.0, sample_rate: int = 16000):
        self.engine = engine
        self.chunk_samples = int(chunk_sec * sample_rate)
        self.overlap_samples = int(overlap_sec * sample_rate)
        self.buffer = deque(maxlen=self.chunk_samples)
        self.sample_rate = sample_rate
        self.offset = 0.0

    def feed(self, audio_chunk: np.ndarray) -> list[TranscriptSegment] | None:
        """Feed 16kHz mono float32 audio. Returns segments when buffer is full."""
        self.buffer.extend(audio_chunk.tolist())

        if len(self.buffer) < self.chunk_samples:
            return None

        audio = np.array(self.buffer, dtype=np.float32)
        # Write temp file for faster-whisper (it expects file paths)
        import tempfile, soundfile as sf
        with tempfile.NamedTemporaryFile(suffix=".wav", delete=False) as f:
            sf.write(f.name, audio, self.sample_rate)
            segments = self.engine.transcribe(f.name, vad_filter=True)

        # Offset timestamps to absolute session time
        for seg in segments:
            seg.start += self.offset
            seg.end += self.offset
            for w in seg.words:
                w.start += self.offset
                w.end += self.offset

        advance = self.chunk_samples - self.overlap_samples
        self.offset += advance / self.sample_rate
        # Keep only overlap portion
        keep = list(self.buffer)[-self.overlap_samples:]
        self.buffer.clear()
        self.buffer.extend(keep)

        return segments
```

## C++ Integration — HTTP Client to Whisper Service

```cpp
#include <juce_core/juce_core.h>

struct TimedText {
    double startSec;
    double endSec;
    juce::String text;
    float confidence;
};

class WhisperClient {
    juce::URL serviceUrl { "http://localhost:8003" };

public:
    std::vector<TimedText> transcribeFile(const juce::File& audioFile) {
        auto url = serviceUrl.getChildURL("/transcribe")
            .withParameter("path", audioFile.getFullPathName())
            .withParameter("word_timestamps", "true")
            .withParameter("vad", "true");

        auto response = url.readEntireTextStream();
        return parseResponse(response);
    }

    void transcribeToMarkers(const juce::File& audioFile, double timelineOffset,
                              std::function<void(TimedText)> onMarker) {
        auto results = transcribeFile(audioFile);
        for (auto& t : results) {
            t.startSec += timelineOffset;
            t.endSec += timelineOffset;
            onMarker(t);
        }
    }

private:
    std::vector<TimedText> parseResponse(const juce::String& json) {
        std::vector<TimedText> result;
        auto parsed = juce::JSON::parse(json);

        if (auto* arr = parsed.getArray()) {
            for (auto& item : *arr) {
                result.push_back({
                    static_cast<double>(item["start"]),
                    static_cast<double>(item["end"]),
                    item["text"].toString(),
                    static_cast<float>(item["confidence"])
                });
            }
        }
        return result;
    }
};
```

## Anti-Patterns

- ❌ Don't feed 48kHz stereo directly to Whisper — always resample to 16kHz mono first
- ❌ Don't skip VAD filtering — Whisper hallucinates text during silence and music-only sections
- ❌ Don't transcribe full mixes — isolate vocals first with Demucs, then transcribe the vocal stem
- ❌ Don't run the Flask service on the same GPU as real-time audio inference without CUDA stream isolation

## Checklist

- [ ] Faster-Whisper installed with CTranslate2 CUDA backend (`pip install faster-whisper`)
- [ ] Model downloaded and cached: `large-v3` for quality, `medium` for speed
- [ ] Input audio resampled to 16kHz mono before transcription
- [ ] Silero VAD enabled with min_silence_duration_ms=500 and threshold=0.5
- [ ] Word-level timestamps validated against manual annotation (±50ms tolerance)
- [ ] Streaming mode buffer sized at 5s chunks with 1s overlap
- [ ] Low-confidence words (< 0.7) filtered or flagged in timeline markers
- [ ] Flask service health-checked at DAW startup on port 8003
