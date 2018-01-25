C++ implementation of Dictation ASR gRPC client.

Contents:
- `libdictation-client`     Library implementing communication with Techmo Dictation ASR gRPC service.
- `dictation-client`        Example of program using the library.

Required steps before build:
- From main directory run:
```
git submodule update --init --recursive
./tools/build_googleapis.sh
```

Build:
```
mkdir build && cd build && cmake .. && make -j 4
```

Required dependencies are:  
- Boost     provided as `boost_pkg`  
- gRPC      provided as `grpc_pkg`  
- OpenSSL   provided as `ssl_pkg`
- DL        provided as `dl_pkg`

Run:
```
./build/dictation_client --service-address 192.168.1.1:4321 --wav-path /path/to/audio.wav
```

Options:
```
  --help                      Print help message.
  --service-address arg       IP address and port (address:port) of a service 
                              the client will connect to.
  --wav-path arg              Path to wave file with audio content to be sent 
                              to service via RPC.
  --session-id arg            Session ID to be passed to the service. If not 
                              specified, the service will generate a default 
                              session ID itself.
  --streaming                 If present, will perform asynchronous RPC.
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
```
