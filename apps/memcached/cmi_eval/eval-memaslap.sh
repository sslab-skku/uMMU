#!/bin/bash -eu
cd "$(dirname "$0")"
MC_ROOT=$(realpath ../)
PROJ=$(realpath "$MC_ROOT/../../")


# Transfomr bitcode
BC_IN=$MC_ROOT/out/memcached.wpa.bc
$PROJ/llvm-pass/run-anno-cmi-transform.sh $BC_IN
BC_OUT=$MC_ROOT/out/memcached.wpa.anno-cmi.inst.bc
BIN_PREFIX=${BC_OUT%.bc}
# MEMASLAP_BIN=$PWD/build-libmemcached/src/bin/memslap
MEMASLAP_BIN=$PWD/build-libmemcached/contrib/bin/memaslap/memaslap

CMI_IMPL_LIST=(
    base
    xor2
    # aes
    # page6_rasm_indjmp_base
    oramB64Z4L12
    page6_oramB64Z4L12
    page6_rasm_indjmp_base
    page6_base_base
    page6_rasm_indjmp_base_sse2
    page6_rasm_indjmp_aes
    page6_rasm_indjmp_aes_sse2
)

# Build Binary
$PROJ/build-cmi-app.sh base $BC_IN
for CMI_IMPL in "${CMI_IMPL_LIST[@]}";do
    $PROJ/build-cmi-app.sh $CMI_IMPL $BC_OUT &
done
wait

MC_ARGS=(
    --threads=1
    --max-reqs-per-event=20
)

main(){
    PORT=${1:-12345}
    DO_DEBUG=${2:-false}
    # RunEval page6_rasm_indjmp_base_sse2
    # RunEval page6_rasm_indjmp_aes_sse2
    # RunEval
    #for i in $(seq 16 32);do
    # E=$((2**(i/2+1)+(2**(i/2))*(i%2)))
    TESTS=(
        # mget
        get
        set
    )
    CLIENT_NUMS=(
        1
        #2
        #4
        #8
    )
    TARGETS=(
        # memcached
        # $BIN_PREFIX.base.out
        #$BIN_PREFIX.oramB64Z4L12.out
        $BIN_PREFIX.page6_oramB64Z4L12.out
    )
    LEN_MIN=500
    LEN_MAX=32000
    #LEN_MAX=2000
    LEN_SCALE=2

    for Target in "${TARGETS[@]}" ;do
        for Test in "${TESTS[@]}";do
            for C in "${CLIENT_NUMS[@]}";do
                for (( LEN=LEN_MIN; LEN<=LEN_MAX; LEN*=LEN_SCALE ));do
                    memslap_RunEval $Target -t $Test -c $C -e $LEN
                done
            done
        done
    done
}

memslap_RunEval(){
    KillPidWithPort $PORT
    WaitPortClose $PORT
    local target=$1
    shift

    local prefix=$(basename $target)
    prefix=$(printf '%20s, ' ${prefix%.out})

    Run $target -p $PORT "${MC_ARGS[@]}" &


    WaitPortOpen $PORT
    memslap -s 127.0.0.1:$PORT "$@" | sed "s/^/$prefix/"

    KillPidWithPort $PORT
    WaitPortClose $PORT
}

Run(){
    if $DO_DEBUG;then
        RunWithGDB "$@"
    else
        "$@"
    fi
}

RunWithGDB(){
    GDB_CMD=(gdb -ex 'set follow-fork-mode child' -ex run -ex 'disas' -ex backtrace --args)
    "${GDB_CMD[@]}" "$@"
}

KillPidWithPort(){
    local pid=$(lsof -t -i:$1)
    kill -9 $pid &> /dev/null || true
}

WaitPortClose(){
    while lsof -t -i:$1 &> /dev/null;do
        sleep 0.5
    done
}

WaitPortOpen(){
    while ! lsof -t -i:$1 > /dev/null ;do
        sleep 0.5
    done
}


main "$@"
