#!/bin/bash

make
echo ">> Done compiling 'kcov-branch-identify.cpp"
echo "-------------------------------------------"

echo ">> Reporting the branch coverage of 'kcov-branch-identify' using gcov on 'example-kcov.c'"
./kcov-branch-identify ../example-kcov/example-kcov.c &> branch-identified-example
gcov -b kcov-branch-identify.cpp &> branch-coverage-example
echo ">> Saved branch identified information on 'branch-identified-example'"
echo ">> Saved branch coverage information on 'branch-coverage-example'"
echo "-------------------------------------------"

rm *.gcda
echo ">> Removed *.gcda file for renewal on branch coverage"
echo "-------------------------------------------"

echo ">> Reporting the branch coverage of 'kcov-branch-identify' using gcov on 'grep.c'"
./kcov-branch-identify ../grep-v2.0-simplified/grep.c &> branch-identified-grep
gcov -b kcov-branch-identify.cpp &> branch-coverage-grep
echo ">> Saved branch identified information on 'branch-identified-grep'"
echo ">> Saved branch coverage information on 'branch-coverage-grep'"
echo "-------------------------------------------"
