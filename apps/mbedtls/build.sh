#!/bin/bash -eux

cd "$(dirname "$0")"

# 1. Build
export CC=wllvm
export LLVM_COMPILER=clang
export LLVM_CC_NAME=musl-clang
CMI="$(realpath ../../cmi/)"

[ ${DO_CLEAN:-0} -eq 1 ] && make clean

make CFLAGS="-I${CMI}/include -O0 -Wno-unused-function -Wno-unused-command-line-argument" no_test -j4

# 2. Extract bitcode
outdir="$PWD/out"
mkdir -p ${outdir}

target_list=(
    programs/ssl/ssl_server2
    programs/ssl/ssl_client2
    programs/pkey/ecdsa
    programs/pkey/rsa_decrypt
    programs/pkey/rsa_encrypt
    programs/test/eval
    programs/test/eval_key
    programs/test/eval_rsa
)

for f in "${target_list[@]}";do
    cp $f $outdir
    extract-bc $outdir/$(basename $f)
done
