# uMMU: Securing Data Confidentiality with Unobservable Memory Subsystem.

This repository contains the proof-of-concept implementation of the *uMMU software hardening framework*, published at ACM CCS 2024.

# Project Structure
## `/`
- `run-cmi.sh`: Run the instrumented application.
    - ex) `./run-cmi.sh page6_rasm_indjmp_aes $APP.inst.bc`
## `llvm-pass/`: Load/Store Instrumentation Pass
Instrumentation passes for the uMMU framework.
- SVF is used to analyze the program and identify the sensitive load/store instructions.
- Rewrite load/store instructions to invoke the `cmi/`.
- `llvm-pass/run-passes.sh`: Run the LLVM passes.
    - ex) `./run-passes.sh $APP.bc` to get the instrumented bitcode(=`$APP.inst.bc`).

## `cmi/`: Custom Memory Interface (CMI) Library
`cmi/` includes the implementation of customized memory interfaces that invoked by the instrumented applications.

- Implementation of *Memory Confidentiality Policy(MCP)*
    - *Oblivious RAM* using *Path ORAM*.
    - *In-Memory Encryption* using *AES*.
    - *In-Memory Salting* using *rdrand*.
- *Register as Memory(RasM)* implementation.
- *uMMU*'s Page Management System
- Algorithm Benchmarks.

## `apps/`: Target Applications
`apps/` includes the target applications including dependencies that are used to evaluate the uMMU framework.

- `mbedtls/`: MbedTLS
- `memcached/`: Memcached


# Setup
Add `$HOME/.local/musl/bin` to the `PATH` variable.

```bash
export PATH=$HOME/.local/musl/bin:$PATH
./setup.sh
```

# Build

## Build LLVM pass & libs
```bash
cd llvm-pass
./build.sh
cd -
cd cmi/
./setup.sh
./build.sh
cd -
```

## Build Applications
### MbedTlS
```bash
cd apps/mbedtls
./build.sh
cd -
```

### memcached
```bash
cd apps/memcached
./build.sh
cd -
```


# Examples

## Algorithm Benchmarks

### In-Memory Encryption with SSE supports

```bash
export TEST=all
export LEN=1700
export CNT=1000
cmi/eval-sse-sup.sh
```

### In-Memory Salting with rdrand

```bash
cmi/eval-rdrand.sh base
cmi/eval-rdrand.sh page6_rdrand
```

### ORAM

```bash
cmi/eval-oram.sh matmul
```


### Run Mbed-TLS

```bash
APP=apps/mbedtls/out/eval
llvm-pass/run-passes.sh $APP.bc

echo ">>>> BASELINE <<<<"
./run-cmi.sh base apps/mbedtls/out/eval.inst.bc

echo ">>>> uMMU, MCP: In-Memory Encryption <<<<"
./run-cmi.sh page6_rasm_indjmp_aes $APP.inst.bc 2> /tmp/cmi.log
```

### Run Memcached

```bash
cd apps/memcached/cmi_eval/
./build-libmemcached.sh
./eval-memaslap.sh
```

# Bibtex
TODO
