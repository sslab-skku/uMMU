#!/bin/bash -eu

# Release build
mkdir -p release/
cmake -S . -B release/ -DCMAKE_BUILD_TYPE=Release

# Debug
mkdir -p debug/
cmake -S . -B debug/ -DCMAKE_BUILD_TYPE=Debug
ln -sf "./debug/compile_commands.json" compile_commands.json


