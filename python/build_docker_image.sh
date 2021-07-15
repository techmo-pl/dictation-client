#!/bin/bash
# coding=utf-8

set -euo pipefail
IFS=$'\n\t'

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "${SCRIPT}")

COMMIT_TAG=""

if [ -z "$1" ]
then
    echo "No commit tag provided!"
else
    COMMIT_TAG=$1
fi

docker build -f "${SCRIPTPATH}/Dockerfile" -t dictation-client-python:"${COMMIT_TAG}" "${SCRIPTPATH}/.."
