#include "smitty.h"

// #define SMITTY_VERBOSE

//--------------------------------------------------------------------------------------------------
// Expects
//--------------------------------------------------------------------------------------------------

smitty_expect_result expect_int_equal_internal(const int actual, const int expected, const char *test_name, const char *file, const int line) {
    if (actual == expected) return EXPECT_PASS;

    smitty_print_test_failure(test_name, file, line);

    print_red("Actual:\t  %d\n", actual);
    printf("Expected: %d\n\n", expected);

    return EXPECT_FAIL;
}

smitty_expect_result expect_string_equal_internal(const char *actual, const char* expected, const char *test_name, const char *file, const int line) {
    if (strcmp(actual, expected) == 0) return EXPECT_PASS;

    smitty_print_test_failure(test_name, file, line);

    print_red("Actual:\t  %s\n", actual);
    printf("Expected: %s\n\n", expected);

    return EXPECT_FAIL;
}

smitty_expect_result expect_true_internal(const bool actual, const char *test_name, const char *file, const int line) {
    if (actual == true) return EXPECT_PASS;
    
    smitty_print_test_failure(test_name, file, line);

    print_red("Actual:\t  %d\n", actual);
    printf("Expected to be true\n\n");

    return EXPECT_FAIL;
}

smitty_expect_result expect_false_internal(const bool actual, const char *test_name, const char *file, const int line) {
    if (actual == false) return EXPECT_PASS;
    
    smitty_print_test_failure(test_name, file, line);

    print_red("Actual:\t  %d\n", actual);
    printf("Expected to be false\n\n");

    return EXPECT_FAIL;
}

smitty_expect_result expect_null_internal(const void *actual, const char *test_name, const char *file, const int line) {
    if (actual == NULL) return EXPECT_PASS;

    smitty_print_test_failure(test_name, file, line);

    print_red("Actual:\t  %p\n", actual);
    printf("Expected: %p\n\n", NULL);

    return EXPECT_FAIL;
}

smitty_expect_result expect_non_null_internal(const void *actual, const char *test_name, const char *file, const int line) {
    if (actual != NULL) return EXPECT_PASS;
    
    smitty_print_test_failure(test_name, file, line);

    print_red("Actual:\t  %p\n", actual);
    printf("Expected to be not NULL\n\n");

    return EXPECT_FAIL;
}

smitty_expect_result expect_pointer_equal_internal(const void *actual, const void *expected, const char *test_name, const char *file, const int line) {
    if (actual == expected) return EXPECT_PASS;
    
    smitty_print_test_failure(test_name, file, line);

    print_red("Actual:\t  %p\n", actual);
    printf("Expected: %p\n\n", expected);

    return EXPECT_FAIL;
}

//--------------------------------------------------------------------------------------------------
// Test runner core
//--------------------------------------------------------------------------------------------------

void smitty_run_tests(smitty_test_case_func tests[], void (*before_each)(), void (*after_each)()) {
    clock_t start = clock();

    int passed_test_count = 0;
    int failed_test_count = 0;
    int total_test_count = 0;

    // Count the number of tests, assuming that the tests array is null terminated.
    for (int i = 0; tests[i] != NULL; i++) {
        total_test_count++;
    }

    // The maximum amount of failures is the total amount of tests, so allocate that much space.
    const char *failed_test_names[total_test_count];

    // Run each test, its hooks, and record results.
    for (int i = 0; tests[i] != NULL; i++) {

        // TODO: maybe we should have a way to see if the before_each function failed?
        // Run before_each if we have one.
        if (before_each != NULL) before_each();

        const char *test_name = smitty_test_case_name(tests[i]);
        const smitty_test_result result = smitty_run_test(
            tests[i],
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
    print_bold(get_most_readable_time(time_spent));
    printf("================================\n");
}

smitty_test_result smitty_run_test(smitty_test_case_func test, void (*before_each)(), void (*after_each)()) {
    if (before_each != NULL) before_each();
    
    const smitty_test_result result = test == NULL ? TEST_NOT_FOUND : test();
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

//--------------------------------------------------------------------------------------------------
// Test utilities
//--------------------------------------------------------------------------------------------------

void smitty_print_test_failure(const char *test_name, const char *file, const int line) {
    print_red_bold(">-- FAIL --> ");
    print_red("%s", test_name);
    printf(" | (%s - line %d)\n\n", file, line);
}

const char *smitty_test_result_to_string(smitty_test_result result) {
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

const short time_in_seconds(double time) {
    // Cast to short to truncate meaningless decimal places.
    return (short)time;
}

const short time_in_milliseconds(double time) {
    // Cast to short to truncate meaningless decimal places.
    return (short)(time * 1000);
}

const short time_in_microseconds(double time) {
    // Cast to short to truncate meaningless decimal places.
    return (short)(time * 1000000);
}

const short time_in_nanoseconds(double time) {
    // Cast to short to truncate meaningless decimal places.
    return (short)(time * 1000000000);
}

const char *get_most_readable_time(double time) {
    static char buffer[42];

    if      (time < 0.000001) sprintf(buffer, "%d ns\n", time_in_nanoseconds(time));
    else if (time < 0.001)    sprintf(buffer, "%d µs\n", time_in_microseconds(time));
    else if (time < 1)        sprintf(buffer, "%d ms\n", time_in_milliseconds(time));
    else                      sprintf(buffer, "%d s",    time_in_seconds(time));

    assert(strlen(buffer) < 42 && "Time string is too long. Aborting to avoid any undefined behavior.");

    return buffer;
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
