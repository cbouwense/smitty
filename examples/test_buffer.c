#include "../src/smitty.h"
#include "buffer.h"

smitty_test(it_creates_a_default_buffer_with_1024_bytes_of_capacity, {
    ScrumpBuffer *buffer = scrump_scrump_buffer_create_default();

    expect_int_equal(buffer->capacity,  1024);

    free(buffer);
});

smitty_test(it_creates_a_default_buffer_with_a_non_null_pointer_to_its_data, {
    ScrumpBuffer *buffer = scrump_scrump_buffer_create_default();

    expect_non_null(buffer->data);

    free(buffer);
});

smitty_test(it_returns_null_when_scrump_buffer_created_with_zero_capacity, {
    ScrumpBuffer *buffer = scrump_buffer_create(0);

    expect_null(buffer);

    free(buffer);
});

smitty_test(it_returns_attempted_overflow_when_a_user_attempts_an_overflow, {
    ScrumpBuffer *buffer = scrump_buffer_create(3);
    char *data = "Hello, world!";

    ReturnCode result = scrump_buffer_write(buffer, data, strlen(data));

    expect_int_equal(result, SCRUMP_ATTEMPTED_WRITE_OVERFLOW);

    free(buffer);
});

smitty_test(it_writes_the_data_to_the_buffer, {
    ScrumpBuffer *buffer = scrump_buffer_create(1024);
    char *data = "Hello, world!";

    scrump_buffer_write(buffer, data, strlen(data));

    expect_string_equal(buffer->data, data);

    free(buffer);
});

smitty_test(it_writes_the_data_to_the_buffer_when_the_size_exactly_equals_remaining_capacity, {
    ScrumpBuffer *buffer = scrump_buffer_create(13);
    char *data = "Hello, world!";

    scrump_buffer_write(buffer, data, strlen(data));

    expect_string_equal(buffer->data, data);

    free(buffer);
});

smitty_test(it_returns_attempted_write_overflow_when_write_cursor_is_at_capacity, {
    ScrumpBuffer *buffer = scrump_buffer_create(5);
    char *data = "Hello";

    // Fill up the buffer.
    scrump_buffer_write(buffer, data, strlen(data));
    
    // Attempt to add one more byte.
    char one_more_byte = '!';
    const ReturnCode result = scrump_buffer_write(buffer, &one_more_byte, 1);

    expect_int_equal(result, SCRUMP_ATTEMPTED_WRITE_OVERFLOW);

    free(buffer);
});

smitty_test(it_returns_attempted_read_overflow_when_too_many_read_bytes_requested, {
    ScrumpBuffer *buffer = scrump_scrump_buffer_create_default();
    char *data = "Hello";

    scrump_buffer_write(buffer, data, strlen(data));
    
    // Attempt to read one more byte than is available.
    char read_buffer[6];
    const ReturnCode result = scrump_buffer_read(buffer, read_buffer, 6);

    expect_int_equal(result, SCRUMP_ATTEMPTED_READ_OVERFLOW);

    free(buffer);
});

smitty_test(it_returns_data_when_there_is_no_attempted_read_overflow, {
    ScrumpBuffer *buffer = scrump_scrump_buffer_create_default();
    char *data = "Hello";

    scrump_buffer_write(buffer, data, strlen(data));
    
    char read_buffer[5];
    scrump_buffer_read(buffer, read_buffer, 5);

    expect_string_equal(read_buffer, data);

    free(buffer);
});

//--------------------------------------------------------------------------------------------------
// Smitty boilerplate
//--------------------------------------------------------------------------------------------------

smitty_register_tests(
    smitty_test_as_name_and_callback(it_creates_a_default_buffer_with_1024_bytes_of_capacity),
    smitty_test_as_name_and_callback(it_creates_a_default_buffer_with_a_non_null_pointer_to_its_data),
    smitty_test_as_name_and_callback(it_returns_null_when_scrump_buffer_created_with_zero_capacity),
    smitty_test_as_name_and_callback(it_returns_attempted_overflow_when_a_user_attempts_an_overflow),
    smitty_test_as_name_and_callback(it_writes_the_data_to_the_buffer),
    smitty_test_as_name_and_callback(it_writes_the_data_to_the_buffer_when_the_size_exactly_equals_remaining_capacity),
    smitty_test_as_name_and_callback(it_returns_attempted_write_overflow_when_write_cursor_is_at_capacity),
    smitty_test_as_name_and_callback(it_returns_attempted_read_overflow_when_too_many_read_bytes_requested),
    smitty_test_as_name_and_callback(it_returns_data_when_there_is_no_attempted_read_overflow),
);

smitty_run_test_suite();
