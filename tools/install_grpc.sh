#!/bin/bash
# coding=utf-8

jobs=32
version="v1.7.2"
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
(( $EUID != 0 )) && echo "sudo required! - run with sudoer privileges: 'sudo $0'" && exit 1

# download and build
cwd=`pwd`

git clone https://github.com/grpc/grpc grpc_tmp

mv grpc_tmp "${grpc_path}"
cd "${grpc_path}"

git checkout "tags/${version}"
git submodule update --init

# zlib
cd third_party/zlib
./configure
make -j $jobs
cd ../..

# build grpc
make -j $jobs

#ln -sf ${grpc_path} /opt/grpc

cd "${cwd}"

