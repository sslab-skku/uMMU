#!/bin/bash -eu
cd "$(dirname "$0")"

BIN_DIR="$PWD/debug/bin"

export IS_FIRST_TEST=true
set +e


run_oram_test(){
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

sse_list=(
    ""
    # _sse
    # _sse2
)

pre_list=(
    #""
    #page6_
    page6_stats_
)


target=${1:-"matmul"}
rep=${2:-1}
base_rep=100
MAX_L=5

for SSE in "${sse_list[@]}";do
    for PRE in "${pre_list[@]}";do
        for L in $(seq 1 ${MAX_L});do
            K=$((64*(2**L)))
            run_oram_test "${PRE}"oramB64Z4L${L}${SSE}  $(((4*K)+(0*K)-10)) $rep $rep "$target"
            run_oram_test "${PRE}"oramB64Z4L${L}${SSE}  $(((4*K)+(1*K)-10)) $rep $rep "$target"
            run_oram_test "${PRE}"oramB64Z4L${L}${SSE}  $(((4*K)+(2*K)-10)) $rep $rep "$target"
            run_oram_test "${PRE}"oramB64Z4L${L}${SSE}  $(((4*K)+(3*K)-10)) $rep $rep "$target"
        done
    done
done


set -e

