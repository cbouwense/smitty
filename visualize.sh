gcc -g -o remove-elements remove-elements.c
valgrind --tool=massif ./remove-elements
# ms_print massif.out. > memory_usage.txt
