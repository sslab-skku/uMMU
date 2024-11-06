#!/bin/bash -eu

DO_CONFIGURE=${1:-1}

cd "$(dirname "$0")"
export CC=wllvm
# export CXX=wllvm++
export LLVM_CC_NAME=musl-clang
export LLVM_COMPILER=clang
CMI="$(realpath ../../cmi/)"

mkdir -p "$PWD/build"

outdir="$PWD/out"
mkdir -p ${outdir}

./autogen.sh
if [ $DO_CONFIGURE -eq 1 ];then
    CONFIG=(
        --prefix="$PWD/build/"
        --disable-openssl
        --disable-thread-support
        --disable-malloc-replacment
        --disable-debug-mode 
        --disable-libevent-regress
        --disable-samples
        --disable-largefile 
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


make -B -j$(nproc)
make install

cp build/lib/libevent.a ${outdir}
extract-bc ${outdir}/libevent.a