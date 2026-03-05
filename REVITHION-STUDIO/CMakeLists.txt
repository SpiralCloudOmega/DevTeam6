cmake_minimum_required(VERSION 3.22)
project(REVITHION_STUDIO VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Options ─────────────────────────────────────────────────────
option(REVITHION_BUILD_STANDALONE "Build standalone app"       ON)
option(REVITHION_BUILD_PLUGINS    "Build VST3/AU/LV2 plugins" OFF)
option(REVITHION_BUILD_TESTS      "Build unit tests"          ON)
option(REVITHION_ENABLE_CUDA      "Enable CUDA acceleration"  OFF)

# ── CUDA (optional — for acestep.cpp integration later) ─────────
if(REVITHION_ENABLE_CUDA)
    include(CheckLanguage)
    check_language(CUDA)
    if(CMAKE_CUDA_COMPILER)
        enable_language(CUDA)
        message(STATUS "CUDA enabled: ${CMAKE_CUDA_COMPILER}")
    else()
        message(WARNING "CUDA requested but no compiler found — disabling")
        set(REVITHION_ENABLE_CUDA OFF)
    endif()
endif()

# ── JUCE (local copy in external/) ──────────────────────────────
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/JUCE/CMakeLists.txt")
    add_subdirectory(external/JUCE)
else()
    message(FATAL_ERROR "JUCE not found in external/JUCE. "
        "Copy or symlink JUCE 8 into external/JUCE/")
endif()

# ── RTNeural (local copy in external/) ──────────────────────────
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/RTNeural/CMakeLists.txt")
    set(RTNEURAL_XSIMD ON CACHE BOOL "Use xsimd backend for RTNeural")
    add_subdirectory(external/RTNeural)
endif()

# ── acestep.cpp GGML (reference, not built here — use pre-built binaries) ──
# REVITHION communicates with ACE-Step via REST API or invokes binaries

# ── Collect all source files ────────────────────────────────────
file(GLOB_RECURSE REVITHION_SOURCES CONFIGURE_DEPENDS
    src/engine/*.cpp
    src/ai/*.cpp
    src/blocks/*.cpp
    src/neural/*.cpp
    src/plugins/*.cpp
    src/dsp/*.cpp
    src/ui/*.cpp
)

# ── Standalone App ──────────────────────────────────────────────
if(REVITHION_BUILD_STANDALONE)
    juce_add_gui_app(RevithionStudio
        PRODUCT_NAME        "REVITHION STUDIO"
        COMPANY_NAME        "SpiralCloudOmega"
        BUNDLE_ID           "com.spiralcloudomega.revithion"
        VERSION             "0.1.0"
        PLUGIN_NAME         "REVITHION"
        PLUGIN_MANUFACTURER_CODE "SpCO"
        PLUGIN_CODE         "Rvth"
    )

    target_sources(RevithionStudio PRIVATE
        src/Main.cpp
        ${REVITHION_SOURCES}
    )

    target_compile_definitions(RevithionStudio PRIVATE
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:RevithionStudio,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:RevithionStudio,JUCE_VERSION>"
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
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

    if(TARGET RTNeural)
        target_link_libraries(RevithionStudio PRIVATE RTNeural)
    endif()

    target_include_directories(RevithionStudio PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    if(REVITHION_ENABLE_CUDA)
        target_compile_definitions(RevithionStudio PRIVATE REVITHION_CUDA=1)
    endif()
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
