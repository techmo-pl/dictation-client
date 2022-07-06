#!/bin/bash

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "${SCRIPT}")

function cleanup()
{
    rm -rf "${tmp_dir}"
}
trap cleanup EXIT


grpc_root=/opt/grpc_v1.38.1

tmp_dir="${SCRIPTPATH}/../tmp_googleapis"
target_dir="${SCRIPTPATH}/../googleapis_files"

mkdir "${target_dir}"

cp -r "${grpc_root}/third_party/googleapis/" "${tmp_dir}"
cd "${tmp_dir}"

jobs=8
[ $# -ge 1 ] && jobs=$1

proto_src="${grpc_root}/third_party/protobuf/src"
protoc="${grpc_root}/build/third_party/protobuf/protoc"
plugin="$grpc_root/build/grpc_cpp_plugin"

# rebuild - necesseary fornewer versions
make clean

make -j $jobs GRPCPLUGIN=${plugin} PROTOINCLUDE=${proto_src} PROTOC=${protoc} LANGUAGE=cpp #|| exit 1
# This build is allowed to fail in general but some of the files are required.
# Check for required files:

required_files=(    "google/api/annotations"
                    "google/longrunning/operations"
                    "google/rpc/status"
)

extensions=( ".pb.h" ".pb.cc" ".grpc.pb.h" ".grpc.pb.cc" )

ok=true
for file in ${required_files[@]}; do
    for ext in ${extensions[@]}; do
        filename="gens/${file}${ext}"
        [[ -f $filename ]] && found=true || found=false
        if ! $found; then
            echo "File not found in googleapis:    ${filename}"
        fi
        [[ "$ok" == true && "$found" == true ]] && ok=true || ok=false
    done
done

if ! $ok; then
    echo "Failed to build required googleapis files."
    exit 1
fi

cp -r gens "${target_dir}"
cp -r google "${target_dir}"

echo "All required googleapis files found."
