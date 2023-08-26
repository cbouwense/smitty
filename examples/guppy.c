#include "guppy.h"

void guppy_print_int_array_internal(int array[], size_t size_of_array) {
    const size_t length = size_of_array / sizeof(int);
    
    printf("[");
    for (size_t i = 0; i < length; i++) {
        printf("%d", array[i]);
        if (i < length - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    int foo[4] = {1, 2, 3, 4};

    guppy_print_int_array(foo);

    return 0;
}
