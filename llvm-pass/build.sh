#!/bin/bash -eux

release(){
    cmake --build release/
}

debug(){
    cmake --build debug/
}

"${1:-debug}"

internal/build.sh
