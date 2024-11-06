#!/bin/bash -ex

cd "$(dirname "$0")" && cd "$(git rev-parse --show-toplevel)"

TARGET=${1:-base}

PORT=10080
ARGS=(
    -p $PWD/tests/data/nginx/
    -c $PWD/tests/data/nginx-auth.conf
     )

USERNAME="username"
PASSWORD="password"
HTPASSWD_FILE="$PWD/tests/data/nginx/.htpasswd"

mkdir -p .log
LOG_FILE=.log/nginx-auth-$TARGET.$(date +%s).log

set +e
BIND_PID="$(sudo lsof -t -i:$PORT -sTCP:LISTEN)"
[ -n "$BIND_PID" ] && sudo kill -9 $BIND_PID
set -eu

# RUN
# RUN_CMD=(
#     ./run-cmi.sh $TARGET
# )

# "${RUN_CMD[@]}" apps/nginx/out/nginx.inst.bc "${ARGS[@]}" 2>&1 \
#     | tee $LOG_FILE \
#     | sed -e 's/^/\x1B[35m[SERVER]\x1B[0m /;' &
# # PID=$!
# sleep 10


sudo nginx "${ARGS[@]}" 2>&1 \
    | tee $LOG_FILE \
    | sed -e 's/^/\x1B[35m[SERVER]\x1B[0m /;' &
# PID=$!
sleep 10

echo -n "${PASSWORD}" | htpasswd -i -c $PWD/tests/data/nginx/.htpasswd $USERNAME

# wget -O- localhost:$PORT/
curl --user "${USERNAME}:${PASSWORD}" "http://localhost:$PORT/secure"

set +e
BIND_PID="$(sudo lsof -t -i:$PORT -sTCP:LISTEN)"
[ -n "$BIND_PID" ] && sudo kill -9 $BIND_PID
set -e