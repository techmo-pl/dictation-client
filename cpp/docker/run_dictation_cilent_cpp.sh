#!/bin/bash
# coding=utf-8

# This script sends request to dictation service using dictation client inside docker container
# Requires "dictation-client-cpp:2.3.0" docker image loaded locally


set -euo pipefail
IFS=$'\n\t'

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "${SCRIPT}")


docker_image="dictation-client-cpp:2.3.0"

service_address="UNSPECIFIED"
filename="UNSPECIFIED"
context_phrase="UNSPECIFIED"
grpc_timeout="UNSPECIFIED"
interim_results="UNSPECIFIED"
max_alternatives="UNSPECIFIED"
service_settings="UNSPECIFIED"
session_id="UNSPECIFIED"
single_utterance="UNSPECIFIED"
tls_dir="UNSPECIFIED"
streaming="UNSPECIFIED"
time_offsets="UNSPECIFIED"


service_address_option=""
filename_option=""
context_phrase_option=""
grpc_timeout_option=""
interim_results_option=""
max_alternatives_option=""
service_settings_option=""
session_id_option=""
single_utterance_option=""
tls_dir_option=""
streaming_option=""
time_offsets_option=""


usage() {

echo "

Dictation ASR gRPC client 2.3.0

Dictation ASR gRPC client options:

  -h, --help                  Print help message.
  -s, --service-address arg   IP address and port (address:port) of a service the client will connect to.
  -f, --filename WAVE         Name of the wave file with speech to be recognized. File should be inside 'wav' directory. Should be mono, 8kHz or 16kHz.
  --tls                       If set, uses tls authentication, otherwise use insecure channel (default). The tls credential files (client.crt, client.key, ca.crt) should be placed inside 'tls' directory.
  --session-id arg            Session ID to be passed to the service. If not specified, the service will generate a default session ID itself.
  --grpc-timeout arg          Timeout in milliseconds used to set gRPC deadline - how long the client is willing to wait for a reply from the server. If not specified, the service will set the deadline to a very large number.
  --streaming                 If present, will perform asynchronous RPC. This is obligatory for audio content larger than 3.5 MB.
  --time-offsets arg          If true, returns also recognized word time offsets.
  --single-utterance arg      Whether to perform continuous recognition (false) or stop after single utterance when detected pause in speech (true, default).
  --interim-results arg       Whether to receive interim results (true) or not (false, default).
  --service-settings arg      Semicolon-separated list of key=value pairs defining settings to be sent to service via gRPC request
  --max-alternatives arg (=1) Maximum number of recognition hypotheses to be returned.
  --context-phrase arg        Specifies which context model to use.

"
}

while [[ "$#" -gt 0 ]]; do
    case $1 in
        -h|--help)                   usage; exit 0 ;;
        -s|--service-address)        service_address="$2"; shift ;;
        -f|--filename)               filename="$2"; shift ;;
        --context-phrase)            context_phrase="$2"; shift ;;
        --grpc-timeout)              grpc_timeout="$2"; shift ;;
        --interim-results)           interim_results="$2"; shift ;;
        --max-alternatives)          max_alternatives="$2"; shift ;;
        --service-settings)          service_settings="$2"; shift ;; 
        --session-id)                session_id="$2"; shift ;;
        --single-utterance)          single_utterance="$2"; shift ;;
        --tls)                       tls="true" ;;
        --streaming)                 streaming="true" ;;
        --time-offsets)              time_offsets="$2"; shift ;;
        *) usage; exit 0 ;;
    esac
    shift
done               


if [[ "${service_address}" = "UNSPECIFIED" ]]; then
    echo "Service addres not specified! Use option '-s | --service-address ADDRESS:PORT' to specify service address"
    exit 0
else
    service_address_option="--service-address ${service_address}"
fi
if [[ "${filename}" = "UNSPECIFIED" ]]; then
    echo "No input specified! Use '-f | --filename FILENAME' to specify wav file to be processed."
    exit 0
else
    filename_option="--wav-path /volumen/wav/${filename##*/}"
fi
if [[ "${context_phrase}" != "UNSPECIFIED" ]]; then
    context_phrase_option="--context-phrase ${context_phrase}"
fi
if [[ "${grpc_timeout}" != "UNSPECIFIED" ]]; then
    grpc_timeout_option="--grpc-timeout ${grpc_timeout}"
fi
if [[ "${interim_results}" != "UNSPECIFIED" ]]; then
    interim_results_option="--interim-results ${interim_results}"
fi
if [[ "${max_alternatives}" != "UNSPECIFIED" ]]; then
    max_alternatives_option="--max-alternatives ${max_alternatives}"
fi
if [[ "${service_settings}" != "UNSPECIFIED" ]]; then
    service_settings_option="--service-settings ${service_settings}" 
fi
if [[ "${session_id}" != "UNSPECIFIED" ]]; then
    session_id_option="--session-id ${session_id}"
fi
if [[ "${single_utterance}" != "UNSPECIFIED" ]]; then
    single_utterance_option="--single-utterance ${single_utterance}"
fi
if [[ "${tls}" != "UNSPECIFIED" ]]; then
    ssl_dir_option="--ssl-dir /volumen/tls"
fi
if [[ "${streaming}" != "UNSPECIFIED" ]]; then
    streaming_option="--streaming ${streaming}"
fi
if [[ "${time_offsets}" != "UNSPECIFIED" ]]; then
    time_offsets_option="--time-offsets ${time_offsets}"
fi


IFS=$'\x20'

docker run --rm -it -v "${SCRIPTPATH}:/volumen" --network host "${docker_image}"  \
./cpp/build/dictation_client \
${service_address_option} \
${filename_option} \
${context_phrase_option} \
${grpc_timeout_option} \
${interim_results_option} \
${max_alternatives_option} \
${service_settings_option} \
${session_id_option} \
${single_utterance_option} \
${ssl_dir_option} \
${streaming_option} \
${time_offsets_option}
