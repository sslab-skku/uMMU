#!/bin/bash
set -eux

[ ${DO_CONFIG:-1} -eq 1 ] && ./configure \
                                 --prefix=$HOME/.local/musl \
                                 --enable-optimize= \
                                 --enable-wrapper=all

make install
