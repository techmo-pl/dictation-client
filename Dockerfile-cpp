FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

ADD ./cpp /dictation-client/cpp
ADD ./proto /dictation-client/proto
ADD ./tools /dictation-client/tools
ADD ./submodules /dictation-client/submodules

WORKDIR /dictation-client

RUN apt-get update \
    && apt-get dist-upgrade -y \
    && apt-get install -y --no-install-recommends \
    automake \
    autoconf \
    build-essential \
    ca-certificates \
    curl \
    git \
    libogg-dev \
    libssl-dev \
    libtool \
    libvorbis-dev \
    unzip \
    wget

RUN curl -fsSL "https://github.com/Kitware/CMake/releases/download/v3.20.3/cmake-3.20.3-Linux-x86_64.sh" -o cmake-linux.sh \
 && sh cmake-linux.sh -- --skip-license --prefix=/usr && rm cmake-linux.sh

RUN curl -fsSL "https://github.com/ninja-build/ninja/releases/download/v1.10.2/ninja-linux.zip" -o ninja-linux.zip \
 && unzip ninja-linux.zip -d /usr/bin \ 
 && rm ninja-linux.zip 

RUN ./tools/install_boost.sh 4 \
    && ./tools/install_grpc.sh 4

RUN ./tools/build_googleapis.sh && cd cpp && ./make_proto.sh
                                                                                      
RUN cd cpp && cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build build -- -j 4
