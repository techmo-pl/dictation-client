#!/bin/bash

grpc_root=/opt/grpc_v1.24.3
cd submodules/googleapis

jobs=32
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

echo "All required googleapis files found."

cd ../..
