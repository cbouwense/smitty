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
    // Don't want to have to do this in the actual test code.
    test_result did_test_pass = true;

    Buffer *buffer = buffer_new_default();

    char *data_address = buffer->data;
    char *read_cursor_address = buffer->read_cursor;
    char *write_cursor_address = buffer->write_cursor;

    expect(data_address == read_cursor_address);
    expect(data_address == write_cursor_address);

    // TODO: Maybe have some nice utility to free all used memory from unit tests. Unsure if that's
    // overengineering or not.
    free(buffer);

    // Don't want to have to do this in the actual test code.
    return did_test_pass;
}

//--------------------------------------------------------------------------------------------------
// Driver
//--------------------------------------------------------------------------------------------------

Test tests[] = {
    register_test(it_creates_a_default_buffer_with_1024_bytes_of_capacity),
    register_test(it_creates_a_default_buffer_with_1_byte_of_data_size),
    register_test(it_creates_a_default_buffer_with_a_non_null_data_pointer),
    register_test(it_creates_a_default_buffer_with_the_same_address_for_data_read_cursor_and_write_cursor),
    {NULL, NULL}
};

int main() {
    run_test_suite(tests);

    return 0;
}
