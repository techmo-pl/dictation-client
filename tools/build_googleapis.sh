#!/bin/bash

set -euo pipefail

SCRIPT="$(realpath "$0")"
SCRIPTPATH="$(dirname "${SCRIPT}")"

parallel_jobs="${1:-8}"
grpc_root="${GRPC_ROOT:-/opt/grpc_v1.43.0}"
proto_src="${PROTO_SRC:-${grpc_root}/third_party/protobuf/src}"

protoc="${PROTOC:-${grpc_root}/build/third_party/protobuf/protoc}"
plugin="${GRPC_PLUGIN:-${grpc_root}/build/grpc_cpp_plugin}"

tmp_dir="${SCRIPTPATH}/../tmp_googleapis"
target_dir="${SCRIPTPATH}/../googleapis_files"

trap 'if [[ "${tmp_dir}" == *"/tmp_googleapis" ]]; then rm -rf "${tmp_dir}"; fi' EXIT

if [[ "${target_dir}" == *"/googleapis_files" ]]; then rm -rf "${target_dir}" && mkdir "${target_dir}"; else echo "Invalid target_dir path!"; fi
cp -r "${grpc_root}/third_party/googleapis/" "${tmp_dir}"

# rebuild - necesseary for newer versions
make -C "${tmp_dir}" clean || echo "[NOTICE] cleaning ALL of the googleapis failed; this is likely expected"

# build only the required sources
make -C "${tmp_dir}" -j "${parallel_jobs}" \
	GRPCPLUGIN="${plugin}" \
	PROTOINCLUDE="${proto_src}" \
	PROTOC="${protoc}" \
	LANGUAGE=cpp \
	"google/api/annotations.pb.cc" \
	"google/api/http.pb.cc" \
	"google/longrunning/operations.pb.cc" \
	"google/rpc/status.pb.cc"

cp -r "${tmp_dir}/gens" "${target_dir}/"
cp -r "${tmp_dir}/google" "${target_dir}/"
