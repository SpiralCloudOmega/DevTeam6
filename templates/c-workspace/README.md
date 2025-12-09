# C Development Workspace

Professional C development environment for systems programming, embedded development, and low-level applications.

## 🚀 Quick Start

### Option 1: GitHub Codespaces (Recommended)
1. Click "Code" → "Codespaces" → "Create codespace"
2. Wait for container to build
3. Start coding!

### Option 2: Local Dev Container
1. Install Docker and VS Code with Dev Containers extension
2. Open this folder in VS Code
3. Click "Reopen in Container" when prompted

## 📦 What's Included

### Tools
- **GCC 13+** - GNU C Compiler
- **Make** - Traditional build system
- **CMake** - Modern build system (alternative)
- **GDB** - GNU Debugger
- **Valgrind** - Memory leak and profiling tool

### Libraries
- **OpenGL** - Graphics API
- **SDL2** - Media library for games/multimedia

### VS Code Extensions
- C/C++ - IntelliSense and debugging
- CMake Tools - CMake integration
- Makefile Tools - Makefile support

## 🔨 Building the Project

### Using Make (Default)

```bash
# Build in debug mode (default)
make

# Build in release mode
make release

# Run the program
make run

# Clean build artifacts
make clean

# Check for memory leaks
make valgrind
```

### Using CMake (Alternative)

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build

# Run
./build/main
```

### Using VS Code
Press `Ctrl+Shift+B` (or `Cmd+Shift+B` on Mac) and select:
- **Make: Build (Debug)** - Default build
- **Make: Build (Release)** - Optimized build
- **CMake: Build** - Build with CMake
- **Make: Clean** - Clean artifacts

## 🐛 Debugging

### VS Code
1. Set breakpoints in the editor
2. Press `F5` or select a debug configuration:
   - **(GDB) Launch - Make** - Debug Make build
   - **(GDB) Launch - CMake** - Debug CMake build
   - **(GDB) Attach** - Attach to running process

### Command Line
```bash
# GDB
gdb ./bin/main
# Commands: run, break, step, next, continue, print, backtrace, quit

# GDB with arguments
gdb --args ./bin/main arg1 arg2

# Core dump analysis
gdb ./bin/main core
```

## 🧪 Memory Debugging with Valgrind

```bash
# Check for memory leaks
make valgrind

# Or run directly
valgrind --leak-check=full --show-leak-kinds=all ./bin/main

# Check for memory errors
valgrind --track-origins=yes ./bin/main

# Profiling with Callgrind
valgrind --tool=callgrind ./bin/main
kcachegrind callgrind.out.*
```

## 📚 Project Structure

```
c-workspace/
├── .devcontainer/          # Dev container config
│   ├── devcontainer.json
│   └── Dockerfile
├── .vscode/                # VS Code settings
│   ├── tasks.json
│   └── launch.json
├── src/                    # Source files
│   └── main.c
├── build/                  # Build artifacts (Make)
├── bin/                    # Executables (Make)
├── Makefile                # Make build script
├── CMakeLists.txt          # CMake build script
└── README.md
```

## 📖 Makefile Targets

| Target | Description |
|--------|-------------|
| `make` or `make debug` | Build in debug mode with symbols |
| `make release` | Build optimized release version |
| `make clean` | Remove build artifacts |
| `make run` | Build and run the program |
| `make valgrind` | Run with Valgrind memory checker |
| `make print` | Print Makefile variables (debugging) |

## 🎯 Adding Source Files

### With Make
Add new `.c` files to the `src/` directory. The Makefile will automatically detect and compile them.

### With CMake
Edit `CMakeLists.txt`:
```cmake
add_executable(main
    src/main.c
    src/utils.c     # Add new file here
    src/parser.c
)
```

## 📦 Adding Libraries

### OpenGL
Uncomment in `CMakeLists.txt`:
```cmake
find_package(OpenGL REQUIRED)
target_link_libraries(main PRIVATE OpenGL::GL)
```

Or in `Makefile`:
```makefile
LDFLAGS = -lGL -lGLU
```

### SDL2
```cmake
find_package(SDL2 REQUIRED)
target_link_libraries(main PRIVATE SDL2::SDL2)
```

Or:
```makefile
LDFLAGS = `sdl2-config --libs`
CFLAGS += `sdl2-config --cflags`
```

### Custom Libraries
```makefile
# Link library
LDFLAGS = -lmylib -L/path/to/lib

# Include headers
CFLAGS += -I/path/to/include
```

## 🎨 Code Style

The project uses standard C formatting:
- 4-space indentation
- LLVM-style formatting
- C17 standard

Format code:
```bash
# Format a file
clang-format -i src/main.c

# Format all C files
find src -name "*.c" | xargs clang-format -i
```

## 🔧 Common C Patterns

### Error Handling
```c
FILE* fp = fopen("file.txt", "r");
if (fp == NULL) {
    perror("fopen");
    return EXIT_FAILURE;
}
// Use fp
fclose(fp);
```

### Memory Management
```c
int* array = (int*)malloc(10 * sizeof(int));
if (array == NULL) {
    fprintf(stderr, "malloc failed\n");
    return EXIT_FAILURE;
}
// Use array
free(array);
array = NULL;  // Good practice
```

### String Safety
```c
char buffer[64];
strncpy(buffer, source, sizeof(buffer) - 1);
buffer[sizeof(buffer) - 1] = '\0';  // Ensure null termination
```

## 🎓 Example Projects

### Systems Programming
- Command-line utilities
- Shell scripting
- Process management
- Network sockets

### Embedded Systems
- Microcontroller programming
- Hardware drivers
- Real-time systems

### Game Development
- SDL2 games
- OpenGL graphics
- Physics simulations

### Data Structures
- Custom containers
- Algorithms
- Performance-critical code

## 🐛 Common Issues

### "Undefined reference" errors
Make sure you're linking required libraries:
```makefile
LDFLAGS = -lm  # Math library
```

### Segmentation faults
Run with Valgrind to find memory errors:
```bash
make valgrind
```

### Header not found
Add include path to Makefile:
```makefile
CFLAGS += -I/usr/include/something
```

## 📖 Learning Resources

### C Language
- [The C Programming Language](https://en.wikipedia.org/wiki/The_C_Programming_Language) (K&R Book)
- [Learn C](https://www.learn-c.org/) - Interactive tutorial
- [C Reference](https://en.cppreference.com/w/c) - Complete reference

### Tools
- [GDB Tutorial](https://www.cs.cmu.edu/~gilpin/tutorial/) - Debugging guide
- [Valgrind Quick Start](https://valgrind.org/docs/manual/quick-start.html)
- [Make Documentation](https://www.gnu.org/software/make/manual/)

### Systems Programming
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [The Linux Programming Interface](http://man7.org/tlpi/)

## 📝 License

MIT License - Free to use for any project!

## 🤝 Contributing

Contributions welcome! Submit issues or pull requests.

---

**Happy Coding! 🚀**
