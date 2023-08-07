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
    TEST_PASS,
    TEST_FAIL,
    TEST_NOT_FOUND,
} test_result;

// A function pointer to a function that returns a test_result, named test_case_ptr.
typedef test_result (*test_case_ptr)();

typedef struct {
    const char *name;
    test_case_ptr test_case;
} test_case_info;

//--------------------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------------------

/*
 * If an assertion is true, the test continues. If it fails, the test returns TEST_FAIL. This means
 * that the first assertion to fail will cause the test to stop. It would be nicer if the entire
 * test ran, all the expects were evaluated, and then the test result showed each expect.
 */
#define expect(assertion) if (!(assertion)) return TEST_FAIL

#define register_test(name) {#name, name}

#define run_smitty_suite() \
    int main() { \
        run_test_suite(tests, NULL); \
        return 0; \
    }

#define run_smitty_suite_with_before_each(before_each) \
    int main() { \
        run_test_suite(tests, before_each); \
        return 0; \
    }

#define smitty_test(description, body) \
    test_result description() { \
        body \
        return TEST_PASS; \
    }

//--------------------------------------------------------------------------------------------------
// Test runner core
//--------------------------------------------------------------------------------------------------

void run_test_suite(
    test_case_info tests[],
    void (*before_each)()
);

test_result (*find_test_by_name(const char *name, test_case_info tests[]))();

test_result run_test(const char *name, test_case_info tests[]);

char *test_result_to_string(test_result result);

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

void reset_output_color();
