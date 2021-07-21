#!/bin/bash
# coding=utf-8

set -euo pipefail
IFS=$'\n\t'

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "${SCRIPT}")

test_result="fail"
test_service_address="demo.devtechmo.pl:51190"

cp "${SCRIPTPATH}/data/test.wav" "${SCRIPTPATH}/../docker/wav/ci_tmp_test.wav"
cp  "${SCRIPTPATH}/data/test_context.wav" "${SCRIPTPATH}/../docker/wav/ci_tmp_test_context.wav"

test_wave="ci_tmp_test.wav"
test_context_wave="ci_tmp_test_context.wav"

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
    output=$({ eval "$cmd"  1>&2; }  2>&1 ) # we want grep over output whether the command succeeds or not
    set -e
    echo "${output}" | grep "$phrase" > /dev/null 2>&1; # if this line returns non-0-code, 'set -e' will cause exit 
    echo "--> OK"
}

echo
echo "Testing: basic recognition"
cmd="${SCRIPTPATH}/../docker/run_dictation_client_python.sh --service-address=${test_service_address} --filename=${test_wave}"
phrase="serwis testowy"
check_output "${cmd}" "${phrase}"

echo
echo "Testing: --interim-results"
cmd="${SCRIPTPATH}/../docker/run_dictation_client_python.sh --service-address=${test_service_address} --filename=${test_wave} --interim-results"
phrase="serwis"
check_output "${cmd}" "${phrase}"

echo
echo "Testing: --grpc-timeout"
cmd="${SCRIPTPATH}/../docker/run_dictation_client_python.sh --service-address=${test_service_address} --filename=${test_wave} --grpc-timeout=50"
phrase="\"grpc_status\":4}"
check_output "${cmd}" "${phrase}"

echo
echo "Testing: --time-offsets"
cmd="${SCRIPTPATH}/../docker/run_dictation_client_python.sh --service-address=${test_service_address} --filename=${test_wave} --time-offsets"
phrase="testowy \[0.51 - 1.20\]"
check_output "${cmd}" "${phrase}"

echo
echo "Testing: --recognition-timeout"
cmd="${SCRIPTPATH}/../docker/run_dictation_client_python.sh --service-address=${test_service_address} --filename=${test_wave} --recognition-timeout=10"
phrase="serwis"
check_output "${cmd}" "${phrase}"

echo
echo "Testing: --context-phrase"
cmd="${SCRIPTPATH}/../docker/run_dictation_client_python.sh --service-address=${test_service_address} --filename=${test_context_wave} --context-phrase=context"
phrase="przedsięwzięcie"
check_output "${cmd}" "${phrase}"


test_result="success"
