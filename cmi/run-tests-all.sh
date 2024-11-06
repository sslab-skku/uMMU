#!/bin/bash -eu
cd "$(dirname "$0")"

BIN_DIR="$PWD/debug/bin"

prefix_output(){
    while IFS= read -r line; do
        printf "$@" "$line"
    done
}

set +e

LEN=${1:-1700}
CNT=${2:-1000000}

page_sizes=(5 6 7 8)
rasm_impls=(rasm_indjmp rasm_cmov rasm_perm_cmov rasm_perm_indjmp)
swap_impls=(base aes)
sse_comps=("" _sse _sse2)

for LEN in 200 400 800 1200 1600 2400 3200 4800 6400;do
    for PGSZ in "${page_sizes[@]}";do
        for RASM in "${rasm_impls[@]}";do
            for SWAP in "${swap_impls[@]}";do
                $BIN_DIR/test_cmi_page${PGSZ}_${RASM}_${SWAP} $LEN $CNT \
                    | prefix_output "%4s, %20s, %10s, %7s, %s\n" $PGSZ $RASM $SWAP $LEN
            done
        done
    done
done

set -e

