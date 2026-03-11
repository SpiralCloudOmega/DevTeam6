;; ═══════════════════════════════════════════════════════════════════════
;; REVITHION STUDIO — Omega Agent Fleet Registry
;; ═══════════════════════════════════════════════════════════════════════
;; Generated: 2026-03-11T14:42:40.343318
;; Total agents: 243 (236 custom + 7 omega)
;; Source: .github/agents/*.agent.md + .github/omega/agents/*.agent.md
;;
;; This file catalogs every agent in the system with its category,
;; skill assignments, critic wiring, and heartbeat interval.
;; ═══════════════════════════════════════════════════════════════════════

(require "../../omega.config.sexp")
(require "critic-assignments.sexp")
(require "heartbeat-config.sexp")

;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CATEGORY: AGENT-SYSTEM                                             ║
;; ║  Agents: 3                                                          ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "context-loader"
             :path ".github/agents/context-loader.agent.md"
             :category "agent-system"
             :skills ["context-management" "memory-persistence" "session-recording" "agent-orchestration"]
             :critic "omega-security"
             :heartbeat-interval 5)

(agent-entry :name "memory-keeper"
             :path ".github/agents/memory-keeper.agent.md"
             :category "agent-system"
             :skills ["context-management" "memory-persistence" "session-recording" "agent-orchestration"]
             :critic "omega-security"
             :heartbeat-interval 5)

(agent-entry :name "session-recorder"
             :path ".github/agents/session-recorder.agent.md"
             :category "agent-system"
             :skills ["context-management" "memory-persistence" "session-recording" "agent-orchestration"]
             :critic "omega-security"
             :heartbeat-interval 5)

;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CATEGORY: AUDIO                                                    ║
;; ║  Agents: 8                                                          ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "ace-step-expert"
             :path ".github/agents/ace-step-expert.agent.md"
             :category "audio"
             :skills ["audio-dsp" "faust-dsp" "spectral-analysis" "mastering"]
             :critic "omega-critic"
             :heartbeat-interval 15)

(agent-entry :name "amp-modeling-expert"
             :path ".github/agents/amp-modeling-expert.agent.md"
             :category "audio"
             :skills ["audio-dsp" "faust-dsp" "spectral-analysis" "mastering"]
             :critic "omega-critic"
             :heartbeat-interval 15)

(agent-entry :name "audio-dsp-expert"
             :path ".github/agents/audio-dsp-expert.agent.md"
             :category "audio"
             :skills ["audio-dsp" "faust-dsp" "spectral-analysis" "mastering"]
             :critic "omega-critic"
             :heartbeat-interval 15)

(agent-entry :name "faust-dsp-expert"
             :path ".github/agents/faust-dsp-expert.agent.md"
             :category "audio"
             :skills ["audio-dsp" "faust-dsp" "spectral-analysis" "mastering"]
             :critic "omega-critic"
             :heartbeat-interval 15)

(agent-entry :name "mastering-engineer"
             :path ".github/agents/mastering-engineer.agent.md"
             :category "audio"
             :skills ["audio-dsp" "faust-dsp" "spectral-analysis" "mastering"]
             :critic "omega-critic"
             :heartbeat-interval 15)

(agent-entry :name "midi-expert"
             :path ".github/agents/midi-expert.agent.md"
             :category "audio"
             :skills ["audio-dsp" "faust-dsp" "spectral-analysis" "mastering"]
             :critic "omega-critic"
             :heartbeat-interval 15)

(agent-entry :name "mix-engineer"
             :path ".github/agents/mix-engineer.agent.md"
             :category "audio"
             :skills ["audio-dsp" "faust-dsp" "spectral-analysis" "mastering"]
             :critic "omega-critic"
             :heartbeat-interval 15)

;; ╔═══════════════════════════════════════════════════════════════════╗
(agent-entry :name "spectral-analysis-expert"
             :path ".github/agents/spectral-analysis-expert.agent.md"
             :category "audio"
             :skills ["audio-dsp" "faust-dsp" "spectral-analysis" "mastering"]
             :critic "omega-critic"
             :heartbeat-interval 15)

;; ║  CATEGORY: DATA-ML                                                  ║
;; ║  Agents: 5                                                          ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "linux-nvidia-cuda-python"
             :path ".github/agents/linux-nvidia-cuda-python.agent.md"
             :category "data-ml"
             :skills ["pandas" "pytorch" "scikit-learn" "deep-learning"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "pandas-scikit-learn-guide"
             :path ".github/agents/pandas-scikit-learn-guide.agent.md"
             :category "data-ml"
             :skills ["pandas" "pytorch" "scikit-learn" "deep-learning"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "pyqt6-eeg-processing"
             :path ".github/agents/pyqt6-eeg-processing.agent.md"
             :category "data-ml"
             :skills ["pandas" "pytorch" "scikit-learn" "deep-learning"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "python-llm-ml-workflow"
             :path ".github/agents/python-llm-ml-workflow.agent.md"
             :category "data-ml"
             :skills ["pandas" "pytorch" "scikit-learn" "deep-learning"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "pytorch-scikit-learn"
             :path ".github/agents/pytorch-scikit-learn.agent.md"
             :category "data-ml"
             :skills ["pandas" "pytorch" "scikit-learn" "deep-learning"]
             :critic "omega-critic"
             :heartbeat-interval 30)

;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CATEGORY: DEVOPS                                                   ║
;; ║  Agents: 7                                                          ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "git-conventional-commit-messages"
             :path ".github/agents/git-conventional-commit-messages.agent.md"
             :category "devops"
             :skills ["git" "github-actions" "docker" "kubernetes"]
             :critic "omega-security"
             :heartbeat-interval 30)

(agent-entry :name "gitflow-modern"
             :path ".github/agents/gitflow-modern.agent.md"
             :category "devops"
             :skills ["git" "github-actions" "docker" "kubernetes"]
             :critic "omega-security"
             :heartbeat-interval 30)

(agent-entry :name "github-code-quality"
             :path ".github/agents/github-code-quality.agent.md"
             :category "devops"
             :skills ["git" "github-actions" "docker" "kubernetes"]
             :critic "omega-security"
             :heartbeat-interval 30)

(agent-entry :name "github-cursorrules-prompt-file-instructions"
             :path ".github/agents/github-cursorrules-prompt-file-instructions.agent.md"
             :category "devops"
             :skills ["git" "github-actions" "docker" "kubernetes"]
             :critic "omega-security"
             :heartbeat-interval 30)

(agent-entry :name "kubernetes-mkdocs-documentation-cursorrules-prompt"
             :path ".github/agents/kubernetes-mkdocs-documentation-cursorrules-prompt.agent.md"
             :category "devops"
             :skills ["git" "github-actions" "docker" "kubernetes"]
             :critic "omega-security"
             :heartbeat-interval 30)

(agent-entry :name "manifest-yaml"
             :path ".github/agents/manifest-yaml.agent.md"
             :category "devops"
             :skills ["git" "github-actions" "docker" "kubernetes"]
             :critic "omega-security"
             :heartbeat-interval 30)

(agent-entry :name "netlify-official"
             :path ".github/agents/netlify-official.agent.md"
             :category "devops"
             :skills ["git" "github-actions" "docker" "kubernetes"]
             :critic "omega-security"
             :heartbeat-interval 30)

;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CATEGORY: DOCUMENTATION                                            ║
;; ║  Agents: 7                                                          ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "engineering-ticket-template"
             :path ".github/agents/engineering-ticket-template.agent.md"
             :category "documentation"
             :skills ["technical-writing" "test-cases" "bug-reports" "api-docs"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "how-to-documentation"
             :path ".github/agents/how-to-documentation.agent.md"
             :category "documentation"
             :skills ["technical-writing" "test-cases" "bug-reports" "api-docs"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "pr-template"
             :path ".github/agents/pr-template.agent.md"
             :category "documentation"
             :skills ["technical-writing" "test-cases" "bug-reports" "api-docs"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "project-epic-template"
             :path ".github/agents/project-epic-template.agent.md"
             :category "documentation"
             :skills ["technical-writing" "test-cases" "bug-reports" "api-docs"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "qa-bug-report"
             :path ".github/agents/qa-bug-report.agent.md"
             :category "documentation"
             :skills ["technical-writing" "test-cases" "bug-reports" "api-docs"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "testrail-test-case"
             :path ".github/agents/testrail-test-case.agent.md"
             :category "documentation"
             :skills ["technical-writing" "test-cases" "bug-reports" "api-docs"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "xray-test-case"
             :path ".github/agents/xray-test-case.agent.md"
             :category "documentation"
             :skills ["technical-writing" "test-cases" "bug-reports" "api-docs"]
             :critic "omega-critic"
             :heartbeat-interval 60)

;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CATEGORY: GAME-DEV                                                 ║
;; ║  Agents: 5                                                          ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "ascii-simulation-game"
             :path ".github/agents/ascii-simulation-game.agent.md"
             :category "game-dev"
             :skills ["unity" "unreal-engine" "game-physics" "game-design"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "dragonruby-best-practices"
             :path ".github/agents/dragonruby-best-practices.agent.md"
             :category "game-dev"
             :skills ["unity" "unreal-engine" "game-physics" "game-design"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "graphical-apps-development"
             :path ".github/agents/graphical-apps-development.agent.md"
             :category "game-dev"
             :skills ["unity" "unreal-engine" "game-physics" "game-design"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "unity-cursor-ai-c"
             :path ".github/agents/unity-cursor-ai-c.agent.md"
             :category "game-dev"
             :skills ["unity" "unreal-engine" "game-physics" "game-design"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "webassembly-z80-cellular-automata-cursorrules-prom"
             :path ".github/agents/webassembly-z80-cellular-automata-cursorrules-prom.agent.md"
             :category "game-dev"
             :skills ["unity" "unreal-engine" "game-physics" "game-design"]
             :critic "omega-critic"
             :heartbeat-interval 30)

;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CATEGORY: INFRASTRUCTURE                                           ║
;; ║  Agents: 7                                                          ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "cmake-cpp20-expert"
             :path ".github/agents/cmake-cpp20-expert.agent.md"
             :category "infrastructure"
             :skills ["cmake" "cpp20" "juce-framework" "plugin-formats"]
             :critic "omega-critic"
             :heartbeat-interval 15)

(agent-entry :name "cpp-modern"
             :path ".github/agents/cpp-modern.agent.md"
             :category "infrastructure"
             :skills ["cmake" "cpp20" "juce-framework" "plugin-formats"]
             :critic "omega-critic"
             :heartbeat-interval 15)

(agent-entry :name "cpp-programming-guidelines"
             :path ".github/agents/cpp-programming-guidelines.agent.md"
             :category "infrastructure"
             :skills ["cmake" "cpp20" "juce-framework" "plugin-formats"]
             :critic "omega-critic"
             :heartbeat-interval 15)

(agent-entry :name "juce-framework-expert"
             :path ".github/agents/juce-framework-expert.agent.md"
             :category "infrastructure"
             :skills ["cmake" "cpp20" "juce-framework" "plugin-formats"]
             :critic "omega-critic"
             :heartbeat-interval 15)

(agent-entry :name "neural-inference-expert"
             :path ".github/agents/neural-inference-expert.agent.md"
             :category "infrastructure"
             :skills ["cmake" "cpp20" "juce-framework" "plugin-formats"]
             :critic "omega-critic"
             :heartbeat-interval 15)

(agent-entry :name "plugin-format-expert"
             :path ".github/agents/plugin-format-expert.agent.md"
             :category "infrastructure"
             :skills ["cmake" "cpp20" "juce-framework" "plugin-formats"]
             :critic "omega-critic"
             :heartbeat-interval 15)

(agent-entry :name "vulkan-compute-expert"
             :path ".github/agents/vulkan-compute-expert.agent.md"
             :category "infrastructure"
             :skills ["cmake" "cpp20" "juce-framework" "plugin-formats"]
             :critic "omega-critic"
             :heartbeat-interval 15)

;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CATEGORY: MOBILE                                                   ║
;; ║  Agents: 11                                                         ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "android-jetpack-compose"
             :path ".github/agents/android-jetpack-compose.agent.md"
             :category "mobile"
             :skills ["flutter" "react-native" "swift" "android"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "flutter-app-expert"
             :path ".github/agents/flutter-app-expert.agent.md"
             :category "mobile"
             :skills ["flutter" "react-native" "swift" "android"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "flutter-riverpod"
             :path ".github/agents/flutter-riverpod.agent.md"
             :category "mobile"
             :skills ["flutter" "react-native" "swift" "android"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nativescript"
             :path ".github/agents/nativescript.agent.md"
             :category "mobile"
             :skills ["flutter" "react-native" "swift" "android"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nativescript-modern"
             :path ".github/agents/nativescript-modern.agent.md"
             :category "mobile"
             :skills ["flutter" "react-native" "swift" "android"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "react-native-expo"
             :path ".github/agents/react-native-expo.agent.md"
             :category "mobile"
             :skills ["flutter" "react-native" "swift" "android"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "react-native-expo-router-typescript-windows"
             :path ".github/agents/react-native-expo-router-typescript-windows.agent.md"
             :category "mobile"
             :skills ["flutter" "react-native" "swift" "android"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "swift-uikit"
             :path ".github/agents/swift-uikit.agent.md"
             :category "mobile"
             :skills ["flutter" "react-native" "swift" "android"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "swiftui-guidelines"
             :path ".github/agents/swiftui-guidelines.agent.md"
             :category "mobile"
             :skills ["flutter" "react-native" "swift" "android"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-expo-jest-detox"
             :path ".github/agents/typescript-expo-jest-detox.agent.md"
             :category "mobile"
             :skills ["flutter" "react-native" "swift" "android"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "uikit-guidelines"
             :path ".github/agents/uikit-guidelines.agent.md"
             :category "mobile"
             :skills ["flutter" "react-native" "swift" "android"]
             :critic "omega-critic"
             :heartbeat-interval 30)

;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CATEGORY: OMEGA                                                    ║
;; ║  Agents: 7                                                          ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "omega-builder"
             :path ".github/omega/agents/omega-builder.agent.md"
             :category "omega"
             :skills ["general-purpose"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "omega-critic"
             :path ".github/omega/agents/omega-critic.agent.md"
             :category "omega"
             :skills ["general-purpose"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "omega-fleet-commander"
             :path ".github/omega/agents/omega-fleet-commander.agent.md"
             :category "omega"
             :skills ["general-purpose"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "omega-heartbeat"
             :path ".github/omega/agents/omega-heartbeat.agent.md"
             :category "omega"
             :skills ["general-purpose"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "omega-orchestrator"
             :path ".github/omega/agents/omega-orchestrator.agent.md"
             :category "omega"
             :skills ["general-purpose"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "omega-researcher"
             :path ".github/omega/agents/omega-researcher.agent.md"
             :category "omega"
             :skills ["general-purpose"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "omega-security"
             :path ".github/omega/agents/omega-security.agent.md"
             :category "omega"
             :skills ["general-purpose"]
             :critic "omega-critic"
             :heartbeat-interval 30)

;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CATEGORY: PERSONAL                                                 ║
;; ║  Agents: 22                                                         ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "my-bugfixer"
             :path ".github/agents/my-bugfixer.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-c++"
             :path ".github/agents/my-c++.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-content.eval"
             :path ".github/agents/my-content.eval.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-context7"
             :path ".github/agents/my-context7.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-electron"
             :path ".github/agents/my-electron.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-gpt5beast"
             :path ".github/agents/my-gpt5beast.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-hlbpa"
             :path ".github/agents/my-hlbpa.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-idea.generator"
             :path ".github/agents/my-idea.generator.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-javamcp"
             :path ".github/agents/my-javamcp.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-mentor"
             :path ".github/agents/my-mentor.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-neo4j"
             :path ".github/agents/my-neo4j.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-nextjs"
             :path ".github/agents/my-nextjs.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-php"
             :path ".github/agents/my-php.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-planner"
             :path ".github/agents/my-planner.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-prd"
             :path ".github/agents/my-prd.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-python.mcp.server"
             :path ".github/agents/my-python.mcp.server.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-react"
             :path ".github/agents/my-react.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-research"
             :path ".github/agents/my-research.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-software.expert"
             :path ".github/agents/my-software.expert.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-specification"
             :path ".github/agents/my-specification.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-terraform"
             :path ".github/agents/my-terraform.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "my-transcendent.coding"
             :path ".github/agents/my-transcendent.coding.agent.md"
             :category "personal"
             :skills ["custom-workflows" "personal-tooling" "project-specific"]
             :critic "omega-critic"
             :heartbeat-interval 30)

;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CATEGORY: QUALITY                                                  ║
;; ║  Agents: 7                                                          ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "clean-code-modern"
             :path ".github/agents/clean-code-modern.agent.md"
             :category "quality"
             :skills ["code-review" "clean-code" "solid-principles" "refactoring"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "code-guidelines"
             :path ".github/agents/code-guidelines.agent.md"
             :category "quality"
             :skills ["code-review" "clean-code" "solid-principles" "refactoring"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "code-pair-interviews"
             :path ".github/agents/code-pair-interviews.agent.md"
             :category "quality"
             :skills ["code-review" "clean-code" "solid-principles" "refactoring"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "code-style-consistency"
             :path ".github/agents/code-style-consistency.agent.md"
             :category "quality"
             :skills ["code-review" "clean-code" "solid-principles" "refactoring"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "codequality-modern"
             :path ".github/agents/codequality-modern.agent.md"
             :category "quality"
             :skills ["code-review" "clean-code" "solid-principles" "refactoring"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "javascript-typescript-code-quality"
             :path ".github/agents/javascript-typescript-code-quality.agent.md"
             :category "quality"
             :skills ["code-review" "clean-code" "solid-principles" "refactoring"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "optimize-dry-solid-principles-cursorrules-prompt-f"
             :path ".github/agents/optimize-dry-solid-principles-cursorrules-prompt-f.agent.md"
             :category "quality"
             :skills ["code-review" "clean-code" "solid-principles" "refactoring"]
             :critic "omega-critic"
             :heartbeat-interval 60)

;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CATEGORY: REVITHION                                                ║
;; ║  Agents: 2                                                          ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "lisp-token-expert"
             :path ".github/agents/lisp-token-expert.agent.md"
             :category "revithion"
             :skills ["lisp-tokens" "sexp-macros" "routing-grid" "axe-fx-modeling"]
             :critic "omega-critic"
             :heartbeat-interval 15)

(agent-entry :name "routing-grid-expert"
             :path ".github/agents/routing-grid-expert.agent.md"
             :category "revithion"
             :skills ["lisp-tokens" "sexp-macros" "routing-grid" "axe-fx-modeling"]
             :critic "omega-critic"
             :heartbeat-interval 15)

;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CATEGORY: SECURITY                                                 ║
;; ║  Agents: 6                                                          ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "optimize-rell-blockchain-code-cursorrules-prompt-f"
             :path ".github/agents/optimize-rell-blockchain-code-cursorrules-prompt-f.agent.md"
             :category "security"
             :skills ["smart-contracts" "solidity" "blockchain" "vulnerability-scanning"]
             :critic "omega-security"
             :heartbeat-interval 30)

(agent-entry :name "solidity-foundry"
             :path ".github/agents/solidity-foundry.agent.md"
             :category "security"
             :skills ["smart-contracts" "solidity" "blockchain" "vulnerability-scanning"]
             :critic "omega-security"
             :heartbeat-interval 30)

(agent-entry :name "solidity-hardhat"
             :path ".github/agents/solidity-hardhat.agent.md"
             :category "security"
             :skills ["smart-contracts" "solidity" "blockchain" "vulnerability-scanning"]
             :critic "omega-security"
             :heartbeat-interval 30)

(agent-entry :name "solidity-react-blockchain-apps-cursorrules-prompt-"
             :path ".github/agents/solidity-react-blockchain-apps-cursorrules-prompt-.agent.md"
             :category "security"
             :skills ["smart-contracts" "solidity" "blockchain" "vulnerability-scanning"]
             :critic "omega-security"
             :heartbeat-interval 30)

(agent-entry :name "wordpress-php-guzzle-gutenberg-cursorrules-prompt-"
             :path ".github/agents/wordpress-php-guzzle-gutenberg-cursorrules-prompt-.agent.md"
             :category "security"
             :skills ["smart-contracts" "solidity" "blockchain" "vulnerability-scanning"]
             :critic "omega-security"
             :heartbeat-interval 30)

(agent-entry :name "xian-smart-contracts-cursor-rules"
             :path ".github/agents/xian-smart-contracts-cursor-rules.agent.md"
             :category "security"
             :skills ["smart-contracts" "solidity" "blockchain" "vulnerability-scanning"]
             :critic "omega-security"
             :heartbeat-interval 30)

;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CATEGORY: TESTING                                                  ║
;; ║  Agents: 13                                                         ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "cypress-accessibility-testing"
             :path ".github/agents/cypress-accessibility-testing.agent.md"
             :category "testing"
             :skills ["e2e-testing" "unit-testing" "integration-testing" "accessibility-testing"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "cypress-api-testing"
             :path ".github/agents/cypress-api-testing.agent.md"
             :category "testing"
             :skills ["e2e-testing" "unit-testing" "integration-testing" "accessibility-testing"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "cypress-defect-tracking"
             :path ".github/agents/cypress-defect-tracking.agent.md"
             :category "testing"
             :skills ["e2e-testing" "unit-testing" "integration-testing" "accessibility-testing"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "cypress-e2e-testing"
             :path ".github/agents/cypress-e2e-testing.agent.md"
             :category "testing"
             :skills ["e2e-testing" "unit-testing" "integration-testing" "accessibility-testing"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "cypress-integration-testing"
             :path ".github/agents/cypress-integration-testing.agent.md"
             :category "testing"
             :skills ["e2e-testing" "unit-testing" "integration-testing" "accessibility-testing"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "gherkin-style-testing"
             :path ".github/agents/gherkin-style-testing.agent.md"
             :category "testing"
             :skills ["e2e-testing" "unit-testing" "integration-testing" "accessibility-testing"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "jest-unit-testing"
             :path ".github/agents/jest-unit-testing.agent.md"
             :category "testing"
             :skills ["e2e-testing" "unit-testing" "integration-testing" "accessibility-testing"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "playwright-accessibility-testing"
             :path ".github/agents/playwright-accessibility-testing.agent.md"
             :category "testing"
             :skills ["e2e-testing" "unit-testing" "integration-testing" "accessibility-testing"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "playwright-api-testing"
             :path ".github/agents/playwright-api-testing.agent.md"
             :category "testing"
             :skills ["e2e-testing" "unit-testing" "integration-testing" "accessibility-testing"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "playwright-defect-tracking"
             :path ".github/agents/playwright-defect-tracking.agent.md"
             :category "testing"
             :skills ["e2e-testing" "unit-testing" "integration-testing" "accessibility-testing"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "playwright-e2e-testing"
             :path ".github/agents/playwright-e2e-testing.agent.md"
             :category "testing"
             :skills ["e2e-testing" "unit-testing" "integration-testing" "accessibility-testing"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "playwright-integration-testing"
             :path ".github/agents/playwright-integration-testing.agent.md"
             :category "testing"
             :skills ["e2e-testing" "unit-testing" "integration-testing" "accessibility-testing"]
             :critic "omega-critic"
             :heartbeat-interval 60)

(agent-entry :name "vitest-unit-testing"
             :path ".github/agents/vitest-unit-testing.agent.md"
             :category "testing"
             :skills ["e2e-testing" "unit-testing" "integration-testing" "accessibility-testing"]
             :critic "omega-critic"
             :heartbeat-interval 60)

;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CATEGORY: WEB-BACKEND                                              ║
;; ║  Agents: 51                                                         ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "aspnet-abp"
             :path ".github/agents/aspnet-abp.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "convex"
             :path ".github/agents/convex.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "cursorrules-cursor-ai-wordpress-draft-macos-prompt"
             :path ".github/agents/cursorrules-cursor-ai-wordpress-draft-macos-prompt.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "cursorrules-file-cursor-ai-python-fastapi-api"
             :path ".github/agents/cursorrules-file-cursor-ai-python-fastapi-api.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "database-modern"
             :path ".github/agents/database-modern.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "deno-integration-techniques"
             :path ".github/agents/deno-integration-techniques.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "drupal-11"
             :path ".github/agents/drupal-11.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "elixir-engineer-guidelines"
             :path ".github/agents/elixir-engineer-guidelines.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "elixir-phoenix-docker-setup"
             :path ".github/agents/elixir-phoenix-docker-setup.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "es-module-nodejs-guidelines"
             :path ".github/agents/es-module-nodejs-guidelines.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "fastapi-modern"
             :path ".github/agents/fastapi-modern.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "go-backend-scalability"
             :path ".github/agents/go-backend-scalability.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "go-servemux-rest-api"
             :path ".github/agents/go-servemux-rest-api.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "go-temporal-dsl"
             :path ".github/agents/go-temporal-dsl.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "java-general-purpose"
             :path ".github/agents/java-general-purpose.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "java-springboot-jpa"
             :path ".github/agents/java-springboot-jpa.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "knative-istio-typesense-gpu"
             :path ".github/agents/knative-istio-typesense-gpu.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "kotlin-ktor-development"
             :path ".github/agents/kotlin-ktor-development.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "kotlin-springboot-best-practices"
             :path ".github/agents/kotlin-springboot-best-practices.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "laravel-php-83"
             :path ".github/agents/laravel-php-83.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "laravel-tall-stack-best-practices-cursorrules-prom"
             :path ".github/agents/laravel-tall-stack-best-practices-cursorrules-prom.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "medusa"
             :path ".github/agents/medusa.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "medusa-modern"
             :path ".github/agents/medusa-modern.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "node-express-modern"
             :path ".github/agents/node-express-modern.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nodejs-mongodb-cursorrules-prompt-file-tutorial"
             :path ".github/agents/nodejs-mongodb-cursorrules-prompt-file-tutorial.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nodejs-mongodb-jwt-express-react"
             :path ".github/agents/nodejs-mongodb-jwt-express-react.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "plasticode-telegram-api"
             :path ".github/agents/plasticode-telegram-api.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "py-fast-api"
             :path ".github/agents/py-fast-api.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "python--typescript-guide"
             :path ".github/agents/python--typescript-guide.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "python-312-fastapi-best-practices-cursorrules-prom"
             :path ".github/agents/python-312-fastapi-best-practices-cursorrules-prom.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "python-containerization"
             :path ".github/agents/python-containerization.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "python-cursorrules-prompt-file-best-practices"
             :path ".github/agents/python-cursorrules-prompt-file-best-practices.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "python-developer"
             :path ".github/agents/python-developer.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "python-django-best-practices-cursorrules-prompt-fi"
             :path ".github/agents/python-django-best-practices-cursorrules-prompt-fi.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "python-fastapi"
             :path ".github/agents/python-fastapi.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "python-fastapi-best-practices-cursorrules-prompt-f"
             :path ".github/agents/python-fastapi-best-practices-cursorrules-prompt-f.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "python-fastapi-scalable-api"
             :path ".github/agents/python-fastapi-scalable-api.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "python-flask-json-guide"
             :path ".github/agents/python-flask-json-guide.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "python-github-setup"
             :path ".github/agents/python-github-setup.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "python-modern"
             :path ".github/agents/python-modern.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "python-projects-guide"
             :path ".github/agents/python-projects-guide.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "r-cursorrules-prompt-file-best-practices"
             :path ".github/agents/r-cursorrules-prompt-file-best-practices.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "rails"
             :path ".github/agents/rails.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "rust-modern"
             :path ".github/agents/rust-modern.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "salesforce-apex"
             :path ".github/agents/salesforce-apex.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "scala-kafka"
             :path ".github/agents/scala-kafka.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "temporal-python"
             :path ".github/agents/temporal-python.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-nestjs-best-practices"
             :path ".github/agents/typescript-nestjs-best-practices.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-nodejs-nextjs-ai"
             :path ".github/agents/typescript-nodejs-nextjs-ai.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-nodejs-nextjs-app-cursorrules-prompt-fi"
             :path ".github/agents/typescript-nodejs-nextjs-app-cursorrules-prompt-fi.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typo3cms-extension"
             :path ".github/agents/typo3cms-extension.agent.md"
             :category "web-backend"
             :skills ["python" "fastapi" "django" "nodejs"]
             :critic "omega-critic"
             :heartbeat-interval 30)

;; ╔═══════════════════════════════════════════════════════════════════╗
;; ║  CATEGORY: WEB-FRONTEND                                             ║
;; ║  Agents: 82                                                         ║
;; ╚═══════════════════════════════════════════════════════════════════╝

(agent-entry :name "angular-novo-elements"
             :path ".github/agents/angular-novo-elements.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "angular-typescript"
             :path ".github/agents/angular-typescript.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "astro-typescript"
             :path ".github/agents/astro-typescript.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "beefreeSDK-modern"
             :path ".github/agents/beefreeSDK-modern.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "beefreeSDK-nocode-content-editor"
             :path ".github/agents/beefreeSDK-nocode-content-editor.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "chrome-extension-dev-js-typescript"
             :path ".github/agents/chrome-extension-dev-js-typescript.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "cursor-ai-react-typescript-shadcn-ui"
             :path ".github/agents/cursor-ai-react-typescript-shadcn-ui.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "cursorrules-cursor-ai-nextjs-14-tailwind-seo-setup"
             :path ".github/agents/cursorrules-cursor-ai-nextjs-14-tailwind-seo-setup.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "html-tailwind-css-javascript-cursorrules-prompt-fi"
             :path ".github/agents/html-tailwind-css-javascript-cursorrules-prompt-fi.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "htmx-basic"
             :path ".github/agents/htmx-basic.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "htmx-django"
             :path ".github/agents/htmx-django.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "htmx-flask"
             :path ".github/agents/htmx-flask.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "htmx-go-basic"
             :path ".github/agents/htmx-go-basic.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "htmx-go-fiber"
             :path ".github/agents/htmx-go-fiber.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "javascript-astro-tailwind-css-cursorrules-prompt-f"
             :path ".github/agents/javascript-astro-tailwind-css-cursorrules-prompt-f.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "javascript-chrome-apis"
             :path ".github/agents/javascript-chrome-apis.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "next-type-llm"
             :path ".github/agents/next-type-llm.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nextjs-app-router"
             :path ".github/agents/nextjs-app-router.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nextjs-material-ui-tailwind-css-cursorrules-prompt"
             :path ".github/agents/nextjs-material-ui-tailwind-css-cursorrules-prompt.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nextjs-modern"
             :path ".github/agents/nextjs-modern.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nextjs-react-tailwind"
             :path ".github/agents/nextjs-react-tailwind.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nextjs-react-typescript"
             :path ".github/agents/nextjs-react-typescript.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nextjs-seo-dev"
             :path ".github/agents/nextjs-seo-dev.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nextjs-supabase-shadcn-pwa"
             :path ".github/agents/nextjs-supabase-shadcn-pwa.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nextjs-supabase-todo-app"
             :path ".github/agents/nextjs-supabase-todo-app.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nextjs-tailwind-typescript-apps-cursorrules-prompt"
             :path ".github/agents/nextjs-tailwind-typescript-apps-cursorrules-prompt.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nextjs-typescript"
             :path ".github/agents/nextjs-typescript.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nextjs-typescript-app"
             :path ".github/agents/nextjs-typescript-app.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nextjs-typescript-tailwind"
             :path ".github/agents/nextjs-typescript-tailwind.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nextjs-vercel-supabase"
             :path ".github/agents/nextjs-vercel-supabase.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nextjs-vercel-typescript"
             :path ".github/agents/nextjs-vercel-typescript.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "nextjs15-react19-vercelai-tailwind"
             :path ".github/agents/nextjs15-react19-vercelai-tailwind.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "qwik-basic"
             :path ".github/agents/qwik-basic.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "qwik-tailwind"
             :path ".github/agents/qwik-tailwind.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "react-chakra-ui"
             :path ".github/agents/react-chakra-ui.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "react-components-creation"
             :path ".github/agents/react-components-creation.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "react-graphql-apollo-client"
             :path ".github/agents/react-graphql-apollo-client.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "react-mobx"
             :path ".github/agents/react-mobx.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "react-modern"
             :path ".github/agents/react-modern.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "react-nextjs-ui-development"
             :path ".github/agents/react-nextjs-ui-development.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "react-query"
             :path ".github/agents/react-query.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "react-redux-typescript"
             :path ".github/agents/react-redux-typescript.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "react-styled-components"
             :path ".github/agents/react-styled-components.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "react-typescript-nextjs-nodejs-cursorrules-prompt-"
             :path ".github/agents/react-typescript-nextjs-nodejs-cursorrules-prompt-.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "react-typescript-symfony"
             :path ".github/agents/react-typescript-symfony.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "solidjs-basic"
             :path ".github/agents/solidjs-basic.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "solidjs-tailwind"
             :path ".github/agents/solidjs-tailwind.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "solidjs-typescript"
             :path ".github/agents/solidjs-typescript.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "svelte-5-vs-svelte-4"
             :path ".github/agents/svelte-5-vs-svelte-4.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "svelte-modern"
             :path ".github/agents/svelte-modern.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "sveltekit-restful-api-tailwind-css"
             :path ".github/agents/sveltekit-restful-api-tailwind-css.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "sveltekit-tailwindcss-typescript"
             :path ".github/agents/sveltekit-tailwindcss-typescript.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "sveltekit-typescript-guide"
             :path ".github/agents/sveltekit-typescript-guide.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "tailwind-css-nextjs-guide"
             :path ".github/agents/tailwind-css-nextjs-guide.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "tailwind-modern"
             :path ".github/agents/tailwind-modern.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "tailwind-react-firebase"
             :path ".github/agents/tailwind-react-firebase.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "tailwind-shadcn-ui-integration-cursorrules-prompt-"
             :path ".github/agents/tailwind-shadcn-ui-integration-cursorrules-prompt-.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "tauri-svelte-typescript-guide-cursorrules-prompt-f"
             :path ".github/agents/tauri-svelte-typescript-guide-cursorrules-prompt-f.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-axios"
             :path ".github/agents/typescript-axios.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-clasp"
             :path ".github/agents/typescript-clasp.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-code-convention"
             :path ".github/agents/typescript-code-convention.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-llm-tech-stack"
             :path ".github/agents/typescript-llm-tech-stack.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-modern"
             :path ".github/agents/typescript-modern.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-nextjs"
             :path ".github/agents/typescript-nextjs.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-nextjs-react"
             :path ".github/agents/typescript-nextjs-react.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-nextjs-react-tailwind-supabase-cursorru"
             :path ".github/agents/typescript-nextjs-react-tailwind-supabase-cursorru.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-nextjs-supabase"
             :path ".github/agents/typescript-nextjs-supabase.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-nodejs-nextjs-react-ui-css-cursorrules-"
             :path ".github/agents/typescript-nodejs-nextjs-react-ui-css-cursorrules-.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-nodejs-react-vite-cursorrules-prompt-fi"
             :path ".github/agents/typescript-nodejs-react-vite-cursorrules-prompt-fi.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-react"
             :path ".github/agents/typescript-react.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-react-nextjs-cloudflare"
             :path ".github/agents/typescript-react-nextjs-cloudflare.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-react-nextui-supabase"
             :path ".github/agents/typescript-react-nextui-supabase.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-shadcn-ui-nextjs"
             :path ".github/agents/typescript-shadcn-ui-nextjs.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-vite-tailwind"
             :path ".github/agents/typescript-vite-tailwind.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-vuejs"
             :path ".github/agents/typescript-vuejs.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "typescript-zod-tailwind-nextjs-cursorrules-prompt-"
             :path ".github/agents/typescript-zod-tailwind-nextjs-cursorrules-prompt-.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "vscode-extension-dev-typescript"
             :path ".github/agents/vscode-extension-dev-typescript.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "vue-3-nuxt-3-development"
             :path ".github/agents/vue-3-nuxt-3-development.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "vue-3-nuxt-3-typescript"
             :path ".github/agents/vue-3-nuxt-3-typescript.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "vue-modern"
             :path ".github/agents/vue-modern.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "vue3-composition-api"
             :path ".github/agents/vue3-composition-api.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

(agent-entry :name "web-app-optimization"
             :path ".github/agents/web-app-optimization.agent.md"
             :category "web-frontend"
             :skills ["react" "nextjs" "vue" "svelte"]
             :critic "omega-critic"
             :heartbeat-interval 30)

;; ═══════════════════════════════════════════════════════════════════════
;; END OF REGISTRY — 243 agents across 16 categories
;; ═══════════════════════════════════════════════════════════════════════

;; ── LATE ADDITION (spectral-analysis-expert was missing from initial gen) ──
