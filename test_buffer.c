#include "buffer.h"
#include "smitty.h"

//--------------------------------------------------------------------------------------------------
// Time functions
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
// Output style functions
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

//--------------------------------------------------------------------------------------------------
// Tests
//--------------------------------------------------------------------------------------------------

test it_creates_a_default_buffer_with_1024_bytes_of_capacity() {
    Buffer *buffer = buffer_new_default();

    expect(buffer->capacity == 1024);

    free(buffer);
}

test it_creates_a_default_buffer_with_1_byte_of_data_size() {
    Buffer *buffer = buffer_new_default();

    expect(buffer->data_size == 1);

    free(buffer);
}

test it_creates_a_default_buffer_with_a_non_null_data_pointer() {
    Buffer *buffer = buffer_new_default();

    expect(buffer->data != NULL);

    free(buffer);
}

test it_creates_a_default_buffer_with_the_same_address_for_data_read_cursor_and_write_cursor() {
    Buffer *buffer = buffer_new_default();

    char *data_address = buffer->data;
    char *read_cursor_address = buffer->read_cursor;
    char *write_cursor_address = buffer->write_cursor;

    expect(data_address == read_cursor_address);
    expect(data_address == write_cursor_address);

    free(buffer);
}

//--------------------------------------------------------------------------------------------------
// Driver
//--------------------------------------------------------------------------------------------------

int main() {
    clock_t start = clock();

    int total_tests = 4;
    int passed_tests = 0;

    // If a test passes, it returns true (1). Otherwise, it returns false (0).
    // So we can just add the result of each test to determine how many passed.
    passed_tests += it_creates_a_default_buffer_with_1024_bytes_of_capacity();
    passed_tests += it_creates_a_default_buffer_with_1_byte_of_data_size();
    passed_tests += it_creates_a_default_buffer_with_a_non_null_data_pointer();
    passed_tests += it_creates_a_default_buffer_with_the_same_address_for_data_read_cursor_and_write_cursor();

    if (total_tests == passed_tests) {
        set_output_color_to_green();
        set_output_style_to_bold();
        printf("All tests passed!\n\n");
        reset_output_color();
    } else {
        set_output_color_to_red();
        set_output_style_to_bold();
        printf("Some tests failed!\n\n");
        reset_output_color();
    }

    printf("Total tests:\t");
    set_output_style_to_bold();
    printf("%d\n", total_tests);
    reset_output_color();

    set_output_color_to_green();
    printf("Passed tests:\t");
    set_output_style_to_bold();
    printf("%d ", passed_tests);
    reset_output_color();
    set_output_color_to_green();
    printf("(%d%%)\n", (passed_tests * 100) / total_tests);

    set_output_color_to_red();
    printf("Failed tests:\t");
    set_output_style_to_bold();
    printf("%d ", total_tests - passed_tests);
    reset_output_color();
    printf("(%d%%)\n", ((total_tests - passed_tests) * 100) / total_tests);
    
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nTime spent:\t");
    set_output_style_to_bold();
    print_most_readable_time(time_spent);
    reset_output_color();

    return 0;
}
