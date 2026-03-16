#!/bin/bash
cd ../build || exit
if [ "$1" == "-D" ]; then
    gdb ./SMH
else
    ulimit -c unlimited
    ./SMH
fi