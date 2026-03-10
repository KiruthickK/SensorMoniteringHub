#!/bin/bash
cd ../build || exit
if [ "$1" == "-D" ]; then
    gdb ./SMH
else
    ./SMH
fi