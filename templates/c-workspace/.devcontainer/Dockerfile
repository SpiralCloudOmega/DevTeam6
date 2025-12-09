FROM mcr.microsoft.com/devcontainers/base:ubuntu

# Install C development tools
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    # Build tools
    build-essential \
    gcc \
    make \
    cmake \
    ninja-build \
    pkg-config \
    # Debuggers and profiling
    gdb \
    valgrind \
    # Graphics libraries
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    # SDL2
    libsdl2-dev \
    libsdl2-image-dev \
    # Additional utilities
    wget \
    curl \
    git \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set up working directory
WORKDIR /workspace

# Default command
CMD ["/bin/bash"]
