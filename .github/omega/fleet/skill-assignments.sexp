;; ═══════════════════════════════════════════════════════════════════════
;; REVITHION STUDIO — Omega Skill-to-Agent Assignments
;; ═══════════════════════════════════════════════════════════════════════
;; Skills: 46 base + 6 omega = 52 total
;; Each skill is assigned to agent categories whose members can execute it.
;; At dispatch time, the orchestrator selects the best agent from the
;; assigned categories based on availability and health status.
;; ═══════════════════════════════════════════════════════════════════════

;; ── AGENT ORCHESTRATION ──────────────────────────────────

(skill-assignment :skill "context-management"
                  :path ".github/skills/agent-orchestration/context-management/SKILL.md"
                  :categories ["agent-system" "omega"]
                  :priority normal)

(skill-assignment :skill "multi-agent-coordinator"
                  :path ".github/skills/agent-orchestration/multi-agent-coordinator/SKILL.md"
                  :categories ["agent-system" "omega"]
                  :priority normal)

(skill-assignment :skill "skill-design-patterns"
                  :path ".github/skills/agent-orchestration/skill-design-patterns/SKILL.md"
                  :categories ["agent-system" "omega"]
                  :priority normal)

(skill-assignment :skill "sub-agent-delegation"
                  :path ".github/skills/agent-orchestration/sub-agent-delegation/SKILL.md"
                  :categories ["agent-system" "omega"]
                  :priority normal)

(skill-assignment :skill "yolo-autonomous"
                  :path ".github/skills/agent-orchestration/yolo-autonomous/SKILL.md"
                  :categories ["agent-system" "omega"]
                  :priority normal)

;; ── AI INTEGRATION ──────────────────────────────────

(skill-assignment :skill "ace-step-inference"
                  :path ".github/skills/ai-integration/ace-step-inference/SKILL.md"
                  :categories ["audio" "revithion" "data-ml"]
                  :priority normal)

(skill-assignment :skill "demucs-separation"
                  :path ".github/skills/ai-integration/demucs-separation/SKILL.md"
                  :categories ["audio" "revithion" "data-ml"]
                  :priority normal)

(skill-assignment :skill "matchering-mastering"
                  :path ".github/skills/ai-integration/matchering-mastering/SKILL.md"
                  :categories ["audio" "revithion" "data-ml"]
                  :priority normal)

(skill-assignment :skill "ollama-copilot"
                  :path ".github/skills/ai-integration/ollama-copilot/SKILL.md"
                  :categories ["audio" "revithion" "data-ml"]
                  :priority normal)

(skill-assignment :skill "whisper-transcription"
                  :path ".github/skills/ai-integration/whisper-transcription/SKILL.md"
                  :categories ["audio" "revithion" "data-ml"]
                  :priority normal)

;; ── AUDIO DSP ──────────────────────────────────

