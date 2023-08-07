#include "add.h"
#include "smitty.h"

//--------------------------------------------------------------------------------------------------
// Test runner core
//--------------------------------------------------------------------------------------------------

void run_test_suite(test_case_info tests[]) {
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

    // Run each test and record results.
    for (int i = 0; tests[i].name != NULL; i++) {
        const char *test_name = tests[i].name;
        const test_result result = run_test(tests[i].name, tests);

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
        }
    }

    // Display results.
    if (failed_test_count == 0) {
        set_output_color_to_green();
        set_output_style_to_bold();
        printf("All tests passed!\n\n");
        reset_output_color();
    } else if (failed_test_count != total_test_count) {
        set_output_color_to_red();
        set_output_style_to_bold();
        printf("Some tests failed!\n\n");
        reset_output_color();

        for(int i = 0; i < failed_test_count; i++) {
            set_output_color_to_red();
            printf("%s\n\n", failed_test_names[i]);
            reset_output_color();
        }
    } else { // Every test failed
        set_output_color_to_red();
        set_output_style_to_bold();
        printf("All tests failed!\n\n");
        reset_output_color();

        for(int i = 0; i < failed_test_count; i++) {
            set_output_color_to_red();
            printf("%s\n\n", failed_test_names[i]);
            reset_output_color();
        }
    }

    printf("Total tests:\t");
    set_output_style_to_bold();
    printf("%d\n", total_test_count);
    reset_output_color();

    set_output_color_to_green();
    printf("Passed tests:\t");
    set_output_style_to_bold();
    printf("%d ", passed_test_count);
    reset_output_color();
    set_output_color_to_green();
    printf("(%d%%)\n", (passed_test_count * 100) / total_test_count);

    set_output_color_to_red();
    printf("Failed tests:\t");
    set_output_style_to_bold();
    printf("%d ", total_test_count - passed_test_count);
    reset_output_color();
    printf("(%d%%)\n\n", ((total_test_count - passed_test_count) * 100) / total_test_count);
    
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Time spent:\t");
    set_output_style_to_bold();
    print_most_readable_time(time_spent);
    reset_output_color();
}

test_result (*find_test_by_name(const char *name, test_case_info tests[]))() {
    for (int i = 0; tests[i].name != NULL; i++) {
        if (strcmp(name, tests[i].name) == 0) {
            return tests[i].test_case;
        }
    }

    printf("Test not found: %s\n", name);
    return NULL;
}

test_result run_test(const char *name, test_case_info tests[]) {
    test_result (*test_case)() = find_test_by_name(name, tests);

    const test_result result = test_case == NULL ? TEST_NOT_FOUND : test_case();

    switch (result) {
        case TEST_PASS:
        default:
            return TEST_PASS;
            
        case TEST_FAIL:
        case TEST_NOT_FOUND:
            return TEST_FAIL;
    }
}

char *test_result_to_string(test_result result) {
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

void reset_output_color() {
    printf("\033[0m");
}
