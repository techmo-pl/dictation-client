#!/bin/bash
# coding=utf-8

set -euo pipefail
IFS=$'\n\t'

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "${SCRIPT}")

COMMIT_TAG=""

if [[ "$#" -eq 0 ]]
then
    COMMIT_TAG=$(awk < "${SCRIPTPATH}"/VERSION.py '/__version__/ { print $NF }' | cut -d\" -f2)
else
    COMMIT_TAG="$1"
fi

docker build -f "${SCRIPTPATH}/Dockerfile" -t dictation-client-python:"${COMMIT_TAG}" "${SCRIPTPATH}/.."
