#!/bin/sh
# coding=utf-8

jobs=32
[ $# -ge 1 ] && jobs=$1

DOWNLOAD_URL="https://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.gz/download"
TARBALL="boost_1_60_0.tar.gz"

# login as a root
(( $EUID != 0 )) && echo "sudo required! - run with sudoer privileges: 'sudo $0'" && exit 1

cd /opt

echo "[INFO] Downloading '$DOWNLOAD_URL'"
wget --no-check-certificate "$DOWNLOAD_URL" -O $TARBALL
echo "[INFO] Unpacking $TARBALL"
tar xzf "$TARBALL"

rm $TARBALL

cd boost_1_60_0
./bootstrap.sh
./b2 -j $jobs

echo "/opt/boost_1_60_0/stage/lib" >>/etc/ld.so.conf.d/boost.conf
ldconfig

