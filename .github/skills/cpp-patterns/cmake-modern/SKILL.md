---
name: Modern CMake for Audio Software
description: Target-based CMake patterns for building professional audio plugins, GPU-accelerated DSP, and neural amp models in REVITHION STUDIO.
---
# Modern CMake for Audio Software
Idiomatic CMake 3.24+ patterns for REVITHION STUDIO — C++20, JUCE, RTNeural, CUDA, and Faust.

## Target-Based Commands
Attach every flag, path, and dependency to a named target. Never use global commands.
```cmake
cmake_minimum_required(VERSION 3.24)
project(RevithionStudio VERSION 1.0.0 LANGUAGES CXX)
add_library(revithion_dsp_core STATIC src/dsp/Engine.cpp src/dsp/Graph.cpp)
target_compile_features(revithion_dsp_core PUBLIC cxx_std_20)
target_include_directories(revithion_dsp_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
target_link_libraries(revithion_dsp_core PUBLIC Eigen3::Eigen PRIVATE xsimd)
```

## FetchContent for Dependencies
Pin every dependency to a tag or commit — never a branch.
```cmake
include(FetchContent)
FetchContent_Declare(JUCE GIT_REPOSITORY https://github.com/juce-framework/JUCE.git GIT_TAG 7.0.12)
FetchContent_Declare(RTNeural GIT_REPOSITORY https://github.com/jatinchowdhury18/RTNeural.git GIT_TAG v6.0.0)
FetchContent_Declare(xsimd GIT_REPOSITORY https://github.com/xtensor-stack/xsimd.git GIT_TAG 13.0.0)
FetchContent_Declare(Eigen GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git GIT_TAG 3.4.0 GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(JUCE RTNeural xsimd Eigen)
```

## JUCE CMake API
`juce_add_plugin()` replaces Projucer. Declare every format and link JUCE modules as targets.
```cmake
juce_add_plugin(RevithionAmpSim COMPANY_NAME "REVITHION"
    FORMATS VST3 AU AAX Standalone PRODUCT_NAME "REVITHION Amp Sim"
    IS_SYNTH FALSE NEEDS_MIDI_INPUT FALSE COPY_PLUGIN_AFTER_BUILD TRUE)
juce_generate_juce_header(RevithionAmpSim)
target_sources(RevithionAmpSim PRIVATE src/PluginProcessor.cpp src/PluginEditor.cpp)
target_link_libraries(RevithionAmpSim
    PRIVATE juce::juce_audio_utils juce::juce_dsp revithion_dsp_core
    PUBLIC juce::juce_recommended_config_flags)
```

## GLOB_RECURSE and Generator Expressions
```cmake
file(GLOB_RECURSE AMP_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
target_sources(RevithionAmpSim PRIVATE ${AMP_SOURCES})
target_compile_options(revithion_dsp_core PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /fp:fast /arch:AVX2>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -ffast-math -mavx2>)
```

## Cross-Platform Builds
```cmake
if(APPLE)
    target_link_libraries(revithion_dsp_core PUBLIC "-framework Accelerate" "-framework CoreAudio")
elseif(WIN32)
    target_compile_definitions(revithion_dsp_core PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
elseif(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ALSA REQUIRED IMPORTED_TARGET alsa)
    pkg_check_modules(JACK IMPORTED_TARGET jack)
    target_link_libraries(revithion_dsp_core PUBLIC PkgConfig::ALSA $<$<BOOL:${JACK_FOUND}>:PkgConfig::JACK>)
endif()
```

## CUDA Integration
```cmake
option(REVITHION_ENABLE_CUDA "GPU-accelerated DSP kernels" ON)
if(REVITHION_ENABLE_CUDA)
    include(CheckLanguage)
    check_language(CUDA)
    if(CMAKE_CUDA_COMPILER)
        enable_language(CUDA)
        add_library(revithion_gpu_dsp STATIC src/gpu/ConvolutionKernel.cu src/gpu/FFTKernel.cu)
        target_link_libraries(revithion_gpu_dsp PRIVATE CUDA::cufft CUDA::cudart)
        target_link_libraries(revithion_dsp_core PUBLIC revithion_gpu_dsp)
    endif()
endif()
```

## RTNeural and Faust
Set RTNeural's SIMD backend before `MakeAvailable`. Faust `.dsp` compiles to C++ at build time.
```cmake
set(RTNEURAL_XSIMD ON CACHE BOOL "xsimd backend")
FetchContent_MakeAvailable(RTNeural)
target_link_libraries(RevithionAmpSim PRIVATE RTNeural)

find_program(FAUST_EXECUTABLE faust)
function(revithion_add_faust_dsp TARGET DSP_FILE)
    get_filename_component(N ${DSP_FILE} NAME_WE)
    set(GEN "${CMAKE_CURRENT_BINARY_DIR}/faust_gen/${N}.cpp")
    add_custom_command(OUTPUT ${GEN}
        COMMAND ${FAUST_EXECUTABLE} -lang cpp -o ${GEN} ${DSP_FILE} DEPENDS ${DSP_FILE})
    target_sources(${TARGET} PRIVATE ${GEN})
endfunction()
revithion_add_faust_dsp(RevithionAmpSim "${CMAKE_CURRENT_SOURCE_DIR}/dsp/CabSim.dsp")
```

## Install and Export Targets
```cmake
install(TARGETS revithion_dsp_core EXPORT RevithionTargets
    LIBRARY DESTINATION lib ARCHIVE DESTINATION lib INCLUDES DESTINATION include)
install(EXPORT RevithionTargets FILE RevithionTargets.cmake
    NAMESPACE Revithion:: DESTINATION lib/cmake/Revithion)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/RevithionConfigVersion.cmake" COMPATIBILITY SameMajorVersion)
```

## Anti-Patterns
| Pattern | Problem |
|---|---|
| `include_directories()` / `link_libraries()` | Global scope leaks flags to every target |
| `file(GLOB_RECURSE)` without `CONFIGURE_DEPENDS` | New files invisible until manual re-configure |
| Hardcoded paths (`/usr/local/lib/libjuce.a`) | Breaks on other machines and CI |
| Mixing `add_subdirectory` + `find_package` for same dep | Duplicate targets cause link or ODR errors |

## Checklist
- [ ] All targets use `target_*` commands — zero global includes or links
- [ ] FetchContent pinned to specific tags or commit SHAs
- [ ] `CONFIGURE_DEPENDS` on every `file(GLOB_RECURSE)` call
- [ ] JUCE formats (VST3, AU, AAX, Standalone) tested on macOS, Windows, Linux
- [ ] CUDA optional — build succeeds with `REVITHION_ENABLE_CUDA=OFF`
- [ ] RTNeural backend set before `FetchContent_MakeAvailable`
- [ ] Faust custom commands re-trigger on `.dsp` changes
- [ ] `install(EXPORT)` produces a working `find_package(Revithion)` config
