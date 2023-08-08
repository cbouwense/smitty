#include "../src/smitty.h"
#include "buffer.h"

smitty_test_result it_creates_a_default_buffer_with_1024_bytes_of_capacity() {
    Buffer *buffer = buffer_new_default();

    expect(buffer->capacity == 1024);

    free(buffer);
}

smitty_test_result it_creates_a_default_buffer_with_1_byte_of_data_element_size() {
    Buffer *buffer = buffer_new_default();

    expect(buffer->data_element_size == 1);

    free(buffer);
}

smitty_test_result it_creates_a_default_buffer_with_a_non_null_data_pointer() {
    Buffer *buffer = buffer_new_default();

    expect(buffer->data != NULL);

    free(buffer);
}

smitty_test_result it_creates_a_default_buffer_with_the_same_address_for_data_read_cursor_and_write_cursor() {
    Buffer *buffer = buffer_new_default();

    char *data_address = buffer->data;
    char *read_cursor_address = buffer->read_cursor;
    char *write_cursor_address = buffer->write_cursor;

    expect(data_address == read_cursor_address);
    expect(data_address == write_cursor_address);

    free(buffer);
}

smitty_test_result it_creates_a_buffer_with_the_specified_capacity() {
    Buffer *buffer = buffer_new(2048);

    expect(buffer->capacity == 2048);

    free(buffer);
}

smitty_test_result it_returns_null_when_buffer_created_with_zero_capacity() {
    Buffer *buffer = buffer_new(0);

    expect(buffer == NULL);

    free(buffer);
}

//--------------------------------------------------------------------------------------------------
// Smitty boilerplate
//--------------------------------------------------------------------------------------------------

smitty_register_tests(
    smitty_test_as_name_and_callback(it_creates_a_default_buffer_with_1024_bytes_of_capacity),
    smitty_test_as_name_and_callback(it_creates_a_default_buffer_with_1_byte_of_data_element_size),
    smitty_test_as_name_and_callback(it_creates_a_default_buffer_with_a_non_null_data_pointer),
    smitty_test_as_name_and_callback(it_creates_a_default_buffer_with_the_same_address_for_data_read_cursor_and_write_cursor),
    smitty_test_as_name_and_callback(it_creates_a_buffer_with_the_specified_capacity),
    smitty_test_as_name_and_callback(it_returns_null_when_buffer_created_with_zero_capacity),
)

smitty_run_test_suite()