(skill-assignment :skill "biquad-filters"
                  :path ".github/skills/audio-dsp/biquad-filters/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

(skill-assignment :skill "delay-reverb"
                  :path ".github/skills/audio-dsp/delay-reverb/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

(skill-assignment :skill "dynamics"
                  :path ".github/skills/audio-dsp/dynamics/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

(skill-assignment :skill "fft-spectral"
                  :path ".github/skills/audio-dsp/fft-spectral/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

(skill-assignment :skill "metering"
                  :path ".github/skills/audio-dsp/metering/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

(skill-assignment :skill "oscillators"
                  :path ".github/skills/audio-dsp/oscillators/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

(skill-assignment :skill "realtime-audio"
                  :path ".github/skills/audio-dsp/realtime-audio/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

(skill-assignment :skill "sample-rate"
                  :path ".github/skills/audio-dsp/sample-rate/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

(skill-assignment :skill "saturation"
                  :path ".github/skills/audio-dsp/saturation/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

(skill-assignment :skill "spatial-audio"
                  :path ".github/skills/audio-dsp/spatial-audio/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

;; ── CPP PATTERNS ──────────────────────────────────

(skill-assignment :skill "cmake-modern"
                  :path ".github/skills/cpp-patterns/cmake-modern/SKILL.md"
                  :categories ["infrastructure" "revithion"]
                  :priority normal)

(skill-assignment :skill "constexpr-generation"
                  :path ".github/skills/cpp-patterns/constexpr-generation/SKILL.md"
                  :categories ["infrastructure" "revithion"]
                  :priority normal)

(skill-assignment :skill "error-handling"
                  :path ".github/skills/cpp-patterns/error-handling/SKILL.md"
                  :categories ["infrastructure" "revithion"]
                  :priority normal)

(skill-assignment :skill "lock-free-audio"
                  :path ".github/skills/cpp-patterns/lock-free-audio/SKILL.md"
                  :categories ["infrastructure" "revithion"]
                  :priority normal)

(skill-assignment :skill "memory-management"
                  :path ".github/skills/cpp-patterns/memory-management/SKILL.md"
                  :categories ["infrastructure" "revithion"]
                  :priority normal)

(skill-assignment :skill "simd-optimization"
                  :path ".github/skills/cpp-patterns/simd-optimization/SKILL.md"
                  :categories ["infrastructure" "revithion"]
                  :priority normal)

;; ── JUCE FRAMEWORK ──────────────────────────────────

(skill-assignment :skill "apvts-parameters"
                  :path ".github/skills/juce-framework/apvts-parameters/SKILL.md"
                  :categories ["infrastructure" "audio"]
                  :priority normal)

(skill-assignment :skill "audio-processor"
                  :path ".github/skills/juce-framework/audio-processor/SKILL.md"
                  :categories ["infrastructure" "audio"]
                  :priority normal)

(skill-assignment :skill "component-ui"
                  :path ".github/skills/juce-framework/component-ui/SKILL.md"
                  :categories ["infrastructure" "audio"]
                  :priority normal)

(skill-assignment :skill "look-and-feel"
                  :path ".github/skills/juce-framework/look-and-feel/SKILL.md"
                  :categories ["infrastructure" "audio"]
                  :priority normal)

(skill-assignment :skill "plugin-hosting"
                  :path ".github/skills/juce-framework/plugin-hosting/SKILL.md"
                  :categories ["infrastructure" "audio"]
                  :priority normal)

(skill-assignment :skill "threading-model"
                  :path ".github/skills/juce-framework/threading-model/SKILL.md"
                  :categories ["infrastructure" "audio"]
                  :priority normal)

;; ── MUSIC PRODUCTION ──────────────────────────────────

(skill-assignment :skill "automation"
                  :path ".github/skills/music-production/automation/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

(skill-assignment :skill "loudness-metering"
                  :path ".github/skills/music-production/loudness-metering/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

(skill-assignment :skill "mastering-chain"
                  :path ".github/skills/music-production/mastering-chain/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

(skill-assignment :skill "midi-system"
                  :path ".github/skills/music-production/midi-system/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

(skill-assignment :skill "mixing-techniques"
                  :path ".github/skills/music-production/mixing-techniques/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

(skill-assignment :skill "routing-grid"
                  :path ".github/skills/music-production/routing-grid/SKILL.md"
                  :categories ["audio" "revithion"]
                  :priority normal)

;; ── NEURAL AUDIO ──────────────────────────────────

(skill-assignment :skill "ai-mastering"
                  :path ".github/skills/neural-audio/ai-mastering/SKILL.md"
                  :categories ["audio" "infrastructure" "data-ml"]
                  :priority normal)

(skill-assignment :skill "amp-training"
                  :path ".github/skills/neural-audio/amp-training/SKILL.md"
                  :categories ["audio" "infrastructure" "data-ml"]
                  :priority normal)

(skill-assignment :skill "cab-modeling"
                  :path ".github/skills/neural-audio/cab-modeling/SKILL.md"
                  :categories ["audio" "infrastructure" "data-ml"]
                  :priority normal)

(skill-assignment :skill "music-generation"
                  :path ".github/skills/neural-audio/music-generation/SKILL.md"
                  :categories ["audio" "infrastructure" "data-ml"]
                  :priority normal)

(skill-assignment :skill "rtneural-inference"
                  :path ".github/skills/neural-audio/rtneural-inference/SKILL.md"
                  :categories ["audio" "infrastructure" "data-ml"]
                  :priority normal)

(skill-assignment :skill "stem-separation"
                  :path ".github/skills/neural-audio/stem-separation/SKILL.md"
                  :categories ["audio" "infrastructure" "data-ml"]
                  :priority normal)

(skill-assignment :skill "voice-processing"
                  :path ".github/skills/neural-audio/voice-processing/SKILL.md"
                  :categories ["audio" "infrastructure" "data-ml"]
                  :priority normal)

(skill-assignment :skill "wavenet-convnet"
                  :path ".github/skills/neural-audio/wavenet-convnet/SKILL.md"
                  :categories ["audio" "infrastructure" "data-ml"]
                  :priority normal)

;; ── OMEGA SKILLS ──────────────────────────────────────────

(skill-assignment :skill "omega-context-efficiency"
                  :path ".github/omega/skills/omega-context-efficiency/SKILL.md"
                  :categories ["omega" "agent-system"]
                  :priority critical)

(skill-assignment :skill "omega-critique"
                  :path ".github/omega/skills/omega-critique/SKILL.md"
                  :categories ["omega" "agent-system"]
                  :priority critical)

(skill-assignment :skill "omega-fleet-management"
                  :path ".github/omega/skills/omega-fleet-management/SKILL.md"
                  :categories ["omega" "agent-system"]
                  :priority critical)

(skill-assignment :skill "omega-orchestration"
                  :path ".github/omega/skills/omega-orchestration/SKILL.md"
                  :categories ["omega" "agent-system"]
                  :priority critical)

(skill-assignment :skill "omega-recursive-delegation"
                  :path ".github/omega/skills/omega-recursive-delegation/SKILL.md"
                  :categories ["omega" "agent-system"]
                  :priority critical)

(skill-assignment :skill "omega-terminal-bridge"
                  :path ".github/omega/skills/omega-terminal-bridge/SKILL.md"
                  :categories ["omega" "agent-system"]
                  :priority critical)

;; ═══════════════════════════════════════════════════════════════════════
;; END OF SKILL ASSIGNMENTS — 52 skills mapped
;; ═══════════════════════════════════════════════════════════════════════
