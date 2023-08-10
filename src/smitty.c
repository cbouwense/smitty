#include "smitty.h"

// #define SMITTY_VERBOSE

//--------------------------------------------------------------------------------------------------
// Expects
//--------------------------------------------------------------------------------------------------

// TODO: DRY up the error reporting
smitty_expect_result expect_int_equal_internal(const int actual, const int expected, const char *test_name, const char *file, const int line) {
    if (actual != expected) {
        print_red_bold(">-- FAIL --> ");
        print_red("%s", test_name);
        printf(" | (%s - line %d)\n\n", file, line);

        print_red("Actual:\t  %d\n", actual);
        printf("Expected: %d\n\n", expected);

        return EXPECT_FAIL;
    }

    return EXPECT_PASS;
}

smitty_expect_result expect_string_equal_internal(const char *actual, const char* expected, const char *test_name, const char *file, const int line) {
    if (strcmp(actual, expected) == 0) return EXPECT_PASS;

    print_red_bold(">-- FAIL --> ");
    print_red("%s", test_name);
    printf(" | (%s - line %d)\n\n", file, line);

    print_red("Actual:\t  %s\n", actual);
    printf("Expected: %s\n\n", expected);

    return EXPECT_FAIL;
}

smitty_expect_result expect_true_internal(const bool actual, const char *test_name, const char *file, const int line) {
    if (actual == true) return EXPECT_PASS;
    
    print_red_bold(">-- FAIL --> ");
    print_red("%s", test_name);
    printf(" | (%s - line %d)\n\n", file, line);

    print_red("Actual:\t  %d\n", actual);
    printf("Expected to be true\n\n");

    return EXPECT_FAIL;
}

smitty_expect_result expect_false_internal(const bool actual, const char *test_name, const char *file, const int line) {
    if (actual == false) return EXPECT_PASS;
    
    print_red_bold(">-- FAIL --> ");
    print_red("%s", test_name);
    printf(" | (%s - line %d)\n\n", file, line);

    print_red("Actual:\t  %d\n", actual);
    printf("Expected to be false\n\n");

    return EXPECT_FAIL;
}

smitty_expect_result expect_null_internal(const void *actual, const char *test_name, const char *file, const int line) {
    if (actual == NULL) return EXPECT_PASS;

    print_red_bold(">-- FAIL --> ");
    print_red("%s", test_name);
    printf(" | (%s - line %d)\n\n", file, line);

    print_red("Actual:\t  %p\n", actual);
    printf("Expected: %p\n\n", NULL);

    return EXPECT_FAIL;
}

smitty_expect_result expect_non_null_internal(const void *actual, const char *test_name, const char *file, const int line) {
    if (actual != NULL) return EXPECT_PASS;
    
    print_red_bold(">-- FAIL --> ");
    print_red("%s", test_name);
    printf(" | (%s - line %d)\n\n", file, line);

    print_red("Actual:\t  %p\n", actual);
    printf("Expected to be not NULL\n\n");

    return EXPECT_FAIL;
}

smitty_expect_result expect_pointer_equal_internal(const void *actual, const void *expected, const char *test_name, const char *file, const int line) {
    if (actual == expected) return EXPECT_PASS;
    
    print_red_bold(">-- FAIL --> ");
    print_red("%s", test_name);
    printf(" | (%s - line %d)\n\n", file, line);

    print_red("Actual:\t  %p\n", actual);
    printf("Expected: %p\n\n", expected);

    return EXPECT_FAIL;
}

//--------------------------------------------------------------------------------------------------
// Test runner core
//--------------------------------------------------------------------------------------------------

