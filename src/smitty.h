#pragma once

#include <assert.h>
#include <stdarg.h>
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
    const char    *name;
    test_case_ptr  test_case;
} smitty_test_case_info;

//--------------------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------------------

#define smitty_test(name, body) \
    smitty_test_result name() { \
        int failed_expect_count = 0; \
        body \
        return failed_expect_count == 0 ? TEST_PASS : TEST_FAIL; \
    }

#define smitty_test_as_name_and_callback(name) {#name, name}

#define smitty_register_test(test_case) scrump_buffer_write_func_ptr(test_case_func_ptrs_buffer, test_case);

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

#define expect_int_equal(actual, expected) \
    failed_expect_count += expect_int_equal_internal(actual, expected, __func__, __FILE__, __LINE__) == EXPECT_FAIL == EXPECT_FAIL ? 1 : 0;

#define expect_string_equal(actual, expected) \
    failed_expect_count += expect_string_equal_internal(actual, expected, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;

#define expect_true(actual) \
    failed_expect_count += expect_true_internal(actual, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;

#define expect_false(actual) \
    failed_expect_count += expect_false_internal(actual, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;

#define expect_null(actual) \
    failed_expect_count += expect_null_internal(actual, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;

#define expect_non_null(actual) \
    failed_expect_count += expect_non_null_internal(actual, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;

#define expect_pointer_equal(actual, expected) \
    failed_expect_count += expect_pointer_equal_internal(actual, expected, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;

//--------------------------------------------------------------------------------------------------
// Expects
//--------------------------------------------------------------------------------------------------

smitty_expect_result expect_int_equal_internal(const int actual, const int expected, const char *test_name, const char *file, const int line);

smitty_expect_result expect_string_equal_internal(const char *actual, const char* expected, const char *test_name, const char *file, const int line);

smitty_expect_result expect_true_internal(const bool actual, const char *test_name, const char *file, const int line);

smitty_expect_result expect_false_internal(const bool actual, const char *test_name, const char *file, const int line);

smitty_expect_result expect_null_internal(const void *actual, const char *test_name, const char *file, const int line);

smitty_expect_result expect_non_null_internal(const void *actual, const char *test_name, const char *file, const int line);

smitty_expect_result expect_pointer_equal_internal(const void *actual, const void *expected, const char *test_name, const char *file, const int line);

//--------------------------------------------------------------------------------------------------
// Test runner core
//--------------------------------------------------------------------------------------------------

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

void print_bold(const char *string, ...);
void print_green(const char *string, ...);
void print_red(const char *string, ...);
void print_green_bold(const char *string, ...);
void print_red_bold(const char *string, ...);

void set_output_color_to_green();
void set_output_color_to_red();
void set_output_style_to_bold();
void reset_output_style();
