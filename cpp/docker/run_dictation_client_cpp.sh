#!/bin/bash
# coding=utf-8

# This script sends request to dictation service using dictation client inside docker container
# Requires "dictation-client-cpp:$IMAGE_VERSION" docker image loaded locally


set -euo pipefail
IFS=$'\n\t'

IMAGE_VERSION=2.4.0

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "${SCRIPT}")
docker_image="dictation-client-cpp:${IMAGE_VERSION}"

usage() {

echo "
Dictation ASR gRPC client ${IMAGE_VERSION}
Dictation ASR gRPC client options:
  -h, --help            Print help message.
  -s=ADDRESS, --service-address=ADDRESS   
                        IP address and port (address:port) of a service the client will connect to.
  --tls                 If set, uses tls authentication, otherwise use insecure channel (default). The tls credential files (client.crt, client.key, ca.crt) should be placed inside 'tls' directory.
  -f=AUDIO, --filename=AUDIO        
                        Path to wave file with audio content to be sent to service via RPC.
  --session-id=SESSION_ID                
                        Session ID to be passed to the service. If not specified, the service will generate a default session ID itself.
  --grpc-timeout=GRPC_TIMEOUT
                        Timeout in milliseconds used to set gRPC deadline - how long the client is willing to wait for a reply from the server. If not specified, the service will set the deadline to a very large number.
  --streaming           If present, will perform asynchronous RPC. This is obligatory for audio content larger than 3.5 MB.
  --time-offsets        If true, returns also recognized word time offsets.
  --single-utterance    If set - the recognizer will detect a single spoken utterance.
  --interim-results     If set - messages with temporal results will be shown.
  --service-settings=SETTINGS         
                        Semicolon-separated list of key=value pairs defining settings to be sent to service via gRPC request.
  --max-alternatives=MAX_ALTERNATIVES 
                        Maximum number of recognition hypotheses to be returned.
  --context-phrase=CONTEXT_PHRASE
                        Specifies which context model to use.

"
}

while getopts "f:hs:-:" optchar; do
    case "${optchar}" in
        -)
            case "${OPTARG}" in
                help)   
                    usage; exit 0 
                    ;;
                tls)  
                    opts+=( "--ssl-dir" "/volume/tls" )
                    ;;
                time-offsets)  
                    opts+=( "--time-offsets=true" )
                    ;;
                single-utterance)  
                    opts+=( "--single-utterance=true" )
                    ;;
                interim-results)  
                    opts+=( "--interim-results=true" )
                    ;;
                streaming)  
                    opts+=( "--streaming" )
                    ;;
                filename=*)
                    val=${OPTARG#*=}
                    opt=${OPTARG%=$val}
                    opts+=( "--audio-path" "/volume/wav/${val##*/}" )
                    ;;
                *=*)
                    val=${OPTARG#*=}
                    opt=${OPTARG%=$val}
                    opts+=( "--$opt" "$val" )
                    ;;
                *)
                    if [[ "$OPTERR" = 1 ]]; then
                        echo "Unknown option --${OPTARG}" >&2
                    fi
                    ;;
            esac;;
        f)                      
            val=${OPTARG#*=}
            opt=${OPTARG%=$val}
            opts+=( "--audio-path" "/volume/wav/${val##*/}" )
            ;;
        h)  
            usage; exit 0 
            ;;
        s)  
            val=${OPTARG#*=}
            opt=${OPTARG%=$val}
            opts+=( "--service-address" "${val}" )
            ;;
        *)
            usage 
            exit 1
            ;;
    esac
done

docker run --rm -it -v "${SCRIPTPATH}:/volume" --network host "${docker_image}"  \
./cpp/build/dictation_client "${opts[@]}"
