# C++ implementation of Dictation ASR gRPC client


## Docker usage

### Prerequisites

To use Dictation Client on Docker, Docker Engine has to be installed (recommended version 20.10.0 or higher).

For details about Docker Engine installation, check out: https://docs.docker.com/engine/install/


### Submodules

After cloning a git repository, download submodules:
```
git submodule update --init --recursive
```
(this command has to be invoked from the project's root directory)

If project was downloaded without using git, `googleapis` submodule have to be downloaded manually. 
In this case open project repository in web browser, go to the `submodules` directory and use the link located there to open the relevant commit in the googleapis repository. Then download it, unpack and copy all files to the `submodules/googleapis` directory.

### Build docker image

To prepare the docker image with C++ implementation of the Dictation Client, open project's main directory and run following command:

```
docker build -f Dockerfile-cpp -t dictation-client-cpp:2.3.2 . 
```
**Note:** The build process may take a several dozen minutes (for shorter build time use the python implementation instead).

When the build process is complete, following a message will be shown:
```
Successfully tagged dictation-client-cpp:2.3.2
```

### Run Dictation client

To use Dictation client on Docker container, go to `dictation-client/cpp/docker` directory and run `run_dictation_client_cpp.sh` script.

To send simple request to the Dictation service, use:
```
./run_dictation_client_cpp.sh --service-address IP_ADDRESS:PORT --filename WAV_FILE_NAME
```

To print list of available options, use:
```
./run_dictation_client_cpp.sh --help
```
Audio files to be transcribed should be placed inside `dictation-client/cpp/docker/wav` directory.
TLS credentials should be placed inside `dictation-client/cpp/docker/tls` directory, if used.


## Local instance usage

### Contents:
- `libdictation-client`     Library implementing communication with Techmo Dictation ASR gRPC service.
- `dictation-client`        Example of program using the library.

This project uses cmake build.


### Dependencies:

- **cmake** (installation in Ubuntu: `sudo snap install cmake --classic`)
- **git** (installation in Ubuntu: `sudo apt install -y git`)
- **gcc** (installation in Ubuntu: `sudo apt install -y gcc`)
- **g++** (installation in Ubuntu: `sudo apt install -y g++`)
- **make** (installation in Ubuntu: `sudo apt install -y make`)
- **autoconf** (installation in Ubuntu: `sudo apt-get install -y autoconf`)
- **libtool** (installation in Ubuntu: `sudo apt-get install -y libtool`)
- **Boost** provided as `boost_pkg`
    
    Default location: `/opt/boost_1.60.0`
    
    If not installed, from parent directory run: `sudo ./tools/install_boost.sh 4`
    
    **Note:** The number at the end of the command above specifies number of parallel jobs, and should be set appropriately depending on the machine (eg. 32 on strong server machine, 1 on small virtualbox instance)

- **gRPC** provided as `grpc_pkg`
    
    Default location: `/opt/grpc_v1.38.1`
    
    If not installed, from parent directory run `sudo ./tools/install_grpc.sh 4`
    
    **Note:** The number at the end of the command above specifies number of parallel jobs, and should be set appropriately depending on the machine (eg. 32 on strong server machine, 1 on small virtualbox instance)

- **OpenSSL** provided as `ssl_pkg` (installation in Ubuntu: `sudo apt-get install libssl-dev`)
- **DL** provided as `dl_pkg`


### Required steps before build:

- download googleapis submodule:
```
git submodule update --init --recursive
```
- build googleapis:
```
./tools/build_googleapis.sh
```
**Note:** This build might generate some errors, however, if the message "All required googleapis files found" pops up, it means that overall process was successful.

The other steps described below should be performed inside `cpp` directory.

To regenerate sources from `.proto`, run:
```
./make_proto.sh
```
This might be required when using other gRPC or Protocol Buffers versions.


### Build:
```
mkdir build && cd build && cmake .. && make -j 4 && cd ..
```
**Note:** The number in the command above specifies number of parallel jobs, and should be set appropriately depending on the machine.
Default value `4` should be appropriate for the average personal computer.


### Run:
```
./build/dictation_client --service-address 192.168.1.1:4321 --wav-path /path/to/audio.wav
```
**Note:** In command above sample service address and path to audio file have to be replaced with the actual values.


### Options:
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
