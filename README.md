# dictation-client
Dictation ASR gRPC client

Contents:  
- `proto`   Protocol Buffers API definition.  
- `cpp`     Dictation ASR gRPC client C++ implementation.  

Required steps:
- Install Boost:
```
./tools/install_boost.sh
```
- Install gRPC:
```
./tools/install_grpc.sh
```
- Update submodule:
```
git submodule update --init --recursive
```
- Build googleapis:
```
./tools/build_googleapis.sh
```

To regenerate sources from `.proto`, run:
```
./proto/make_proto.sh
```
This might be required when using other gRPC or Protocol Buffers version.
