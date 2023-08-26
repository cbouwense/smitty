clear
mkdir -p build/
gcc -Wall -Werror -o build/test_$1 examples/test_$1.c examples/$1.c src/smitty.c
build/test_$1
