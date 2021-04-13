#!/bin/bash
# coding=utf-8

set -euo pipefail
IFS=$'\n\t'

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
    if [ $(dpkg-query -W -f='${Status}' python3-dev 2>/dev/null | grep -c "ok installed") -eq 0 ];
    then
        while true; do
            read -p "The required package 'python3-dev' is not installed. Do you want to install it now? [y/n]" yn
            case $yn in
                [Yy]*) apt-get update && apt-get install -y python3-dev; break ;;
                [Nn]*) exit 0 ;;
            esac
        done
    fi
    if [ $(dpkg-query -W -f='${Status}' portaudio19-dev 2>/dev/null | grep -c "ok installed") -eq 0 ];
    then
        while true; do
            read -p "The required package 'portaudio19-dev' is not installed. Do you want to install it now? [y/n]" yn
            case $yn in
                [Yy]*) apt-get update && apt-get install -y portaudio19-dev; break ;;
                [Nn]*) exit 0 ;;
            esac
        done
    fi
    if [ $(dpkg-query -W -f='${Status}' python3-pip 2>/dev/null | grep -c "ok installed") -eq 0 ];
    then
        while true; do
            read -p "The required package 'python3-pip' is not installed. Do you want to install it now? [y/n]" yn
            case $yn in
                [Yy]*) apt-get update && apt-get install -y python3-pip; break ;;
                [Nn]*) exit 0 ;;
            esac
        done
    fi
    if ! python3 -c "import virtualenv" &> /dev/null; then
        while true; do
                read -p "The required module 'virtualenv' is not installed. Do you want to install it now? [y/n]" yn
                case $yn in
                [Yy]*) pip3 install virtualenv; break ;;
                [Nn]*) exit 0 ;;
            esac
        done
    fi

else
    #sudo installed
    if [ $(dpkg-query -W -f='${Status}' python3-dev 2>/dev/null | grep -c "ok installed") -eq 0 ];
    then
        while true; do
            read -p "The required package 'python3-dev' is not installed. Do you want to install it now? [y/n]" yn
            case $yn in
                [Yy]*) sudo apt-get update && apt-get install -y python3-dev; break ;;
                [Nn]*) exit 0 ;;
            esac
        done
    fi
    if [ $(dpkg-query -W -f='${Status}' portaudio19-dev 2>/dev/null | grep -c "ok installed") -eq 0 ];
    then
        while true; do
            read -p "The required package 'portaudio19-dev' is not installed. Do you want to install it now? [y/n]" yn
            case $yn in
                [Yy]*) sudo apt-get update && apt-get install -y portaudio19-dev; break ;;
                [Nn]*) exit 0 ;;
            esac
        done
    fi
    if [ $(dpkg-query -W -f='${Status}' python3-pip 2>/dev/null | grep -c "ok installed") -eq 0 ];
    then
        while true; do
            read -p "The required package 'python3-pip' is not installed. Do you want to install it now? [y/n]" yn
            case $yn in
                [Yy]*) sudo apt-get update && apt-get install -y python3-pip; break ;;
                [Nn]*) exit 0 ;;
            esac
        done
    fi
    if ! python3 -c "import virtualenv" &> /dev/null; then
        while true; do
                read -p "The required module 'virtualenv' is not installed. Do you want to install it now? [y/n]" yn
                case $yn in
                [Yy]*) sudo pip3 install virtualenv; break ;;
                [Nn]*) exit 0 ;;
            esac
        done
    fi
fi

virtualenv -p python3 .env
source .env/bin/activate
pip install -r requirements.txt

echo "Setup finished!"