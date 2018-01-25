# dictation-client
Dictation ASR gRPC client

Contents:  
- `proto`   Protocol Buffers API definition.  
- `cpp`     Dictation ASR gRPC client C++ implementation.  

Required steps:
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
