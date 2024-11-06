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

target_suffixes=(
    dummy
    base

    rasm_indjmp
    rasm_indjmp_sse
    rasm_indjmp_sse2

    rasm_dirjmp
    rasm_dirjmp_sse
    rasm_dirjmp_sse2

    rasm_cmov
    rasm_cmov_sse
    rasm_cmov_sse2


    rasm_perm_cmov
    rasm_perm_cmov_sse
    rasm_perm_cmov_sse2

    rasm_perm_indjmp
    rasm_perm_indjmp_sse
    rasm_perm_indjmp_sse2

    # rasm_perm_indjmp
    # oram_64_4_10
    # page5_rasm_cmov_base
    # page6_rasm_cmov_base
    # page7_rasm_cmov_base
)

for X in "${target_suffixes[@]}";do
    $BIN_DIR/test_cmi_$X $LEN $CNT | prefix_output "%20s, %7s, %s\n" $X $LEN
done


set -e

