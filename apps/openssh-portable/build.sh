#!/bin/bash -eu

cd "$(dirname "$0")"
export CC=wllvm
export LLVM_CC_NAME=musl-clang
export LLVM_COMPILER=clang
CMI="$(realpath ../../cmi/)"
LIBMD="$(realpath ../libmd/)"

DO_CONFIGURE=${1:-1}

autoreconf

if [ $DO_CONFIGURE -eq 1 ];then
    CONFIG=(
        --with-cflags='-O0 -I$CMI/include'
        --without-openssl # do not use libcrypto.so
        --without-zlib
        --without-stackprotect
        --without-hardening
        --without-rpath
        --without-pie
    )
    ./configure "${CONFIG[@]}"
fi

make -B -j$(nproc)

# Build WPA
mkdir -p out
app_list=(ssh-agent ssh-keygen ssh)
for t in "${app_list[@]}";do
    cp $t out/$t
    extract-bc out/$t
    llvm-link --only-needed out/$t.bc $LIBMD/out/libmd.bca -o out/$t.wpa.bc
done
