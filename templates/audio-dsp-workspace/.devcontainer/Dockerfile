FROM mcr.microsoft.com/devcontainers/cpp:latest

# Install audio development libraries
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    # Build tools
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    # Audio libraries and tools
    libasound2-dev \
    libjack-jackd2-dev \
    libpulse-dev \
    libfreetype6-dev \
    libx11-dev \
    libxinerama-dev \
    libxext-dev \
    libxrandr-dev \
    libxcursor-dev \
    libgl1-mesa-dev \
    # Additional utilities
    wget \
    curl \
    git \
    unzip \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install JUCE framework
RUN cd /tmp \
    && git clone --depth 1 --branch 7.0.12 https://github.com/juce-framework/JUCE.git \
    && cd JUCE \
    && cmake -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --install build --prefix /usr/local \
    && cd .. \
    && rm -rf JUCE

# Set up VST3 SDK path (optional, for plugin development)
ENV VST3_SDK_DIR=/usr/local/include/vst3sdk

# Working directory
WORKDIR /workspace

CMD ["/bin/bash"]
