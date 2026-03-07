---
name: spatial-audio
description: Spatial audio processing — stereo width, panning laws, HRTF, Ambisonics, surround formats
---

# Spatial Audio Processing

Spatial audio places sounds in a 3D sound field. From simple stereo panning to full Ambisonics and Dolby Atmos, spatial processing creates the illusion of directionality, distance, and immersion. The three fundamental binaural cues are ITD (interaural time difference — timing offset between ears), ILD (interaural level difference — amplitude offset between ears), and spectral cues introduced by the pinnae filtering sound differently depending on direction of arrival.

## Stereo Panning Laws

### Linear Pan
```cpp
float leftGain = 1.0f - pan;   // pan: 0.0 = hard left, 1.0 = hard right
float rightGain = pan;
```
Problem: center sums to −6 dB relative to hard-panned signal — phantom center sounds quieter.

### Constant Power Pan (Equal Power)
```cpp
float angle = pan * juce::MathConstants<float>::halfPi;  // pan: 0=left, 1=right
float leftGain  = std::cos(angle);
float rightGain = std::sin(angle);
```
At center: each channel −3 dB, power sums to 0 dB. Correct default for nearly all stereo panning.

### −4.5 dB Pan Law
Compromise between linear (−6 dB center) and constant power (−3 dB). Pro Tools default. Useful when the mix will be summed to mono frequently.

## Mid/Side Processing
```cpp
float mid  = (left + right) * 0.5f;   // Encode
float side = (left - right) * 0.5f;
side *= widthFactor;                   // 0 = mono, 1 = unchanged, >1 = widened
float newLeft  = mid + side;           // Decode
float newRight = mid - side;
```
Applications: stereo width control, frequency-selective M/S EQ, M/S compression (compress mid harder to keep vocals controlled).

## HRTF (Head-Related Transfer Function)
An HRTF describes how sound from a specific direction is filtered by the head, pinnae, shoulders, and torso before reaching each eardrum. A dataset contains HRIR (impulse response) pairs measured at hundreds of directions.

- **Databases**: CIPIC (UC Davis, 45 subjects), MIT KEMAR (mannequin), SADIE II (high-res)
- **File format**: SOFA (Spatially Oriented Format for Acoustics) — the modern standard
- **Personalization**: generic HRTFs cause front/back confusion; individualized or ML-estimated HRTFs improve localization

To render a mono source, convolve with the L/R HRIR pair for the target direction. Interpolate between nearby measured positions to avoid jumps.

## Binaural Rendering
Binaural audio is two-channel audio for headphone playback, synthesizing 3D perception via HRTF convolution:

- **ITD**: up to ~0.7 ms at 90° azimuth. Use a fractional delay line per ear with allpass interpolation.
- **ILD**: frequency-dependent level difference, significant above ~1.5 kHz (head shadow).
- **Spectral cues**: pinnae notches/resonances above ~4 kHz encode elevation.

Crossfade between HRIR pairs over 5–20 ms for moving sources to prevent clicks.

## Ambisonics
Ambisonics is a full-sphere surround framework based on Spherical Harmonic (SH) decomposition:

- **FOA (First-Order)**: 4 channels — W (omni), Y (left-right), Z (up-down), X (front-back)
- **HOA (Higher-Order)**: order N requires (N+1)² channels (3rd order = 16 ch), improving spatial resolution
- **AmbiX**: modern standard — SN3D normalization, ACN channel ordering

### FOA Encoding (AmbiX / SN3D)
```cpp
// Encode mono source at (azimuth, elevation) in radians
float W = source;                                              // ACN 0: omnidirectional
float Y = source * std::sin(azimuth);                          // ACN 1: left-right
float Z = source * std::sin(elevation);                        // ACN 2: up-down
float X = source * std::cos(azimuth) * std::cos(elevation);   // ACN 3: front-back
```

Decoding to speakers: multiply SH channels by a decoder matrix (AllRAD, VBAP-based, or mode-matching). For binaural output: decode to a virtual speaker array, then apply HRTF per virtual speaker.

## Surround Formats

| Format | Channels | Use Case |
|--------|----------|----------|
| 5.1 | L, R, C, LFE, Ls, Rs | Film / broadcast standard |
| 7.1 | 5.1 + Lss, Rss (side) | Cinema, gaming |
| 7.1.4 | 7.1 + Ltf, Rtf, Ltr, Rtr | Dolby Atmos bed layer |
| Atmos Objects | Metadata-driven | Dynamic positioning by renderer |

In JUCE, declare multi-channel layouts via `juce::AudioChannelSet` and configure buses in `BusesProperties`. Use `getTotalNumInputChannels()` / `getTotalNumOutputChannels()` at runtime.

## Distance Attenuation

```cpp
// Inverse-distance attenuation clamped at reference distance
float distanceGain(float distance, float refDistance = 1.0f)
{
    return refDistance / std::max(distance, refDistance);
}
```

- **Inverse distance** (1/d): physically correct amplitude decay for a point source in free field
- **Inverse square** (1/d²): intensity-based decay, use when driving loudness parameters
- **Air absorption**: HF attenuates over distance (~−0.2 dB/m at 10 kHz). Model as a one-pole LPF with cutoff decreasing with distance.
- **Proximity effect**: bass boost below ~200 Hz for sources closer than ~0.5 m

## Room Simulation

Spatial audio combined with room acoustics:

1. **Direct sound**: panned or HRTF-convolved, attenuated by distance
2. **Early reflections** (5–80 ms): image-source method, each reflection from a distinct direction
3. **Late reverb** (>80 ms): diffuse, decorrelated across channels (FDN or convolution)
4. **Source directivity**: voice radiates 6–10 dB louder forward above 2 kHz

## Anti-Patterns

- ❌ Don't use linear panning — the phantom center sounds 6 dB quieter than sides
- ❌ Don't apply HRTF filters to loudspeaker output — HRTFs are strictly for headphone rendering
- ❌ Don't ignore ITD — level-only panning sounds flat and internalized on headphones
- ❌ Don't sum Ambisonic channels directly to stereo without a proper decoder matrix
- ❌ Don't forget mono↔stereo and stereo↔surround bus conversions in JUCE `BusesProperties`
- ❌ Don't hardcode channel counts — always query `AudioChannelSet` at runtime

## Checklist

- [ ] Panning law matches output format (constant power for stereo, VBAP for surround)
- [ ] M/S encode and decode are exact inverses — no cumulative gain error
- [ ] HRTF interpolation is smooth between measured positions (crossfade or SH interpolation)
- [ ] Ambisonics uses SN3D normalization and ACN ordering for AmbiX compatibility
- [ ] Distance attenuation clamps at minimum reference distance to prevent infinite gain
- [ ] Binaural rendering models both ITD and ILD, not level-only
- [ ] Multi-channel bus layout declared correctly in JUCE `BusesProperties`
- [ ] All spatial processing respects `prepareToPlay` sample rate for delay and filter calculations
