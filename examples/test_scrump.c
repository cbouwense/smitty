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
    const char *data = "Hello, world!";

    const ScrumpReturnCodeType return_code = scrump_string_buffer_write(buffer, data, strlen(data));

    expect_enum_equal(return_code, SCRUMP_ATTEMPTED_WRITE_OVERFLOW, scrump_return_code_to_string);

    free(buffer);
});

smitty_test(it_returns_attempted_write_overflow_when_write_cursor_is_at_capacity, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(5);
    const char *data = "Hello";

    // Fill up the buffer.
    const ScrumpReturnCodeType first_write_return_code = scrump_string_buffer_write(buffer, data, strlen(data));
    
    // Attempt to add one more byte.
    const char one_more_byte = '!';
    const ScrumpReturnCodeType second_write_return_code = scrump_string_buffer_write(buffer, &one_more_byte, 1);

    expect_enum_equal(first_write_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(second_write_return_code, SCRUMP_ATTEMPTED_WRITE_OVERFLOW, scrump_return_code_to_string);

    free(buffer);
});

smitty_test(it_successfully_writes_when_data_is_bigger_than_capacity_but_size_is_within_capacity, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(5);
    const char *data = "Hello, world!";

    ScrumpReturnCodeType return_code = scrump_string_buffer_write(buffer, data, 5);

    expect_enum_equal(return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);

    free(buffer);
});

smitty_test(it_returns_attempted_read_overflow_when_user_attempts_to_read_past_the_write_cursor, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(42);
    char *data = "Hello";

    const ScrumpReturnCodeType write_return_code = scrump_string_buffer_write(buffer, data, strlen(data));
    
    char read_buffer[strlen(data) + 1];
    const ScrumpReturnCodeType read_return_code = scrump_string_buffer_read(buffer, read_buffer, strlen(data) + 1, strlen(data) + 1);

    expect_enum_equal(write_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(read_return_code, SCRUMP_ATTEMPTED_READ_OVERFLOW, scrump_return_code_to_string);

    free(buffer);
});

// This is a bit redundant because the read cursor should always be behind the write cursor, so an
// attempt to read more bytes than the capacity is an attempt to read past the write cursor. But, I
// kinda like this test anyways.
smitty_test(it_returns_attempted_read_overflow_when_user_attempts_to_read_past_the_capacity, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(1024);
    
    char read_buffer[2048];
    const ScrumpReturnCodeType return_code = scrump_string_buffer_read(buffer, read_buffer, 2048, 2048);

    expect_enum_equal(return_code, SCRUMP_ATTEMPTED_READ_OVERFLOW, scrump_return_code_to_string);

    free(buffer);
});

smitty_test(it_returns_read_buffer_too_small_error_when_the_read_buffer_isnt_big_enough_to_be_null_terminated, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(1024);
    const char *hello = "Hello";

    const ScrumpReturnCodeType write_return_code = scrump_string_buffer_write(buffer, hello, strlen(hello));

    char read_buffer[5];
    const ScrumpReturnCodeType read_return_code = scrump_string_buffer_read(buffer, read_buffer, 5, 5);

    expect_enum_equal(write_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(read_return_code, SCRUMP_READ_BUFFER_TOO_SMALL, scrump_return_code_to_string);
});

smitty_test(it_reads_when_there_is_no_attempted_read_overflow, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(1024);
    const char *data = "Hello, world!";

    const ScrumpReturnCodeType write_return_code = scrump_string_buffer_write(buffer, data, strlen(data));

    char read_buffer[14];
    const ScrumpReturnCodeType return_code = scrump_string_buffer_read(buffer, read_buffer, 13, 14);

    expect_enum_equal(write_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_string_equal(read_buffer, "Hello, world!");

    free(buffer);
});

smitty_test(it_reads_when_the_user_tries_to_read_every_byte_written_so_far, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(1024);
    const char *hello = "Hello";
    const char *world = "World";

    const ScrumpReturnCodeType write_one_return_code = scrump_string_buffer_write(buffer, hello, strlen(hello));
    const ScrumpReturnCodeType write_rwo_return_code = scrump_string_buffer_write(buffer, world, strlen(world));
    
    char read_buffer[11];
    ScrumpReturnCodeType return_code = scrump_string_buffer_read(buffer, read_buffer, strlen(hello) + strlen(world), 11);

    expect_enum_equal(write_one_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(write_rwo_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_string_equal(read_buffer, "HelloWorld");

    free(buffer);
});

smitty_test(it_can_write_and_read_multiple_times, {
    ScrumpBuffer *buffer = scrump_string_buffer_create(1024);
    const char *one = "I'm like";
    const char *two = "hey what's up";
    const char *three = "hello";

    const ScrumpReturnCodeType write_one_return_code = scrump_string_buffer_write(buffer, one, strlen(one));
    char read_buffer_one[strlen(one) + 1];
    const ScrumpReturnCodeType read_one_return_code = scrump_string_buffer_read(buffer, read_buffer_one, strlen(one), strlen(one) + 1);

    const ScrumpReturnCodeType write_two_return_code = scrump_string_buffer_write(buffer, two, strlen(two));
    char read_buffer_two[strlen(two) + 1];
    const ScrumpReturnCodeType read_two_return_code = scrump_string_buffer_read(buffer, read_buffer_two, strlen(two), strlen(two) + 1);

    const ScrumpReturnCodeType write_three_return_code = scrump_string_buffer_write(buffer, three, strlen(three));
    char read_buffer_three[strlen(three) + 1];
    const ScrumpReturnCodeType read_three_return_code = scrump_string_buffer_read(buffer, read_buffer_three, strlen(three), strlen(three) + 1);

    expect_enum_equal(write_one_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(read_one_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(write_two_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(read_two_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(write_three_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(read_three_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);

    expect_string_equal(read_buffer_one, "I'm like");
    expect_string_equal(read_buffer_two, "hey what's up");
    expect_string_equal(read_buffer_three, "hello");

    free(buffer);
});

// TODO: It would be nice to detect when a test has not been registered and print a warning.
smitty_register_and_run_tests(
    it_returns_null_when_scrump_string_buffer_created_with_zero_capacity,
    it_returns_non_null_when_scrump_string_buffer_created_with_non_zero_capacity,
    it_returns_a_write_overflow_error_when_a_user_attempts_to_write_past_the_buffer_capacity,
    it_successfully_writes_when_data_is_bigger_than_capacity_but_size_is_within_capacity,
    it_returns_attempted_write_overflow_when_write_cursor_is_at_capacity,
    it_returns_attempted_read_overflow_when_user_attempts_to_read_past_the_write_cursor,
    it_returns_attempted_read_overflow_when_user_attempts_to_read_past_the_capacity,
    it_reads_when_there_is_no_attempted_read_overflow,
    it_reads_when_the_user_tries_to_read_every_byte_written_so_far,
    it_can_write_and_read_multiple_times,
    it_returns_read_buffer_too_small_error_when_the_read_buffer_isnt_big_enough_to_be_null_terminated,
);