void smitty_run_tests(smitty_test_case_info tests[], void (*before_each)(), void (*after_each)()) {
    clock_t start = clock();

    int passed_test_count = 0;
    int failed_test_count = 0;
    int total_test_count = 0;

    // TODO: it would be cool if we used one of my SafeScrumpBuffers for this.
    // Count the number of tests, assuming that the tests array is null terminated.
    for (int i = 0; tests[i].name != NULL; i++) {
        total_test_count++;
    }

    // The maximum amount of failures is the total amount of tests, so allocate that much space.
    const char *failed_test_names[total_test_count];

    // Run each test, its hooks, and record results.
    for (int i = 0; tests[i].name != NULL; i++) {

        // TODO: maybe we should have a way to see if the before_each function failed?
        // Run before_each if we have one.
        if (before_each != NULL) before_each();

        const char *test_name = tests[i].name;
        const smitty_test_result result = smitty_run_test(
            tests[i].name, 
            tests,
            before_each,
            after_each
        );

        // TODO: maybe we should have a way to see if the after_each function failed?
        // Run after_each if we have one.
        if (after_each != NULL) after_each();

        switch (result) {
            case TEST_PASS:
                passed_test_count++;
                break;

            /* 
             * NOTE: Currently TEST_NOT_FOUND is counted as a failure, but it might be better to
             * count it as a separate category to report.
             */
            case TEST_NOT_FOUND:
            case TEST_FAIL:
                failed_test_names[failed_test_count] = test_name;
                failed_test_count++;
                break;

            default:
                passed_test_count++;
                break;
        }
    }

    printf("================================\n");
    printf("Total tests:\t");
    print_bold("%d\n", total_test_count);

    print_green("Passed tests:\t");

    print_green_bold("%d ", passed_test_count);
    print_green("(%d%%)\n", (passed_test_count * 100) / total_test_count);

    print_red_bold("Failed tests:\t%d ", total_test_count - passed_test_count);
    print_red("(%d%%)\n", ((total_test_count - passed_test_count) * 100) / total_test_count);

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("--------------------------------\n");
    printf("Time spent:\t");
    // TODO: refactor this to use a to string function so we can pass it into our normal print functions.
    set_output_style_to_bold();
    print_most_readable_time(time_spent);
    reset_output_style();
    printf("================================\n");
}

smitty_test_result (*find_test_by_name(const char *name, smitty_test_case_info tests[]))() {
    for (int i = 0; tests[i].name != NULL; i++) {
        if (strcmp(name, tests[i].name) == 0) {
            return tests[i].test_case;
        }
    }

    printf("Test not found: %s\n", name);
    return NULL;
}

smitty_test_result smitty_run_test(const char *name, smitty_test_case_info tests[], void (*before_each)(), void (*after_each)()) {
    smitty_test_result (*test_case)() = find_test_by_name(name, tests);

    if (before_each != NULL) before_each();
    
    const smitty_test_result result = test_case == NULL ? TEST_NOT_FOUND : test_case();
    // TODO: we could even have a "simple" mode where it only says "All passed" or "Some failed" with the failures.
    #ifdef SMITTY_VERBOSE
    printf("%s: %s\n", name, smitty_test_result_to_string(result));
    #endif

    if (after_each != NULL) after_each();

    switch (result) {
        case TEST_PASS:    
        default:
            return TEST_PASS;
        case TEST_FAIL:
        case TEST_NOT_FOUND:
            return TEST_FAIL;
    }
}

char *smitty_test_result_to_string(smitty_test_result result) {
    switch (result) {
        case TEST_PASS:
            return "PASS";
        case TEST_FAIL:
            return "FAIL";
        case TEST_NOT_FOUND:
            return "NOT FOUND";
        default:
            return "UNKNOWN";
    }
}

//--------------------------------------------------------------------------------------------------
// Time utilities
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
        printf("%f ns\n", time_in_nanoseconds(time));
        return;
    }
    
    if (time < 0.001) {
        printf("%f µs\n", time_in_microseconds(time));
        return;
    }
    
    if (time < 1) {
        printf("%f ms\n", time_in_milliseconds(time));
        return;
    }

    printf("%f s", time_in_seconds(time));
}

//--------------------------------------------------------------------------------------------------
// Output style utilities
//--------------------------------------------------------------------------------------------------

void print_bold(const char *string, ...) {
    set_output_style_to_bold();

    va_list args;
    va_start(args, string);
    vprintf(string, args);
    va_end(args);

    reset_output_style();
}

void print_green(const char *string, ...) {
    set_output_color_to_green();
    
    va_list args;
    va_start(args, string);
    vprintf(string, args);
    va_end(args);

    reset_output_style();
}

void print_red(const char *string, ...) {
    set_output_color_to_red();
    
    va_list args;
    va_start(args, string);
    vprintf(string, args);
    va_end(args);

    reset_output_style();
}

void print_green_bold(const char *string, ...) {
    set_output_color_to_green();
    set_output_style_to_bold();

    va_list args;
    va_start(args, string);
    vprintf(string, args);
    va_end(args);

    reset_output_style();
}

void print_red_bold(const char *string, ...) {
    set_output_color_to_red();
    set_output_style_to_bold();

    va_list args;
    va_start(args, string);
    vprintf(string, args);
    va_end(args);

    reset_output_style();
}

void set_output_color_to_green() {
    printf("\033[0;32m");
}

void set_output_color_to_red() {
    printf("\033[0;31m");
}

void set_output_style_to_bold() {
    printf("\033[1m");
}

void reset_output_style() {
    printf("\033[0m");
}