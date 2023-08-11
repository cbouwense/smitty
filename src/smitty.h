#pragma once

#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//--------------------------------------------------------------------------------------------------
// Types
//--------------------------------------------------------------------------------------------------

typedef enum {
    EXPECT_PASS,
    EXPECT_FAIL,
} SmittyExpectResultType;

typedef enum {
    TEST_PASS,
    TEST_FAIL,
} SmittyTestResultType;

// A function pointer to a function that returns a SmittyTestResultType, named smitty_test_case_func.
typedef SmittyTestResultType (*smitty_test_case_func)();

//--------------------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------------------

#define smitty_test(name, body) \
    SmittyTestResultType name() { \
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

#define smitty_run_test_suite_with_before_each(before_each, ...) \
    int main() { \
        smitty_test_case_func tests[] = { \
            __VA_ARGS__ \
            NULL \
        }; \
        smitty_run_tests(tests, before_each, NULL); \
        return 0; \
    }

#define smitty_run_test_suite_with_after_each(after_each, ...) \
    int main() { \
        smitty_test_case_func tests[] = { \
            __VA_ARGS__ \
            NULL \
        }; \
        smitty_run_tests(tests, NULL, after_each); \
        return 0; \
    }

#define smitty_run_test_suite_with_before_and_after_each(before_each, after_each, ...) \
    int main() { \
        smitty_test_case_func tests[] = { \
            __VA_ARGS__ \
            NULL \
        }; \
        smitty_run_tests(tests, before_each, after_each); \
        return 0; \
    }

#define smitty_print_va_list(string) \
    va_list args; \
    va_start(args, string); \
    vprintf(string, args); \
    va_end(args);

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

SmittyExpectResultType expect_int_equal_internal(    const int   actual, const int   expected, const char *test_name, const char *file, const int line);
SmittyExpectResultType expect_string_equal_internal( const char *actual, const char *expected, const char *test_name, const char *file, const int line);
SmittyExpectResultType expect_true_internal(         const bool  actual,                       const char *test_name, const char *file, const int line);
SmittyExpectResultType expect_false_internal(        const bool  actual,                       const char *test_name, const char *file, const int line);
SmittyExpectResultType expect_null_internal(         const void *actual,                       const char *test_name, const char *file, const int line);
SmittyExpectResultType expect_non_null_internal(     const void *actual,                       const char *test_name, const char *file, const int line);
SmittyExpectResultType expect_pointer_equal_internal(const void *actual, const void *expected, const char *test_name, const char *file, const int line);

//--------------------------------------------------------------------------------------------------
// Test runner core
//--------------------------------------------------------------------------------------------------

void smitty_run_tests(smitty_test_case_func tests[],void (*before_each)(),void (*after_each)());
SmittyTestResultType smitty_run_test(smitty_test_case_func test, void (*before_each)(), void (*after_each)(), const char* test_name);

//--------------------------------------------------------------------------------------------------
// Test utilities
//--------------------------------------------------------------------------------------------------

const char *SmittyTestResultType_to_string(SmittyTestResultType result);

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
