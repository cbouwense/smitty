#include "buffer.h"
#include "smitty.h"

//--------------------------------------------------------------------------------------------------
// Tests
//--------------------------------------------------------------------------------------------------

test_result it_creates_a_default_buffer_with_1024_bytes_of_capacity() {
    test_result did_test_pass = true;

    Buffer *buffer = buffer_new_default();

    expect(buffer->capacity == 1024);

    free(buffer);
    return did_test_pass;
}

test_result it_creates_a_default_buffer_with_1_byte_of_data_size() {
    test_result did_test_pass = true;

    Buffer *buffer = buffer_new_default();

    expect(buffer->data_size == 1);

    free(buffer);
    return did_test_pass;
}

test_result it_creates_a_default_buffer_with_a_non_null_data_pointer() {
    test_result did_test_pass = true;

    Buffer *buffer = buffer_new_default();

    expect(buffer->data != NULL);

    free(buffer);
    return did_test_pass;
}

test_result it_creates_a_default_buffer_with_the_same_address_for_data_read_cursor_and_write_cursor() {
    test_result did_test_pass = true;

    Buffer *buffer = buffer_new_default();

    char *data_address = buffer->data;
    char *read_cursor_address = buffer->read_cursor;
    char *write_cursor_address = buffer->write_cursor;

    expect(data_address == read_cursor_address);
    expect(data_address == write_cursor_address);

    free(buffer);
    return did_test_pass;
}

//--------------------------------------------------------------------------------------------------
// Driver
//--------------------------------------------------------------------------------------------------

typedef struct {
    const char *name;

    /** 
     * The return type of tests is a bool pointer for now to indicate whether or not the test
     * passed. In the future it might be better, or necessary, for this to be some sort of struct
     * that contains more information about the test, such as the time it took to run, or the
     * expected and actual values.  
     */
    test_result (*test)();
} TestTable;

TestTable tests[] = {
    {
        "it_creates_a_default_buffer_with_1024_bytes_of_capacity",
        it_creates_a_default_buffer_with_1024_bytes_of_capacity
    },
    {
        "it_creates_a_default_buffer_with_1_byte_of_data_size",
        it_creates_a_default_buffer_with_1_byte_of_data_size
    },
    {
        "it_creates_a_default_buffer_with_a_non_null_data_pointer",
        it_creates_a_default_buffer_with_a_non_null_data_pointer
    },
    {
        "it_creates_a_default_buffer_with_the_same_address_for_data_read_cursor_and_write_cursor",
        it_creates_a_default_buffer_with_the_same_address_for_data_read_cursor_and_write_cursor
    },
    {NULL, NULL}
};

test_result call_test(const char *name) {
    for (int i = 0; tests[i].name != NULL; i++) {
        if (strcmp(name, tests[i].name) == 0) {
            return tests[i].test();
        }
    }

    printf("Function not found: %s\n", name);
    return false;
}

int main() {
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
        const test_result did_test_pass = call_test(tests[i].name);

        if (did_test_pass) {
            passed_test_count++;
        } else {
            // Add test to list of failures.
            failed_test_names[failed_test_count] = test_name;
            failed_test_count++;
        }
    }

    // Display results.
    if (failed_test_count == 0) {
        set_output_color_to_green();
        set_output_style_to_bold();
        printf("All tests passed!\n\n");
        reset_output_color();
    } else {
        set_output_color_to_red();
        set_output_style_to_bold();
        printf("Some tests failed!\n\n");
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
    printf("(%d%%)\n", ((total_test_count - passed_test_count) * 100) / total_test_count);
    
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nTime spent:\t");
    set_output_style_to_bold();
    print_most_readable_time(time_spent);
    reset_output_color();

    return 0;
}
