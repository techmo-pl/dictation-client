C++ implementation of Dictation ASR gRPC client.

Contents:
- `libdictation-client`     Library implementing communication with Techmo Dictation ASR gRPC service.
- `dictation-client`        Example of program using the library.

This project uses cmake build.

Dependencies are:
- Boost     provided as `boost_pkg`
    Default location: `/opt/boost_1.60.0`
    If not installed, from parent directory run `sudo ./tools/install_boost.sh`
- gRPC      provided as `grpc_pkg`
    Default location: `/opt/grpc_v1.24.3`
    If not installed, from parent directory run `sudo ./tools/install_grpc.sh`
- OpenSSL   provided as `ssl_pkg`
- DL        provided as `dl_pkg`

Required steps before build:
- Build googleapis. From parent directory run:
```
git submodule update --init --recursive
./tools/build_googleapis.sh
```
Note: This build is allowed to fail in general but some of the files are required.
The script checks if all required files have been built
Message "All required googleapis files found." means success in that case.

To regenerate sources from `.proto`, run:
```
./make_proto.sh
```
This might be required when using other gRPC or Protocol Buffers version.

Build:
```
mkdir build && cd build && cmake .. && make -j 4
```

Run:
```
./build/dictation_client --service-address 192.168.1.1:4321 --wav-path /path/to/audio.wav
```

Options:
```
  --help                      Print help message.
  --service-address arg       IP address and port (address:port) of a service
                              the client will connect to.
  --ssl-dir arg               If set to a path with ssl credential files
                              (client.crt, client.key, ca.crt), use ssl
                              authentication. Otherwise use insecure channel
                              (default).
  --wav-path arg              Path to wave file with audio content to be sent
                              to service via RPC.
  --session-id arg            Session ID to be passed to the service. If not
                              specified, the service will generate a default
                              session ID itself.
  --grpc-timeout arg (=0)     Timeout in milliseconds used to set gRPC deadline
                              - how long the client is willing to wait for a
                              reply from the server. If not specified, the
                              service will set the deadline to a very large
                              number.
  --streaming                 If present, will perform asynchronous RPC. This
                              is obligatory for audio content larger than 3.5
                              MB.
  --time-offsets arg (=0)     If true, returns also recognized word time
                              offsets.
  --single-utterance arg (=1) Whether to perform continuous recognition (false)
                              or stop after single utterance when detected
                              pause in speech (true, default).
  --interim-results arg (=0)  Whether to receive interim results (true) or not
                              (false, default).
  --service-settings arg      Semicolon-separated list of key=value pairs
                              defining settings to be sent to service via gRPC
                              request
  --max-alternatives arg (=1) Maximum number of recognition hypotheses to be
                              returned.
  --context-phrase arg        Specifies which context model to use.
```
