#!/bin/bash
# coding=utf-8

set -euo pipefail
IFS=$'\n\t'

SCRIPT="$(realpath "$0")"
SCRIPTPATH="$(dirname "${SCRIPT}")"

grpc_root="${GRPC_ROOT:-/opt/grpc_v1.43.0}"

PROTOC="${PROTOC:-${grpc_root}/build/third_party/protobuf/protoc}"
PROTOBUF="${grpc_root}/third_party/protobuf/src"
PLUGIN="--plugin=protoc-gen-grpc=${GRPC_PLUGIN:-${grpc_root}/build/grpc_cpp_plugin}"
GAPIS="${SCRIPTPATH}/../googleapis_files"

[ ! -x "$(command -v "${PROTOC}" 2> /dev/null)" ] \
	&& echo "${PROTOC}: executable not found" \
	&& exit 1

echo "Generating dictation C++ protobuf/grpc sources."
path_i="${SCRIPTPATH}/../proto"
path_o="${SCRIPTPATH}/libdictation-client"
${PROTOC}   -I${path_i} \
            -I${grpc_root}/src/proto/grpc/health/v1 \
            -I${GAPIS} \
            -I${PROTOBUF} \
            ${PLUGIN} \
            --cpp_out=${path_o} \
            --grpc_out=${path_o} \
            ${grpc_root}/src/proto/grpc/health/v1/health.proto \
            ${path_i}/dictation_asr.proto
