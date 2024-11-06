#!/bin/bash
# input : INPUT.bc
# output : INPUT.inst.bc & INPUT.noinst.bc
set -eu

F=$(realpath ${1})
B=${F%.bc}

cd "$(dirname "$0")"

# Build Internal bitcode
internal/build.sh

BUILD_DIR=${BUILD_DIR:-debug}

vmi_opt(){
    opt -enable-new-pm=0 -load "./${BUILD_DIR}/src/libVMIPass.so" "$@"
}

set -x
## Link internal libc & extract-annotation
llvm-link $F ./internal/internal.bc -o $B.internal.bc
vmi_opt --libc-transform $B.internal.bc -o $B.libc.bc
vmi_opt --extract-annotation $B.libc.bc -o $B.anno.bc

## Analysis
vmi_opt --prop-secret $B.anno.bc -o $B.prop.bc

## Instrumentation
llvm-link $B.prop.bc ./internal/cmi.bc -o $B.cmi.bc
vmi_opt --inst-secret $B.cmi.bc -o $B.inst.bc

set +x

## Print Stats
vmi_opt --vmi-stats $B.inst.bc -o /dev/null

## build noinst version
cp $B.libc.bc $B.noinst.bc

llvm-dis $B.noinst.bc
llvm-dis $B.inst.bc
