#!/bin/bash

set -euo pipefail

jobs=$1

if [ $# -eq 0 ]; then
    echo "Missing parameter for './b2 -j' command. Provide N to run commands in parallel." && exit 1
fi

DOWNLOAD_URL="https://sourceforge.net/projects/boost/files/boost/1.74.0/boost_1_74_0.tar.gz/download"
TARBALL="boost_1_74_0.tar.gz"

# login as a root
((EUID != 0)) && echo "sudo required! - run with sudoer privileges: 'sudo $0'" && exit 1

cd /opt

echo "[INFO] Downloading '${DOWNLOAD_URL}'"
wget --no-check-certificate "${DOWNLOAD_URL}" -O "${TARBALL}"
echo "[INFO] Unpacking ${TARBALL}"
tar xzf "${TARBALL}"

rm "${TARBALL}"

cd boost_1_74_0
./bootstrap.sh
./b2 --without-python -j "${jobs}"
rm -rf bin.v2

echo "/opt/boost_1_74_0/stage/lib" >> /etc/ld.so.conf.d/boost.conf
ldconfig
