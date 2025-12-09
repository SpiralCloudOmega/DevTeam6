# Graphics Engine / Vulkan Workspace

Professional graphics engine development environment with Vulkan, OpenGL, and modern rendering techniques. Perfect for game engines, visualization tools, and real-time graphics applications.

## 🚀 Quick Start

### GitHub Codespaces
1. Create a codespace from this template
2. Vulkan SDK is pre-installed
3. Start building your graphics engine!

### Local Development
1. Install Docker and VS Code with Dev Containers extension
2. Open folder and click "Reopen in Container"
3. Everything is pre-configured

## 📦 What's Included

### Graphics APIs
- **Vulkan 1.3** - Modern low-level graphics API
- **OpenGL 4.6** - Traditional graphics API
- **GLFW** - Window and input management
- **GLM** - Mathematics library (vectors, matrices)

### Tools
- **glslc** - GLSL to SPIR-V shader compiler
- **glslang** - Shader compilation tools
- **Vulkan validation layers** - Debug and validate

### VS Code Extensions
- C/C++ Tools
- CMake Tools
- clangd - Code completion
- Shader language support

## 🔨 Building the Project

### Quick Build
```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release -G Ninja

# Build (includes shader compilation)
cmake --build build

# Run
./build/graphics-engine
```

### Using VS Code
Press `Ctrl+Shift+B` and select "CMake: Build"
- Shaders are automatically compiled to SPIR-V
- Output goes to `build/shaders/*.spv`

## 🎨 Shader Development

### Writing Shaders
Shaders are in `shaders/` directory:
- `shader.vert` - Vertex shader
- `shader.frag` - Fragment shader
- `shader.comp` - Compute shader (add as needed)

### GLSL Example
```glsl
// shader.vert
#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = vec4(inPosition, 1.0);
    fragColor = inColor;
}
```

### Shader Compilation
Shaders are automatically compiled during build:
```bash
# Manual compilation (if needed)
glslc shaders/shader.vert -o build/shaders/shader.vert.spv
glslc shaders/shader.frag -o build/shaders/shader.frag.spv
```

### Loading Compiled Shaders
```cpp
std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    return buffer;
}

auto vertShaderCode = readFile("shaders/shader.vert.spv");
auto fragShaderCode = readFile("shaders/shader.frag.spv");
```

## 🎮 Vulkan Basics

### Application Structure
```cpp
class VulkanApplication {
    // 1. Instance and physical device
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue graphicsQueue;
    
    // 2. Swapchain and images
    VkSwapchainKHR swapchain;
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;
    
    // 3. Render pass and framebuffers
    VkRenderPass renderPass;
    std::vector<VkFramebuffer> framebuffers;
    
    // 4. Pipeline
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    
    // 5. Command buffers
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    
    // 6. Synchronization
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
};
```

### Render Loop
```cpp
void drawFrame() {
    // 1. Wait for previous frame
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    
    // 2. Acquire swapchain image
    uint32_t imageIndex;
    vkAcquireNextImageKHR(device, swapchain, UINT64_MAX,
                          imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, 
                          &imageIndex);
    
    // 3. Reset fence
    vkResetFences(device, 1, &inFlightFences[currentFrame]);
    
    // 4. Record command buffer
    vkResetCommandBuffer(commandBuffers[currentFrame], 0);
    recordCommandBuffer(commandBuffers[currentFrame], imageIndex);
    
    // 5. Submit command buffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];
    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]);
    
    // 6. Present image
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    VkSwapchainKHR swapchains[] = {swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchains;
    presentInfo.pImageIndices = &imageIndex;
    
    vkQueuePresentKHR(presentQueue, &presentInfo);
    
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
```

## 📐 Mathematics with GLM

### Basic Transformations
```cpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Model matrix (object transform)
glm::mat4 model = glm::rotate(glm::mat4(1.0f), 
                              time * glm::radians(90.0f), 
                              glm::vec3(0.0f, 0.0f, 1.0f));

// View matrix (camera)
glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),  // Camera position
                             glm::vec3(0.0f, 0.0f, 0.0f),  // Look at
                             glm::vec3(0.0f, 0.0f, 1.0f)); // Up vector

// Projection matrix
glm::mat4 proj = glm::perspective(glm::radians(45.0f),     // FOV
                                  width / (float)height,    // Aspect ratio
                                  0.1f,                     // Near plane
                                  10.0f);                   // Far plane

// Vulkan uses inverted Y-axis in clip space
proj[1][1] *= -1;
```

