gcc -g -o remove-elements remove-elements.c
valgrind --leak-check=full ./remove-elements
