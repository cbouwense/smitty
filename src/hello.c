#include <assert.h>
#include <stdio.h>

#define STACK_ARRAY_SIZE 1024

typedef struct {
    size_t size;
    int array[STACK_ARRAY_SIZE];
    int read_cursor;
    int write_cursor;
    int high_water_mark;
} StackArray;



const StackArray remove_first_element(StackArray stackArray) {
    StackArray new_stack_array;
    for (int i = 1; i < STACK_ARRAY_SIZE; i++) {
        new_stack_array.array[i-1] = stackArray.array[i];
    }
    return new_stack_array;
}

int main() {
    StackArray full_array = {{1,2,3}};
    StackArray array_without_first_element = remove_first_element(full_array);

    printf("[%d, %d, %d]\n", full_array.array[0], full_array.array[1], full_array.array[2]);
    printf(
        "[%d, %d, %d]\n",
        array_without_first_element.array[0],
        array_without_first_element.array[1],
        array_without_first_element.array[2]
    );

    return 0;
}
