cmake_minimum_required(VERSION 3.20)
project(CWorkspace VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build types
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose Debug, Release, or RelWithDebInfo" FORCE)
endif()

# Compiler flags
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        $<$<CONFIG:Debug>:-g -O0 -DDEBUG>
        $<$<CONFIG:Release>:-O3 -DNDEBUG>
        $<$<CONFIG:RelWithDebInfo>:-g -O2>
    )
endif()

# Main executable
add_executable(main
    src/main.c
)

# Link libraries (uncomment as needed)
# find_package(OpenGL)
# if(OpenGL_FOUND)
#     target_link_libraries(main PRIVATE OpenGL::GL)
# endif()

# find_package(SDL2)
# if(SDL2_FOUND)
#     target_link_libraries(main PRIVATE SDL2::SDL2)
# endif()
