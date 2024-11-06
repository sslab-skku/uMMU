#!/bin/bash -eu

cd "$(dirname "$0")"

source ./my_utils.sh

BIN_DIR="$PWD/debug/bin"

# LEN=${1:-1600}

# Rf: page fault rate
# Ks: uStroage Size
# L: array length
Rf=${1:-0}
Ks=$((28*64))
LEN=$(((100*Ks)/(100-Rf)))

CNT=${2:-1}

for LEN in $(seq 1000 5000 64000);do
    my_run "${LEN} base" ${BIN_DIR}/test_cmi_base $LEN $CNT
    my_run "${LEN} aes" ${BIN_DIR}/test_cmi_aes $LEN $CNT
    # for i in $(seq 3 8);do
    for i in $(seq 6 6);do
        my_run "${LEN} page${i}" ${BIN_DIR}/test_cmi_vmi_page${i}_perm_aes $LEN $CNT
    done
done

