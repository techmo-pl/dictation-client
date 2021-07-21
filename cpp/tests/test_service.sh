#!/bin/bash
# coding=utf-8

set -euo pipefail
IFS=$'\n\t'

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "${SCRIPT}")

test_result="fail"
test_service_address="demo.devtechmo.pl:51190"
test_wave="${SCRIPTPATH}/data/test.wav"
test_context_wave="${SCRIPTPATH}/data/test_context.wav"

function on_exit {

    if [[ $test_result == "fail" ]]; then
        echo "---------------------"
        echo " SERVICE TEST FAILED!" 
        echo "---------------------"
    else
        echo "-------------------------------------"
        echo " SERVICE TEST COMPLETED SUCCESSFULLY!" 
        echo "-------------------------------------"
    fi
}
trap on_exit EXIT


function check_output () {

    cmd="$1"
    phrase="$2"

    set +e
    output=$({ eval "$cmd"  1>&2; }  2>&1 )
    set -e
    echo "${output}" | grep "$phrase" > /dev/null 2>&1; #if this line returns non 0 code, 'set -e' will cause exit 
    echo "--> OK"
}

echo
echo "Testing: basic recognition"
cmd="${SCRIPTPATH}/../build/dictation_client --service-address ${test_service_address} --wav-path ${test_wave}"
phrase="serwis testowy"
check_output "${cmd}" "${phrase}"

echo
echo "Testing: --interim-results"
cmd="${SCRIPTPATH}/../build/dictation_client --service-address ${test_service_address} --wav-path ${test_wave} --streaming --interim-results true"
phrase="transcript: \"serwis\""
check_output "${cmd}" "${phrase}"

echo
echo "Testing: --grpc-timeout"
cmd="${SCRIPTPATH}/../build/dictation_client --service-address ${test_service_address} --wav-path ${test_wave} --grpc-timeout 50"
phrase="DEADLINE_EXCEEDED"
check_output "${cmd}" "${phrase}"

echo
echo "Testing: --time-offsets"
cmd="${SCRIPTPATH}/../build/dictation_client --service-address ${test_service_address} --wav-path ${test_wave} --time-offsets true"
phrase="nanos: 60000000"
check_output "${cmd}" "${phrase}"

echo
echo "Testing: --context-phrase"
cmd="${SCRIPTPATH}/../build/dictation_client --service-address ${test_service_address} --wav-path ${test_context_wave} --context-phrase context"
phrase="przedsi\\\\304\\\\231wzi\\\\304\\\\231cie"
check_output "${cmd}" "${phrase}"


test_result="success"
