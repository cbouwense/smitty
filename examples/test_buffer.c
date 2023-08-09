#include "../src/smitty.h"
#include "buffer.h"

smitty_test(it_creates_a_default_buffer_with_1024_bytes_of_capacity, {
    Buffer *buffer = buffer_new_default();

    expect_equal(buffer->capacity,  1024);

    free(buffer);
});

// TODO: we need more expects for these to work
// smitty_test_result it_creates_a_default_buffer_with_a_non_null_data_pointer() {
//     Buffer *buffer = buffer_new_default();

//     expect_equal(buffer->data,  NULL);

//     free(buffer);
// }

// smitty_test_result it_creates_a_default_buffer_with_the_same_address_for_data_read_cursor_and_write_cursor() {
//     Buffer *buffer = buffer_new_default();

//     char *data_address = buffer->data;
//     char *read_cursor_address = buffer->read_cursor;
//     char *write_cursor_address = buffer->write_cursor;

//     expect_equal(data_address, read_cursor_address);
//     expect_equal(data_address, write_cursor_address);

//     free(buffer);
// }

// smitty_test_result it_creates_a_buffer_with_the_specified_capacity() {
//     Buffer *buffer = buffer_new(2048);

//     expect_equal(buffer->capacity, 2048);

//     free(buffer);
// }

// smitty_test_result it_returns_null_when_buffer_created_with_zero_capacity() {
//     Buffer *buffer = buffer_new(0);

//     expect_equal(buffer, NULL);

//     free(buffer);
// }

// smitty_test_result it_returns_failure_when_user_attempts_an_overflow() {
//     Buffer *buffer = buffer_new(3);
//     char *data = "Hello, world!";

//     ReturnCode result = buffer_write(buffer, data);

//     expect_equal(result, TEST_FAIL);

//     free(buffer);
// }

//--------------------------------------------------------------------------------------------------
// Smitty boilerplate
//--------------------------------------------------------------------------------------------------

smitty_register_tests(
    smitty_test_as_name_and_callback(it_creates_a_default_buffer_with_1024_bytes_of_capacity),
    // smitty_test_as_name_and_callback(it_creates_a_default_buffer_with_a_non_null_data_pointer),
    // smitty_test_as_name_and_callback(it_creates_a_default_buffer_with_the_same_address_for_data_read_cursor_and_write_cursor),
    // smitty_test_as_name_and_callback(it_creates_a_buffer_with_the_specified_capacity),
    // smitty_test_as_name_and_callback(it_returns_null_when_buffer_created_with_zero_capacity),
    // smitty_test_as_name_and_callback(it_returns_failure_when_user_attempts_an_overflow),
);

smitty_run_test_suite();
