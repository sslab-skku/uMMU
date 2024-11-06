#!/bin/bash -eux

cd "$(dirname "$0")"

# 1. Build
export CC=wllvm
export LLVM_COMPILER=clang
export LLVM_CC_NAME=musl-clang
CMI="$(realpath ../../cmi/)"

DO_CONFIGURE=${1:-1}

if [ $DO_CONFIGURE -eq 1 ];then
    export CFLAGS="-I${CMI}/include -O0 -Wno-unused-function -Wno-unused-command-line-argument"
    ./configure --disable-shared
fi

make -B -j$(nproc)

# 2. Extract bitcode
outdir="$PWD/out"
mkdir -p ${outdir}

target_list=(

)

for f in "${target_list[@]}";do
    cp $f $outdir
    extract-bc $outdir/$(basename $f)
done
