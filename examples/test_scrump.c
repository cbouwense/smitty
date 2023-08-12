#include "../src/smitty.h"
#include "scrump.h"

//--------------------------------------------------------------------------------------------------
// Smitty string buffer
//--------------------------------------------------------------------------------------------------

smitty_test(it_returns_null_when_scrump_string_buffer_created_with_zero_capacity, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(0);

    expect_null(buffer);

    free(buffer);
});

smitty_test(it_returns_non_null_when_scrump_string_buffer_created_with_non_zero_capacity, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(42);

    expect_non_null(buffer);

    free(buffer);
});

smitty_test(it_returns_a_write_overflow_error_when_a_user_attempts_to_write_past_the_buffer_capacity, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(3);
    char *data = "Hello, world!";

    ScrumpReturnCodeType result = scrump_string_buffer_write(buffer, data, strlen(data));

    expect_int_equal(result, SCRUMP_ATTEMPTED_WRITE_OVERFLOW);

    free(buffer);
});

smitty_test(it_returns_attempted_write_overflow_when_write_cursor_is_at_capacity, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(5);
    char *data = "Hello";

    // Fill up the buffer.
    scrump_string_buffer_write(buffer, data, strlen(data));
    
    // Attempt to add one more byte.
    char one_more_byte = '!';
    const ScrumpReturnCodeType result = scrump_string_buffer_write(buffer, &one_more_byte, 1);

    expect_int_equal(result, SCRUMP_ATTEMPTED_WRITE_OVERFLOW);

    free(buffer);
});

smitty_test(it_successfully_writes_when_data_is_bigger_than_capacity_but_size_is_within_capacity, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(5);
    char *data = "Hello, world!";

    ScrumpReturnCodeType result = scrump_string_buffer_write(buffer, data, 5);

    expect_int_equal(result, SCRUMP_SUCCESS);

    free(buffer);
});

smitty_test(it_returns_attempted_read_overflow_when_user_attempts_to_read_past_the_write_cursor, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(42);
    char *data = "Hello";

    scrump_string_buffer_write(buffer, data, strlen(data));
    
    const short written_byte_count = strlen(data);
    char read_buffer[written_byte_count + 1];
    const ScrumpReturnCodeType result = scrump_string_buffer_read(buffer, read_buffer, written_byte_count + 1);

    expect_int_equal(result, SCRUMP_ATTEMPTED_READ_OVERFLOW);

    free(buffer);
});

// This is a bit redundant because the read cursor should always be behind the write cursor, so an
// attempt to read more bytes than the capacity is an attempt to read past the write cursor. But, I
// kinda like this test anyways.
smitty_test(it_returns_attempted_read_overflow_when_user_attempts_to_read_past_the_capacity, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(1024);
    
    char read_buffer[2048];
    const ScrumpReturnCodeType result = scrump_string_buffer_read(buffer, read_buffer, 2048);

    expect_int_equal(result, SCRUMP_ATTEMPTED_READ_OVERFLOW);

    free(buffer);
});

smitty_test(it_returns_data_when_there_is_no_attempted_read_overflow, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(1024);
    char *data = "Hello, world!";

    scrump_string_buffer_write(buffer, data, strlen(data));

    char read_buffer[13];
    scrump_string_buffer_read(buffer, read_buffer, 13);

    expect_string_equal(read_buffer, "Hello, world!");

    free(buffer);
});

// smitty_test(it_returns_data_when_the_user_tries_to_read_every_byte_written_so_far, {
//     ScrumpBuffer *buffer = scrump_buffer_create(1024);
//     char *hello = "Hello";
//     char *world = "World";

//     scrump_buffer_write(buffer, hello, strlen(hello));
//     scrump_buffer_write(buffer, world, strlen(world));
    
//     char read_buffer[10];
//     scrump_buffer_read(buffer, read_buffer, strlen(hello) + strlen(world));

//     expect_string_equal(read_buffer, "HelloWorld");

//     free(buffer);
// });

// TODO: It would be nice to detect when a test has not been registered and print a warning.
smitty_register_and_run_tests(
    it_returns_null_when_scrump_string_buffer_created_with_zero_capacity,
    it_returns_non_null_when_scrump_string_buffer_created_with_non_zero_capacity,
    it_returns_a_write_overflow_error_when_a_user_attempts_to_write_past_the_buffer_capacity,
    it_successfully_writes_when_data_is_bigger_than_capacity_but_size_is_within_capacity,
    it_returns_attempted_write_overflow_when_write_cursor_is_at_capacity,
    it_returns_attempted_read_overflow_when_user_attempts_to_read_past_the_write_cursor,
    it_returns_attempted_read_overflow_when_user_attempts_to_read_past_the_capacity,
    it_returns_data_when_there_is_no_attempted_read_overflow,
    // it_returns_data_when_the_user_tries_to_read_every_byte_written_so_far,
);
