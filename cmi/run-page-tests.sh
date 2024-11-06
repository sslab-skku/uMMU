#!/bin/bash -eu
cd "$(dirname "$0")"

BIN_DIR="$PWD/debug/bin"

LEN=${1:-1700}
CNT=${2:-1000000}

prefix_output(){
    while IFS= read -r line; do
        printf "$@" "$line"
    done
}

set +e

page_sizes=(5 6 7 8)
rasm_impls=(rasm_indjmp rasm_cmov rasm_perm_cmov rasm_perm_indjmp)
swap_impls=(base)
sse_comps=("" _sse _sse2)

for LEN in 500 1000 1500 2000 2500 3000 6000;do
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

