#!/bin/bash

set -euo pipefail

PYTHON_VERSION='3.9.16'
curl -L "https://www.python.org/ftp/python/${PYTHON_VERSION}/Python-${PYTHON_VERSION}.tgz" -o python.tgz
tar -xvf python.tgz
rm -f python.tgz
pushd "Python-${PYTHON_VERSION}"
CC=gcc-9 CXX=g++-9 AR=gcc-ar-9 NM=gcc-nm-9 RANLIB=gcc-ranlib-9 ./configure --enable-optimizations
make -j12
make install
popd
rm -rf "Python-${PYTHON_VERSION}"
