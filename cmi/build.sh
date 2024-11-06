#!/bin/bash -eu

release(){
    cmake --build release/ -j$(($(nproc))/2)
}

debug(){
    cmake --build debug/ -j$(($(nproc)/2))
}

"${1:-debug}"
