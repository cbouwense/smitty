clear
mkdir -p build/
gcc -Wall -Werror --pedantic -o build/test_$1 examples/test_$1.c
build/test_$1
