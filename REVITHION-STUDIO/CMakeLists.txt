cmake_minimum_required(VERSION 3.22)
project(REVITHION_STUDIO VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Options ─────────────────────────────────────────────────────
option(REVITHION_BUILD_STANDALONE "Build standalone app"       ON)
option(REVITHION_BUILD_PLUGINS    "Build VST3/AU/LV2 plugins" OFF)
option(REVITHION_BUILD_TESTS      "Build unit tests"          ON)
option(REVITHION_ENABLE_CUDA      "Enable CUDA acceleration"  ON)

# ── JUCE (submodule in external/) ───────────────────────────────
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/JUCE/CMakeLists.txt")
    add_subdirectory(external/JUCE)
else()
    message(WARNING "JUCE not found in external/JUCE. Run: git submodule update --init")
endif()

# ── RTNeural (submodule in external/) ───────────────────────────
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/RTNeural/CMakeLists.txt")
    set(RTNEURAL_XSIMD ON CACHE BOOL "Use xsimd backend for RTNeural")
    add_subdirectory(external/RTNeural)
endif()

# ── acestep.cpp GGML (reference, not built here — use pre-built binaries) ──
# acestep.cpp lives at ~/projects/ace-step/acestep.cpp/
# REVITHION communicates with ACE-Step via REST API or invokes binaries

# ── Source directories ──────────────────────────────────────────
# (Will be populated as development progresses)
# add_subdirectory(src/engine)
# add_subdirectory(src/dsp)
# add_subdirectory(src/neural)
# add_subdirectory(src/ai)
# add_subdirectory(src/blocks)
# add_subdirectory(src/ui)

# ── Standalone App ──────────────────────────────────────────────
if(REVITHION_BUILD_STANDALONE AND TARGET juce::juce_gui_basics)
    juce_add_gui_app(RevithionStudio
        PRODUCT_NAME "REVITHION STUDIO"
        COMPANY_NAME "SpiralCloudOmega"
        BUNDLE_ID "com.spiralcloudomega.revithion"
        VERSION "0.1.0"
    )

    target_sources(RevithionStudio PRIVATE
        src/Main.cpp
    )

    target_compile_definitions(RevithionStudio PRIVATE
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:RevithionStudio,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:RevithionStudio,JUCE_VERSION>"
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=1
    )

    target_link_libraries(RevithionStudio PRIVATE
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_opengl
    )

    target_include_directories(RevithionStudio PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
endif()

# ── Tests ───────────────────────────────────────────────────────
if(REVITHION_BUILD_TESTS)
    enable_testing()
    # add_subdirectory(tests)
endif()

message(STATUS "")
message(STATUS "╔══════════════════════════════════════════════════╗")
message(STATUS "║          REVITHION STUDIO v${PROJECT_VERSION}                 ║")
message(STATUS "╠══════════════════════════════════════════════════╣")
message(STATUS "║ Standalone: ${REVITHION_BUILD_STANDALONE}")
message(STATUS "║ Plugins:    ${REVITHION_BUILD_PLUGINS}")
message(STATUS "║ Tests:      ${REVITHION_BUILD_TESTS}")
message(STATUS "║ CUDA:       ${REVITHION_ENABLE_CUDA}")
message(STATUS "╚══════════════════════════════════════════════════╝")
message(STATUS "")
