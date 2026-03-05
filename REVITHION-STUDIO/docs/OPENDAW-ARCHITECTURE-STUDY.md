# OpenDAW Deep Architecture Study
## Foundation Analysis for REVITHION STUDIO Native Port

**Date:** 2025-07-18
**Codebase:** `~/projects/openDAW` (andremichelle/opendaw)
**Stats:** ~1,751 TypeScript files, ~113,864 lines of code
**License:** AGPL v3 (dual-licensed with commercial option)

---

## Table of Contents

1. [Complete Package/Module Architecture](#1-complete-packagemodule-architecture)
2. [Audio Engine: BoxGraph ECS, AudioWorklet, Processing Pipeline](#2-audio-engine)
3. [Plugin/Device System](#3-plugindevice-system)
4. [State Management & Serialization](#4-state-management--serialization)
5. [Build System](#5-build-system)
6. [Native Port Analysis](#6-native-port-analysis)
7. [Key TypeScript Interfaces](#7-key-typescript-interfaces)

---

## 1. Complete Package/Module Architecture

OpenDAW is a **Turborepo monorepo** with 22 packages organized in three tiers:

### Dependency Graph (Bottom-Up)

```
┌─────────────────────────────────────────────────────────────────────┐
│                         APP LAYER (Vite)                            │
│  ┌─────────────┐  ┌──────────┐  ┌──────────┐                       │
│  │ app-studio  │  │  app-lab │  │ nam-test │                        │
│  └──────┬──────┘  └─────┬────┘  └─────┬────┘                       │
├─────────┼───────────────┼──────────────┼────────────────────────────┤
│         │     STUDIO LAYER (Engine)    │                            │
│  ┌──────┴──────────────────────────────┴──────────────────────┐     │
│  │                    studio-core                             │     │
│  │  (Engine, EngineWorklet, EngineFacade, Project, Storage)   │     │
│  ├────────────┬──────────────────┬───────────────┬────────────┤     │
│  │ core-      │ core-            │ studio-       │ studio-    │     │
│  │ processors │ workers          │ adapters      │ scripting  │     │
│  │ (AudioWork-│ (Web Workers     │ (BoxAdapters, │ (Script    │     │
│  │  let DSP)  │  for OPFS,peaks) │  DeviceAdap.) │  sandbox) │     │
│  ├────────────┴──────────────────┴───────┬───────┴────────────┤     │
│  │ studio-boxes (auto-generated Box      │ studio-enums       │     │
│  │  classes: ~70 box types via forge)    │ (Pointer types)    │     │
│  ├───────────────────────────────────────┼────────────────────┤     │
│  │ forge-boxes (code generator: box      │ studio-sdk         │     │
│  │  schema → TypeScript classes)         │ (npm published)    │     │
├──┴───────────────────────────────────────┴────────────────────┴─────┤
│                         LIB LAYER (Pure Logic)                      │
│  ┌─────────┐ ┌─────────┐ ┌──────────┐ ┌──────────┐ ┌───────┐      │
│  │ lib-std │ │ lib-dsp │ │ lib-box  │ │ lib-     │ │lib-   │      │
│  │ (Option,│ │ (ADSR,  │ │ (BoxGraph│ │ fusion  │ │midi   │      │
│  │  UUID,  │ │  FFT,   │ │  Field,  │ │ (Live   │ │(MIDI  │      │
│  │  arrays,│ │  Biquad,│ │  Pointer,│ │  Stream,│ │ file  │      │
│  │  math,  │ │  Osc,   │ │  Sync,   │ │  OPFS,  │ │ parse)│      │
│  │  etc.)  │ │  Mixer) │ │  Serial.)│ │  Prefs) │ │       │      │
│  ├─────────┤ ├─────────┤ ├──────────┤ ├──────────┤ ├───────┤      │
│  │ lib-dom │ │ lib-jsx │ │lib-xml   │ │ lib-    │ │lib-box│      │
│  │ (Canvas,│ │ (Custom │ │(XML DOM  │ │ runtime │ │-forge │      │
│  │  events,│ │  JSX    │ │ parser)  │ │ (Commu- │ │(code  │      │
│  │  drag)  │ │  engine)│ │          │ │  nicator│ │ gen)  │      │
│  │         │ │         │ │          │ │  Msg)   │ │       │      │
│  └─────────┘ └─────────┘ └──────────┘ └──────────┘ └───────┘      │
├─────────────────────────────────────────────────────────────────────┤
│ SERVER: yjs-server (Y.js WebSocket for real-time collab)            │
├─────────────────────────────────────────────────────────────────────┤
│ CONFIG: eslint-config, typescript-config                            │
└─────────────────────────────────────────────────────────────────────┘
```

### Package Details

| Package | Type | Published | Files | Description |
|---------|------|-----------|-------|-------------|
| `@opendaw/lib-std` | lib | ✅ npm | ~134 | Standard library: `Option<T>`, `UUID`, `Observable`, `SortedSet`, `DataInput`/`DataOutput` binary serialization, terminable lifecycle |
| `@opendaw/lib-dsp` | lib | ✅ npm | ~128 | DSP primitives: `AudioBuffer`, `ADSR`, `BiquadFilter`, `FFT`, `Oscillator`, `LFO`, `Resampler`, `PPQN` timing, `Graph`/`TopologicalSort`, `AudioAnalyser` |
| `@opendaw/lib-box` | lib | ✅ npm | ~46 | **The ECS core**: `Box`, `BoxGraph`, `Field`, `PointerField`, `PrimitiveField`, `ArrayField`, binary serializer, graph edges, transaction system |
| `@opendaw/lib-box-forge` | lib | private | ~7 | Code generator: schema definition → TypeScript Box classes |
| `@opendaw/lib-fusion` | lib | ✅ npm | ~149 | SharedArrayBuffer-based live streaming (`LiveStreamBroadcaster`/`Receiver`), OPFS worker, peak computation, preferences system |
| `@opendaw/lib-dom` | lib | ✅ npm | ~46 | Browser DOM utilities: canvas, drag, keyboard, shortcuts, compression |
| `@opendaw/lib-jsx` | lib | ✅ npm | ~23 | Custom JSX engine (NOT React) with `createElement`/`Fragment` |
| `@opendaw/lib-runtime` | lib | ✅ npm | ~21 | `Communicator` RPC framework, `Messenger` (MessagePort abstraction), promises, fetch |
| `@opendaw/lib-midi` | lib | ✅ npm | ~26 | MIDI file parser/decoder, event types, control types |
| `@opendaw/lib-xml` | lib | ✅ npm | ~6 | XML DOM parser |
| `@opendaw/lib-dawproject` | lib | ✅ npm | ~9 | DAWproject format import/export |
| `@opendaw/studio-boxes` | studio | ✅ npm | **auto-gen** | ~70 Box type classes (auto-generated by forge-boxes) |
| `@opendaw/studio-enums` | studio | ✅ npm | small | Pointer type enum definitions |
| `@opendaw/studio-adapters` | studio | ✅ npm | ~120+ | BoxAdapter pattern: wraps raw Box data with domain logic. One adapter per box type. Factory system for devices. |
| `@opendaw/studio-core` | studio | ✅ npm | ~100+ | **Main engine orchestration**: `Engine`, `EngineWorklet`, `EngineFacade`, `Project`, storage, cloud, capture/recording, FFmpeg, Y.js sync |
| `@opendaw/studio-core-processors` | studio | private | ~90+ | **AudioWorklet processors**: `EngineProcessor`, all device processors, voicing system, mixer |
| `@opendaw/studio-core-workers` | studio | private | ~4 | Web Worker entry points: transient detection, OPFS, peaks |
| `@opendaw/studio-forge-boxes` | studio | private | small | Schema definitions that generate studio-boxes |
| `@opendaw/studio-scripting` | studio | ✅ npm | small | User script sandbox with API declarations |
| `@opendaw/studio-sdk` | studio | ✅ npm | meta | Umbrella package re-exporting all public packages |
| `@opendaw/app-studio` | app | private | large | Main DAW UI application (custom JSX, Sass, Vite) |
| `@opendaw/app-lab` | app | private | small | Experimental testing app |
| `yjs-server` | server | private | small | Y.js WebSocket server for collaboration |

---

## 2. Audio Engine

### 2.1 Architecture Overview: Three-Thread Model

```
┌─────────────────────┐     MessagePort/       ┌─────────────────────────────┐
│    MAIN THREAD      │     SharedArrayBuffer   │    AUDIO WORKLET THREAD     │
│                     │◄───────────────────────►│    (AudioWorkletProcessor)  │
│ EngineFacade        │   Communicator RPC      │                             │
│  └─ EngineWorklet   │   SyncSource/Target     │ EngineProcessor             │
│     (AudioWorklet   │   SyncStream (state)    │  ├─ BoxGraph (mirror copy)  │
│      Node)          │   LiveStream (peaks)    │  ├─ BoxAdapters             │
│                     │                         │  ├─ AudioUnit[] (per track) │
│ Project             │                         │  │  ├─ MidiDeviceChain      │
│  ├─ BoxGraph        │                         │  │  ├─ AudioDeviceChain     │
│  │  (source of      │                         │  │  └─ ChannelStrip         │
│  │   truth)         │                         │  ├─ BlockRenderer           │
│  ├─ SampleManager   │                         │  ├─ Mixer                   │
│  ├─ SoundfontMgr    │                         │  ├─ Metronome               │
│  └─ LiveStreamRcvr  │                         │  ├─ ClipSequencing          │
│                     │                         │  └─ Graph<Processor> +      │
│ UI (custom JSX)     │                         │     TopologicalSort         │
└─────────────────────┘                         └─────────────────────────────┘
         │
         │ Web Worker
         ▼
┌─────────────────────┐
│   WORKER THREAD     │
│ (workers-main.ts)   │
│  ├─ OpfsWorker      │
│  ├─ SamplePeakWorker│
│  └─ TransientDetect │
└─────────────────────┘
```

### 2.2 The BoxGraph ECS System

The **BoxGraph** is the central data model — a custom Entity-Component-System where:

- **Box** = Entity. Each Box has a UUID, a name (type), and a set of Fields.
- **Field** = Component. Types: `PrimitiveField<V>` (float, int, string, bool, bytes), `PointerField` (reference to another Box/Field), `ArrayField`, `ObjectField`.
- **BoxGraph** = World. Manages all Boxes, handles transactions, dispatches updates.

**Key Design Principles:**
1. **Transaction-based mutations**: All changes wrapped in `beginTransaction()`/`endTransaction()`
2. **Binary serialization**: Every Box serializes to `ArrayBuffer` via `DataInput`/`DataOutput` (custom binary protocol, NOT JSON)
3. **Pointer graph**: Boxes reference each other via `PointerField` with pointer types (enum), creating a directed graph with mandatory/exclusive constraints
4. **Code-generated types**: Box classes are auto-generated from schema definitions in `forge-boxes` using `ts-morph`
5. **Observable**: Every PrimitiveField is an `ObservableValue<V>` — changes propagate via subscription

**Box Type Examples (~70 types):**
```
AudioUnitBox, AudioFileBox, AudioRegionBox, AudioClipBox,
VaporisateurDeviceBox, PlayfieldDeviceBox, NanoDeviceBox, TapeDeviceBox,
SoundfontDeviceBox, DelayDeviceBox, ReverbDeviceBox, CrusherDeviceBox,
CompressorDeviceBox, FoldDeviceBox, StereoToolDeviceBox, RevampDeviceBox,
DattorroReverbDeviceBox, TidalDeviceBox, GateDeviceBox, MaximizerDeviceBox,
NeuralAmpDeviceBox, ModularDeviceBox,
ArpeggioDeviceBox, PitchDeviceBox, VelocityDeviceBox, ZeitgeistDeviceBox,
TrackBox, TimelineBox, MarkerBox, NoteRegionBox, NoteEventBox,
ValueRegionBox, ValueEventBox, AuxSendBox, AudioBusBox, ...
```

### 2.3 Engine Processing Pipeline

The `EngineProcessor` (runs in AudioWorklet thread) implements `AudioWorkletProcessor.process()`:

```
process(inputs, outputs) → render(inputs, outputs) → boolean
  │
  ├─ 1. Notify ProcessPhase.Before
  ├─ 2. TopologicalSort audio graph (if dirty)
  ├─ 3. BlockRenderer.process(callback):
  │      │
  │      ├─ Splits RenderQuantum (128 samples) into blocks based on:
  │      │   - Tempo changes (TempoChangeGrid = 1/48 beat)
  │      │   - Loop boundaries
  │      │   - Marker jumps
  │      │   - Recording count-in
  │      │
  │      └─ For each block, calls callback(ProcessInfo{blocks})
  │           │
  │           └─ For each Processor in topological order:
  │                processor.process(processInfo)
  │
  ├─ 4. Copy primary output AudioUnit's buffer to output[]
  ├─ 5. Mix metronome if enabled
  ├─ 6. Process peaks and spectrum analysis
  ├─ 7. Notify ProcessPhase.After
  ├─ 8. Flush clip sequencing notifications
  ├─ 9. Write state to SyncStream (position, bpm, playing, recording)
  └─ 10. Flush LiveStream data (peaks, spectrum, waveform)
```

### 2.4 Block and ProcessInfo Types

```typescript
type Block = Readonly<{
    index: int           // block index within this quantum
    p0: ppqn             // start position in PPQN (pulses per quarter note)
    p1: ppqn             // end position in PPQN
    s0: int              // start sample index within 128-sample quantum
    s1: int              // end sample index
    bpm: number          // tempo for this block
    flags: int           // BlockFlag bitmask: transporting, discontinuous, playing, bpmChanged
}>

// RenderQuantum = 128 samples (Web Audio standard)
// PPQN resolution: configurable, high-precision timing
```

### 2.5 Audio Graph (DSP Routing)

The audio graph uses `Graph<Processor>` from `lib-dsp` with `TopologicalSort`:

- Each `Processor` is a vertex in the audio graph
- Edges represent signal flow (audio or event routing)
- `EngineProcessor.registerProcessor()` / `registerEdge()` build the graph dynamically
- When the graph changes, `TopologicalSort.update()` recomputes processing order
- The sorted processor list is cached until the graph is invalidated

### 2.6 Communication Between Threads

**Communicator RPC** (`lib-runtime`):
- Type-safe RPC over `MessagePort` via `Communicator.sender<T>()` / `Communicator.executor<T>()`
- Supports fire-and-forget (`dispatchAndForget`) and request-response (`dispatchAndReturn`)
- Used for: engine commands, audio data fetching, soundfont loading, MIDI setup

**SyncSource/SyncTarget** (`lib-box`):
- One-way sync: main thread BoxGraph → AudioWorklet BoxGraph
- Sends transaction updates (new box, delete box, primitive change, pointer change)
- Binary-encoded updates for efficiency

**SyncStream** (`lib-std`):
- SharedArrayBuffer-based lock-free state transfer
- Used for engine state → main thread (position, bpm, playing status)
- Schema-based with `EngineStateSchema`

**LiveStream** (`lib-fusion`):
- SharedArrayBuffer-based broadcasting for real-time data
- Peak meters, spectrum analysis, waveform display
- Subscriber-based: only computes if someone is listening

---

## 3. Plugin/Device System

### 3.1 Device Architecture (Three-Layer Pattern)

Every device (instrument/effect) follows a strict three-layer pattern:

```
┌─────────────────────────────────────────────────────────────┐
│ Layer 1: BOX (Data Schema)                                   │
│ e.g., VaporisateurDeviceBox                                  │
│ - Auto-generated from forge-boxes schema                     │
│ - PrimitiveFields for every parameter (cutoff, resonance...) │
│ - PointerFields for connections (automation, samples)        │
│ - Binary serializable                                        │
└──────────────────────┬──────────────────────────────────────┘
                       │
┌──────────────────────┴──────────────────────────────────────┐
│ Layer 2: ADAPTER (Domain Logic)                              │
│ e.g., VaporisateurDeviceBoxAdapter                           │
│ - Wraps Box with typed accessors                             │
│ - Parameter ranges, formatting, constraints                  │
│ - UI-friendly observable properties                          │
│ - Runs in BOTH main thread and audio thread                  │
└──────────────────────┬──────────────────────────────────────┘
                       │
┌──────────────────────┴──────────────────────────────────────┐
│ Layer 3: PROCESSOR (DSP)                                     │
│ e.g., VaporisateurDeviceProcessor                            │
│ - Extends AudioProcessor (instruments) or                    │
│   AudioEffectDeviceProcessor (effects)                       │
│ - Runs ONLY in AudioWorklet thread                           │
│ - Processes audio sample-by-sample within blocks             │
│ - Binds parameters via AutomatableParameter system           │
└─────────────────────────────────────────────────────────────┘
```

### 3.2 Device Categories

**Instruments** (extend `InstrumentDeviceProcessor`):
| Device | Processor | Description |
|--------|-----------|-------------|
| Vaporisateur | `VaporisateurDeviceProcessor` | Subtractive synth: 2 band-limited oscillators, modulated biquad filter, ADSR, LFO, glide, unison |
| Playfield | `PlayfieldDeviceProcessor` | Sample drum machine: per-pad sample loading, individual effect chains |
| Nano | `NanoDeviceProcessor` | Single-sample sampler |
| Tape | `TapeDeviceProcessor` | Audio region playback: pitch, time-stretch, direct, pingpong, repeat modes |
| Soundfont | `SoundfontDeviceProcessor` | SoundFont2 player |
| MIDI Output | `MIDIOutputDeviceProcessor` | Sends MIDI to external devices |
| Audio Bus | `AudioBusProcessor` | Bus/group routing |

**Audio Effects** (extend `AudioEffectDeviceProcessor`):
| Device | Processor | DSP Source |
|--------|-----------|------------|
| Stereo Tool | `StereoToolDeviceProcessor` | Volume, pan, invert (lib-dsp `Mixing`) |
| Delay | `DelayDeviceProcessor` | Stereo delay with cross/filter (lib-dsp `Delay`) |
| Crusher | `CrusherDeviceProcessor` | Bit/sample rate reduction (lib-dsp `Crusher`) |
| Reverb (Cheap) | `ReverbDeviceProcessor` | FreeVerb implementation (core-processors `FreeVerb`) |
| Dattorro Reverb | `DattorroReverbDeviceProcessor` | Algorithmic reverb |
| Revamp (EQ) | `RevampDeviceProcessor` | Graphic EQ + spectrum analyzer |
| Compressor | `CompressorDeviceProcessor` | CTAG Dynamic Range Compressor (lib-dsp/ctagdrc) |
| Gate | `GateDeviceProcessor` | Noise gate |
| Maximizer | `MaximizerDeviceProcessor` | Brick-wall limiter |
| Fold | `FoldDeviceProcessor` | Wavefolding with oversampling |
| Tidal | `TidalDeviceProcessor` | Rhythmic volume/pan shaping |
| Neural Amp | `NeuralAmpDeviceProcessor` | Neural amp modeling (NAM WASM) |

**MIDI Effects** (extend `MidiEffectProcessor`):
| Device | Processor |
|--------|-----------|
| Arpeggio | `ArpeggioDeviceProcessor` |
| Pitch | `PitchDeviceProcessor` |
| Velocity | `VelocityDeviceProcessor` |
| Zeitgeist | `ZeitgeistDeviceProcessor` |

### 3.3 Device Instantiation Flow

```
1. Box created in BoxGraph (via forge-generated factory)
2. BoxAdapters.adapterFor(box, AdapterType) creates domain adapter
3. AudioUnit detects new input/device via subscription
4. DeviceProcessorFactory.create(context, box) → new DeviceProcessor
5. DeviceProcessor binds parameters via bindParameter(adapter.field)
6. Processor registered in audio graph
7. TopologicalSort invalidated → recomputed on next render
```

### 3.4 Voicing System

For instruments, a complete voice management system:

```
Voicing (VoicingStrategy pattern)
  ├─ PolyphonicStrategy (up to N voices)
  ├─ MonophonicStrategy (single voice with glide)
  └─ VoiceUnison (detuned copies per voice)

Voice interface:
  - start(event, frequency, gain, spread)
  - stop()
  - forceStop()
  - startGlide(targetFrequency, duration)
  - process(output, block, fromIndex, toIndex) → boolean (alive?)
```

### 3.5 AutomatableParameter System

Every device parameter that can be automated:

```typescript
// In Processor constructor:
this.#parameterCutoff = this.bindParameter(adapter.cutoff)

// bindParameter() creates AutomatableParameter which:
// 1. Reads current value from Box's PrimitiveField
// 2. Watches for PointerField connections (automation curves)
// 3. On each block, if automated: reads ValueEventCollection → interpolates
// 4. Calls parameterChanged(parameter) on the processor
```

---

## 4. State Management & Serialization

### 4.1 BoxGraph Binary Serialization

The entire project state lives in a single `BoxGraph`. Serialization:

```
BoxGraph.toArrayBuffer():
  ├─ Write: numBoxes (int32)
  └─ For each Box (sorted by creationIndex):
       ├─ Write: byteLength (int32)
       ├─ Write: creationIndex (int32)
       ├─ Write: name (string - box type name)
       ├─ Write: uuid (16 bytes)
       └─ Write: all fields recursively:
            ├─ PrimitiveField: type-specific encoding (float32, int32, string, bool, bytes)
            ├─ PointerField: target address (uuid + field keys) or null
            ├─ ArrayField: length + child fields
            └─ ObjectField: child fields
```

This is a **custom binary format** — NOT JSON, NOT protobuf. The `DataInput`/`DataOutput` classes in `lib-std` implement the encoding.

### 4.2 Project Persistence

```
Project Bundle (.opendaw):
  └─ JSZip archive containing:
       ├─ project.bin (BoxGraph.toArrayBuffer())
       ├─ meta.json (name, date, version)
       └─ samples/ (embedded audio files)
```

### 4.3 Real-Time Collaboration (Y.js)

The `YSync` class bridges BoxGraph ↔ Y.js CRDT:

- `populateRoom()`: BoxGraph → Y.Map (initial sync)
- `joinRoom()`: Y.Map → BoxGraph (join existing session)
- Bidirectional: BoxGraph transactions → Y.js transactions, Y.js remote changes → BoxGraph transactions
- Server: `yjs-server` (standalone WebSocket server using `y-websocket`)

### 4.4 Cross-Thread Sync

```
Main Thread BoxGraph ──SyncSource──► MessagePort ──SyncTarget──► AudioWorklet BoxGraph
                                    (binary updates)

Update types:
  - new: { name, uuid, buffer }
  - delete: { uuid }
  - update-primitive: { address, value }
  - update-pointer: { address, target }
```

---

## 5. Build System

### 5.1 Turborepo Pipeline

```
turbo.json tasks:
  build: dependsOn [^build] → tsc (or esbuild for workers/processors)
  
Special build order:
  1. forge-boxes: tsx forge.ts → generates boxes/src/*.ts
  2. studio-boxes: tsc (compiles generated code)
  3. core-processors: tsc --noEmit + esbuild → processors.js (AudioWorklet bundle)
  4. core-workers: esbuild → workers-main.js + offline-engine.js (Worker bundles)
  5. studio-core: tsc (depends on processors + workers being built)
  6. app-studio: tsc + vite build
```

### 5.2 Key Build Tools

| Tool | Usage |
|------|-------|
| **TypeScript** 5.9+ | All source code, ESNext target, Bundler module resolution |
| **Turborepo** 2.5+ | Monorepo task orchestration with caching |
| **Vite** 7.0+ | App bundling, dev server (HTTPS required for SharedArrayBuffer) |
| **esbuild** | Bundling AudioWorklet processors and Web Workers into single files |
| **ts-morph** | Code generation in forge-boxes |
| **Sass** | Stylesheets |
| **Vitest** | Unit tests |
| **Lerna** | npm package publishing |

### 5.3 Critical Build Constraints

- **HTTPS required**: `SharedArrayBuffer` needs COOP/COEP headers → HTTPS dev server
- **Cross-Origin Isolation**: `Cross-Origin-Opener-Policy: same-origin` + `Cross-Origin-Embedder-Policy: require-corp`
- **AudioWorklet bundle**: `core-processors` compiled to single `processors.js` via esbuild (no imports in AudioWorklet scope)
- **Worker bundle**: `core-workers` compiled to single `workers-main.js` via esbuild
- **No external frameworks**: Custom JSX engine, no React/Vue/Angular

---

## 6. Native Port Analysis

### 6.1 What is Web-Only (Must Be Replaced)

| Component | Web API | Replacement Strategy |
|-----------|---------|---------------------|
| **AudioWorklet** | `AudioWorkletProcessor`, `AudioWorkletNode` | Native audio thread (PortAudio/JACK/CoreAudio/WASAPI via RtAudio or miniaudio) |
| **Web Audio API** | `AudioContext`, `OfflineAudioContext` | Direct audio device access + custom render loop |
| **SharedArrayBuffer** | Used for cross-thread communication | C++ `std::atomic`, shared memory, or lock-free queues |
| **MessagePort/Communicator** | `MessagePort`, `postMessage()` | C++ message queues, ZeroMQ, or direct function calls |
| **Web Workers** | `new Worker()` | `std::thread` or thread pool |
| **Custom JSX UI** | `lib-jsx` → DOM elements | **Full replacement**: Dear ImGui, Qt, JUCE GUI, SDL2+custom, or GTK |
| **Vite/esbuild** | Build tools | CMake + compiler |
| **IndexedDB/OPFS** | Browser storage | File system I/O |
| **FFmpeg WASM** | `@ffmpeg/ffmpeg` wasm build | Native FFmpeg library (libavcodec, libavformat) |
| **Fetch API** | `fetch()` for samples | `libcurl` or direct file I/O |
| **Canvas 2D** | Waveform/spectrum rendering | Custom GPU rendering or Cairo/Skia |
| **Sass/CSS** | Styling | Native UI framework styling |
| **Y.js** | CRDT collaboration | Native Y.js port or alternative CRDT lib |

### 6.2 What is Portable (Can Be Translated to C++)

| Component | Current | Native Equivalent | Effort |
|-----------|---------|-------------------|--------|
| **lib-dsp** (128 files) | Pure TypeScript math/DSP | **Direct C++ port** — all algorithms are platform-independent: ADSR, Biquad, FFT, Oscillators, Delay, Reverb, Convolution, Resampler | MEDIUM — straightforward translation, use NumPy/SciPy for prototyping |
| **lib-box** (46 files) | BoxGraph ECS + serialization | **C++ port with templates** — binary serialization maps cleanly to C++ structs | MEDIUM-HIGH — need to port Observable, transaction system |
| **lib-std** (134 files) | Option, UUID, SortedSet, etc. | C++ STL equivalents: `std::optional`, UUID libs, `std::set` | LOW — mostly use existing C++ idioms |
| **studio-boxes** (auto-gen) | ~70 Box type definitions | Port the forge tool to generate C++ classes, or hand-write | MEDIUM — need code generator for C++ |
| **studio-adapters** (120+ files) | BoxAdapter wrappers | C++ adapter classes | HIGH — large surface area |
| **All DSP processors** (~90 files) | Device processors | **Direct C++ port** — the algorithms themselves are pure math | MEDIUM — largest single effort, but well-structured |
| **Processing model** | Block/ProcessInfo/Processor | C++ audio callback model — nearly identical pattern | LOW — clean mapping |
| **Voicing system** | Polyphonic/Monophonic | Standard C++ voice management | LOW |
| **TopologicalSort/Graph** | Graph traversal | `boost::graph` or custom C++ implementation | LOW |
| **Binary serialization** | DataInput/DataOutput | Custom C++ binary reader/writer | LOW |

### 6.3 Bridge Strategies

#### Option A: Full C++ Rewrite (Recommended for "real native software")

```
┌─────────────────────────────────────────────────┐
│                REVITHION STUDIO                  │
│                                                  │
│  ┌──────────────────────────────────────────┐    │
│  │ C++ Core Engine                          │    │
│  │  ├─ BoxGraph (ported from lib-box)       │    │
│  │  ├─ DSP Library (ported from lib-dsp)    │    │
│  │  ├─ All Processors (ported)              │    │
│  │  ├─ Audio I/O (RtAudio/PortAudio)       │    │
│  │  ├─ MIDI I/O (RtMidi)                   │    │
│  │  └─ File I/O (native filesystem)        │    │
│  └──────────────┬───────────────────────────┘    │
│                 │ pybind11 / C API               │
│  ┌──────────────┴───────────────────────────┐    │
│  │ Python Layer (optional)                  │    │
│  │  ├─ NumPy-based DSP prototyping          │    │
│  │  ├─ Scripting API                        │    │
│  │  └─ Plugin development                   │    │
│  └──────────────────────────────────────────┘    │
│                                                  │
│  ┌──────────────────────────────────────────┐    │
│  │ GUI (Dear ImGui / Qt / JUCE)             │    │
│  │  ├─ Timeline view                        │    │
│  │  ├─ Mixer                                │    │
│  │  ├─ Device UIs                           │    │
│  │  └─ Waveform rendering                   │    │
│  └──────────────────────────────────────────┘    │
│                                                  │
│  Build: CMake → EXE (Windows) / .app (macOS)     │
└─────────────────────────────────────────────────┘
```

**Pros:** True native performance, no web overhead, distributable as standalone EXE/ISO
**Cons:** Largest effort (~6-12 months for full parity), need to rebuild UI from scratch

#### Option B: Tauri/Electron Wrapper (Fastest Path)

```
┌─────────────────────────────────────────────────┐
│ Tauri (Rust backend) or Electron                 │
│  ├─ Existing web UI (mostly unchanged)           │
│  ├─ AudioWorklet → stays in browser engine       │
│  ├─ File system access via Tauri/Electron APIs   │
│  └─ Package as .exe / .dmg / .AppImage           │
└─────────────────────────────────────────────────┘
```

**Pros:** Working in weeks, UI reuse, minimal code changes
**Cons:** Still browser-based audio, not "real native", large binary size (~100MB+)
**Note:** OpenDAW README explicitly lists this as a community goal: "Offline desktop build (e.g., via Tauri)"

#### Option C: Hybrid — C++ Audio Engine + Web UI (Best of Both)

```
┌─────────────────────────────────────────────────┐
│ Tauri Shell                                      │
│  ├─ Web UI (existing, with modifications)        │
│  ├─ C++ audio engine (native plugin, loaded      │
│  │   via Tauri command / FFI)                    │
│  ├─ Audio I/O: RtAudio (bypasses Web Audio)      │
│  └─ Bridge: TypeScript ↔ C++ via IPC/FFI        │
└─────────────────────────────────────────────────┘
```

**Pros:** Native audio performance, UI reuse, incremental migration
**Cons:** Complex bridging, two languages to maintain

### 6.4 Porting Priority Order

For a **full C++ rewrite** approach:

```
Phase 1: Foundation (Month 1-2)
  ├─ Port lib-std → C++ standard library mappings
  ├─ Port lib-dsp → C++ DSP library (use NumPy for validation)
  ├─ Port lib-box → C++ BoxGraph with binary serialization
  └─ CMake project structure, CI/CD

Phase 2: Audio Engine (Month 2-4)
  ├─ Implement audio callback (RtAudio/PortAudio)
  ├─ Port processing model (Block, ProcessInfo, Processor)
  ├─ Port TopologicalSort for audio graph
  ├─ Port BlockRenderer
  └─ Port 3-4 core processors (StereoTool, Delay, Vaporisateur)

Phase 3: Full Processor Suite (Month 4-6)
  ├─ Port all instrument processors
  ├─ Port all effect processors
  ├─ Port voicing system
  ├─ Port automation system
  └─ Port MIDI I/O (RtMidi)

Phase 4: Project System (Month 5-7)
  ├─ Port project serialization (binary format)
  ├─ Port studio-adapters
  ├─ Port sample loading (libsndfile / dr_wav / minimp3)
  ├─ Port FFmpeg integration
  └─ Port SoundFont loading

Phase 5: GUI (Month 6-12)
  ├─ Choose framework (Dear ImGui / Qt / JUCE)
  ├─ Timeline view
  ├─ Mixer view
  ├─ Device UIs
  └─ Waveform/spectrum rendering

Phase 6: Distribution (Month 10-12)
  ├─ Windows installer (NSIS/WiX → EXE)
  ├─ macOS bundle (.app / .dmg)
  ├─ Linux AppImage
  └─ Optional: ISO for live-boot studio OS
```

### 6.5 C++ Library Recommendations

| Need | Recommended Library |
|------|-------------------|
| Audio I/O | **RtAudio** or **PortAudio** (cross-platform) |
| MIDI I/O | **RtMidi** |
| Audio file loading | **libsndfile**, **dr_wav** (header-only), **minimp3** |
| FFT | **FFTW3** or **KissFFT** (simpler) |
| GUI | **Dear ImGui** (rapid dev) or **JUCE** (industry standard) or **Qt** |
| Build system | **CMake** (with vcpkg or Conan for deps) |
| Python binding | **pybind11** |
| SoundFont | **tinysoundfont** (header-only) |
| Neural Amp | **RTNeural** (real-time neural inference) |
| Serialization | Custom binary (matching OpenDAW format) or **FlatBuffers** |
| UUID | **stduuid** or `boost::uuid` |
| Threading | `std::thread`, `std::atomic`, lock-free queues |
| CRDT collab | **yrs** (Rust Y.js port via FFI) or defer |

---

## 7. Key TypeScript Interfaces

### 7.1 Core Engine Interface

```typescript
interface Engine extends Terminable {
    play(): void
    stop(): void
    setPosition(position: ppqn): void
    prepareRecordingState(countIn: boolean): void
    stopRecording(): void
    isReady(): Promise<void>
    queryLoadingComplete(): Promise<boolean>
    panic(): void
    sleep(): void
    wake(): void
    loadClickSound(index: 0 | 1, data: AudioData): void
    noteSignal(signal: NoteSignal): void
    subscribeNotes(observer: Observer<NoteSignal>): Subscription
    ignoreNoteRegion(uuid: UUID.Bytes): void
    scheduleClipPlay(clipIds: ReadonlyArray<UUID.Bytes>): void
    scheduleClipStop(trackIds: ReadonlyArray<UUID.Bytes>): void
    subscribeClipNotification(observer: Observer<ClipNotification>): Subscription

    get position(): ObservableValue<ppqn>
    get bpm(): ObservableValue<bpm>
    get isPlaying(): ObservableValue<boolean>
    get isRecording(): ObservableValue<boolean>
    get isCountingIn(): ObservableValue<boolean>
    get playbackTimestamp(): ObservableValue<ppqn>
    get countInBeatsRemaining(): ObservableValue<number>
    get markerState(): ObservableValue<Nullable<[UUID.Bytes, int]>>
    get project(): Project
    get preferences(): EnginePreferences
}
```

### 7.2 Processing Model

```typescript
type Block = Readonly<{
    index: int
    p0: ppqn    // start position in pulses
    p1: ppqn    // end position in pulses
    s0: int     // start sample index
    s1: int     // end sample index
    bpm: number
    flags: int  // BlockFlag bitmask
}>

interface ProcessInfo { blocks: ReadonlyArray<Block> }

interface Processor extends EventReceiver {
    reset(): void
    process(processInfo: ProcessInfo): void
}

interface AudioGenerator { get audioOutput(): AudioBuffer }
interface AudioInput { setAudioSource(source: AudioBuffer): Terminable }
interface EventGenerator { setEventTarget(target: EventBuffer): Terminable }
```

### 7.3 Box/ECS System

```typescript
abstract class Box<P extends PointerTypes, F extends Fields> implements Vertex<P, F> {
    get address(): Address          // UUID-based address
    get graph(): BoxGraph           // parent graph
    get name(): string              // type name
    fields(): ReadonlyArray<Field>  // all fields
    read(input: DataInput): void    // deserialize
    write(output: DataOutput): void // serialize
    serialize(): ArrayBufferLike    // full serialization with metadata
    delete(): void                  // cascade delete with dependencies
    incomingEdges(): ReadonlyArray<PointerField>
    outgoingEdges(): ReadonlyArray<[PointerField, Address]>
}

class BoxGraph<BoxMap> {
    beginTransaction(): void
    endTransaction(): void
    stageBox<B extends Box>(box: B, constructor?: Procedure<B>): B
    unstageBox(box: Box): void
    findBox<B extends Box>(uuid: UUID.Bytes): Option<B>
    boxes(): ReadonlyArray<Box>
    toArrayBuffer(): ArrayBufferLike
    fromArrayBuffer(arrayBuffer: ArrayBufferLike): void
    dependenciesOf(box: Box): { boxes: Iterable<Box>, pointers: Iterable<PointerField> }
}
```

### 7.4 Audio Data Types

```typescript
type AudioData = {
    sampleRate: number
    numberOfFrames: number
    numberOfChannels: number
    frames: ReadonlyArray<Float32Array<SharedArrayBuffer>>
}

class AudioBuffer {
    constructor(numberOfChannels: int = 2)  // always RenderQuantum (128) samples
    clear(start?: int, end?: int): void
    getChannel(channelIndex: int): Float32Array
    channels(): ReadonlyArray<Float32Array>
    replace(output: AudioBuffer): void      // copy to output
    mixInto(target: ReadonlyArray<Float32Array>): void  // additive mix
}

// Key DSP types
type ppqn = number   // Pulses Per Quarter Note (time position)
type bpm = number    // Beats Per Minute
const RenderQuantum = 128  // samples per audio callback
```

### 7.5 Observable/Reactive System

```typescript
interface ObservableValue<V> {
    getValue(): V
    subscribe(observer: Observer<ObservableValue<V>>): Subscription
    catchupAndSubscribe(observer: Observer<ObservableValue<V>>): Subscription
}

interface Terminable { terminate(): void }  // RAII-like resource cleanup
class Terminator implements Terminable {    // Owns multiple Terminables
    own<T extends Terminable>(t: T): T
    terminate(): void  // terminates all owned resources
}

type Option<T> = None | Some<T>  // Rust-like Option type
type Optional<T> = T | undefined
type Nullable<T> = T | null
```

---

## Summary: Critical Insights for REVITHION STUDIO

1. **The DSP is clean and portable**: `lib-dsp` contains ~128 files of pure math DSP — no web dependencies. This translates almost 1:1 to C++/NumPy.

2. **The BoxGraph ECS is the architectural soul**: Everything (project state, device parameters, timeline, routing) is a Box in the BoxGraph. Porting this correctly is the #1 priority.

3. **Binary serialization is custom**: The format is NOT standard (not protobuf, not JSON). You must port `DataInput`/`DataOutput` exactly to read existing `.opendaw` project files.

4. **The three-layer device pattern is excellent**: Box → Adapter → Processor is a clean separation that maps well to C++ (data struct → wrapper class → DSP processor).

5. **No native code exists yet**: The entire codebase is TypeScript. There are zero `.cpp`, `.c`, `.h`, or `CMakeLists.txt` files (outside `node_modules`).

6. **The audio processing model matches native DAW patterns**: 128-sample blocks, topologically-sorted processor graph, PPQN timing, automatable parameters — this is standard DAW architecture that maps directly to JUCE/VST/AU patterns.

7. **The UI is the hardest part to port**: Custom JSX engine (not React), custom Canvas rendering, Sass styling — this is tightly coupled to the browser and would need a complete rewrite in a native UI framework.

8. **Collaboration (Y.js) is optional for v1**: Can defer real-time collab to a later phase.

9. **The README explicitly asks for desktop builds**: "Offline desktop build (e.g., via Tauri)" is listed as a community goal. A Tauri wrapper would be the fastest path to a `.exe`, but a full C++ rewrite gives true native performance.

10. **License consideration**: AGPL v3 requires source disclosure for network use. For a commercial closed-source product (REVITHION STUDIO), you'd need either the commercial license or a clean-room reimplementation of the architecture patterns.
