#!/bin/bash -eu

cd "$(dirname "$0")"

BIN_DIR="$PWD/debug/bin"

prefix_output(){
    while IFS= read -r line; do
        printf "$@" "$line"
    done
}

cmi_targets=(
    base
    rasm_perm_cmov
    rasm_indjmp
    rasm_cmov
)

TEST=${1:-all}
LEN=${2:-1700}
CNT=${3:-1000}

$BIN_DIR/test_cmi_dummy $LEN $CNT $CNT $TEST | prefix_output "%10s, %10s, %s\n" dummy nosse

for CMI in "${cmi_targets[@]}";do
    for SSE in "nosse" "sse" "sse2";do
        $BIN_DIR/test_cmi_${CMI}_${SSE} $LEN $CNT $CNT $TEST | prefix_output "%10s, %10s, %s\n" $CMI $SSE
    done
done
