#!/bin/bash

echo ">> NOW MEASURING BRANCH COVERAGE ON 'grep' with following commands"
echo "$ ./grep -n \"if\" grep.c > test1_output"
./grep -n "if" grep.c > test1_output

echo "$ ./grep -E \"[0-9][0-9]+\" grep.c > test2_output"
./grep -E "[0-9][0-9]+" grep.c > test2_output

echo "$ ./grep -E \"[[:digit:]][[:alpha:]]\" grep.c > test3_output"
./grep -E "[[:digit:]][[:alpha:]]" grep.c > test3_output

cat coverage.dat
