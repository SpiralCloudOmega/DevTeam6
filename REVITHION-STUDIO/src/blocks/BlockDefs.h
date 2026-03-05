#pragma once

#include "RoutingGrid.h"
#include <string>

namespace revithion::blocks {

// Initialize default parameters for a block based on its type
inline void initBlockDefaults(GridBlock& block) {
    block.params.clear();
    switch (block.type) {
        case BlockType::Amp:
            block.params["gain"] = 5.0f;
            block.params["master"] = 5.0f;
            block.params["treble"] = 5.0f;
            block.params["middle"] = 5.0f;
            block.params["bass"] = 5.0f;
            block.params["presence"] = 5.0f;
            block.params["depth"] = 5.0f;
            block.params["bright"] = 0.0f;
            block.params["saturation"] = 5.0f;
            block.params["inputTrim"] = 0.0f;
            block.params["modelId"] = 0.0f;
            break;
        case BlockType::Cabinet:
            block.params["irIndex"] = 0.0f;
            block.params["lowCut"] = 80.0f;
            block.params["highCut"] = 12000.0f;
            block.params["proximity"] = 0.0f;
            block.params["airLevel"] = 0.0f;
            block.params["micDistance"] = 2.0f;
            break;
        case BlockType::Drive:
            block.params["drive"] = 5.0f;
            block.params["tone"] = 5.0f;
            block.params["level"] = 5.0f;
            block.params["type"] = 0.0f;
            block.params["mix"] = 10.0f;
            break;
        case BlockType::Delay:
            block.params["time"] = 375.0f;
            block.params["feedback"] = 0.35f;
            block.params["mix"] = 0.3f;
            block.params["type"] = 0.0f;
            block.params["modRate"] = 0.5f;
            block.params["modDepth"] = 0.0f;
            block.params["lowCut"] = 100.0f;
            block.params["highCut"] = 8000.0f;
            block.params["spread"] = 0.5f;
            break;
        case BlockType::Reverb:
            block.params["time"] = 2.0f;
            block.params["mix"] = 0.25f;
            block.params["predelay"] = 20.0f;
            block.params["damping"] = 0.5f;
            block.params["size"] = 0.7f;
            block.params["diffusion"] = 0.8f;
            block.params["lowCut"] = 200.0f;
            block.params["highCut"] = 6000.0f;
            block.params["modRate"] = 0.3f;
            block.params["modDepth"] = 0.2f;
            break;
        case BlockType::Chorus:
            block.params["rate"] = 1.0f;
            block.params["depth"] = 0.5f;
            block.params["mix"] = 0.5f;
            block.params["voices"] = 2.0f;
            block.params["spread"] = 0.5f;
            break;
        case BlockType::Flanger:
            block.params["rate"] = 0.3f;
            block.params["depth"] = 0.5f;
            block.params["feedback"] = 0.5f;
            block.params["mix"] = 0.5f;
            break;
        case BlockType::Phaser:
            block.params["rate"] = 0.5f;
            block.params["depth"] = 0.7f;
            block.params["feedback"] = 0.5f;
            block.params["stages"] = 4.0f;
            block.params["mix"] = 0.5f;
            break;
        case BlockType::Compressor:
            block.params["threshold"] = -20.0f;
            block.params["ratio"] = 4.0f;
            block.params["attack"] = 10.0f;
            block.params["release"] = 100.0f;
            block.params["makeupGain"] = 0.0f;
            block.params["knee"] = 3.0f;
            break;
        case BlockType::Gate:
            block.params["threshold"] = -40.0f;
            block.params["attack"] = 1.0f;
            block.params["hold"] = 50.0f;
            block.params["release"] = 100.0f;
            block.params["range"] = -80.0f;
            break;
        case BlockType::ParametricEQ:
            for (int i = 1; i <= 4; ++i) {
                auto prefix = "band" + std::to_string(i);
                block.params[prefix + "Freq"] = 250.0f * static_cast<float>(i);
                block.params[prefix + "Gain"] = 0.0f;
                block.params[prefix + "Q"] = 1.0f;
                block.params[prefix + "Type"] = 0.0f;
            }
            break;
        case BlockType::GraphicEQ:
            for (int i = 0; i < 10; ++i)
                block.params["band" + std::to_string(i)] = 0.0f;
            break;
        case BlockType::PitchShift:
            block.params["shift"] = 0.0f;
            block.params["fine"] = 0.0f;
            block.params["mix"] = 1.0f;
            break;
        case BlockType::Wah:
            block.params["position"] = 0.5f;
            block.params["range"] = 1.0f;
            block.params["type"] = 0.0f;
            block.params["resonance"] = 0.5f;
            break;
        case BlockType::VolumePan:
            block.params["volume"] = 1.0f;
            block.params["pan"] = 0.0f;
            block.params["mute"] = 0.0f;
            break;
        case BlockType::Tremolo:
            block.params["rate"] = 4.0f;
            block.params["depth"] = 0.5f;
            block.params["shape"] = 0.0f;
            break;
        case BlockType::Rotary:
            block.params["speed"] = 0.0f;
            block.params["depth"] = 0.7f;
            block.params["mix"] = 1.0f;
            break;
        default:
            break;
    }
}

// Human-readable display name for each block type
inline const char* blockTypeName(BlockType type) {
    switch (type) {
        case BlockType::Empty:          return "Empty";
        case BlockType::Input:          return "Input";
        case BlockType::Output:         return "Output";
        case BlockType::Mixer:          return "Mixer";
        case BlockType::FeedbackSend:   return "FB Send";
        case BlockType::FeedbackReturn: return "FB Return";
        case BlockType::SendReturn:     return "Send/Ret";
        case BlockType::Splitter:       return "Splitter";
        case BlockType::Multiplexer:    return "Mux";
        case BlockType::Amp:            return "Amp";
        case BlockType::Cabinet:        return "Cabinet";
        case BlockType::ToneMatch:      return "ToneMatch";
        case BlockType::Drive:          return "Drive";
        case BlockType::Enhancer:       return "Enhancer";
        case BlockType::Chorus:         return "Chorus";
        case BlockType::Flanger:        return "Flanger";
        case BlockType::Phaser:         return "Phaser";
        case BlockType::Rotary:         return "Rotary";
        case BlockType::Tremolo:        return "Tremolo";
        case BlockType::FormantShift:   return "Formant";
        case BlockType::Delay:          return "Delay";
        case BlockType::MultitapDelay:  return "Multitap";
        case BlockType::Reverb:         return "Reverb";
        case BlockType::GraphicEQ:      return "GEQ";
        case BlockType::ParametricEQ:   return "PEQ";
        case BlockType::Filter:         return "Filter";
        case BlockType::Gate:           return "Gate";
        case BlockType::Compressor:     return "Comp";
        case BlockType::Multicomp:      return "Multicomp";
        case BlockType::PitchShift:     return "Pitch";
        case BlockType::Harmonizer:     return "Harmony";
        case BlockType::Synth:          return "Synth";
        case BlockType::Vocoder:        return "Vocoder";
        case BlockType::Wah:            return "Wah";
        case BlockType::Looper:         return "Looper";
        case BlockType::Metering:       return "Meter";
        case BlockType::VolumePan:      return "Vol/Pan";
        case BlockType::Crossover:      return "Xover";
        default:                        return "???";
    }
}

// Category grouping for UI block palette
enum class BlockCategory {
    SignalFlow, AmpCab, DriveDist, Modulation,
    Delay, Reverb, EQDynamics, PitchSynth, Utility
};

inline BlockCategory blockCategory(BlockType type) {
    switch (type) {
        case BlockType::Input: case BlockType::Output: case BlockType::Mixer:
        case BlockType::FeedbackSend: case BlockType::FeedbackReturn:
        case BlockType::SendReturn: case BlockType::Splitter: case BlockType::Multiplexer:
            return BlockCategory::SignalFlow;
        case BlockType::Amp: case BlockType::Cabinet: case BlockType::ToneMatch:
            return BlockCategory::AmpCab;
        case BlockType::Drive: case BlockType::Enhancer:
            return BlockCategory::DriveDist;
        case BlockType::Chorus: case BlockType::Flanger: case BlockType::Phaser:
        case BlockType::Rotary: case BlockType::Tremolo: case BlockType::FormantShift:
            return BlockCategory::Modulation;
        case BlockType::Delay: case BlockType::MultitapDelay:
            return BlockCategory::Delay;
        case BlockType::Reverb:
            return BlockCategory::Reverb;
        case BlockType::GraphicEQ: case BlockType::ParametricEQ: case BlockType::Filter:
        case BlockType::Gate: case BlockType::Compressor: case BlockType::Multicomp:
            return BlockCategory::EQDynamics;
        case BlockType::PitchShift: case BlockType::Harmonizer: case BlockType::Synth:
        case BlockType::Vocoder: case BlockType::Wah:
            return BlockCategory::PitchSynth;
        default:
            return BlockCategory::Utility;
    }
}

} // namespace revithion::blocks
