#!/bin/bash

echo "$ make"
make

echo "$ gcc -E grep.c -o grep.i"
gcc -E grep.c -o grep.i

echo "$./kcov grep.i"
./kcov grep.i &> kcov_warnings

echo "$ gcc grep-cov.c -o grep"
gcc grep-cov.c -o grep &> gcc_warnings
