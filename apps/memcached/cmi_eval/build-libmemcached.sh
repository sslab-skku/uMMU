#!/bin/bash -eu

cd "$(dirname "$0")"

git clone https://github.com/awesomized/libmemcached --branch 1.1.0
mkdir build-libmemcached
cd build-libmemcached
cmake ../libmemcached
make
sudo make install
