#!/bin/bash -eu

CMI_IMPL=$1
IN=$(realpath $2) # BC code
BASE=${IN%.bc}

cd "$(dirname "$0")"

CMILIB=${CMILIB:-./cmi/debug/lib}

inline_bc=$BASE-$CMI_IMPL.inline.bc
in_o=$BASE.$CMI_IMPL.o
out=$BASE.$CMI_IMPL.out

cp $IN $inline_bc
opt -always-inline $IN -o $inline_bc

llc --relocation-model=pic -filetype=obj $inline_bc -o $in_o
musl-clang -L$CMILIB -Wno-unused-command-line-argument -static -O0 -fno-lto $in_o -o $out -lcmi_$CMI_IMPL

# if [ ${DO_DEBUG:-0} -eq 1 ];then
#     GDB_CMD=(gdb -ex 'set follow-fork-mode child' -ex run -ex 'disas' -ex backtrace --args)
#     "${GDB_CMD[@]}" $out "$@"
# elif [ ${DO_STRACE:-0} -eq 1 ];then
#     exec strace -k -f $out "$@"
# else
#     exec $out "$@"
# fi

