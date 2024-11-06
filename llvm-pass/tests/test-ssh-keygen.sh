#!/bin/bash

set -eux

cd "$(git rev-parse --show-toplevel)"

BC_IN="WPA/out/ssh-keygen.wpa.bc"
BC_OUT="${BC_IN%.bc}.inst.bc"
# BC_OUT="${BC_IN%.bc}.anno.bc"

./run-passes.sh $BC_IN

KEY_OUT=/tmp/sshkey
rm -rf $KEY_OUT

# export LD_PRELOAD=libSegFault.so
export WPA_DEBUG=1
WPA/run-bc.sh $BC_OUT -f $KEY_OUT -N ""
