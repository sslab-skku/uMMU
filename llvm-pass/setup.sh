#!/bin/bash -eu

export SVF_DIR=$(realpath ../SVF)

# Release build
mkdir -p release/
cmake -S . -B release/ -DCMAKE_BUILD_TYPE=Release -DSVF_DIR=${SVF_DIR}

# Debug
mkdir -p debug/
cmake -S . -B debug/ -DCMAKE_BUILD_TYPE=Debug -DSVF_DIR=${SVF_DIR}
ln -sf "$PWD/debug/compile_commands.json" compile_commands.json
