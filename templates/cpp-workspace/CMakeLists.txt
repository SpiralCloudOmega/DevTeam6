cmake_minimum_required(VERSION 3.20)
project(CPPWorkspace VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build types
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose Debug, Release, or RelWithDebInfo" FORCE)
endif()

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O3 -DNDEBUG>
        $<$<CONFIG:RelWithDebInfo>:-g -O2>
    )
endif()

# Find packages (optional, comment out if not needed)
# OpenGL
find_package(OpenGL)
if(OpenGL_FOUND)
    message(STATUS "OpenGL found")
endif()

# Vulkan
find_package(Vulkan)
if(Vulkan_FOUND)
    message(STATUS "Vulkan found: ${Vulkan_VERSION}")
endif()

# SDL2
find_package(SDL2)
if(SDL2_FOUND)
    message(STATUS "SDL2 found")
endif()

# Qt6
find_package(Qt6 COMPONENTS Core Widgets)
if(Qt6_FOUND)
    message(STATUS "Qt6 found: ${Qt6_VERSION}")
endif()

# Main executable
add_executable(main
    src/main.cpp
)

# Link libraries (uncomment as needed)
# if(OpenGL_FOUND)
#     target_link_libraries(main PRIVATE OpenGL::GL OpenGL::GLU)
# endif()

# if(Vulkan_FOUND)
#     target_link_libraries(main PRIVATE Vulkan::Vulkan)
# endif()

# if(SDL2_FOUND)
#     target_link_libraries(main PRIVATE SDL2::SDL2)
# endif()

# if(Qt6_FOUND)
#     target_link_libraries(main PRIVATE Qt6::Core Qt6::Widgets)
# endif()

# Enable testing
enable_testing()

# Add GoogleTest
find_package(GTest)
if(GTest_FOUND)
    add_executable(tests
        tests/test_main.cpp
    )
    target_link_libraries(tests PRIVATE GTest::gtest_main)
    
    include(GoogleTest)
    gtest_discover_tests(tests)
    
    message(STATUS "GoogleTest enabled")
endif()
