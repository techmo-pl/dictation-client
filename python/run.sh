#!/bin/bash
# coding=utf-8

# This script sends request to dictation service using python dictation client 
# Before using this script, run 'setup.sh' to check dependencies and prepare virtual environment

set -euo pipefail
IFS=$'\n\t'

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "${SCRIPT}")

source "${SCRIPTPATH}/.env/bin/activate"
export PYTHONIOENCODING=utf8
python3 "${SCRIPTPATH}/dictation_client.py" "$@"