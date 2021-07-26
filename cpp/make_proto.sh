#!/bin/bash
# coding=utf-8

set -eo pipefail

grpc_root="/opt/grpc_v1.38.1"

PROTOC="$grpc_root/build/third_party/protobuf/protoc"
PLUGIN="--plugin=protoc-gen-grpc=$grpc_root/build/grpc_cpp_plugin"
GAPIS="../submodules/googleapis"

if [ ! -x ${PROTOC} ]; then
    echo "${PROTOC}; no such file."
    exit 1
fi

echo "Generating dictation C++ protobuf/grpc sources."
path_i="../proto"
path_o="libdictation-client"
${PROTOC}   -I${path_i} \
            -I${GAPIS} \
            -I${GAPIS}/third_party/protobuf/src \
            ${PLUGIN} \
            --cpp_out=${path_o} \
            --grpc_out=${path_o} \
            ${path_i}/dictation_asr.proto
            