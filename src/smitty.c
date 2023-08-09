#include "smitty.h"

//--------------------------------------------------------------------------------------------------
// Test runner core
//--------------------------------------------------------------------------------------------------

smitty_expect_result expect_int_equal(const int expected, const int actual) {
    if (expected != actual) {
        set_output_color_to_green();
        printf("Expected: %d\n", expected);
        set_output_color_to_red();
        printf("Actual:\t  %d\n\n", actual);
        reset_output_style();

        return EXPECT_FAIL;
    }
    
    return EXPECT_PASS;
}

void smitty_run_tests(smitty_test_case_info tests[], void (*before_each)(), void (*after_each)()) {
    clock_t start = clock();

    int passed_test_count = 0;
    int failed_test_count = 0;
    int total_test_count = 0;

    // Count the number of tests.
    for (int i = 0; tests[i].name != NULL; i++) {
        total_test_count++;
    }

    // The maximum amount of failures is the total amount of tests, so allocate that much space.
    const char *failed_test_names[total_test_count];

    // Run each test, its hooks, and record results.
    for (int i = 0; tests[i].name != NULL; i++) {

        // TODO: maybe we should have a way to see if the after_each function failed?
        // Run before_each if we got one.
        if (before_each != NULL) before_each();

        const char *test_name = tests[i].name;
        const smitty_test_result result = smitty_run_test(
            tests[i].name, 
            tests,
            before_each,
            after_each
        );
        
        printf("result: %s\n", smitty_test_result_to_string(result));

        // TODO: maybe we should have a way to see if the after_each function failed?
        // Run after_each if we got one.
        if (after_each != NULL) after_each();

        switch (result) {
            case TEST_PASS:
            default:
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
        }
    }

    // Display results.
    if (failed_test_count == 0) {
        print_green_bold("All tests passed!\n\n");
    } else if (failed_test_count != total_test_count) {
        print_red_bold("Some tests failed!\n\n");

        for(int i = 0; i < failed_test_count; i++) {
            set_output_color_to_red();
            printf("%s\n\n", failed_test_names[i]);
            reset_output_style();
        }
    } else { // Every test failed
        print_red("All tests failed!\n\n");

        for(int i = 0; i < failed_test_count; i++) {
            set_output_color_to_red();
            printf("%s\n\n", failed_test_names[i]);
            reset_output_style();
        }
    }

    printf("Total tests:\t");
    set_output_style_to_bold();
    printf("%d\n", total_test_count);
    reset_output_style();

    print_green("Passed tests:\t");

    set_output_color_to_green();
    set_output_style_to_bold();
    printf("%d ", passed_test_count);
    reset_output_style();

    set_output_color_to_green();
    printf("(%d%%)\n", (passed_test_count * 100) / total_test_count);

    print_red("Failed tests:\t");
    set_output_color_to_red();
    printf("%d ", total_test_count - passed_test_count);

    printf("(%d%%)\n\n", ((total_test_count - passed_test_count) * 100) / total_test_count);
    reset_output_style();

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Time spent:\t");
    set_output_style_to_bold();
    print_most_readable_time(time_spent);
    reset_output_style();
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

smitty_test_result smitty_run_test(           
    const char             *name,
    smitty_test_case_info   tests[],
    void                  (*before_each)(),
    void                  (*after_each)()
) {
    smitty_test_result (*test_case)() = find_test_by_name(name, tests);

    if (before_each != NULL) before_each();
    
    const smitty_test_result result = test_case == NULL ? TEST_NOT_FOUND : test_case();

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

void print_green(const char *string) {
    set_output_color_to_green();
    printf("%s", string);
    reset_output_style();
}

void print_red(const char *string) {
    set_output_color_to_red();
    printf("%s", string);
    reset_output_style();
}

void print_green_bold(const char *string) {
    set_output_color_to_green();
    set_output_style_to_bold();
    printf("%s", string);
    reset_output_style();
}

void print_red_bold(const char *string) {
    set_output_color_to_red();
    set_output_style_to_bold();
    printf("%s", string);
    reset_output_style();
}
