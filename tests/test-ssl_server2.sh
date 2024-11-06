#!/bin/bash -eu

cd "$(dirname "$0")" && cd "$(git rev-parse --show-toplevel)"

TARGET=$1

PORT=3344
ARGS=(server_port=$PORT
      server_addr=127.0.0.1
      ca_file=tests/data/ca.crt
      crt_file=tests/data/host.crt
      key_file=tests/data/host.key
      auth_mode=optional
     )

mkdir -p .log
LOG_FILE=.log/$TARGET.$(date +%s).log

set +e
BIND_PID="$(sudo lsof -t -i:$PORT -sTCP:LISTEN)"
[ -n "$BIND_PID" ] &&  -9 $BIND_PID
set -e

# RUN
./run-cmi.sh $TARGET apps/mbedtls/out/ssl_server2.inst.bc "${ARGS[@]}" 2>&1 \
    | tee $LOG_FILE \
    | sed -e 's/^/\x1B[35m[SERVER]\x1B[0m /;'&
PID=$!

sleep 3

REQ_SIZE=200
for i in $(seq 1);do
    apps/mbedtls/out/ssl_client2  "${ARGS[@]}" request_size=${REQ_SIZE} 2>&1 | sed -e 's/^/\x1B[36m[CLIENT]\x1b[0m: /;'
done

kill $PID || true
sleep 1
grep "\[E\]" $LOG_FILE > .log/eval-$TARGET.txt

echo "================= RESULT =================="
cat .log/eval-$TARGET.txt
