#!/bin/bash -eu
cd "$(dirname "$0")"

BIN_DIR="$PWD/debug/bin"

export IS_FIRST_TEST=true
set +e

run_test(){
    X=$1
    LEN=$2
    CNT=$3
    SORT_CNT=$4
    TARGET=$5
    $BIN_DIR/test_cmi_$X $LEN $CNT $SORT_CNT $TARGET | {
        while IFS= read -r line; do
            (echo $line | grep -q 'HEADER') && ! $IS_FIRST_TEST && continue
            printf "%30s, %7s, %10s, %s\n" $X $LEN $TARGET "$line"
        done
    }
    $IS_FIRST_TEST && IS_FIRST_TEST=false
}

target_suffixes=(
    base
    #    rasm_indjmp
    # rasm_perm_cmov
    #aes
    #page6_rasm_perm_indjmp_aes
    #page6_rasm_perm_indjmp_aes_sse
    #page6_rasm_perm_indjmp_aes_sse2
    #page6_rasm_indjmp_aes
    #page6_rasm_indjmp_aes_sse
    # page6_rasm_indjmp_aes_sse2
    # page6_rasm_perm_cmov_base
    # page6_rasm_perm_cmov_aes
    # page6_rasm_perm_cmov_base_sse
    page6_rasm_perm_cmov_base_sse2
    # page6_rasm_perm_cmov_oram
)
MAX_L=7

target=${1:-all}
rep=${2:-100}


for SUFF in "${target_suffixes[@]}";do
    for L in $(seq 1 ${MAX_L});do
        K=$((64*(2**L)))
        for I in $(seq 0 3);do
            LEN=$(((4*K)+(I*K)))
            run_test $SUFF $LEN $rep $rep "$target"
        done
    done
done

set -e

