# Nuendo 14 Operation Manual — Complete Feature Extraction for REVITHION STUDIO

> **Source:** Nuendo 14.0.30 Operation Manual (1,806 pages, 2,536 TOC entries)
> **Purpose:** Target feature list for REVITHION STUDIO (OpenDAW reference implementation)
> **Date:** Extracted from official Steinberg documentation

---

## Table of Contents

1. [Audio Engine](#1-audio-engine)
2. [Signal Routing](#2-signal-routing)
3. [MixConsole](#3-mixconsole)
4. [Effects](#4-effects)
5. [MIDI](#5-midi)
6. [Automation](#6-automation)
7. [Mastering & Metering](#7-mastering--metering)
8. [Surround / Immersive Audio](#8-surround--immersive-audio)
9. [Game Audio / Post-Production](#9-game-audio--post-production)
10. [Control Room](#10-control-room)
11. [Plugin System](#11-plugin-system)
12. [Additional Systems](#12-additional-systems)

---

## 1. Audio Engine

### 1.1 Sample Rates
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Standard sample rates | 44.1 kHz, 48 kHz, 88.2 kHz, 96 kHz, 176.4 kHz, 192 kHz | **Critical** | Medium |
| Per-project sample rate | Set at project creation; changing later requires converting all files | **Critical** | Low |
| Sample rate conversion on import | Auto-convert imported files to project sample rate | **Critical** | Medium |

### 1.2 Bit Depth / Record Format
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Record bit depth | 16-bit, 24-bit, 32-bit float, 64-bit float WAV recording | **Critical** | Medium |
| Internal processing | 64-bit floating point audio engine | **Critical** | High |
| Dither on output | Dither effects available for bit-depth reduction on master bus | **Critical** | Medium |

### 1.3 Audio Driver / Hardware
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| ASIO driver support | Native ASIO driver interface for low-latency audio (Windows) | **Critical** | High |
| Core Audio support | macOS native audio driver support | **Critical** | High |
| ASIO-Guard | Intelligent pre-processing of audio paths not requiring real-time; moves processing ahead of playback position to reduce CPU spikes | **Critical** | High |
| Buffer size configuration | Adjustable audio buffer sizes for latency/performance tradeoff | **Critical** | Medium |
| Word clock | External clock source synchronization for sample-accurate sync | Useful | High |
| Multiple driver support | Use several audio applications simultaneously (Release Driver in Background) | Useful | Medium |

### 1.4 Audio Performance
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Audio Performance Monitor | Real-time CPU/disk load monitoring panel | **Critical** | Medium |
| Smart plug-in processing | VST3 plugins auto-suspend when no signal present, reducing CPU | **Critical** | High |
| Track freeze | Render tracks to audio to free CPU; unfreeze to edit | **Critical** | Medium |
| Track disable | Completely remove track from processing without deleting | Useful | Low |
| ASIO-Guard levels | Normal/High/Custom guard levels per plugin for optimization | **Critical** | High |

### 1.5 Project Settings
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Project frame rate | 23.976, 24, 25, 29.97, 30 fps (drop/non-drop) and more | **Critical** | Medium |
| Project start time | Configurable timecode start position | **Critical** | Low |
| Audio pull-up/pull-down | Speed compensation for film transfers | Later | High |
| Pan law settings | 0 dB, -3 dB, -6 dB, equal power pan law options | **Critical** | Low |
| Record file format | WAV, AIFF, Broadcast WAV selectable per project | **Critical** | Low |

---

## 2. Signal Routing

### 2.1 Channel / Track Types
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Audio Track | Mono/stereo/multi-channel audio recording and playback | **Critical** | Medium |
| Instrument Track | Combined MIDI + VSTi output on single track | **Critical** | Medium |
| Sampler Track | Chromatic playback of any audio sample via MIDI | **Critical** | High |
| Drum Track | Specialized MIDI track with drum map support | **Critical** | Medium |
| MIDI Track | Standard MIDI sequencing routed to VSTi or external | **Critical** | Medium |
| Group Channel Track | Submix bus — route multiple channels for collective processing | **Critical** | Medium |
| Effect Channel Track (FX) | Dedicated return channel for send effects | **Critical** | Medium |
| VCA Fader Track | Controls linked channel faders without summing audio; nested VCA support | **Critical** | High |
| Folder Track | Organizational container; can show composite waveform of children | Useful | Low |
| Marker Track | Position markers and cycle markers for navigation | **Critical** | Low |
| Ruler Track | Additional timeline ruler with independent time format | Useful | Low |
| Tempo Track | Tempo automation (BPM changes over time) | **Critical** | Medium |
| Signature Track | Time signature changes over project timeline | **Critical** | Medium |
| Arranger Track | Non-destructive song structure arrangement | Useful | Medium |
| Transpose Track | Global pitch transposition over time | Useful | Medium |
| Loudness Track | Real-time loudness measurement display over time | Useful | Medium |
| Chord Track | Chord progression authoring; drives harmonic content | Useful | High |
| Video Track | Video playback synced to timeline | Later | High |

### 2.2 Bus Architecture
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Input busses | Configurable mono/stereo/surround input busses mapped to hardware | **Critical** | Medium |
| Output busses | Configurable output busses with Main Mix designation | **Critical** | Medium |
| Child busses | Sub-busses within surround busses (e.g., stereo pair within 5.1) | **Critical** | High |
| Bus presets | Save/recall entire bus configurations | Useful | Low |
| Non-exclusive port assignment | Same hardware port assignable to multiple busses | Useful | Medium |
| Monitoring bus | Dedicated monitor bus separate from main output | **Critical** | Medium |

### 2.3 Routing Features
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Insert effects | 16 insert slots per channel (pre/post fader split at slot 7/8 boundary) | **Critical** | Medium |
| Send effects | 8 sends per channel, freely routable to FX channels | **Critical** | Medium |
| Direct Routing | Up to 8 output destinations per channel, switchable post-fader/post-pan; supports automation of destination switches | **Critical** | High |
| Cue sends | Dedicated sends to Control Room cue channels for performer monitoring | **Critical** | Medium |
| Side-chain routing | Route any audio signal to side-chain input of supported VST3 effects | **Critical** | High |
| External effects | Hardware insert routing with automatic latency compensation | Useful | High |
| External instruments | Route to/from external hardware synths with latency compensation | Useful | High |
| Automatic downmixing | Automatic format conversion when routing between different channel widths | **Critical** | High |
| Channel configuration switching | Switch tracks between mono and stereo | **Critical** | Low |
| Split multi-channel to mono | Extract individual channels from multi-channel tracks | Useful | Medium |
| Merge mono to multi-channel | Combine mono tracks into stereo/surround | Useful | Medium |

---

## 3. MixConsole

### 3.1 Core MixConsole
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Multiple MixConsole windows | Open 3+ independent MixConsole windows with different views | **Critical** | Medium |
| Lower zone MixConsole | Integrated MixConsole in project window lower zone | **Critical** | Medium |
| Linked MixConsoles | Scroll/visibility linking between MixConsole instances | Useful | Medium |
| Channel type filtering | Show/hide channels by type (audio, group, FX, VSTi, etc.) | **Critical** | Low |
| Channel visibility agents | Saved visibility configurations, quick filter presets | **Critical** | Medium |
| Channel linking | Link channel parameters (volume, pan, sends, etc.) with configurable link groups | **Critical** | Medium |
| Undo/redo MixConsole changes | Full undo history for all mixer parameter changes | **Critical** | Medium |
| Meter bridge | Peak/VU meter display across all channels | **Critical** | Medium |
| Snapshots | Save and recall complete mixer states | **Critical** | Medium |

### 3.2 Channel Sections (top to bottom in MixConsole)
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Routing section | Input/output routing assignment per channel | **Critical** | Medium |
| Pre section | High-cut filter, low-cut filter, input gain, phase invert per channel | **Critical** | Medium |
| Inserts section | 16 insert effect slots (pre-fader 1-6, post-fader 7-16) | **Critical** | Medium |
| Equalizer section | 4-band parametric EQ per channel with curve display | **Critical** | Medium |
| Strip section | Integrated channel strip (gate, compressor, EQ, tools, saturation, limiter) | **Critical** | High |
| Sends section | 8 send slots with level/pan per send | **Critical** | Medium |
| Cue Sends section | Sends to Control Room cue outputs | **Critical** | Medium |
| Direct Routing section | Up to 8 switchable output destinations | **Critical** | High |
| Fader section | Level fader, pan, solo, mute, record enable, monitor, automation R/W | **Critical** | Medium |

### 3.3 Equalizer Details
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| 4-band parametric EQ | Bands 1 & 4: parametric/shelving/cut; Bands 2 & 3: parametric only | **Critical** | Medium |
| Frequency range | 20 Hz – 20 kHz per band | **Critical** | Medium |
| Gain range | ±24 dB per band | **Critical** | Low |
| EQ curve display | Interactive graphical EQ editing with click-drag on curve | **Critical** | Medium |
| EQ presets | Save/load EQ settings | **Critical** | Low |
| Note-based frequency entry | Enter frequency as musical note (e.g., A3 = 440 Hz) with cent offset | Useful | Low |
| EQ curve inversion | Invert entire EQ curve via context menu | Useful | Low |
| High-cut / Low-cut filters | Part of Pre section; up to 48 dB/octave slopes | **Critical** | Medium |

### 3.4 Channel Strip Modules
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Noise Gate | Threshold, range, attack, release, auto-release, internal side-chain filter, external side-chain | **Critical** | High |
| Standard Compressor | Threshold, ratio, attack, release, auto-makeup, auto-release, makeup gain, side-chain | **Critical** | High |
| Tube Compressor | Tube-simulation compression with input/output gain, drive, mix, side-chain | **Critical** | High |
| Vintage Compressor | Modeled vintage compression with input/output gain, attack, release | **Critical** | High |
| EQ (strip) | Integrated EQ within strip signal chain | **Critical** | Medium |
| Tools | EnergizedLow/High, DeEsser, and other utility processors | Useful | High |
| Saturation | Warmth/saturation module with drive control | Useful | Medium |
| Limiter | Brickwall limiter for clipping prevention | **Critical** | Medium |
| Drag-and-drop module order | Reorder strip modules in the signal chain via drag-and-drop | **Critical** | Medium |

### 3.5 MixConsole Utilities
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Copy/move channel settings | Drag-and-drop to copy or move entire channel settings between channels | **Critical** | Medium |
| Save/load channel presets | Per-channel preset save and recall | **Critical** | Low |
| FX chain presets | Save/load insert chains as presets | **Critical** | Low |
| Track Quick Controls | 8 assignable quick-access knobs per track for any automatable parameter | **Critical** | Medium |

---

## 4. Effects

### 4.1 Insert Effects
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| 16 insert slots per channel | Pre-fader and post-fader insert positions | **Critical** | Medium |
| VST3 and VST2 support | Host both VST3 and VST2 effect plugins | **Critical** | High |
| Effect bypass | Individual and global bypass per insert slot | **Critical** | Low |
| Effect presets | Save/load/browse presets per effect via MediaBay integration | **Critical** | Medium |
| A/B comparison | Compare two effect settings | Useful | Low |
| Effect control panel | Resizable, detachable plugin windows | **Critical** | Medium |

### 4.2 Send Effects
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| 8 sends per channel | Pre-fader or post-fader send routing | **Critical** | Medium |
| Send level and pan | Independent level and pan per send | **Critical** | Low |
| FX channel return | Dedicated effect return channels with full insert/EQ chain | **Critical** | Medium |

### 4.3 Side-Chaining
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| VST3 side-chain inputs | Route any audio to side-chain input of supported effects | **Critical** | High |
| Side-chain from sends | Use sends to feed side-chain inputs | **Critical** | Medium |
| Side-chain for instruments | Route audio to side-chain of VSTi plugins | Useful | High |
| Side-chain for modulators | Route audio to modulator side-chain inputs | Useful | High |

### 4.4 Direct Offline Processing (DOP)
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Non-destructive offline effects | Apply plugins and processes to events/clips without affecting original files | **Critical** | High |
| Unlimited undo | Remove/reorder any process at any time, any order | **Critical** | High |
| Auto-apply mode | Instant preview of processing changes | **Critical** | Medium |
| Process chain | Stack multiple effects/processes in sequence per clip | **Critical** | Medium |
| Favorites | Quick-access list of frequently used DOP processes | Useful | Low |
| Copy/paste processes | Transfer DOP chains between events | **Critical** | Medium |
| Make permanent | Apply all DOP permanently to audio file | **Critical** | Medium |

### 4.5 Built-In Audio Processes (for DOP)
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Envelope | Apply volume envelope to audio with spline/linear curves | **Critical** | Medium |
| Fade In / Fade Out | Offline fade application with curve editing | **Critical** | Medium |
| Gain | Apply gain change with optional normalization | **Critical** | Low |
| Normalize | Normalize to peak or loudness target | **Critical** | Medium |
| Phase Reverse | Invert audio phase | **Critical** | Low |
| Pitch Shift | Offline pitch shifting with algorithm selection (élastique, MPEX, Standard) | **Critical** | High |
| Resample | Convert sample rate | **Critical** | Medium |
| Reverse | Reverse audio playback direction | **Critical** | Low |
| Silence | Replace selection with silence | **Critical** | Low |
| Stereo Flip | Swap/mirror/merge stereo channels | Useful | Low |
| Time Stretch | Offline time stretching with algorithm selection | **Critical** | High |

### 4.6 Time Stretch / Pitch Shift Algorithms
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| élastique | High-quality real-time stretching (Pro, Efficient, SOLOIST modes) | **Critical** | High |
| MPEX | MPEG-based high-quality offline algorithm | Useful | High |
| Standard | Legacy algorithm with formant options | Useful | Medium |
| Musical Mode | Auto-stretch audio files to project tempo | **Critical** | High |
| Free Warp | Manual warp marker placement for timing correction | **Critical** | High |
| AudioWarp quantize | Quantize audio timing using warp markers and hitpoints | **Critical** | High |

---

## 5. MIDI

### 5.1 MIDI Editing
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Key Editor | Piano-roll MIDI editor with velocity, controller lanes | **Critical** | High |
| Drum Editor | Grid-based drum editing with drum maps | **Critical** | High |
| List Editor | Event-list MIDI editor showing all MIDI data | **Critical** | Medium |
| In-Place Editor | Edit MIDI directly on the track in the project window | **Critical** | Medium |
| Score Editor | Music notation display and editing (basic in Nuendo) | Useful | High |
| Controller display | Multi-lane controller editing below note display | **Critical** | Medium |
| MIDI note expression | Per-note continuous controller data (VST3 Note Expression) | **Critical** | High |
| Expression maps | Map articulations/playing techniques to key switches and controllers | **Critical** | High |

### 5.2 MIDI Recording
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Real-time recording | Record MIDI input with latency compensation | **Critical** | Medium |
| Step input | Enter notes one at a time with configurable duration | **Critical** | Medium |
| Cycle recording | Loop recording with stacked takes or merge modes | **Critical** | Medium |
| Retrospective recording | Capture MIDI input played without being in record mode | **Critical** | Medium |
| MIDI thru | Real-time MIDI monitoring through VSTi | **Critical** | Low |
| On-screen keyboard | Virtual keyboard for MIDI input | Useful | Low |

### 5.3 MIDI Functions
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Quantize | Start, length, end quantization with swing, humanize, iterative Q | **Critical** | Medium |
| Transpose | Chromatic, scale-aware transposition with keep-in-octave option | **Critical** | Medium |
| Legato | Extend notes to fill gaps | **Critical** | Low |
| Fixed lengths | Set all notes to specific duration | **Critical** | Low |
| Fixed velocities | Set all notes to specific velocity | **Critical** | Low |
| Delete doubles | Remove duplicate MIDI notes | **Critical** | Low |
| Delete overlaps | Remove note overlaps (mono/poly modes) | **Critical** | Low |
| Thin controller data | Reduce controller event density | Useful | Low |
| Dissolve parts | Split by channel or pitch to separate tracks | **Critical** | Medium |
| Merge MIDI events | Combine multiple parts into one | **Critical** | Low |
| Bounce MIDI | Consolidate MIDI with all real-time parameters applied | **Critical** | Medium |
| Logical Editor | Advanced MIDI event filtering and transformation via conditions/actions | **Critical** | High |
| Project Logical Editor | Project-level event/track filtering and batch operations | Useful | High |
| Input Transformer | Filter/transform incoming MIDI in real-time | **Critical** | High |

### 5.4 MIDI Realtime Parameters
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Transpose | Real-time transposition per track | **Critical** | Low |
| Velocity shift | Real-time velocity offset | **Critical** | Low |
| Velocity compression | Real-time velocity range compression/expansion | **Critical** | Low |
| Length compression | Real-time note length scaling | **Critical** | Low |
| Random variations | Random range for position, pitch, velocity, length | Useful | Medium |
| Range limiting | Constrain note range with filter/limit/mirror modes | Useful | Medium |

### 5.5 MIDI Remote
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| MIDI Remote tab | Dedicated GUI for hardware controller mapping | **Critical** | High |
| Controller surface editor | Visual layout creation for MIDI controllers | Useful | High |
| Mapping Assistant | Quick parameter assignment from controller to DAW parameter | **Critical** | Medium |
| MIDI Remote scripts | JavaScript-based scripting API for custom controller integration | **Critical** | High |
| MIDI Remote Manager | Central management of all connected MIDI controllers | **Critical** | Medium |

### 5.6 Expression Maps
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Articulation mapping | Define playing techniques mapped to key switches, program changes, controllers | **Critical** | High |
| Articulation lanes | Visual articulation display/editing in Key Editor | **Critical** | Medium |
| Expression map presets | Save/load/share expression maps | **Critical** | Low |
| Direction and Attribute types | Two articulation types: continuous (direction) and one-shot (attribute) | **Critical** | Medium |

### 5.7 Note Expression (VST3)
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Per-note pitch bend | Individual pitch control per note (not per-channel) | **Critical** | High |
| Per-note volume | Individual volume per note | **Critical** | High |
| Per-note pan | Individual panning per note | Useful | High |
| Per-note tuning | Micro-tonal per-note tuning | Useful | High |
| Custom CC per note | Arbitrary controller data per note for VST3 instruments | **Critical** | High |
| Note Expression editor | Dedicated editing interface for per-note data | **Critical** | High |

### 5.8 Chord Functions
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Chord Track | Author chord progressions on dedicated track | Useful | High |
| Chord Pads | Interactive chord pad interface for performance/composition | Useful | High |
| Chord Assistant | AI-based chord suggestion (Circle of Fifths, Proximity, List) | Useful | High |
| Voicing system | Players (piano, guitar, basic) with adaptive voicing rules | Useful | High |
| Chord-to-MIDI | Convert chord events to MIDI notes on tracks | Useful | Medium |
| Scale events | Define musical scales that constrain editing/quantization | Useful | Medium |

---

## 6. Automation

### 6.1 Automation Modes
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Touch mode | Write while touching control, punch out on release, return to previous value | **Critical** | High |
| Auto-Latch mode | Continue writing until stop/disable; keeps last value on release | **Critical** | High |
| Cross-Over mode | Write until curve crosses original value, then auto-punch-out | **Critical** | High |
| Trim mode | Modify existing automation curve relatively (offset) | **Critical** | High |
| Read (R) | Play back automation data | **Critical** | Medium |
| Write (W) | Record automation data; auto-enables Read | **Critical** | Medium |

### 6.2 Automation Curves
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Ramp curves | Smooth interpolation between automation points (continuous parameters) | **Critical** | Medium |
| Step curves | Instant value changes (on/off parameters like mute) | **Critical** | Medium |
| Manual drawing | Draw tool, line tool, parabola, triangle, square wave shapes | **Critical** | Medium |
| Virgin Territory | Unwritten sections use no automation (pass-through); vs. Initial Value mode | **Critical** | High |
| Static value line | Default parameter value when no automation exists | **Critical** | Low |

### 6.3 Automation Panel
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Global R/W controls | Enable/disable read/write for all tracks simultaneously | **Critical** | Low |
| Fill options | Fill to Punch, Fill to End, Fill to Start, Fill Loop — auto-fill gaps after punch-out | **Critical** | High |
| Join options | Resume writing after interrupted pass | **Critical** | High |
| Preview mode | Audition new settings without recording the steps to find them | **Critical** | High |
| Suspend Read/Write | Selectively suspend automation per parameter type (volume, pan, sends, etc.) | **Critical** | Medium |
| Punch Log | History of recent automation punch-in operations | Useful | Medium |
| Freeze Trim | Apply trim automation permanently to underlying curve | **Critical** | Medium |
| Terminator point | Define end-point of automation curve section, creating a gap | Useful | Medium |

### 6.4 Automation Editing
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Automation tracks | Separate sub-tracks per automated parameter | **Critical** | Medium |
| Show Used automation | Reveal only automation tracks that have data | **Critical** | Low |
| Selection tools | Select, move, scale, stretch automation events | **Critical** | Medium |
| Automation follows events | Automation data moves with audio events when repositioned | **Critical** | High |
| VCA automation | VCA faders control linked channel faders; combinable automation | **Critical** | High |
| Cleaning automation | Remove redundant automation points | Useful | Medium |

### 6.5 Automatic Attenuation (Auto-Duck)
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Background signal attenuation | Automatically duck background audio when foreground signal is present (dialogue over music) | Useful | High |

---

## 7. Mastering & Metering

### 7.1 Metering
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Master Meter | Multi-channel true peak meter with multiple scale standards | **Critical** | Medium |
| PPM scales | Digital, DIN, EBU, British, Nordic, K-20, K-14, K-12, +3/+6/+12 dB Digital | **Critical** | Medium |
| Alignment levels | Configurable alignment level offset for broadcast scales (-18 dBFS default) | **Critical** | Low |
| RMS metering | RMS measurement with configurable AES17 mode and integration time | **Critical** | Medium |
| Peak hold | Configurable peak hold time and max peak display | **Critical** | Low |
| Channel meters | Per-channel level meters in MixConsole (peak + RMS) | **Critical** | Medium |

### 7.2 Loudness
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| EBU R128 loudness metering | Integrated loudness (LUFS), short-term, momentary, loudness range (LRA) | **Critical** | High |
| Loudness Track | Real-time loudness measurement displayed as track over timeline | **Critical** | High |
| True Peak metering | dBTP true peak measurement per EBU R128 | **Critical** | Medium |
| Loudness normalization on export | Auto-normalize exported audio to target integrated loudness (LUFS) | **Critical** | Medium |
| Loudness standards compliance | Support for EBU R128, ATSC A/85, ARIB, and custom targets | **Critical** | Medium |

### 7.3 Export / Mixdown
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Export Audio Mixdown | Bounce any channel(s) to audio files | **Critical** | Medium |
| Multi-channel export | Export multiple channels simultaneously as separate files | **Critical** | Medium |
| Cycle marker export | Export regions defined by cycle markers as individual files | **Critical** | Medium |
| File formats | WAV, AIFF, MXF, MP3, FLAC, Ogg Vorbis with format-specific settings | **Critical** | Medium |
| Broadcast WAV | BWF with iXML metadata chunks | **Critical** | Medium |
| Bit depth options | 8, 16, 24, 32-bit integer; 32, 64-bit float export | **Critical** | Low |
| Split/interleaved | Export multi-channel as split mono or interleaved file | **Critical** | Low |
| Job queues | Queue multiple export jobs for batch processing | Useful | Medium |
| Real-time export | Optional real-time bounce for hardware-dependent processing | Useful | Low |
| Import back to project | Auto-import exported file back into project pool/track | Useful | Low |
| Arranger chain export | Export specific arranger chain arrangements | Useful | Medium |
| Render in Place | Quick render of tracks/events to audio with processing | **Critical** | Medium |
| Render Selection | Render only selected portion | **Critical** | Medium |

---

## 8. Surround / Immersive Audio

### 8.1 Surround Configurations
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Mono / Stereo | Standard 1-2 channel configurations | **Critical** | Low |
| LRC (3.0) | Left, Right, Center | Useful | Medium |
| Quadro | 4-channel quadraphonic | Useful | Medium |
| 5.0 / 5.1 | Standard surround (Dolby Digital / DTS / MPEG-2 compatible) | **Critical** | High |
| 6.0 / 6.1 | Cine and Music variants | Useful | High |
| 7.0 / 7.1 | Full surround with side and rear channels | **Critical** | High |
| 7.0.2 / 7.1.2 | 3D with top channels (Dolby Atmos bed format) | **Critical** | High |
| 7.0.4 / 7.1.4 | 3D with 4 top channels | **Critical** | High |
| 7.0.6 / 7.1.6 | 3D with 6 top channels | Useful | High |

### 8.2 Surround Panning
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| VST MultiPanner | Integrated surround panner with 2D/3D positioning, divergence, LFE control | **Critical** | High |
| Top View / Side View / Rear View | Multiple panning views for 3D positioning | **Critical** | High |
| Object size | Spread audio across speakers based on position | **Critical** | High |
| Speaker snap | Snap object audio to nearest active speaker | Useful | Medium |
| Speaker zones | Define active speaker zones for object routing | Useful | High |
| MixConvert V6 | Automatic downmix/upmix between surround formats | **Critical** | High |
| Third-party panner support | Host alternative panner plugins (e.g., IOSONO Anymix Pro) | Useful | Medium |

### 8.3 Object-Based Audio
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Dolby Atmos authoring | Create object-based Dolby Atmos content without additional hardware | **Critical** | High |
| ADM Authoring window | Dedicated window for managing beds, objects, and binaural rendering | **Critical** | High |
| ADM Broadcast WAV export | Export compliant ADM BWF files for Dolby Atmos Renderer | **Critical** | High |
| MPEG-H authoring | Create object-based MPEG-H content natively | Useful | High |
| MPEG-H presets | Scene presets and interactivity definitions for MPEG-H | Useful | High |
| OSC (Open Sound Control) | Live ADM productions with OSC position tracking | Later | High |
| Head-tracking support | VR/AR head-mounted display integration for monitoring | Later | High |

### 8.4 Ambisonics
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| First-order Ambisonics | 4-channel (ACN/SN3D) Ambisonics encoding/decoding | Useful | High |
| Second-order Ambisonics | 9-channel Ambisonics | Useful | High |
| Third-order Ambisonics | 16-channel Ambisonics | Useful | High |
| Ambisonics panning | VST AmbiPanner for encoding mono/stereo to Ambisonics | Useful | High |
| Ambisonics decoding | VST AmbiDecoder for binaural headphone monitoring or speaker decode | Useful | High |
| Headphone monitoring | Binaural rendering of Ambisonics content for headphone mixing | Useful | High |

---

## 9. Game Audio / Post-Production

### 9.1 ADR (Automated Dialogue Replacement)
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| ADR window | Dedicated ADR workflow with rehearse/record/review modes | Useful | High |
| ADR statuses | Pre-record, record, post-record status tracking via markers | Useful | Medium |
| ADR modes | Auto mode (timed recording) and Free Run mode | Useful | Medium |
| ADR Script Reader | Display/edit dialogue text from ADR project, integrated with markers | Useful | High |
| Dialogue Transcription | AI-powered automatic transcription of recorded dialogue with language selection | Useful | High |
| Talkback integration | One-click talkback for booth communication | Useful | Medium |
| Video overlay | Visual countdown and cue indicators for talent | Useful | High |
| ADR API | Network API for external ADR system integration | Later | High |

### 9.2 Game Audio Connect
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Game Audio Connect | Direct transfer of audio assets to game audio engines (Wwise) | Later | High |
| Perforce integration | Version control integration for game audio projects | Later | High |
| Remote preview | Preview assets in game engine context while editing in Nuendo | Later | High |
| Audio asset management | Track, rename, and organize game audio assets | Later | Medium |

### 9.3 ReConform
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| ReConform | Auto-adjust audio edits when picture cut changes (EDL comparison) | Useful | High |
| EDL support | Import CMX3600, File64 EDL formats for conforming | Useful | High |
| Video cut detection | Auto-detect edit points in video files | Useful | High |
| ReConform preview | Preview changes before applying | Useful | High |

### 9.4 Video Integration
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Video playback | Sync video to timeline for post-production workflows | Later | High |
| Video export | Export video with audio mixdown | Later | High |
| Video cut detection | Detect cuts in imported video files and create markers | Useful | High |
| Video follows editing | Video playback follows audio editing position | Useful | Medium |
| Production audio import | Import location audio with AAF/OMF/AES31 workflows | Useful | High |
| Extract audio from video | Rip audio track from video file | Useful | Medium |

### 9.5 Audio Editing to Picture
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Audio alignment | Automatically align audio events to reference (e.g., sync ADR to production) | **Critical** | High |
| Audio Alignment Panel | Dedicated panel for warp-based alignment of multiple events | **Critical** | High |
| Hitpoint detection | Auto-detect transients for slicing, groove extraction, marker creation | **Critical** | High |
| Detect Silence | Auto-split audio based on silence detection with threshold/timing controls | **Critical** | Medium |

---

## 10. Control Room

### 10.1 Control Room Channels
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Monitor channels | Multiple monitor output configurations (up to 4) for different speaker systems | **Critical** | High |
| Cue channels | Up to 4 independent cue mixes for performer headphone feeds | **Critical** | High |
| Headphone channel | Dedicated headphone monitor with independent source selection | **Critical** | Medium |
| Talkback channel | Built-in talkback mic routing to cue mixes | **Critical** | Medium |
| External input | Route external sources (CD, reference tracks) into monitoring chain | Useful | Medium |
| Main Mix source | Main mix auto-available as monitor source | **Critical** | Low |

### 10.2 Control Room Features
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Monitor switching | Instant switching between monitor outputs (e.g., near-field, main, headphones) | **Critical** | Medium |
| Reference level | Calibrated monitoring level presets | **Critical** | Medium |
| Dim | Reduce monitoring level by configurable amount | **Critical** | Low |
| Mono/Stereo/Left/Right/Side | Listen mode switching for mix checking | **Critical** | Low |
| Listen bus | AFL/PFL solo monitoring through Control Room (not affecting main mix) | **Critical** | Medium |
| Downmix presets | Monitor surround content in stereo/mono for compatibility checking | **Critical** | High |
| Inserts on monitor chain | Insert effects on monitor path (not affecting main mix export) | **Critical** | Medium |
| Exclusive port assignment | Monitor ports separate from main output (preventing feedback) | **Critical** | Medium |

---

## 11. Plugin System

### 11.1 VST Hosting
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| VST3 hosting | Full VST3 standard support with smart processing, side-chain, note expression | **Critical** | High |
| VST2 hosting | Legacy VST2 plugin support | Useful | High |
| Plugin delay compensation | Automatic delay compensation across entire signal chain | **Critical** | High |
| Plugin manager | Central window for managing all installed plugins | **Critical** | Medium |
| Plugin collections | Custom plugin organization lists (collections) | Useful | Medium |
| Plugin blocklist | Disable/re-enable problematic plugins | **Critical** | Low |
| Plugin scanner | Auto-detect installed plugins with background scanning | **Critical** | Medium |
| Constrain delay compensation | Disable high-latency plugins for real-time monitoring | **Critical** | Medium |

### 11.2 VST Instruments
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| VSTi rack | Central rack for loading VST instruments with multi-output support | **Critical** | High |
| Instrument tracks | Combined MIDI + VSTi single-track workflow | **Critical** | Medium |
| Multi-output instruments | Route individual VSTi outputs to separate mixer channels | **Critical** | High |
| VSTi presets | Browse, load, save instrument presets via MediaBay | **Critical** | Medium |
| Freeze instruments | Render VSTi output to audio to free CPU | **Critical** | Medium |
| External instruments | Route to/from hardware synths with latency compensation | Useful | High |

### 11.3 Plugin Presets
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| VST presets | Standardized preset format for all VST3 plugins | **Critical** | Medium |
| MediaBay integration | Browse and tag presets through MediaBay | **Critical** | Medium |
| Track presets | Save entire track configuration (inserts, EQ, routing) as preset | **Critical** | Medium |
| FX chain presets | Save/load complete insert chains | **Critical** | Low |
| Quick preset loading | Preset browser in plugin header | **Critical** | Low |

---

## 12. Additional Systems

### 12.1 VariAudio (Pitch Editing)
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Monophonic pitch detection | Auto-analyze and segment monophonic audio into notes | **Critical** | High |
| Pitch correction | Move segments to correct pitch with snap modes (chromatic, scale) | **Critical** | High |
| Straighten pitch curve | Reduce pitch variation within a note (micro-pitch) | **Critical** | High |
| Timing correction | Warp individual note timing within audio | **Critical** | High |
| Formant shifting | Shift formants independently of pitch | **Critical** | High |
| Volume editing | Per-segment volume adjustment | **Critical** | Medium |
| Harmony voices | Generate harmony voices from analyzed audio | Useful | High |
| Scale Assistant | Constrain pitch correction to musical scale/chord track | **Critical** | Medium |
| MIDI reference | Show MIDI track as reference overlay for pitch matching | Useful | Medium |

### 12.2 Sampler Control
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Drag-to-load samples | Load any audio by dragging to Sampler Control | **Critical** | Medium |
| Chromatic playback | Play audio samples chromatically via MIDI across keyboard | **Critical** | High |
| AudioWarp modes | Fixed Pitch, Resample, Musical (tempo-synced) playback modes | **Critical** | High |
| Multi-sample zones | Define multiple sample zones across keyboard range | Useful | High |
| Filter section | Built-in filter with envelope (LP, HP, BP types) | **Critical** | Medium |
| Amp envelope | ADSR volume envelope per sample | **Critical** | Medium |
| Pitch envelope | ADSR pitch modulation | Useful | Medium |
| LFO | Modulation LFO with multiple destinations | Useful | Medium |
| Transfer to VSTi | Convert sampler track to full VSTi instrument (HALion, Groove Agent) | Later | Medium |

### 12.3 Modulators
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| LFO modulator | Modulate any automatable parameter with LFO waveforms | **Critical** | High |
| Envelope Follower | Modulate parameters based on audio signal envelope | **Critical** | High |
| Shaper | Custom curve-based modulation | Useful | High |
| Macro Knob | Create macro controls that affect multiple parameters simultaneously | **Critical** | Medium |
| Step Modulator | Step-sequencer-based parameter modulation | Useful | High |
| ModScripter | Script-based custom modulator (JavaScript) | Later | High |
| Modulator chaining | Modulators can modulate other modulators | Useful | High |
| Side-chain input | Audio-driven modulation via side-chain routing | **Critical** | High |

### 12.4 Pattern Editor & Drum Machine
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Pattern Editor | Step-sequencer-style MIDI editing with lanes per instrument/note | Useful | High |
| Drum Machine | Integrated drum machine with pad-based interface and sample loading | Useful | High |
| Step lanes | Individual step sequences per drum sound/note | Useful | Medium |
| Step parameter automation | Per-step velocity, pan, pitch, filter automation | Useful | High |

### 12.5 File Exchange
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| AAF import/export | Advanced Authoring Format for Pro Tools/Avid interop | Useful | High |
| OMF import/export | Open Media Framework for legacy DAW interchange | Useful | High |
| AES31 import/export | AES31 edit decision list format | Useful | High |
| ADM import/export | Audio Definition Model for immersive audio metadata | Useful | High |
| MXF files | Material Exchange Format (OP-Atom) | Useful | High |
| DAWproject | Open DAW project exchange format (import/export with channel strip) | **Critical** | High |
| Cue sheets | Export cue sheet reports for music/SFX documentation | Useful | Medium |
| TTAL files | Timed Text Authoring Lineage for subtitle workflows | Later | Medium |

### 12.6 Networking / Collaboration
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Network collaboration | Multi-user project sharing over LAN with permissions | Later | High |
| User permissions | Read/write/admin access control per user | Later | High |
| Chat | Built-in text chat between networked users | Later | Medium |
| Conflict resolution | Handle simultaneous edits to shared projects | Later | High |

### 12.7 Synchronization
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Timecode sync | SMPTE/LTC/MTC synchronization as master or slave | Useful | High |
| MIDI Clock | Musical tempo sync for external gear | **Critical** | Medium |
| VST System Link | Multi-computer DAW linking via audio interface | Later | High |
| MMC (MIDI Machine Control) | Control external recorders/machines via MIDI | Useful | Medium |
| 9-Pin (Sony P2) | RS-422 machine control protocol | Later | High |
| Word clock | Sample-accurate digital audio sync | Useful | High |

### 12.8 MediaBay
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| MediaBay | Central media browser for all audio, presets, loops, and samples | **Critical** | High |
| Tag-based browsing | Filter by attributes, category, sub-category, musical style | **Critical** | Medium |
| Volume databases | Index and search across multiple drives/locations | **Critical** | Medium |
| Preview player | Audition media files before importing | **Critical** | Medium |
| Media Rack | Compact browser in Project window right zone | **Critical** | Medium |

### 12.9 Key Commands & Macros
| Feature | Description | Relevance | Complexity |
|---------|-------------|-----------|------------|
| Customizable key commands | Full keyboard shortcut customization for all functions | **Critical** | Medium |
| Macros | Chain multiple commands into single key command | **Critical** | Medium |
| Key command presets | Save/load/export complete key command configurations | **Critical** | Low |
| Tool modifier keys | Customize modifier key behavior for tools | Useful | Low |

---

## Summary Statistics

### Feature Count by Category
| Category | Total Features | Critical | Useful | Later |
|----------|---------------|----------|--------|-------|
| 1. Audio Engine | 18 | 14 | 4 | 0 |
| 2. Signal Routing | 28 | 20 | 8 | 0 |
| 3. MixConsole | 33 | 28 | 5 | 0 |
| 4. Effects | 32 | 26 | 6 | 0 |
| 5. MIDI | 50 | 36 | 12 | 2 |
| 6. Automation | 26 | 21 | 5 | 0 |
| 7. Mastering & Metering | 18 | 16 | 2 | 0 |
| 8. Surround / Immersive | 22 | 10 | 9 | 3 |
| 9. Game Audio / Post | 22 | 4 | 12 | 6 |
| 10. Control Room | 14 | 12 | 2 | 0 |
| 11. Plugin System | 18 | 15 | 3 | 0 |
| 12. Additional Systems | 38 | 18 | 14 | 6 |
| **TOTAL** | **319** | **220** | **82** | **17** |

### Implementation Priority Phases

**Phase 1 — Core Engine (Critical Foundation)**
- Audio engine (sample rates, bit depths, 64-bit float processing)
- ASIO/Core Audio driver interface
- Basic signal routing (input/output busses, mono/stereo)
- Plugin delay compensation
- Basic transport (play, stop, record, loop)

**Phase 2 — Mixing Infrastructure**
- MixConsole with faders, pan, solo, mute
- Insert effects (16 slots) and send effects (8 sends)
- 4-band parametric EQ per channel
- Channel strip (gate, compressor, EQ, saturation, limiter)
- Group channels and FX channels
- VCA faders
- Basic automation (Touch, Latch, Read/Write)

**Phase 3 — Editing & MIDI**
- Key Editor (piano roll)
- MIDI recording and quantization
- Audio editing (fade, crossfade, time stretch)
- Direct Offline Processing
- VariAudio pitch editing
- Hitpoint detection
- Sampler tracks

**Phase 4 — Advanced Features**
- Full automation system (all modes, Automation Panel, Virgin Territory)
- Direct Routing (8 destinations)
- Control Room (monitors, cues, talkback)
- Loudness metering (EBU R128)
- Expression maps and Note Expression
- Modulators (LFO, Envelope Follower, Step, Macro)
- Surround up to 7.1.4
- MediaBay browser

**Phase 5 — Post-Production & Immersive**
- Dolby Atmos / ADM authoring
- Ambisonics (1st/2nd/3rd order)
- ADR system
- ReConform
- Game Audio Connect
- Video integration
- Network collaboration
- File exchange (AAF, OMF, AES31, DAWproject)
