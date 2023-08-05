#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

const int add(const int a, const int b) {
    return a + b;
}

const void it_can_add_two_positive_integers() {
    assert(add(2, 3) == 5);
}

const void it_can_add_two_negative_integers() {
    assert(add(-2, -3) == -5);
}

int main() {
    it_can_add_two_positive_integers();
    it_can_add_two_negative_integers();

    printf("All tests passed!\n");

    return 0;
}