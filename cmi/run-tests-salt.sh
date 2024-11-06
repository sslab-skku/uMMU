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
    rdrand

)


target=${1:-all}
rep=${2:-1000}
MAX_L=${3:-7}
MIN_L=${4:-7}


LENS=(1000 5000 10000 40000)


# for SUFF in "${target_suffixes[@]}";do
#     for L in $(seq ${MIN_L} ${MAX_L});do
#         K=$((64*(2**L)))
#         for I in $(seq 0 3);do
#             LEN=$(((4*K)+(I*K)))
#             run_test $SUFF $LEN $rep $rep "$target"
#         done
#     done
# done
for SUFF in "${target_suffixes[@]}";do
    for LEN in "${LENS[@]}";do
        run_test $SUFF $LEN $rep $rep "$target"
    done
done


set -e

