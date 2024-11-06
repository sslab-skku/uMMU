#!/bin/bash -eu

set -eux

cd "$(git rev-parse --show-toplevel)"

INST_TYPE=${1:-no-inst}

BC="WPA/out/ssh-agent.wpa.bc"
B="${BC%.bc}"

(cd internal && ./build.sh)
./run-passes.sh $B.bc
T=$B.inst.bc
llvm-dis $T
# T=$B.bc

# opt -always-inline $T -o $T.inline.bc
# llvm-dis $T.inline.bc
# llc --relocation-model=pic -filetype=obj $T.inline.bc -o $T.o
llc --relocation-model=pic -filetype=obj $T -o $T.o

musl-gcc -L./cmi/debug/src -static -O0 $T.o -o $T.out -lcmi_xor2
# musl-gcc -L./cmi/debug/src -static -O0 $T.o -o $T.out -lcmi_xor
# musl-gcc -L./cmi/debug/src -static -O0 $T.o -o $T.out -lcmi_aes
# musl-gcc -L./cmi/debug/src -static -O0 $T.o -o $T.out -lcmi_zmm

# clang $T.o -o $T.out

export WPA_DEBUG=0

export SSH_AUTH_SOCK=/tmp/sshagent.$(date +%s)
#gdb -ex=run -ex=backtrace --args \
$T.out -d -a $SSH_AUTH_SOCK &

SSH_AGENT_PID=$!
sleep 1

run_tests(){
    export KEY_FILE=/tmp/tmpkey$(date +%s%N)
    WPA/openssh-portable/ssh-keygen -f $KEY_FILE -N "1234" > /dev/null
    expect > /dev/null <<EOF
set timeout 3
spawn WPA/openssh-portable/ssh-add "$KEY_FILE"
expect "*ass*for*:*" { send "1234\r" }
expect eof
EOF

}

for i in $(seq 4);do
    run_tests
done

# check
ssh-add -l

