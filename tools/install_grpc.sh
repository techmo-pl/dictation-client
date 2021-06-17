#!/bin/bash
# coding=utf-8

jobs=4
version="v1.24.3"
[ $# -ge 1 ] && jobs="$1"
[ $# -ge 2 ] && version="$2"

grpc_path="/opt/grpc_${version}"

# test if GRPC dir exists and then test if it contains correct version
if [ -d "${grpc_path}" ]; then
	#TODO test if it's correct version
	echo "GRPC already installed at: ${grpc_path}"
	exit 0
fi

# login as a root
(( $EUID != 0 )) && echo "sudo required! - run with sudoer privileges: 'sudo $0'" && exit 1

# download and build
cwd=`pwd`

git clone https://github.com/grpc/grpc grpc
cd grpc
git checkout "tags/v1.24.3"
git submodule update --init
cd third_party/protobuf/
git checkout v3.11.3
cd ../../..

mv grpc "${grpc_path}"

cd "${grpc_path}"

# cares
cd third_party/cares/cares
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCARES_STATIC=ON ../
make -j $jobs

cd "${grpc_path}"

# zlib
cd third_party/zlib
CFLAGS="-fPIC" ./configure 
make -j $jobs
cd ../..

# build grpc
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DgRPC_SSL_PROVIDER=package ..
make -j $jobs

cd "${cwd}"
