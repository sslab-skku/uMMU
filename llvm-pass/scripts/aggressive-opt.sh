#!/bin/bash -eu
# Aggresive optimization
# Function inlining -> gvn + dse ...

input=$1
output=$2

inline_opt_flags=(
    --inline
    --inline-threshold=1000 # default=255
    # --inlinedefault-threshold=1000000
    # --inline-size-allowance=10000000
    # --inlinehint-threshold=10000000
)

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


# inline
opt-14 "${inline_opt_flags[@]}" "${input}"\
    | opt-14 "${mem_opt_flags[@]}" -o "${output}"
