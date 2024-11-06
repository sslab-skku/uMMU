#!/bin/bash

((echo "error">&3 ; echo "stdout") \
     3> >(tee /dev/fd/4 /dev/fd/5)) \
    4> >(sed -e "s/^/4 /;")  5> >(sed -e "s/^/5 /;") 1> >(sed -e "s/^/S /;")
