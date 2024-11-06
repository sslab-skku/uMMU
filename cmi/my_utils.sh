#!/bin/bash

my_run(){
    PREFIX=$(printf '%20s' "$1")
    shift
    echo "---------------------------"
    "$@" | sed -e "s/^/${PREFIX}/;"
}
