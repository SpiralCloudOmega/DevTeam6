FROM mcr.microsoft.com/devcontainers/cpp:latest

# Install additional development tools and libraries
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    # Build tools
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    # Compilers and tools
    clang \
    clang-format \
    clang-tidy \
    llvm \
    # Debuggers and profiling
    gdb \
    lldb \
    valgrind \
    # Graphics libraries
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libvulkan-dev \
    vulkan-tools \
    # SDL2
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-ttf-dev \
    libsdl2-mixer-dev \
    # Qt6
    qt6-base-dev \
    qt6-base-dev-tools \
    # Audio development
    libasound2-dev \
    libjack-jackd2-dev \
    libpulse-dev \
    # Additional utilities
    wget \
    curl \
    git \
    unzip \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install GoogleTest
RUN cd /tmp \
    && git clone --depth 1 --branch v1.14.0 https://github.com/google/googletest.git \
    && cd googletest \
    && cmake -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build \
    && cmake --install build \
    && cd .. \
    && rm -rf googletest

# Set up working directory
WORKDIR /workspace

# Default command
CMD ["/bin/bash"]
