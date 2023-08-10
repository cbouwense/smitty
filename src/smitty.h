#pragma once

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
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

// A function pointer to a function that returns a smitty_test_result, named smitty_test_case_func.
typedef smitty_test_result (*smitty_test_case_func)();

//--------------------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------------------

#define smitty_test(name, body) \
    smitty_test_result name() { \
        int failed_expect_count = 0; \
        body \
        return failed_expect_count == 0 ? TEST_PASS : TEST_FAIL; \
    }

#define smitty_test_case_name(name) #name

#define smitty_register_and_run_tests(...) \
    int main() { \
        smitty_test_case_func tests[] = { \
            __VA_ARGS__ \
            NULL \
        }; \
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

#define smitty_test_debug_params const char *test_name, const char *file, const int line

//--------------------------------------------------------------------------------------------------
// Expects
//--------------------------------------------------------------------------------------------------

#define expect_int_equal(actual, expected)     failed_expect_count += expect_int_equal_internal    (actual, expected, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;
#define expect_string_equal(actual, expected)  failed_expect_count += expect_string_equal_internal (actual, expected, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;
#define expect_true(actual)                    failed_expect_count += expect_true_internal         (actual,           __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;
#define expect_false(actual)                   failed_expect_count += expect_false_internal        (actual,           __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;
#define expect_null(actual)                    failed_expect_count += expect_null_internal         (actual,           __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;
#define expect_non_null(actual)                failed_expect_count += expect_non_null_internal     (actual,           __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;
#define expect_pointer_equal(actual, expected) failed_expect_count += expect_pointer_equal_internal(actual, expected, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;

smitty_expect_result expect_int_equal_internal(    const int   actual, const int   expected, smitty_test_debug_params);
smitty_expect_result expect_string_equal_internal( const char *actual, const char *expected, smitty_test_debug_params);
smitty_expect_result expect_true_internal(         const bool  actual,                       smitty_test_debug_params);
smitty_expect_result expect_false_internal(        const bool  actual,                       smitty_test_debug_params);
smitty_expect_result expect_null_internal(         const void *actual,                       smitty_test_debug_params);
smitty_expect_result expect_non_null_internal(     const void *actual,                       smitty_test_debug_params);
smitty_expect_result expect_pointer_equal_internal(const void *actual, const void *expected, smitty_test_debug_params);

//--------------------------------------------------------------------------------------------------
// Test runner core
//--------------------------------------------------------------------------------------------------

void smitty_run_tests(smitty_test_case_func tests[],void (*before_each)(),void (*after_each)());
smitty_test_result smitty_run_test(smitty_test_case_func test, void (*before_each)(), void (*after_each)());

//--------------------------------------------------------------------------------------------------
// Test utilities
//--------------------------------------------------------------------------------------------------

void smitty_print_test_failure(const char *test_name, const char *file, const int line);
const char *smitty_test_result_to_string(smitty_test_result result);

//--------------------------------------------------------------------------------------------------
// Time utilities
//--------------------------------------------------------------------------------------------------

const short  time_in_seconds(double time);
const short  time_in_milliseconds(double time);
const short  time_in_microseconds(double time);
const short  time_in_nanoseconds(double time);
const char  *get_most_readable_time(double time);

//--------------------------------------------------------------------------------------------------
// Output style utilities
//--------------------------------------------------------------------------------------------------

void print_bold(const char *string, ...);
void print_green(const char *string, ...);
void print_red(const char *string, ...);
void print_green_bold(const char *string, ...);
void print_red_bold(const char *string, ...);

void set_output_color_to_green();
void set_output_color_to_red();
void set_output_style_to_bold();
void reset_output_style();
