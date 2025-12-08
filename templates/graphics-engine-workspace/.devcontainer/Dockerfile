FROM mcr.microsoft.com/devcontainers/cpp:latest

# Install Vulkan SDK and graphics libraries
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    # Build tools
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    # Vulkan SDK
    wget \
    gnupg2 \
    software-properties-common \
    && wget -qO- https://packages.lunarg.com/lunarg-signing-key-pub.asc | \
        gpg --dearmor -o /etc/apt/trusted.gpg.d/lunarg.gpg \
    && wget -qO /etc/apt/sources.list.d/lunarg-vulkan-jammy.list \
        https://packages.lunarg.com/vulkan/lunarg-vulkan-jammy.list \
    && apt-get update \
    && apt-get -y install --no-install-recommends \
    vulkan-sdk \
    # Graphics libraries
    libglfw3-dev \
    libglm-dev \
    libxxf86vm-dev \
    libxi-dev \
    # OpenGL
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    # X11
    libx11-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    # Image loading
    libpng-dev \
    libjpeg-dev \
    # Additional utilities
    git \
    curl \
    unzip \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install glslang for shader compilation
RUN cd /tmp \
    && git clone --depth 1 https://github.com/KhronosGroup/glslang.git \
    && cd glslang \
    && cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local \
    && cmake --build build \
    && cmake --install build \
    && cd .. \
    && rm -rf glslang

# Set Vulkan SDK environment variables
ENV VULKAN_SDK=/usr
ENV VK_LAYER_PATH=/usr/share/vulkan/explicit_layer.d

# Working directory
WORKDIR /workspace

CMD ["/bin/bash"]
