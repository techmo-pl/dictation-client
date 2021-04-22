#!/bin/bash
# coding=utf-8

set -euo pipefail
IFS=$'\n\t'


check_package () {
    # $1 - package name
    # $2 - if == "sudo" use sudo

    if [ $(dpkg-query -W -f='${Status}' "$1" 2>/dev/null | grep -c "ok installed") -eq 0 ];
    then
        while true; do
            read -p "The required package $1 is not installed. Do you want to install it now? [y/n]" yn
            case $yn in
                [Yy]*)
                if [ $# -eq 2 ] && [ $2 == "sudo" ];
                then
                    sudo apt-get update && sudo apt-get install -y "$1"; 
                else
                    apt-get update && apt-get install -y "$1";
                fi;
                break ;;
                [Nn]*) exit 0 ;;
            esac
        done
    fi
}


# check Python version

python3_missing="false"
command -v python3 >/dev/null 2>&1 || python3_missing="true"

if [[ "$python3_missing" == true ]]; then
    echo "Unable to find Python 3! Install Python 3 and run setup again."
    exit 1
fi

python_version_output="$(python3 --version)"
python_version_detailed="${python_version_output##* }"
python_version="${python_version_detailed%.*}"

if [[ ! "$python_version" =~ ^(3\.5|3\.6|3\.7|3\.8|3\.9)$ ]]; then
    echo "Cannot find required Python version! Supported versions are: 3.5, 3.6, 3.7, 3.8, 3.9";
    exit 0
fi

# check required packages

if [ $(dpkg-query -W -f='${Status}' sudo 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
    #sudo not installed (eg. docker container)
    check_package "python3-dev"
    check_package "portaudio19-dev"
    check_package "python3-pip"
    check_package "virtualenv"
else
    #sudo installed
    check_package "python3-dev" "sudo"
    check_package "portaudio19-dev" "sudo"
    check_package "python3-pip" "sudo"
    check_package "virtualenv" "sudo"
fi

virtualenv -p python3 .env
source .env/bin/activate
pip install -r requirements.txt

echo "Setup finished!"