#!/bin/bash
# input : INPUT.bc
# output : INPUT.inst.bc & INPUT.noinst.bc
set -eu

F=$(realpath ${1})
B=${F%.bc}

cd "$(dirname "$0")"

BUILD_DIR=${BUILD_DIR:-debug}

vmi_opt(){
    opt -enable-new-pm=0 -load "./${BUILD_DIR}/src/libVMIPass.so" "$@"
}

llvm-link $F ./internal/cmi.bc -o $B.cmi.bc
vmi_opt --anno-cmi-transform $B.cmi.bc -o $B.anno-cmi.inst.bc
## build noinst version
cp $B.bc $B.anno-cmi.noinst.bc

llvm-dis $B.anno-cmi.noinst.bc
llvm-dis $B.anno-cmi.inst.bc

