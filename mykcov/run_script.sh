echo "-$ gcc -E test.c -o test.i"
gcc -E test.c -o test.i
echo ">> preprocess test.c complete!"
echo "-$ ./kcov test.i"
./kcov test.i
echo ">> made test-cov.c && inserting probe on beginning complete!"
echo "-$ gcc test-cov.c -o test-cov"
gcc test-cov.c -o test-cov
echo ">> compiling test-cov.c to test-cov complete!"
echo "-$ ./test-cov"
./test-cov
echo ">> ran test-cov!"
