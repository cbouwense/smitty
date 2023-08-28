/*
 * "Smitty" is the name of a stuffed animal I own. My wife loves him; therefore, I named this
 * testing library after him.
 */

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
typedef SmittyTestResultType (*smitty_test_case_func)(void);

//--------------------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------------------

#define smitty_test(name, body) \
    SmittyTestResultType name(void) { \
        int failed_expect_count = 0; \
        body \
        return failed_expect_count == 0 ? TEST_PASS : TEST_FAIL; \
    }

#define smitty_test_case_name(name) #name

// smitty_array simply expands to the arguments passed to it in the form of array initializtion.
// This is necessary because if you try to initialize an array inside of a smitty_test normally,
// the preprocessor will think that the commas in the array are separating arguments to the
// smitty_test macro.
//
// TODO: I really dislike this, and should look into how to not do this. But, honestly it's not 
// that big of a deal.
#define smitty_initialize_array(...) { __VA_ARGS__ }

#define smitty_register_and_run_tests(...) \
    int main(void) { \
        smitty_test_case_func tests[] = { \
            __VA_ARGS__ \
            NULL \
        }; \
        smitty_run_tests(tests, NULL, NULL); \
        return 0; \
    }

// TODO: I wonder if there's a better way of doing before_each and after_each other than having a
// bunch of different macros like this. Maybe some kinda options struct?
#define smitty_run_test_suite_with_before_each(before_each, ...) \
    int main(void) { \
        smitty_test_case_func tests[] = { \
            __VA_ARGS__ \
            NULL \
        }; \
        smitty_run_tests(tests, before_each, NULL); \
        return 0; \
    }

#define smitty_run_test_suite_with_after_each(after_each, ...) \
    int main(void) { \
        smitty_test_case_func tests[] = { \
            __VA_ARGS__ \
            NULL \
        }; \
        smitty_run_tests(tests, NULL, after_each); \
        return 0; \
    }

#define smitty_run_test_suite_with_before_and_after_each(before_each, after_each, ...) \
    int main(void) { \
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

#define expect_int_equal(actual, expected) failed_expect_count += expect_int_equal_internal (actual, expected, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;
#define expect_string_equal(actual, expected) failed_expect_count += expect_string_equal_internal (actual, expected, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;
#define expect_true(actual) failed_expect_count += expect_true_internal (actual, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;
#define expect_false(actual) failed_expect_count += expect_false_internal (actual, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;
#define expect_null(actual) failed_expect_count += expect_null_internal (actual, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;
#define expect_non_null(actual) failed_expect_count += expect_non_null_internal (actual, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;
#define expect_pointer_equal(actual, expected) failed_expect_count += expect_pointer_equal_internal (actual, expected, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;
#define expect_enum_equal(actual, expected, enum_to_string_func) failed_expect_count += expect_enum_equal_internal (actual, expected, enum_to_string_func, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;
// TODO: expect_float_equal

#define expect_int_array_equal(actual, expected, size) failed_expect_count += expect_int_array_equal_internal (actual, expected, size, __func__, __FILE__, __LINE__) == EXPECT_FAIL ? 1 : 0;
// TODO: more array types

// TODO: It might be nice to have some sort of internal.h file that contains all the internal stuff.
SmittyExpectResultType expect_int_equal_internal(const int actual, const int expected, const char *test_name, const char *file, const int line);
SmittyExpectResultType expect_string_equal_internal(const char *actual, const char *expected, const char *test_name, const char *file, const int line);
SmittyExpectResultType expect_true_internal(const bool actual, const char *test_name, const char *file, const int line);
SmittyExpectResultType expect_false_internal(const bool actual, const char *test_name, const char *file, const int line);
SmittyExpectResultType expect_null_internal(const void *actual, const char *test_name, const char *file, const int line);
SmittyExpectResultType expect_non_null_internal(const void *actual, const char *test_name, const char *file, const int line);
SmittyExpectResultType expect_pointer_equal_internal(const void *actual, const void *expected, const char *test_name, const char *file, const int line);
SmittyExpectResultType expect_enum_equal_internal(const int actual, const int expected, const char *enum_to_string_func(const int), const char *test_name, const char *file, const int line);
// TODO: expect_float_equal

SmittyExpectResultType expect_int_array_equal_internal(const int actual[], const int expected[], const size_t expected_length, const char *test_name, const char *file, const int line);
// TODO: more array types

//--------------------------------------------------------------------------------------------------
// Test runner core
//--------------------------------------------------------------------------------------------------

void smitty_run_tests(smitty_test_case_func tests[],void (*before_each)(void),void (*after_each)(void));
SmittyTestResultType smitty_run_test(smitty_test_case_func test, void (*before_each)(void), void (*after_each)(void), const char* test_name);

//--------------------------------------------------------------------------------------------------
// Test utilities
//--------------------------------------------------------------------------------------------------

const char *smitty_test_result_type_to_string(SmittyTestResultType result);

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

void set_output_color_to_green(void);
void set_output_color_to_red(void);
void set_output_style_to_bold(void);
void reset_output_style(void);
