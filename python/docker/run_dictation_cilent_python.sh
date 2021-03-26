#!/bin/bash
# coding=utf-8

# This script sends request to dictation service using dictation client inside docker container
# Requires "dictation-client-python:2.3.0" docker image loaded locally

set -euo pipefail
IFS=$'\n\t'

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "${SCRIPT}")


# --- Default values --- #

docker_image="dictation-client-python:2.3.0"

service_address="UNSPECIFIED"
filename="UNSPECIFIED"
mic="UNSPECIFIED"
context_phrase="UNSPECIFIED"
grpc_timeout="UNSPECIFIED"
interim_results="UNSPECIFIED"
max_alternatives="UNSPECIFIED"
no_input_timeout="UNSPECIFIED"
recognition_timeout="UNSPECIFIED"
session_id="UNSPECIFIED"
single_utterance="UNSPECIFIED"
speech_complete_timeout="UNSPECIFIED"
speech_incomplete_timeout="UNSPECIFIED"
tls="UNSPECIFIED"
time_offsets="UNSPECIFIED"


service_address_option=""
filename_option=""
mic_option=""
context_phrase_option=""
grpc_timeout_option=""
interim_results_option=""
max_alternatives_option=""
no_input_timeout_option=""
recognition_timeout_option=""
session_id_option=""
single_utterance_option=""
speech_complete_timeout_option=""
speech_incomplete_timeout_option=""
ssl_dir_option=""
time_offsets_option=""

# --- /Default values --- #

usage() {

echo "

Dictation ASR gRPC client 2.3.0

  -h, --help            show this help message and exit
  -s, --service-address ADDRESS
                        IP address and port (address:port) of a service the client will connect to.
  -f, --filename WAVE   Name of the wave file with speech to be recognized. File should be inside 'wav' directory. Should be mono, 8kHz or 16kHz.
  -m, --mic             Use microphone as an audio source (instead of wave file).
  --tls                 If set, uses tls authentication, otherwise use insecure channel (default). The tls credential files (client.crt, client.key, ca.crt) should be placed inside 'tls' directory.
  --session-id SESSION_ID
                        Session ID to be passed to the service. If not specified, the service will generate a default session ID itself.
  --grpc-timeout GRPC_TIMEOUT
                        Timeout in milliseconds used to set gRPC deadline - how long the client is willing to wait for a reply from the
                        server. If not specified, the service will set the deadline to a very large number.
  --max-alternatives MAX_ALTERNATIVES
                        Maximum number of recognition hypotheses to be returned.
  --time-offsets        If set - the recognizer will return also word time offsets.
  --single-utterance    If set - the recognizer will detect a single spoken utterance.
  --interim-results     If set - messages with temporal results will be shown.
  --no-input-timeout NO_INPUT_TIMEOUT
                        MRCP v2 no input timeout [ms].
  --speech-complete-timeout SPEECH_COMPLETE_TIMEOUT
                        MRCP v2 speech complete timeout [ms].
  --speech-incomplete-timeout SPEECH_INCOMPLETE_TIMEOUT
                        MRCP v2 speech incomplete timeout [ms].
  --recognition-timeout RECOGNITION_TIMEOUT
                        MRCP v2 recognition timeout [ms].
  --context-phrase CONTEXT_PHRASE
                        Specifies which context model to use.
"
}

while [[ "$#" -gt 0 ]]; do
    case $1 in
        -h|--help)                   usage; exit 0 ;;
        -s|--service-address)        service_address="$2"; shift ;;
        -f|--filename)               filename="$2"; shift ;;
        -m|--mic)                    mic="true" ;;
        --context-phrase)            context_phrase="$2"; shift ;;
        --grpc-timeout)              grpc_timeout="$2"; shift ;;
        --interim-results)           interim_results="true" ;;
        --max-alternatives)          max_alternatives="$2"; shift ;;
        --no-input-timeout)          no_input_timeout="$2"; shift ;;
        --recognition-timeout)       recognition_timeout="$2"; shift ;;
        --session-id)                session_id="$2"; shift ;;
        --single-utterance)          single_utterance="true" ;;
        --speech-complete-timeout)   speech_complete_timeout="$2"; shift ;;
        --speech-incomplete-timeout) speech_incomplete_timeout="$2"; shift ;;
        --tls)                       tls="true" ;;
        --time-offsets)              time_offsets="true" ;;
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
if [[ "${filename}" = "UNSPECIFIED" && "${mic}" = "UNSPECIFIED" ]]; then
    echo "No input specified! Use either '-f | --filename FILENAME' to specify wav file to be processed or '-m | --mic' to use microphone input."
    exit 0
elif [[ "${filename}" = "UNSPECIFIED" ]]; then
    mic_option="--mic"
else
    filename_option="--wave-path /volumen/wav/${filename##*/}"
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
if [[ "${no_input_timeout}" != "UNSPECIFIED" ]]; then
    no_input_timeout_option="--no-input-timeout ${no_input_timeout}"
fi
if [[ "${recognition_timeout}" != "UNSPECIFIED" ]]; then
    recognition_timeout_option="--recognition-timeout ${recognition_timeout}"
fi
if [[ "${session_id}" != "UNSPECIFIED" ]]; then
    session_id_option="--session-id ${session_id}"
fi
if [[ "${single_utterance}" != "UNSPECIFIED" ]]; then
    single_utterance_option="--single-utterance ${single_utterance}"
fi
if [[ "${speech_complete_timeout}" != "UNSPECIFIED" ]]; then
    speech_complete_timeout_option="--speech-complete-timeout ${speech_complete_timeout}"
fi
if [[ "${speech_incomplete_timeout}" != "UNSPECIFIED" ]]; then
    speech_incomplete_timeout_option="--speech-incomplete-timeout ${speech_incomplete_timeout}"
fi
if [[ "${tls}" != "UNSPECIFIED" ]]; then
    ssl_dir_option="--ssl-dir /volumen/tls"
fi
if [[ "${time_offsets}" != "UNSPECIFIED" ]]; then
    time_offsets_option="--time-offsets ${time_offsets}"
fi


IFS=$'\x20'

docker run --rm -it -v "${SCRIPTPATH}:/volumen" --network host "${docker_image}"  \
python3 /dictation_client/dictation_client.py \
${service_address_option} \
${filename_option} \
${mic_option} \
${context_phrase_option} \
${grpc_timeout_option} \
${interim_results_option} \
${max_alternatives_option} \
${no_input_timeout_option} \
${recognition_timeout_option} \
${session_id_option} \
${single_utterance_option} \
${speech_complete_timeout_option} \
${speech_incomplete_timeout_option} \
${ssl_dir_option} \
${time_offsets_option}
