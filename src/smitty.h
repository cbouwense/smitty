#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//--------------------------------------------------------------------------------------------------
// Types
//--------------------------------------------------------------------------------------------------

typedef enum {
    EXPECT_PASS,
    EXPECT_FAIL,
} smitty_expect_result;

typedef enum {
    TEST_PASS,
    TEST_FAIL,
    TEST_NOT_FOUND,
} smitty_test_result;

// A function pointer to a function that returns a smitty_test_result, named test_case_ptr.
typedef smitty_test_result (*test_case_ptr)();

typedef struct {
    const char *name;
    test_case_ptr test_case;
} smitty_test_case_info;

//--------------------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------------------


/*
 * If an assertion is true, the test continues. If it fails, the test returns TEST_FAIL. This means
 * that the first assertion to fail will cause the test to stop. It would be nicer if the entire
 * test ran, all the expects were evaluated, and then the test result showed each expect.
 */
#define expect(assertion) if (!(assertion)) return TEST_FAIL

#define smitty_test_as_name_and_callback(name) {#name, name}

#define smitty_register_tests(...) \
    smitty_test_case_info tests[] = { \
        __VA_ARGS__ \
        {NULL, NULL} \
    };

#define smitty_run_test_suite() \
    int main() { \
        smitty_run_tests(tests, NULL, NULL); \
        return 0; \
    }

#define smitty_run_test_suite_with_before_each(before_each) \
    int main() { \
        smitty_run_tests(tests, before_each, NULL); \
        return 0; \
    }

#define smitty_run_test_suite_with_after_each(after_each) \
    int main() { \
        smitty_run_tests(tests, NULL, after_each); \
        return 0; \
    }

#define smitty_run_test_suite_with_before_and_after_each(before_each, after_each) \
    int main() { \
        smitty_run_tests(tests, before_each, after_each); \
        return 0; \
    }

#define smitty_test(function_pointer) internal_smitty_test(#function_pointer, function_pointer, __FILE__, __LINE__)

#define expect_equal(actual, expected) expect_equal_internal(actual, expected, __func__, __FILE__, __LINE__)

//--------------------------------------------------------------------------------------------------
// Expects
//--------------------------------------------------------------------------------------------------

smitty_expect_result expect_equal_internal(const int expected, const int actual, const char *test_name, const char *file, const int line);

//--------------------------------------------------------------------------------------------------
// Test runner core
//--------------------------------------------------------------------------------------------------

void internal_smitty_test(const char *name, test_case_ptr test_case, const char *file, const int line);

void smitty_run_tests(smitty_test_case_info tests[],void (*before_each)(),void (*after_each)());

smitty_test_result (*find_test_by_name(const char *name, smitty_test_case_info tests[]))();

smitty_test_result smitty_run_test( const char *name, smitty_test_case_info tests[], void (*before_each)(), void (*after_each)());

char *smitty_test_result_to_string(smitty_test_result result);

//--------------------------------------------------------------------------------------------------
// Time utilities
//--------------------------------------------------------------------------------------------------

const double time_in_seconds(double time);

const double time_in_milliseconds(double time);

const double time_in_microseconds(double time);

const double time_in_nanoseconds(double time);

void print_most_readable_time(double time);

//--------------------------------------------------------------------------------------------------
// Output style utilities
//--------------------------------------------------------------------------------------------------

void set_output_color_to_green();

void set_output_color_to_red();

void set_output_style_to_bold();

void reset_output_style();

void print_green(const char *string);

void print_red(const char *string);

void print_green_bold(const char *string);

void print_red_bold(const char *string);
