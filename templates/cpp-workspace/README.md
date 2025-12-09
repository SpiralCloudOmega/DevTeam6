# C++ Development Workspace

Professional C++ development environment with modern tooling, libraries, and best practices.

## 🚀 Quick Start

### Option 1: GitHub Codespaces (Recommended)
1. Click the green "Code" button
2. Select "Codespaces" tab
3. Click "Create codespace on main"
4. Wait for the container to build (first time only)

### Option 2: Local Dev Container
1. Install [Docker](https://www.docker.com/) and [VS Code](https://code.visualstudio.com/)
2. Install the [Dev Containers extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)
3. Open this folder in VS Code
4. When prompted, click "Reopen in Container"

## 📦 What's Included

### Compilers & Tools
- **GCC 13+** - GNU Compiler Collection
- **Clang 17+** - LLVM Compiler with clangd LSP
- **CMake 3.28+** - Modern build system
- **Ninja** - Fast build tool
- **GDB & LLDB** - Powerful debuggers
- **Valgrind** - Memory leak detection

### Graphics Libraries
- **OpenGL** - Cross-platform graphics API
- **Vulkan** - High-performance graphics and compute API
- **SDL2** - Simple DirectMedia Layer for window/input management
- **Qt6** - Cross-platform GUI framework

### Audio Libraries
- **ALSA** - Advanced Linux Sound Architecture
- **JACK** - Low-latency audio server
- **PulseAudio** - Sound server

### Testing
- **GoogleTest** - Unit testing framework

### VS Code Extensions
- C/C++ - IntelliSense, debugging
- CMake Tools - CMake integration
- clangd - Fast code completion and analysis
- CodeLLDB - LLDB debugger frontend

## 🔨 Building the Project

### Using VS Code Tasks
Press `Ctrl+Shift+B` (or `Cmd+Shift+B` on Mac) to see build tasks:
- **CMake: Build (Debug)** - Default build task
- **CMake: Build (Release)** - Optimized build
- **CMake: Run Tests** - Run all unit tests
- **CMake: Clean** - Clean build artifacts

### Using Terminal
```bash
# Configure (Debug)
cmake -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja

# Build
cmake --build build

# Run
./build/main

# Run tests
ctest --test-dir build --output-on-failure

# Clean
cmake --build build --target clean
```

### Build Types
- **Debug** - No optimization, debug symbols (`-g -O0`)
- **Release** - Full optimization (`-O3 -DNDEBUG`)
- **RelWithDebInfo** - Optimization with debug symbols (`-g -O2`)

## 🐛 Debugging

### VS Code
1. Set breakpoints by clicking in the gutter
2. Press `F5` or select a debug configuration:
   - **(GDB) Launch** - Debug with GDB
   - **(LLDB) Launch** - Debug with LLDB
   - **(GDB) Attach** - Attach to running process
   - **(GDB) Test** - Debug unit tests

### Command Line
```bash
# GDB
gdb ./build/main
# Commands: run, break, step, next, continue, print, backtrace

# LLDB
lldb ./build/main
# Commands: run, breakpoint, step, next, continue, print, bt

# Valgrind (memory leaks)
valgrind --leak-check=full ./build/main
```

## 📚 Adding Libraries

### Using CMake find_package
Uncomment or add to `CMakeLists.txt`:

```cmake
# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(main PRIVATE OpenGL::GL OpenGL::GLU)

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(main PRIVATE Vulkan::Vulkan)

# SDL2
find_package(SDL2 REQUIRED)
target_link_libraries(main PRIVATE SDL2::SDL2)

# Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
target_link_libraries(main PRIVATE Qt6::Core Qt6::Widgets)
```

### External Libraries

#### JUCE (Audio Framework)
```bash
cd /tmp
git clone https://github.com/juce-framework/JUCE.git
cd JUCE
cmake -B build -DCMAKE_BUILD_TYPE=Release
sudo cmake --install build
```

Then in `CMakeLists.txt`:
```cmake
find_package(JUCE CONFIG REQUIRED)
juce_add_console_app(main)
target_link_libraries(main PRIVATE juce::juce_core juce::juce_audio_basics)
```

#### Vulkan SDK (Full)
```bash
# Already included in devcontainer, but for reference:
wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-jammy.list \
    https://packages.lunarg.com/vulkan/lunarg-vulkan-jammy.list
sudo apt update
sudo apt install vulkan-sdk
```

## 🎨 Code Style

### Formatting
The project uses clang-format with LLVM style (modified):
```bash
# Format single file
clang-format -i src/main.cpp

# Format all files
find src -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

VS Code auto-formats on save (already configured).

### Linting
clang-tidy is enabled for static analysis:
```bash
# Run clang-tidy
clang-tidy src/main.cpp -- -std=c++20

# Run on all files
find src -name "*.cpp" | xargs clang-tidy
```

## 📊 Project Structure

```
cpp-workspace/
├── .devcontainer/          # Dev container configuration
│   ├── devcontainer.json
│   └── Dockerfile
├── .vscode/                # VS Code settings
│   ├── tasks.json          # Build tasks
│   └── launch.json         # Debug configurations
├── src/                    # Source files
│   └── main.cpp
├── tests/                  # Unit tests
│   └── test_main.cpp
├── CMakeLists.txt          # Build configuration
├── .clang-format           # Code style rules
├── .clang-tidy             # Static analysis rules
└── README.md
```

## 🎯 Example Projects

### Game Development
- Use SDL2 or Qt6 for window management
- OpenGL or Vulkan for rendering
- Add Box2D or Bullet for physics

### Audio Plugin
- JUCE framework for plugin development
- VST3/AU/AAX formats
- ALSA/JACK for low-latency audio

### Graphics Engine
- Vulkan for modern rendering
- GLFW for window/input
- GLM for math
- stb_image for texture loading

### Systems Programming
- Direct hardware access
- Network programming with Boost.Asio
- Concurrent programming with std::thread

## 🔧 Troubleshooting

### CMake can't find a library
```bash
# Update package lists
sudo apt-get update

# Search for development packages
apt-cache search <library-name>

# Install package
sudo apt-get install lib<name>-dev
```

### Compiler errors about missing headers
Make sure you've run CMake configure first:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
```

### GDB/LLDB not working
Ensure you built in Debug mode:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## 📖 Resources

### C++ Learning
- [cppreference.com](https://en.cppreference.com/) - C++ reference
- [learncpp.com](https://www.learncpp.com/) - Free C++ tutorial
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/) - Best practices

### Libraries
- [Awesome C++](https://github.com/fffaraz/awesome-cpp) - Curated list of C++ libraries
- [vcpkg](https://vcpkg.io/) - C++ package manager
- [Conan](https://conan.io/) - Another C++ package manager

### Graphics
- [Learn OpenGL](https://learnopengl.com/) - OpenGL tutorial
- [Vulkan Tutorial](https://vulkan-tutorial.com/) - Vulkan guide
- [SDL2 Wiki](https://wiki.libsdl.org/) - SDL2 documentation

### Audio
- [JUCE Tutorials](https://juce.com/learn/tutorials) - Audio plugin development
- [The Audio Programming Book](http://www.music.miami.edu/programs/mue/Research/jvandekieft/jvchapter1.pdf) - DSP fundamentals

## 📝 License

MIT License - Feel free to use this template for any project!

## 🤝 Contributing

Contributions welcome! Feel free to submit issues or pull requests.

---

**Happy Coding! 🚀**
