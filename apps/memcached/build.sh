#!/bin/bash -eu

cd "$(dirname "$0")"

CMI="$(realpath ../../cmi/)"
LIBEVENT="$(realpath ../libevent/)"/build

cd "$(dirname "$0")"
export CC=wllvm
# export CXX=wllvm++
export LLVM_CC_NAME=musl-clang
export LLVM_COMPILER=clang
export CFLAGS="-O0 -I/usr/lib/llvm-14/lib/clang/14.0.0/include -I$CMI/include"


DO_CONFIGURE=${1:-0}

if [ $DO_CONFIGURE -eq 1 ];then
    echo "--- DO CONFIG ---"
    ./autogen.sh
    CONFIG=(
        --with-libevent="$LIBEVENT"
        --disable-coverage
        --disable-docs
        # --host=x86_64-pc-linux-musl
        #   --with-pic=fa
        # "--with-cflags='-O0 -I$CMI/include'"
        # --disable-rpath
        # --without-openssl # do not use libcrypto.so
        # --without-zlib
        # --without-stackprotect
        # --without-hardening
        # --without-rpath
        # --without-pie
    )
    ./configure "${CONFIG[@]}"
fi


# make -B -j1
make -j1

mkdir -p out
app_list=(memcached)

for t in "${app_list[@]}";do
    cp $t out/$t
    cp $LIBEVENT/lib/libevent.a out/
    extract-bc out/$t
    extract-bc out/libevent.a
    llvm-link --only-needed out/$t.bc out/libevent.bca -o out/$t.wpa.bc
done
