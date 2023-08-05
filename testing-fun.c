#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define expect(condition) return condition;

//--------------------------------------------------------------------------------------------------
// Time functions
//--------------------------------------------------------------------------------------------------

const double time_in_seconds(double time) {
    return time;
}

const double time_in_milliseconds(double time) {
    return time * 1000;
}

const double time_in_microseconds(double time) {
    return time * 1000000;
}

const double time_in_nanoseconds(double time) {
    return time * 1000000000;
}

void print_most_readable_time(double time) {
    if (time < 0.000001) {
        printf("%f ns", time_in_nanoseconds(time));
        return;
    }
    
    if (time < 0.001) {
        printf("%f µs", time_in_microseconds(time));
        return;
    }
    
    if (time < 1) {
        printf("%f ms", time_in_milliseconds(time));
        return;
    }

    printf("%f s", time_in_seconds(time));
}

//--------------------------------------------------------------------------------------------------
// Output style functions
//--------------------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------
// Functions under test
//--------------------------------------------------------------------------------------------------

const int add(const int a, const int b) {
    return a + b;
}

//--------------------------------------------------------------------------------------------------
// Tests
//--------------------------------------------------------------------------------------------------

bool it_can_add_two_positive_integers() {
    expect(add(2, 3) == 1);
}

bool it_can_add_two_negative_integers() {
    expect(add(-2, -3) == -2);
}

bool it_can_add_a_positive_and_a_negative_integer() {
    expect(add(-2, 3) == 1);
}

int main() {
    clock_t start = clock();

    int total_tests = 3;
    int passed_tests = 0;

    // If a test passes, it returns true (1). Otherwise, it returns false (0).
    // So we can just add the result of each test to determine how many passed.
    passed_tests += it_can_add_two_positive_integers();
    passed_tests += it_can_add_two_negative_integers();
    passed_tests += it_can_add_a_positive_and_a_negative_integer();

    if (total_tests == passed_tests) {
        set_output_color_to_green();
        set_output_style_to_bold();
        printf("All tests passed!\n\n");
        reset_output_color();
    } else {
        set_output_color_to_red();
        set_output_style_to_bold();
        printf("Some tests failed!\n\n");
        reset_output_color();
    }

    printf("Total tests:\t");
    set_output_style_to_bold();
    printf("%d\n", total_tests);
    reset_output_color();

    set_output_color_to_green();
    printf("Passed tests:\t");
    set_output_style_to_bold();
    printf("%d ", passed_tests);
    reset_output_color();
    set_output_color_to_green();
    printf("(%d%%)\n", (passed_tests * 100) / total_tests);

    set_output_color_to_red();
    printf("Failed tests:\t");
    set_output_style_to_bold();
    printf("%d ", total_tests - passed_tests);
    reset_output_color();
    printf("(%d%%)\n", ((total_tests - passed_tests) * 100) / total_tests);
    
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nTime spent:\t");
    set_output_style_to_bold();
    print_most_readable_time(time_spent);
    reset_output_color();

    return 0;
}
