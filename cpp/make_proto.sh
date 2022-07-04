#!/bin/bash
# coding=utf-8

set -euo pipefail
IFS=$'\n\t'

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "${SCRIPT}")

grpc_root="/opt/grpc_v1.38.1"

PROTOC="$grpc_root/build/third_party/protobuf/protoc"
PROTOBUF="$grpc_root/third_party/protobuf/src"
PLUGIN="--plugin=protoc-gen-grpc=$grpc_root/build/grpc_cpp_plugin"
GAPIS="${SCRIPTPATH}/../googleapis_files"

if [ ! -x ${PROTOC} ]; then
    echo "${PROTOC}; no such file."
    exit 1
fi

echo "Generating dictation C++ protobuf/grpc sources."
path_i="${SCRIPTPATH}/../proto"
path_o="${SCRIPTPATH}/libdictation-client"
${PROTOC}   -I${path_i} \
            -I${GAPIS} \
            -I${PROTOBUF} \
            ${PLUGIN} \
            --cpp_out=${path_o} \
            --grpc_out=${path_o} \
            ${path_i}/dictation_asr.proto
            