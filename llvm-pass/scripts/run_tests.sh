#!/bin/bash -eux

(
    cd tests/
    ./build_targets.sh
)

opt-14 -enable-new-pm=0 -load "./build/TaintAnalysis/libTestSVF.so" \
       --test-svf \
       "../WPA/openssh-portable/ssh-agent.bc" -S -o "ssh-agent.ta.ll"

false && \
    opt-14 -enable-new-pm=0 -load "./build/TaintAnalysis/libTestSVF.so" \
       --test-svf \
       "tests/targets/secret.ll" -S -o "tests/targets/secret.ta.ll"


# opt-14 "tests/targets/gvn.ll" --inline -S | opt-14 --newgvn --dse --gvn-sink --gvn-hoist --mem2reg -S

false && \
opt-14 "tests/targets/gvn.ll" --early-cse --early-cse-memssa --newgvn --gvn-hoist --dse -S \
    | opt-14 --early-cse --early-cse-memssa --enable-cse-in-irtranslator -S \
    | opt-14 --gvn --gvn-hoist --gvn-sink --enable-gvn-hoist --enable-gvn-sink -S \
    | opt-14 --mldst-motion -S
# | opt-14 --dse --dse-memoryssa-otherbb-cost=1 -S

# opt-14 "tests/targets/alias.ll" --mem2reg --gvn -S | opt-14 --cfl-anders-aa --print-alias-sets -S

false && \
    opt-14 "tests/targets/alias.ll" --mem2reg -S

false && \
    opt-14 "tests/targets/gvn.ll" -O3 -S \
    | opt-14 --gvn -S


# opt-14 "tests/targets/aes.ll" -O3 --gvn -S -o "tests/targets/aes.O3.ll"

# --newgvn --dse -S

# opt-14 "tests/targets/hello.ta.ll" -O3 -S
