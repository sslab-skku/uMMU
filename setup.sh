#!/bin/bash -eu

# Build Deps

## Build SVF
cd SVF/
./build.sh
cd ..

## Build musl
cd musl/
./build.sh
cd ..

# Setup

# Setup llvm-pass
cd llvm-pass/
./setup.sh
cd ..

# Setup cmi/
cd cmi/
./setup.sh
cd ..
