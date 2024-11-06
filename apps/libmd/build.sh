#!/bin/bash -eu

DO_CONFIGURE=${1:-1}

cd "$(dirname "$0")"

outdir="$PWD/out"
mkdir -p ${outdir}

export CC=wllvm
export LLVM_CC_NAME=musl-clang
export LLVM_COMPILER=clang

#./autogen
mkdir -p "$PWD/build"
autoreconf

if [ $DO_CONFIGURE -eq 1 ];then
    CONFIG=(
        --enable-shared
        --enable-static
        --prefix="$PWD/build/"
        # --without-openssl # do not use libcrypto.so
        # --without-stackprotect
        # --without-hardening
        # --without-rpath
        # --without-pie
    )
    ./configure "${CONFIG[@]}"
fi

make -j$(nproc)
make install


cp build/lib/libmd.a ${outdir}
extract-bc ${outdir}/libmd.a
