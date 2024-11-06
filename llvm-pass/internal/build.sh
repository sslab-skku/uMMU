#!/bin/bash -eu

cd "$(dirname "$0")"
clang -emit-llvm -c internal.c
clang -O3 -emit-llvm -c cmi.c
llvm-dis cmi.bc

clang -c cmi_example.c

ar rcs libcmi_ex.a cmi_example.o

