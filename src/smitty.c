#include "smitty.h"

// TODO: There should be a better way of enabling these modes.
// #define SMITTY_VERBOSE
// #define SMITTY_ZEN

//--------------------------------------------------------------------------------------------------
// Expects
//--------------------------------------------------------------------------------------------------

SmittyExpectResultType expect_int_equal_internal(const int actual, const int expected, const char *test_name, const char *file, const int line) {
    if (actual == expected) return EXPECT_PASS;

    #ifdef SMITTY_ZEN
    print_red("✘ %s\n", test_name);
    #else
    print_red("✘ %s | (%s - line %d)\n\n", test_name, file, line);
    #endif

    print_red("Actual:\t  %d\n", actual);
    printf("Expected: %d\n\n", expected);

    return EXPECT_FAIL;
}

SmittyExpectResultType expect_string_equal_internal(const char *actual, const char* expected, const char *test_name, const char *file, const int line) {
    if (strcmp(actual, expected) == 0) return EXPECT_PASS;

    #ifdef SMITTY_ZEN
    print_red("✘ %s\n", test_name);
    #else
    print_red("✘ %s | (%s - line %d)\n\n", test_name, file, line);
    #endif

    print_red("Actual:\t  %s\n", actual);
    printf("Expected: %s\n\n", expected);

    return EXPECT_FAIL;
}

SmittyExpectResultType expect_true_internal(const bool actual, const char *test_name, const char *file, const int line) {
    if (actual == true) return EXPECT_PASS;
    
    #ifdef SMITTY_ZEN
    print_red("✘ %s\n", test_name);
    #else
    print_red("✘ %s | (%s - line %d)\n\n", test_name, file, line);
    #endif

    print_red("Actual:\t  %d\n", actual);
    printf("Expected to be true\n\n");

    return EXPECT_FAIL;
}

SmittyExpectResultType expect_false_internal(const bool actual, const char *test_name, const char *file, const int line) {
    if (actual == false) return EXPECT_PASS;
    
    #ifdef SMITTY_ZEN
    print_red("✘ %s\n", test_name);
    #else
    print_red("✘ %s | (%s - line %d)\n\n", test_name, file, line);
    #endif

    print_red("Actual:\t  %d\n", actual);
    printf("Expected to be false\n\n");

    return EXPECT_FAIL;
}

SmittyExpectResultType expect_null_internal(const void *actual, const char *test_name, const char *file, const int line) {
    if (actual == NULL) return EXPECT_PASS;

    #ifdef SMITTY_ZEN
    print_red("✘ %s\n", test_name);
    #else
    print_red("✘ %s | (%s - line %d)\n\n", test_name, file, line);
    #endif

    print_red("Actual:\t  %p\n", actual);
    printf("Expected: %p\n\n", NULL);

    return EXPECT_FAIL;
}

SmittyExpectResultType expect_non_null_internal(const void *actual, const char *test_name, const char *file, const int line) {
    if (actual != NULL) return EXPECT_PASS;
    
    #ifdef SMITTY_ZEN
    print_red("✘ %s\n", test_name);
    #else
    print_red("✘ %s | (%s - line %d)\n\n", test_name, file, line);
    #endif

    print_red("Actual:\t  %p\n", actual);
    printf("Expected to be not NULL\n\n");

    return EXPECT_FAIL;
}

SmittyExpectResultType expect_pointer_equal_internal(const void *actual, const void *expected, const char *test_name, const char *file, const int line) {
    if (actual == expected) return EXPECT_PASS;
    
    #ifdef SMITTY_ZEN
    print_red("✘ %s\n", test_name);
    #else
    print_red("✘ %s | (%s - line %d)\n\n", test_name, file, line);
    #endif

    print_red("Actual:\t  %p\n", actual);
    printf("Expected: %p\n\n", expected);

    return EXPECT_FAIL;
}

SmittyExpectResultType expect_enum_equal_internal(const int actual, const int expected, const char *enum_to_string(int), const char *test_name, const char *file, const int line) {
    if (actual == expected) return EXPECT_PASS;
    
    #ifdef SMITTY_ZEN
    print_red("✘ %s\n", test_name);
    #else
    print_red("✘ %s | (%s - line %d)\n\n", test_name, file, line);
    #endif

    print_red("Actual:\t  %s\n", enum_to_string(actual));
    printf("Expected: %s\n\n", enum_to_string(expected));

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

    // The maximum amount of failures is the total amount of tests, so allocate that much space.
    const char *failed_test_names[total_test_count];

    // Run each test, run each of its hooks, and record results.
    for (int i = 0; tests[i] != NULL; i++) {
        assert(i < INT_MAX);
        total_test_count++;

        const char *test_name = smitty_test_case_name(tests[i]);
        const SmittyTestResultType result = smitty_run_test(
            tests[i],
            before_each,
            after_each,
            test_name
        );

        assert(result == TEST_PASS || result == TEST_FAIL);
        switch (result) {
            case TEST_PASS:
                passed_test_count++;
                break;

            case TEST_FAIL:
                failed_test_names[failed_test_count] = test_name;
                failed_test_count++;
                break;

            default:
                // Woe be upon ye. 
                system("xdg-open https://gabaghoul.info");
                break;
        }
    }

    #ifdef SMITTY_ZEN
    if (passed_test_count == total_test_count) {
        print_green("All tests passed.\n");
    }
    #else
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
    print_bold(get_most_readable_time(time_spent));
    printf("================================\n");
    #endif
}

SmittyTestResultType smitty_run_test(smitty_test_case_func test, void (*before_each)(), void (*after_each)(), const char* test_name) {
    // TODO: maybe I could report an error from the before each as a test failure.
    if (before_each != NULL) {
        #ifdef SMITTY_VERBOSE
        printf("Running %s's before_each...\n", test_name);
        #endif

        before_each();
    }

    assert(test != NULL);
    const SmittyTestResultType result = test();

    // TODO: maybe I could report an error from the after each as a test failure.
    if (after_each != NULL) {
        #ifdef SMITTY_VERBOSE
        printf("Running %s's after_each...\n", test_name);
        #endif
        
        after_each();
    }

    return result;
}

//--------------------------------------------------------------------------------------------------
// Test utilities
//--------------------------------------------------------------------------------------------------

const char *smitty_test_result_type_to_string(SmittyTestResultType result) {
    assert(result == TEST_PASS || result == TEST_FAIL);
    
    switch (result) {
        case TEST_PASS: return "PASS";
        case TEST_FAIL: return "FAIL";
        default:        return "UNKNOWN TEST RESULT";
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
    smitty_print_va_list(string);
    reset_output_style();
}

void print_green(const char *string, ...) {
    set_output_color_to_green();
    smitty_print_va_list(string);
    reset_output_style();
}

void print_red(const char *string, ...) {
    set_output_color_to_red();
    smitty_print_va_list(string);
    reset_output_style();
}

void print_green_bold(const char *string, ...) {
    set_output_color_to_green();
    set_output_style_to_bold();
    smitty_print_va_list(string);
    reset_output_style();
}

void print_red_bold(const char *string, ...) {
    set_output_color_to_red();
    set_output_style_to_bold();
    smitty_print_va_list(string);
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
