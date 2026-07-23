FROM randomdude/gcc-cross-x86_64-elf

# Install dependencies
RUN apt update && apt install -y \
    mingw-w64 \
    build-essential \
    curl \
    unzip \
    pkg-config \
    cmake \
    git \
    libtool \
    autoconf \
    automake \
    && rm -rf /var/lib/apt/lists/*

# Download and extract SDL3 Windows development libs
RUN curl -LO https://github.com/libsdl-org/SDL/releases/download/release-2.30.2/SDL3-devel-2.30.2-mingw.tar.gz && \
    tar -xvzf SDL3-devel-2.30.2-mingw.tar.gz && \
    mkdir -p /opt/SDL3 && \
    cp -r SDL3-2.30.2/x86_64-w64-mingw32/* /opt/SDL3 && \
    rm -rf SDL3*

VOLUME /root/env
WORKDIR /root/env