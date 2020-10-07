#!/bin/bash
# coding=utf-8

set -eo pipefail

grpc_root="/opt/grpc_v1.24.3"

PROTOC="$grpc_root/build/third_party/protobuf/protoc"
PLUGIN="--plugin=protoc-gen-grpc=$grpc_root/build/grpc_cpp_plugin"
GAPIS="submodules/dictation-client/submodules/googleapis"

if [ ! -x ${PROTOC} ]; then
    echo "${PROTOC}; no such file."
    exit 1
fi

echo "Generating dictation C++ protobuf/grpc sources."
path_i="submodules/dictation-client/proto"
path_o="src/service/proto"
${PROTOC}   -I${path_i} \
            -I${GAPIS} \
            -I${GAPIS}/third_party/protobuf/src \
            ${PLUGIN} \
            --cpp_out=${path_o} \
            --grpc_out=${path_o} \
            ${path_i}/dictation_asr.proto
