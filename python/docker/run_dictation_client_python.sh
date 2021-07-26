#!/bin/bash
# coding=utf-8

# This script sends request to dictation service using dictation client inside docker container
# Requires "dictation-client-python:$IMAGE_VERSION" docker image loaded locally

set -euo pipefail
IFS=$'\n\t'

IMAGE_VERSION=2.3.2

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "${SCRIPT}")
docker_image="dictation-client-python:${IMAGE_VERSION}"

usage() {

echo "
Dictation ASR gRPC client ${IMAGE_VERSION}

  -h, --help            show this help message and exit
  -s=ADDRESS, --service-address=ADDRESS
                        IP address and port (address:port) of a service the client will connect to.
  -f=WAVE, --filename=WAVE   
                        Name of the wave file with speech to be recognized. File should be inside 'wav' directory. Should be mono, 8kHz or 16kHz.
  -m, --mic             Use microphone as an audio source (instead of wave file).
  --tls                 If set, uses tls authentication, otherwise use insecure channel (default). The tls credential files (client.crt, client.key, ca.crt) should be placed inside 'tls' directory.
  --session-id=SESSION_ID
                        Session ID to be passed to the service. If not specified, the service will generate a default session ID itself.
  --grpc-timeout=GRPC_TIMEOUT
                        Timeout in milliseconds used to set gRPC deadline - how long the client is willing to wait for a reply from the
                        server. If not specified, the service will set the deadline to a very large number.
  --max-alternatives=MAX_ALTERNATIVES
                        Maximum number of recognition hypotheses to be returned.
  --time-offsets        If set - the recognizer will return also word time offsets.
  --single-utterance    If set - the recognizer will detect a single spoken utterance.
  --interim-results     If set - messages with temporal results will be shown.
  --no-input-timeout=NO_INPUT_TIMEOUT
                        MRCP v2 no input timeout [ms].
  --speech-complete-timeout=SPEECH_COMPLETE_TIMEOUT
                        MRCP v2 speech complete timeout [ms].
  --speech-incomplete-timeout=SPEECH_INCOMPLETE_TIMEOUT
                        MRCP v2 speech incomplete timeout [ms].
  --recognition-timeout=RECOGNITION_TIMEOUT
                        MRCP v2 recognition timeout [ms].
  --context-phrase=CONTEXT_PHRASE
                        Specifies which context model to use.
"
}

optspec=":fhms-:"
while getopts "f:hms:-:" optchar; do
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
                    opts+=( "--time-offsets" )
                    ;;
                single-utterance)  
                    opts+=( "--single-utterance" )
                    ;;
                interim-results)  
                    opts+=( "--interim-results" )
                    ;;
                mic)
                    opts+=("--mic")
                    set +e
                    if [ ! -S /tmp/pulseaudio.socket ];
                    then
                        pactl load-module module-native-protocol-unix socket=/tmp/pulseaudio.socket > /dev/null 2>&1
                    fi
                    set -e
                    ;;
                filename=*)
                    val=${OPTARG#*=}
                    opt=${OPTARG%=$val}
                    opts+=( "--wave-path" "/volume/wav/${val##*/}" )
                    ;;
                *=*)
                    val=${OPTARG#*=}
                    opt=${OPTARG%=$val}
                    opts+=( "--$opt" "$val" )
                    ;;
                *)
                    if [ "$OPTERR" = 1 ] && [ "${optspec:0:1}" != ":" ]; then
                        echo "Unknown option --${OPTARG}" >&2
                    fi
                    ;;
            esac;;
        f)                      
            val=${OPTARG#*=}
            opt=${OPTARG%=$val}
            opts+=( "--wave-path" "/volume/wav/${val##*/}" )
            ;;
        h)  
            usage; exit 0 
            ;;
        m)  
            opts+=("--mic")
            set +e
            if [ ! -S /tmp/pulseaudio.socket ];
            then
                pactl load-module module-native-protocol-unix socket=/tmp/pulseaudio.socket > /dev/null 2>&1
            fi
            set -e
            ;;
        s)  
            val=${OPTARG#*=}
            opt=${OPTARG%=$val}
            opts+=( "--service-address" "${val}" )
            ;;
        *)
            if [ "$OPTERR" != 1 ] || [ "${optspec:0:1}" = ":" ]; then
                echo "Non-option argument: '-${OPTARG}'" >&2
            fi
            ;;
    esac
done

docker run --rm -it \
--env PULSE_SERVER=unix:/tmp/pulseaudio.socket \
--env PULSE_COOKIE=/tmp/pulseaudio.cookie \
-v "${SCRIPTPATH}:/volume" \
-v /home/$USER/.config/pulse/cookie:/tmp/pulseaudio.cookie \
-v /dev/snd:/dev/snd \
-v /tmp/pulseaudio.socket:/tmp/pulseaudio.socket \
-v /usr/share/alsa/alsa.conf:/usr/share/alsa/alsa.conf:ro \
-u `id -u`:`id -g` \
--group-add audio \
--network host \
"${docker_image}" \
/dictation_client_python/run.sh "${opts[@]}"  2>/dev/null
