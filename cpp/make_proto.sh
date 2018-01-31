#!/bin/bash
# coding=utf-8

grpc_root="/opt/grpc_v1.7.2"

PROTOC="$grpc_root/third_party/protobuf/src/protoc"
PLUGIN="--plugin=protoc-gen-grpc=$grpc_root/bins/opt/grpc_cpp_plugin"

if [ ! -x ${PROTOC} ]; then
    echo "${PROTOC}; no such file."
    exit 1
fi

echo "Generating dictation C++ protobuf/grpc sources..."
path_i="../proto"
path_o="libdictation-client"
${PROTOC}   -I${path_i} \
            -I../submodules/googleapis \
            ${PLUGIN} \
            --cpp_out=${path_o} \
            --grpc_out=${path_o} \
            ${path_i}/dictation_asr.proto
