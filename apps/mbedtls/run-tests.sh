#!/bin/bash -eu

cd "$(dirname "$0")"
cd ../../

ARGS=$@

MSG=$(openssl rand -hex 1000 |head -c 64)
echo $MSG

main(){
    echo ""
}



APP=apps/mbedtls/out/eval
llvm-pass/run-passes.sh $APP.bc
set +ue
# ./run-cmi.sh base apps/mbedtls/out/eval.inst.bc
#./run-cmi.sh zmm apps/mbedtls/out/eval.inst.bc
# $APP "$@"
./run-cmi.sh aes $APP.inst.bc "$@"
./run-cmi.sh page6_rasm_perm_indjmp_aes $APP.inst.bc "$@"
# ./run-cmi.sh page6_rasm_indjmp_aes $APP.inst.bc # "${ARGS[@]}"
set -ue
# APP=apps/mbedtls/out/eval_rsa
# APP=apps/mbedtls/out/eval_rsa
# llvm-pass/run-passes.sh $APP.bc
# ./run-cmi.sh page6_rasm_indjmp_aes $APP.inst.bc "${ARGS[@]}"
# ./run-cmi.sh base $APP.inst.bc "${ARGS[@]}"
# ./run-cmi.sh page6_rasm_indjmp_aes_sse2 $APP.inst.bc "${ARGS[@]}"
# ./run-cmi.sh aes $APP.inst.bc "${ARGS[@]}"

# ./run-cmi.sh base $APP.inst.bc
# ./run-cmi.sh zmm_perm $APP.inst.bc
# ./run-cmi.sh aes $APP.inst.bc
# ./run-cmi.sh vmi_page_perm_aes_dbg $APP.inst.bc
# ./run-cmi.sh vmi_page_perm_aes $APP.inst.bc



# APP=apps/mbedtls/out/eval_key
# ARGS=(rsa)
# llvm-pass/run-passes.sh $APP.bc
# ./run-cmi.sh vmi_page_perm_aes $APP.inst.bc "${ARGS[@]}"
# ./run-cmi.sh zmm_perm $APP.inst.bc "${ARGS[@]}"
# ./run-cmi.sh base $APP.bc "${ARGS[@]}"
# ./run-cmi.sh aes $APP.bc "${ARGS[@]}"
# ./run-cmi.sh xor $APP.bc "${ARGS[@]}"
# ./run-cmi.sh zmm_indjmp $APP.inst.bc "${ARGS[@]}"



# llvm-pass/run-passes.sh apps/mbedtls/out/rsa_encrypt.bc
# llvm-pass/run-passes.sh apps/mbedtls/out/rsa_decrypt.bc

# apps/mbedtls/programs/pkey/gen_key && cp apps/mbedtls/programs/pkey/rsa_*.txt ./
# ./run-cmi.sh $TARGET apps/mbedtls/out/rsa_encrypt.inst.bc $MSG
# ./run-cmi.sh $TARGET apps/mbedtls/out/rsa_decrypt.inst.bc


