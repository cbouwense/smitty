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
    TEST_NOT_FOUND
} test_result;

typedef struct {
    const char *name;

    /** 
     * The return type of test is a bool pointer for now to indicate whether or not the test
     * passed. In the future it might be better, or necessary, for this to be some sort of struct
     * that contains more information about the test, such as the time it took to run, or the
     * expected and actual values.  
     */
    test_result (*test)();
} Test;

//--------------------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------------------

#define expect(condition) if(!(condition)) return TEST_FAIL;

#define register_test(name) {#name, name}

#define run_smitty_suite() \
    int main() { \
        run_test_suite(tests); \
        return 0; \
    }

//--------------------------------------------------------------------------------------------------
// Test utilities
//--------------------------------------------------------------------------------------------------

void run_test_suite(Test tests[]);

test_result (*find_test_by_name(const char *name, Test tests[]))();

test_result run_test(const char *name, Test tests[]);

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
