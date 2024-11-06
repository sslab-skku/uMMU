#!/bin/bash -eu

run_ta_opt(){
    opt-14 -enable-new-pm=0 -load "./build/TaintAnalysis/libTestSVF.so" \
	   --test-svf 
	   # "tests/WPA/$1.bc" -S -o "$1.ta.ll"
}

run_aggr_opt(){
    ./aggressive-opt.sh "tests/WPA/$1.bc" "tests/WPA/$1.aggr.bc"
}

run_mem_opt(){
    mem_opt_flags=(
    --gvn --gvn-hoist --gvn-sink --enable-gvn-hoist --enable-gvn-sink
    --gvn-hoist-max-bbs=-1
    --gvn-hoist-max-chain-length=-1
    --gvn-hoist-max-depth=-1
    --gvn-max-block-speculations=2000 # default=600
    --gvn-max-hoisted=-1
    --gvn-max-num-deps=10000
    --dse
    --dse-memoryssa-defs-per-block-limit=100000
    --dse-memoryssa-otherbb-cost=10000
    --dse-memoryssa-partial-store-limit=10000
    --dse-memoryssa-path-check-limit=10000
    --dse-memoryssa-samebb-cost=10000
    --dse-memoryssa-scanlimit=10000
    --dse-memoryssa-walklimit=10000
    --mldst-motion
    )
    opt-14 "${mem_opt_flags[@]}"
}

run_inline_opt(){
    opt-14 --inline --inline-threshold=10000
}

target_list=(ssh-agent ssh ssh-keysign scp)

for TARGET in "${target_list[@]}";do
    input="tests/WPA/${TARGET}.bc"
    echo "==== $TARGET + inline"
    cat $input | run_inline_opt | run_ta_opt 2>&1 1>/dev/null | tail -n 4
    echo "--- inline + mem opt"
    cat $input | run_inline_opt | run_mem_opt | run_ta_opt 2>&1 1>/dev/null | tail -n 4
    # echo "---- aggressive opt"
    # run_aggr_opt $TARGET
    # run_ta_opt $TARGET.aggr 2>&1 | tail -n 4
done
