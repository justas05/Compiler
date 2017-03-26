#!/bin/bash

mips-linux-gnu-gcc -std=c89 -c -S -x c - -o -
exit $?
