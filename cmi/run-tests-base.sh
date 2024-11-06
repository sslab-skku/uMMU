#!/bin/bash -eu
cd "$(dirname "$0")"

BIN_DIR="$PWD/debug/bin"


prefix_output(){
    while IFS= read -r line; do
        printf "$@" "$line"
    done
}

set +e

target_suffixes=(
    base
    rasm_indjmp
    rasm_indjmp_sse2
    page6_rasm_perm_cmov_base
    page6_rasm_perm_cmov_base_sse2
)

LEN=1700
CNT=1000000

for X in "${target_suffixes[@]}";do
    $BIN_DIR/test_cmi_$X $LEN $CNT | prefix_output "%50s, %7s, %s\n" $X $LEN
done

LEN=3400
CNT=1000
target_suffixes=(
    base
#    rasm_indjmp
    page6_rasm_perm_cmov_base
    page6_rasm_perm_cmov_base_sse2
    page6_rasm_perm_cmov_oram
)

for X in "${target_suffixes[@]}";do
    $BIN_DIR/test_cmi_$X $LEN $CNT | prefix_output "%50s, %7s, %s\n" $X $LEN
done




set -e

