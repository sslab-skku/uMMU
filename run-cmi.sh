#!/bin/bash -eu

# ex) ./run-cmi.sh xor2 apps/

CMI_IMPL=$1
IN=$(realpath $2)
shift 2

cd "$(dirname "$0")"

CMILIB=${CMILIB:-./cmi/debug/lib}

inline_bc=$(mktemp).bc
in_o=$(mktemp).o
out=$(mktemp)

cp $IN $inline_bc
opt -always-inline $IN -o $inline_bc

llc --relocation-model=pic -filetype=obj $inline_bc -o $in_o
musl-clang -L$CMILIB -Wno-unused-command-line-argument -static -O0 -fno-lto $in_o -o $out -lcmi_$CMI_IMPL

if [ ${DO_DEBUG:-0} -eq 1 ];then
    GDB_CMD=(gdb -ex 'set follow-fork-mode child' -ex run -ex 'disas' -ex backtrace --args)
    "${GDB_CMD[@]}" $out "$@"
elif [ ${DO_STRACE:-0} -eq 1 ];then
    exec strace -k -f $out "$@"
else
    exec $out "$@"
fi

