#include "../src/smitty.h"
#include "buffer.h"

smitty_test(it_creates_a_default_buffer_with_1024_bytes_of_capacity, {
    ScrumpBuffer *buffer = buffer_create_default();

    expect_int_equal(buffer->capacity,  1024);

    free(buffer);
});

smitty_test(it_creates_a_default_buffer_with_a_non_null_pointer_to_its_data, {
    ScrumpBuffer *buffer = buffer_create_default();

    expect_non_null(buffer->data);

    free(buffer);
});

smitty_test(it_creates_a_default_buffer_with_the_same_address_for_data_read_cursor_and_write_cursor, {
    ScrumpBuffer *buffer = buffer_create_default();

    expect_pointer_equal(buffer->data, buffer->read_cursor);
    expect_pointer_equal(buffer->data, buffer->write_cursor);

    free(buffer);
});

smitty_test(it_creates_a_buffer_with_the_specified_capacity, {
    ScrumpBuffer *buffer = buffer_create(2048);

    expect_int_equal(buffer->capacity, 2048);

    free(buffer);
});

smitty_test(it_returns_null_when_buffer_created_with_zero_capacity, {
    ScrumpBuffer *buffer = buffer_create(0);

    expect_null(buffer);

    free(buffer);
});

smitty_test(it_returns_attempted_overflow_when_a_user_attempts_an_overflow, {
    ScrumpBuffer *buffer = buffer_create(3);
    char *data = "Hello, world!";

    ReturnCode result = buffer_write(buffer, data, strlen(data));

    expect_int_equal(result, ATTEMPTED_OVERFLOW);

    free(buffer);
});

smitty_test(it_writes_the_data_to_the_buffer, {
    ScrumpBuffer *buffer = buffer_create(1024);
    char *data = "Hello, world!";

    buffer_write(buffer, data, strlen(data));

    expect_string_equal(buffer->data, data);

    free(buffer);
});

smitty_test(it_writes_the_data_to_the_buffer_when_the_size_exactly_equals_remaining_capacity, {
    ScrumpBuffer *buffer = buffer_create(13);
    char *data = "Hello, world!";

    buffer_write(buffer, data, strlen(data));

    expect_string_equal(buffer->data, data);

    free(buffer);
});

smitty_test(it_returns_attempted_overflow_when_read_cursor_is_at_capacity, {
    ScrumpBuffer *buffer = buffer_create(5);
    char *data = "Hello";

    // Fill up the buffer.
    buffer_write(buffer, data, strlen(data));
    
    // Attempt to add one more byte.
    char one_more_byte = 'b';
    const ReturnCode result = buffer_write(buffer, &one_more_byte, 1);

    expect_int_equal(result, ATTEMPTED_OVERFLOW);

    free(buffer);
});

//--------------------------------------------------------------------------------------------------
// Smitty boilerplate
//--------------------------------------------------------------------------------------------------

smitty_register_tests(
    smitty_test_as_name_and_callback(it_creates_a_default_buffer_with_1024_bytes_of_capacity),
    smitty_test_as_name_and_callback(it_creates_a_default_buffer_with_a_non_null_pointer_to_its_data),
    smitty_test_as_name_and_callback(it_creates_a_default_buffer_with_the_same_address_for_data_read_cursor_and_write_cursor),
    smitty_test_as_name_and_callback(it_creates_a_buffer_with_the_specified_capacity),
    smitty_test_as_name_and_callback(it_returns_null_when_buffer_created_with_zero_capacity),
    smitty_test_as_name_and_callback(it_returns_attempted_overflow_when_a_user_attempts_an_overflow),
    smitty_test_as_name_and_callback(it_writes_the_data_to_the_buffer),
    smitty_test_as_name_and_callback(it_writes_the_data_to_the_buffer_when_the_size_exactly_equals_remaining_capacity),
    smitty_test_as_name_and_callback(it_returns_attempted_overflow_when_read_cursor_is_at_capacity),
);

smitty_run_test_suite();
