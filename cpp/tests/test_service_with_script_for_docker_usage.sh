#!/bin/bash
# coding=utf-8

set -euo pipefail
IFS=$'\n\t'

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "${SCRIPT}")

service_image="$1"

test_result="fail"
test_service_address="demo.devtechmo.pl:51190"

cp "${SCRIPTPATH}/data/test.wav" "${SCRIPTPATH}/../docker/wav/ci_tmp_test.wav"
cp  "${SCRIPTPATH}/data/test_context.wav" "${SCRIPTPATH}/../docker/wav/ci_tmp_test_context.wav"

test_wave="ci_tmp_test.wav"
test_context_wave="ci_tmp_test_context.wav"

function on_exit {

    if [[ $test_result == "fail" ]]; then
        echo "--------------------------------------------------"
        echo " SERVICE TEST WITH SCRIPT FOR DOCKER USAGE FAILED!" 
        echo "--------------------------------------------------"
    else
        echo "------------------------------------------------------------------"
        echo " SERVICE TEST WITH SCRIPT FOR DOCKER USAGE COMPLETED SUCCESSFULLY!" 
        echo "------------------------------------------------------------------"
    fi
    rm "${SCRIPTPATH}/../docker/wav/ci_tmp_test.wav" "${SCRIPTPATH}/../docker/wav/ci_tmp_test_context.wav"
}
trap on_exit EXIT


function check_output () {

    cmd="$1"
    phrase="$2"

    set +e
    output=$({ eval "$cmd"  1>&2; }  2>&1 ) # we want grep over output whether the command succeeds or not
    echo "output=$output"
    set -e
    echo "${output}" | grep "$phrase" > /dev/null 2>&1; # if this line returns non-0-code, 'set -e' will cause exit 
    echo "--> OK"
}

echo
echo "------------------------------------------"
echo " SERVICE TEST WITH SCRIPT FOR DOCKER USAGE"
echo "------------------------------------------"
#########
set -x
#########
echo
echo "Testing: basic recognition"
cmd="${SCRIPTPATH}/../docker/run_dictation_client_cpp.sh --custom-image=${service_image} --service-address=${test_service_address} --filename=${test_wave}"
phrase="serwis testowy"
check_output "${cmd}" "${phrase}"

echo
echo "Testing: --interim-results"
cmd="${SCRIPTPATH}/../docker/run_dictation_client_cpp.sh --custom-image=${service_image} --service-address=${test_service_address} --filename=${test_wave} --interim-results"
phrase="serwis"
check_output "${cmd}" "${phrase}"

echo
echo "Testing: --grpc-timeout"
cmd="${SCRIPTPATH}/../docker/run_dictation_client_cpp.sh --custom-image=${service_image} --service-address=${test_service_address} --filename=${test_wave} --grpc-timeout=50"
phrase="DEADLINE_EXCEEDED"
check_output "${cmd}" "${phrase}"

echo
echo "Testing: --time-offsets"
cmd="${SCRIPTPATH}/../docker/run_dictation_client_cpp.sh --custom-image=${service_image} --service-address=${test_service_address} --filename=${test_wave} --time-offsets"
phrase="nanos: 60000000"
check_output "${cmd}" "${phrase}"

echo
echo "Testing: --context-phrase"
cmd="${SCRIPTPATH}/../docker/run_dictation_client_cpp.sh --custom-image=${service_image} --service-address=${test_service_address} --filename=${test_context_wave} --context-phrase=context"
phrase="przedsi\\\\304\\\\231wzi\\\\304\\\\231cie"
check_output "${cmd}" "${phrase}"


test_result="success"
