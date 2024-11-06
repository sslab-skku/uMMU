#!/bin/bash -eu

cd "$(dirname "$0")"
MC_ROOT=$(realpath ../)
PROJ=$(realpath "$MC_ROOT/../../")


PORT=${1:-11223}
kill -9 $(lsof -t -i:$PORT) 2&>1 >/dev/null || true

export DO_DEBUG=${2:-0}


BC_IN=$MC_ROOT/out/memcached.wpa.bc
BC_OUT=$MC_ROOT/out/memcached.wpa.anno-cmi.inst.bc
# BC_OUT=$MC_ROOT/out/memcached.wpa.anno-cmi.noinst.bc
# BC_OUT=$MC_ROOT/out/memcached.wpa.cmi.bc

$PROJ/llvm-pass/run-anno-cmi-transform.sh $BC_IN


PRE_LIST=(
    # 1000
    # 2000
    10000
    # 4000
)

#CMI_IMPL=base
# CMI_IMPL=aes
#CMI_IMPL=oram
CMI_IMPL_LIST=(
    #base
    # xor2
    # aes
    # page6_rasm_indjmp_base
    oramB64Z4L12
    page6_oramB64Z4L12
)
# rexport DO_DEBUG=1

REQ_NUM=1000
run_test(){
    kill -9 $(lsof -t -i:$PORT) 2&>1 >/dev/null || true
    $PROJ/run-cmi.sh $1 $2  -p $PORT &
    sleep 5
    MC_PID=$!
    python3 ./run_memcached_test.py --server_port $PORT --preload_num $3 --get_set_ratio=0.5 --test_num=$4 --req_num=$REQ_NUM
    kill $MC_PID
    sleep 1
}
# python3 ./run_memcached_test.py --server_port $PORT --preload_num $3 --get_set_ratio=1.0 --test_num=100 --req_num=1000

TEST_NUM=1000
for PRE in "${PRE_LIST[@]}";do
    for TN in $(seq 1000 200 $PRE);do
        # non inst
        run_test base $BC_IN $PRE $TN
        for CMI_IMPL in "${CMI_IMPL_LIST[@]}";do
            run_test $CMI_IMPL $BC_OUT $PRE $TN
        done
    done
done
