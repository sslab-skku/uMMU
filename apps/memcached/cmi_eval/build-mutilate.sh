#!/bin/bash -eu

cd "$(dirname "$0")"
sudo true
sudo apt-get install -y scons libevent-dev gengetopt libzmq-dev &
git clone https://github.com/leverich/mutilate &
wait

cd mutilate
/usr/bin/env python2.7 $(which scons)
