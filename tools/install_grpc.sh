#!/bin/bash
# coding=utf-8

set -euo pipefail
IFS=$'\n\t'

jobs=4
version="v1.38.1"
[ $# -ge 1 ] && jobs=$1
[ $# -ge 2 ] && version="$2"

grpc_path="/opt/grpc_${version}"

# test if GRPC dir exists and then test if it contains correct version
if [ -d "${grpc_path}" ]; then
	#TODO test if it's correct version
	echo "GRPC already installed at: ${grpc_path}"
	exit 0
fi

# login as a root
(( EUID != 0 )) && echo "sudo required! - run with sudoer privileges: 'sudo $0'" && exit 1

git clone --depth 1 --recurse-submodules --shallow-submodules --branch "${version}" https://github.com/grpc/grpc "${grpc_path}"
cd "${grpc_path}/third_party/protobuf"
git fetch --depth 1 origin tag v3.16.0
git checkout v3.16.0

hash ninja 2> /dev/null && ninja="-GNinja" || ninja=

# cares
cd "${grpc_path}/third_party/cares/cares"
cmake -S . -B build "${ninja}" -DCMAKE_BUILD_TYPE=Release -DCARES_STATIC=ON
cmake --build build -- -j "${jobs}"

# zlib
cd "${grpc_path}/third_party/zlib"
CFLAGS="-fPIC" ./configure
make -j "$jobs"

# build grpc
cd "${grpc_path}"
cmake -S . -B build "${ninja}" -DCMAKE_BUILD_TYPE=Release -DgRPC_SSL_PROVIDER=package
cmake --build build -- -j "${jobs}"


# install abseil cpp
cd "${grpc_path}/third_party/abseil-cpp"
cmake -S . -B cmake/build "${ninja}" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${grpc_path}" -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE
cmake --build cmake/build -- -j "${jobs}"
cmake --build cmake/build --target install -- -j "${jobs}"
