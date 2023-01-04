#!/bin/bash
# coding=utf-8

set -euo pipefail
IFS=$'\n\t'

cd "${BASH_SOURCE%/*}"

automatic_install_mode="false"

if [[ $# -eq 1 ]] && [[ "$1" == "-y" ]]
then
    automatic_install_mode="true"
fi

install_package () {
    # $1 - package name
    # $2 - if == "sudo" use sudo
    if [ $(dpkg-query -W -f='${Status}' "$1" 2>/dev/null | grep -c "ok installed") -eq 0 ];
    then
        if [[ "${automatic_install_mode}" == "true" ]]
        then
            if [[ $# -eq 2 ]] && [[ $2 == "sudo" ]];
            then
                sudo apt-get update && sudo apt-get install -y "$1";
            else
                apt-get update && apt-get install -y "$1";
            fi
        else
            while true; do
                read -p "The required package $1 is not installed. Do you want to install it now? [y/n]" yn
                case $yn in
                    [Yy]*)
                        if [[ $# -eq 2 ]] && [[ $2 == "sudo" ]];
                        then
                            sudo apt-get update && sudo apt-get install -y "$1";
                        else
                            apt-get update && apt-get install -y "$1";
                        fi;
                        break ;;
                    [Nn]*)
                        echo "Permission to install the required package has not been granted. Exiting...";
                        exit 0 ;;
                esac
            done
        fi
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

if [[ ! "$python_version" =~ ^(3\.7|3\.8|3\.9|3\.10)$ ]]; then
    echo "Cannot find required Python version! Supported versions are: 3.7, 3.8, 3.9, 3.10";
    exit 0
fi

# check if sudo is installed

sudo_str=""
if [ $(dpkg-query -W -f='${Status}' sudo 2>/dev/null | grep -c "ok installed") -ne 0 ]; then
    sudo_str="sudo"
fi

# install required packages

install_package "python3-dev" "${sudo_str}"
install_package "python3-venv" "${sudo_str}"
install_package "portaudio19-dev" "${sudo_str}"
install_package "python3-pip" "${sudo_str}"

python3 -m venv .venv
source .venv/bin/activate
pip3 install --upgrade pip
pip3 install wheel
pip3 install -r requirements.txt

echo "Setup finished!"
