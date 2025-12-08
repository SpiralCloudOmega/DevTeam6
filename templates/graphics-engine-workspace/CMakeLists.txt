cmake_minimum_required(VERSION 3.20)
project(GraphicsEngine VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find Vulkan
find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
    message(STATUS "Vulkan SDK found: ${Vulkan_VERSION}")
    message(STATUS "Vulkan Include: ${Vulkan_INCLUDE_DIRS}")
    message(STATUS "Vulkan Library: ${Vulkan_LIBRARIES}")
endif()

# Find GLFW
find_package(glfw3 REQUIRED)

# Find GLM
find_package(glm REQUIRED)

# Main executable
add_executable(graphics-engine
    src/main.cpp
)

# Include directories
target_include_directories(graphics-engine PRIVATE
    ${Vulkan_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(graphics-engine PRIVATE
    Vulkan::Vulkan
    glfw
    glm::glm
)

# Compile shaders
find_program(GLSLC glslc REQUIRED)

# Shader compilation function
function(compile_shader SHADER_SOURCE)
    get_filename_component(SHADER_NAME ${SHADER_SOURCE} NAME)
    set(SHADER_OUTPUT "${CMAKE_BINARY_DIR}/shaders/${SHADER_NAME}.spv")
    
    add_custom_command(
        OUTPUT ${SHADER_OUTPUT}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/shaders"
        COMMAND ${GLSLC} ${CMAKE_SOURCE_DIR}/${SHADER_SOURCE} -o ${SHADER_OUTPUT}
        DEPENDS ${CMAKE_SOURCE_DIR}/${SHADER_SOURCE}
        COMMENT "Compiling shader: ${SHADER_NAME}"
    )
    
    set(SHADER_SPIRV_FILES ${SHADER_SPIRV_FILES} ${SHADER_OUTPUT} PARENT_SCOPE)
endfunction()

# Compile shaders
compile_shader(shaders/shader.vert)
compile_shader(shaders/shader.frag)

# Add shader target
add_custom_target(shaders ALL DEPENDS ${SHADER_SPIRV_FILES})
add_dependencies(graphics-engine shaders)

# Copy shaders to binary directory after build
add_custom_command(TARGET graphics-engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_BINARY_DIR}/shaders
    $<TARGET_FILE_DIR:graphics-engine>/shaders
)
