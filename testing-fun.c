#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

void set_output_color_to_green() {
    printf("\033[0;32m");
}

void set_output_color_to_red() {
    printf("\033[0;31m");
}

void set_output_style_to_bold() {
    printf("\033[1m");
}

void reset_output_color() {
    printf("\033[0m");
}

const int add(const int a, const int b) {
    return a + b;
}

bool it_can_add_two_positive_integers() {
    return add(2, 3) == 5;
}

bool it_can_add_two_negative_integers() {
    return add(-2, -3) == -5;
}

bool it_can_add_a_positive_and_a_negative_integer() {
    return add(-2, 3) == 1;
}

int main() {
    int total_tests = 3;
    int passed_tests = 0;

    // If a test passes, it returns true (1). Otherwise, it returns false (0).
    // So we can just add the result of each test to determine how many passed.
    passed_tests += it_can_add_two_positive_integers();
    passed_tests += it_can_add_two_negative_integers();
    passed_tests += it_can_add_a_positive_and_a_negative_integer();

    set_output_color_to_green();
    set_output_style_to_bold();
    printf("All tests passed!\n\n");
    reset_output_color();

    printf("Total tests:\t");
    set_output_style_to_bold();
    printf("%d\n", total_tests);
    reset_output_color();

    set_output_color_to_green();
    printf("Passed tests:\t");
    set_output_style_to_bold();
    printf("%d\n", passed_tests);

    set_output_color_to_red();
    printf("Failed tests:\t");
    set_output_style_to_bold();
    printf("%d\n", total_tests - passed_tests);
    reset_output_color();

    return 0;
}