## 🎯 Common Graphics Tasks

### Loading Models
```cpp
// Using tinyobjloader
#include <tiny_obj_loader.h>

void loadModel(const std::string& filename) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, 
                          filename.c_str())) {
        throw std::runtime_error(warn + err);
    }
    
    // Process vertices, normals, texcoords...
}
```

### Texture Loading
```cpp
// Using stb_image
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void loadTexture(const std::string& filename) {
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, 
                                 &texChannels, STBI_rgb_alpha);
    
    if (!pixels) {
        throw std::runtime_error("Failed to load texture!");
    }
    
    // Create Vulkan image and copy pixel data
    // ...
    
    stbi_image_free(pixels);
}
```

### Compute Shaders
```glsl
// shader.comp
#version 450

layout(local_size_x = 16, local_size_y = 16) in;
layout(binding = 0, rgba8) uniform image2D outputImage;

void main() {
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
    imageStore(outputImage, pixelCoords, color);
}
```

## 🏗️ Advanced Topics

### Descriptor Sets
```cpp
// Uniform buffer descriptor
VkDescriptorSetLayoutBinding uboLayoutBinding{};
uboLayoutBinding.binding = 0;
uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
uboLayoutBinding.descriptorCount = 1;
uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

// Sampler descriptor
VkDescriptorSetLayoutBinding samplerLayoutBinding{};
samplerLayoutBinding.binding = 1;
samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
samplerLayoutBinding.descriptorCount = 1;
samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
```

### Push Constants
```cpp
VkPushConstantRange pushConstantRange{};
pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
pushConstantRange.offset = 0;
pushConstantRange.size = sizeof(PushConstants);

// In command buffer
vkCmdPushConstants(commandBuffer, pipelineLayout, 
                   VK_SHADER_STAGE_VERTEX_BIT, 0, 
                   sizeof(pushConstants), &pushConstants);
```

### Multi-threading
```cpp
// One command pool per thread
std::vector<VkCommandPool> threadCommandPools;

void recordSecondaryCommandBuffers() {
    #pragma omp parallel for
    for (int i = 0; i < numThreads; ++i) {
        // Each thread records its own secondary command buffer
        VkCommandBuffer secondaryBuffer = secondaryCommandBuffers[i];
        // Record commands...
    }
}
```

## 📖 Resources

### Vulkan Learning
- [Vulkan Tutorial](https://vulkan-tutorial.com/) - Comprehensive guide
- [Vulkan Guide](https://github.com/KhronosGroup/Vulkan-Guide) - Official Khronos guide
- [Vulkan Samples](https://github.com/KhronosGroup/Vulkan-Samples) - Example code
- [Vulkan Spec](https://registry.khronos.org/vulkan/) - Official specification

### Graphics Programming
- [Learn OpenGL](https://learnopengl.com/) - Graphics fundamentals
- [Real-Time Rendering](http://www.realtimerendering.com/) - Graphics book
- [GPU Gems](https://developer.nvidia.com/gpugems) - Advanced techniques
- [Shader Toy](https://www.shadertoy.com/) - Shader examples

### Tools
- [RenderDoc](https://renderdoc.org/) - Graphics debugger
- [Nsight Graphics](https://developer.nvidia.com/nsight-graphics) - NVIDIA debugger
- [AMD Radeon GPU Profiler](https://gpuopen.com/rgp/) - AMD profiler

## 🎮 Example Projects

### Triangle Renderer
Basic Vulkan triangle with colors

### Textured Cube
3D cube with texture mapping

### PBR Renderer
Physically-based rendering with lights

### Particle System
GPU-accelerated particles

### Deferred Renderer
Multi-pass rendering technique

### Ray Tracer
Real-time ray tracing (RTX)

## 🐛 Troubleshooting

### Validation Errors
Enable validation layers:
```cpp
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
```

### Performance Issues
- Use RenderDoc or Nsight to profile
- Check for unnecessary synchronization
- Optimize descriptor set updates
- Use push constants for small data

### Shader Compilation Errors
```bash
# Validate shader
glslangValidator shaders/shader.vert

# Compile with debug info
glslc -g shaders/shader.vert -o shader.vert.spv
```

## 📝 License

MIT License - Free to use for any project!

## 🤝 Contributing

Contributions welcome! Submit issues or pull requests.

---

**Happy Graphics Programming! 🎨**
