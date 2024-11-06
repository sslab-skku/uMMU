#!/bin/bash -eu
cd "$(dirname "$0")"

BIN_DIR="$PWD/debug/bin"

export IS_FIRST_TEST=true
set +e


target=${1:-"all"}
rep=${2:-1000}
MAX_L=5

# TEST_PROG=$BIN_DIR/test_cmi_page6_base_base3_stats

CMI=page6_rdrand_stats
TEST_PROG=$BIN_DIR/test_cmi_$CMI

target_list=(
    mem
    # bin
    # matmul
    # hist
    # rsort
    # qsort32
    # qsort_u64
    # dijk
    # dijk_wo
)

$TEST_PROG 1000 1 1 bin | sed "s/^/$(printf '%20s, %10s, ' target LEN)/" | grep 'HEADER,' | head -n 2
for target in "${target_list[@]}";do
    for L in $(seq 1 ${MAX_L});do
        K=$((64*(2**L)))
        for i in $(seq 0 3);do
            LEN=$(((4*K)+(i*K)-10))
            $TEST_PROG $LEN $rep $rep "$target" | sed "s/^/$(printf '%20s, %10s, ' $target $LEN)/" | grep 'STATS,'
        done
    done
done

set -e

