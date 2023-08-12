#include "../src/smitty.h"
#include "scrump.h"

//--------------------------------------------------------------------------------------------------
// Scrump char buffer
//--------------------------------------------------------------------------------------------------

smitty_test(scrump_char_buffer_returns_null_when_created_with_zero_capacity, {
    ScrumpCharBuffer *buffer = scrump_char_buffer_create(0);

    expect_null(buffer);

    scrump_char_buffer_free(buffer);
});

smitty_test(scrump_char_buffer_returns_non_null_when_created_with_non_zero_capacity, {
    ScrumpCharBuffer *buffer = scrump_char_buffer_create(42);

    expect_non_null(buffer);

    scrump_char_buffer_free(buffer);
});

smitty_test(scrump_char_buffer_returns_attempted_write_overflow_when_user_attempts_to_write_past_the_buffer_capacity, {
    ScrumpCharBuffer *buffer = scrump_char_buffer_create(3);
    const char *data = "Hello, world!";

    const ScrumpReturnCodeType return_code = scrump_char_buffer_write(buffer, data, strlen(data));

    expect_enum_equal(return_code, SCRUMP_ATTEMPTED_WRITE_OVERFLOW, scrump_return_code_to_string);

    scrump_char_buffer_free(buffer);
});

smitty_test(scrump_char_buffer_returns_attempted_write_overflow_when_write_cursor_is_at_capacity, {
    ScrumpCharBuffer *buffer = scrump_char_buffer_create(5);
    const char *data = "Hello";

    // Fill up the buffer.
    const ScrumpReturnCodeType first_write_return_code = scrump_char_buffer_write(buffer, data, strlen(data));
    
    // Attempt to add one more byte.
    const char one_more_byte = '!';
    const ScrumpReturnCodeType second_write_return_code = scrump_char_buffer_write(buffer, &one_more_byte, 1);

    expect_enum_equal(first_write_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(second_write_return_code, SCRUMP_ATTEMPTED_WRITE_OVERFLOW, scrump_return_code_to_string);

    scrump_char_buffer_free(buffer);
});

smitty_test(scrump_char_buffer_can_write_once, {
    ScrumpCharBuffer *buffer = scrump_char_buffer_create(42);
    const char *data = "Hello, world!";

    ScrumpReturnCodeType return_code = scrump_char_buffer_write(buffer, data, strlen(data));

    expect_enum_equal(return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);

    scrump_char_buffer_free(buffer);
});

smitty_test(scrump_char_buffer_can_write_multiple_times, {
    ScrumpCharBuffer *buffer = scrump_char_buffer_create(42);
    const char *data = "Hello, world!";

    ScrumpReturnCodeType   return_code_one = scrump_char_buffer_write(buffer, data, strlen(data));
    ScrumpReturnCodeType   return_code_two = scrump_char_buffer_write(buffer, data, strlen(data));
    ScrumpReturnCodeType return_code_three = scrump_char_buffer_write(buffer, data, strlen(data));
    ScrumpReturnCodeType  return_code_four = scrump_char_buffer_write(buffer, data, strlen(data));

    expect_enum_equal(return_code_one, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code_two, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code_three, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code_four, SCRUMP_ATTEMPTED_WRITE_OVERFLOW, scrump_return_code_to_string);

    scrump_char_buffer_free(buffer);
});

smitty_test(scrump_char_buffer_returns_attempted_read_overflow_when_user_attempts_to_read_past_the_write_cursor, {
    ScrumpCharBuffer *buffer = scrump_char_buffer_create(42);
    char *data = "Hello";
    char read_buffer[strlen(data) + 1];

    const ScrumpReturnCodeType write_return_code = scrump_char_buffer_write(buffer, data, strlen(data));
    const ScrumpReturnCodeType  read_return_code = scrump_char_buffer_read(buffer, read_buffer, strlen(data) + 1, strlen(data) + 1);

    expect_enum_equal(write_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(read_return_code, SCRUMP_ATTEMPTED_READ_OVERFLOW, scrump_return_code_to_string);

    scrump_char_buffer_free(buffer);
});

// This is a bit redundant because the read cursor should always be behind the write cursor, so an
// attempt to read more bytes than the capacity is an attempt to read past the write cursor. But, I
// kinda like this test anyways.
smitty_test(scrump_char_buffer_returns_attempted_read_overflow_when_user_attempts_to_read_past_the_capacity, {
    ScrumpCharBuffer *buffer = scrump_char_buffer_create(1024);
    
    char read_buffer[2048];
    const ScrumpReturnCodeType return_code = scrump_char_buffer_read(buffer, read_buffer, 2048, 2048);

    expect_enum_equal(return_code, SCRUMP_ATTEMPTED_READ_OVERFLOW, scrump_return_code_to_string);

    scrump_char_buffer_free(buffer);
});

smitty_test(scrump_char_buffer_returns_read_buffer_too_small_error_when_the_read_buffer_isnt_big_enough_to_be_null_terminated, {
    ScrumpCharBuffer *buffer = scrump_char_buffer_create(1024);
    const char *hello = "Hello";

    const ScrumpReturnCodeType write_return_code = scrump_char_buffer_write(buffer, hello, strlen(hello));

    char read_buffer[5];
    const ScrumpReturnCodeType read_return_code = scrump_char_buffer_read(buffer, read_buffer, 5, 5);

    expect_enum_equal(write_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(read_return_code, SCRUMP_READ_BUFFER_TOO_SMALL, scrump_return_code_to_string);

    scrump_char_buffer_free(buffer);
});

smitty_test(scrump_char_buffer_reads_when_there_is_no_attempted_read_overflow, {
    ScrumpCharBuffer *buffer = scrump_char_buffer_create(1024);
    const char *data = "Hello, world!";

    const ScrumpReturnCodeType write_return_code = scrump_char_buffer_write(buffer, data, strlen(data));

    char read_buffer[14];
    const ScrumpReturnCodeType return_code = scrump_char_buffer_read(buffer, read_buffer, 13, 14);

    expect_enum_equal(write_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_string_equal(read_buffer, "Hello, world!");

    scrump_char_buffer_free(buffer);
});

smitty_test(scrump_char_buffer_reads_when_the_user_tries_to_read_every_byte_written_so_far, {
    ScrumpCharBuffer *buffer = scrump_char_buffer_create(1024);
    const char *hello = "Hello";
    const char *world = "World";

    const ScrumpReturnCodeType write_one_return_code = scrump_char_buffer_write(buffer, hello, strlen(hello));
    const ScrumpReturnCodeType write_rwo_return_code = scrump_char_buffer_write(buffer, world, strlen(world));
    
    char read_buffer[11];
    ScrumpReturnCodeType return_code = scrump_char_buffer_read(buffer, read_buffer, strlen(hello) + strlen(world), 11);

    expect_enum_equal(write_one_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(write_rwo_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_string_equal(read_buffer, "HelloWorld");

    scrump_char_buffer_free(buffer);
});

smitty_test(scrump_char_buffer_can_write_and_read_multiple_times, {
    ScrumpCharBuffer *buffer = scrump_char_buffer_create(1024);
    const char *one = "I'm like";
    const char *two = "hey what's up";
    const char *three = "hello";

    const ScrumpReturnCodeType write_one_return_code = scrump_char_buffer_write(buffer, one, strlen(one));
    char read_buffer_one[strlen(one) + 1];
    const ScrumpReturnCodeType read_one_return_code = scrump_char_buffer_read(buffer, read_buffer_one, strlen(one), strlen(one) + 1);

    const ScrumpReturnCodeType write_two_return_code = scrump_char_buffer_write(buffer, two, strlen(two));
    char read_buffer_two[strlen(two) + 1];
    const ScrumpReturnCodeType read_two_return_code = scrump_char_buffer_read(buffer, read_buffer_two, strlen(two), strlen(two) + 1);

    const ScrumpReturnCodeType write_three_return_code = scrump_char_buffer_write(buffer, three, strlen(three));
    char read_buffer_three[strlen(three) + 1];
    const ScrumpReturnCodeType read_three_return_code = scrump_char_buffer_read(buffer, read_buffer_three, strlen(three), strlen(three) + 1);

    expect_enum_equal(write_one_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(read_one_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(write_two_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(read_two_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(write_three_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(read_three_return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);

    expect_string_equal(read_buffer_one, "I'm like");
    expect_string_equal(read_buffer_two, "hey what's up");
    expect_string_equal(read_buffer_three, "hello");

    scrump_char_buffer_free(buffer);
});

//--------------------------------------------------------------------------------------------------
// Scrump int buffer
//--------------------------------------------------------------------------------------------------

smitty_test(scrump_int_buffer_returns_null_when_created_with_zero_capacity, {
    ScrumpIntBuffer *buffer = scrump_int_buffer_create(0);

    expect_null(buffer);

    scrump_int_buffer_free(buffer);
})

smitty_test(scrump_int_buffer_returns_non_null_when_created_with_non_zero_capacity, {
    ScrumpIntBuffer *buffer = scrump_int_buffer_create(42);

    expect_non_null(buffer);

    scrump_int_buffer_free(buffer);
});

smitty_test(scrump_int_buffer_returns_attempted_write_overflow_when_user_attempts_to_write_past_the_buffer_capacity, {
    ScrumpIntBuffer *buffer = scrump_int_buffer_create(1);
    const int data = 42;

    const ScrumpReturnCodeType return_code = scrump_int_buffer_write(buffer, &data, 2);

    expect_enum_equal(return_code, SCRUMP_ATTEMPTED_WRITE_OVERFLOW, scrump_return_code_to_string);

    scrump_int_buffer_free(buffer);
});

smitty_test(scrump_int_buffer_can_write_once, {
    ScrumpIntBuffer *buffer = scrump_int_buffer_create(4);
    const int data = 2;

    const ScrumpReturnCodeType return_code = scrump_int_buffer_write(buffer, &data, 1);

    expect_enum_equal(return_code, SCRUMP_SUCCESS, scrump_return_code_to_string);

    scrump_int_buffer_free(buffer);
});

smitty_test(scrump_int_buffer_can_write_multiple_times, {
    ScrumpIntBuffer *buffer = scrump_int_buffer_create(3);
    const int data = 2;

    const ScrumpReturnCodeType return_code_one = scrump_int_buffer_write(buffer, &data, 1);
    const ScrumpReturnCodeType return_code_two = scrump_int_buffer_write(buffer, &data, 1);
    const ScrumpReturnCodeType return_code_three = scrump_int_buffer_write(buffer, &data, 1);

    expect_enum_equal(return_code_one, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code_two, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code_three, SCRUMP_SUCCESS, scrump_return_code_to_string);

    scrump_int_buffer_free(buffer);
});

smitty_test(scrump_int_buffer_returns_attempted_write_overflow_when_user_writes_when_cursor_is_at_capacity, {
    ScrumpIntBuffer *buffer = scrump_int_buffer_create(1);
    const int data = 2;

    const ScrumpReturnCodeType return_code_one = scrump_int_buffer_write(buffer, &data, 1);
    const ScrumpReturnCodeType return_code_two = scrump_int_buffer_write(buffer, &data, 1);

    expect_enum_equal(return_code_one, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code_two, SCRUMP_ATTEMPTED_WRITE_OVERFLOW, scrump_return_code_to_string);

    scrump_int_buffer_free(buffer);
});

smitty_test(scrump_int_buffer_returns_attempted_read_overflow_when_user_attempts_to_read_an_empty_buffer, {
    ScrumpIntBuffer *buffer = scrump_int_buffer_create(1);
    int data = 42;

    const ScrumpReturnCodeType return_code = scrump_int_buffer_read(buffer, &data, 1);

    expect_enum_equal(return_code, SCRUMP_ATTEMPTED_READ_OVERFLOW, scrump_return_code_to_string);

    scrump_int_buffer_free(buffer);
});

smitty_test(scrump_int_buffer_returns_attempted_read_overflow_when_user_attempts_to_read_past_the_write_cursor, {
    ScrumpIntBuffer *buffer = scrump_int_buffer_create(1);
    int data = 42;

    const ScrumpReturnCodeType return_code_one = scrump_int_buffer_write(buffer, &data, 1);
    const ScrumpReturnCodeType return_code_two = scrump_int_buffer_read(buffer, &data, 2);

    expect_enum_equal(return_code_one, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code_two, SCRUMP_ATTEMPTED_READ_OVERFLOW, scrump_return_code_to_string);

    scrump_int_buffer_free(buffer);
});

smitty_test(scrump_int_buffer_can_read_when_user_reads_within_read_cursor, {
    ScrumpIntBuffer *buffer = scrump_int_buffer_create(1);
    int write_data = 42;
    int read_data[] = smitty_initialize_array(0);

    const ScrumpReturnCodeType return_code_write = scrump_int_buffer_write(buffer, &write_data, 1);
    const ScrumpReturnCodeType return_code_read = scrump_int_buffer_read(buffer, read_data, 1);

    expect_enum_equal(return_code_write, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code_read, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_int_equal(read_data[0], 42);

    scrump_int_buffer_free(buffer);
});

smitty_test(scrump_int_buffer_can_read_when_user_reads_every_written_byte, {
    ScrumpIntBuffer *buffer = scrump_int_buffer_create(42);
    int write_data[] = smitty_initialize_array(1, 7, 3, 8);

    const ScrumpReturnCodeType return_code_write = scrump_int_buffer_write(buffer, write_data, 4);
    const ScrumpReturnCodeType return_code_read = scrump_int_buffer_read(buffer, write_data, 4);

    expect_enum_equal(return_code_write, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code_read, SCRUMP_SUCCESS, scrump_return_code_to_string);
    // TODO: replace with expect_array_equal once that is implemented.
    expect_int_equal(write_data[0], 1);
    expect_int_equal(write_data[1], 7);
    expect_int_equal(write_data[2], 3);
    expect_int_equal(write_data[3], 8);

    scrump_int_buffer_free(buffer);
});

smitty_test(scrump_int_buffer_can_write_and_read_multiple_times, {
    ScrumpIntBuffer *buffer = scrump_int_buffer_create(42);
    int write_data[] = smitty_initialize_array(1, 7, 3, 8);
    int read_data_one[] = smitty_initialize_array(0, 0, 0, 0);
    int read_data_two[] = smitty_initialize_array(0, 0, 0, 0);

    const ScrumpReturnCodeType return_code_write_one = scrump_int_buffer_write(buffer, write_data, 4);
    const ScrumpReturnCodeType return_code_read_one = scrump_int_buffer_read(buffer, read_data_one, 4);
    const ScrumpReturnCodeType return_code_write_two = scrump_int_buffer_write(buffer, write_data, 4);
    const ScrumpReturnCodeType return_code_read_two = scrump_int_buffer_read(buffer, read_data_two, 4);

    expect_enum_equal(return_code_write_one, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code_read_one, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code_write_two, SCRUMP_SUCCESS, scrump_return_code_to_string);
    expect_enum_equal(return_code_read_two, SCRUMP_SUCCESS, scrump_return_code_to_string);
    
    expect_int_equal(read_data_one[0], 1);
    expect_int_equal(read_data_one[1], 7);
    expect_int_equal(read_data_one[2], 3);
    expect_int_equal(read_data_one[3], 8);
    expect_int_equal(read_data_two[0], 1);
    expect_int_equal(read_data_two[1], 7);
    expect_int_equal(read_data_two[2], 3);
    expect_int_equal(read_data_two[3], 8);

    scrump_int_buffer_free(buffer);
});

// TODO: It would be nice to detect when a test has not been registered and print a warning.
smitty_register_and_run_tests(
    // Scrump char buffer
    scrump_char_buffer_returns_null_when_created_with_zero_capacity,
    scrump_char_buffer_returns_non_null_when_created_with_non_zero_capacity,
    scrump_char_buffer_returns_attempted_write_overflow_when_user_attempts_to_write_past_the_buffer_capacity,
    scrump_char_buffer_returns_attempted_write_overflow_when_write_cursor_is_at_capacity,
    scrump_char_buffer_can_write_once,
    scrump_char_buffer_can_write_multiple_times,
    scrump_char_buffer_returns_attempted_read_overflow_when_user_attempts_to_read_past_the_write_cursor,
    scrump_char_buffer_returns_attempted_read_overflow_when_user_attempts_to_read_past_the_capacity,
    scrump_char_buffer_reads_when_there_is_no_attempted_read_overflow,
    scrump_char_buffer_reads_when_the_user_tries_to_read_every_byte_written_so_far,
    scrump_char_buffer_can_write_and_read_multiple_times,
    scrump_char_buffer_returns_read_buffer_too_small_error_when_the_read_buffer_isnt_big_enough_to_be_null_terminated,
    
    // Scrump int buffer
    scrump_int_buffer_returns_null_when_created_with_zero_capacity,
    scrump_int_buffer_returns_non_null_when_created_with_non_zero_capacity,
    scrump_int_buffer_returns_attempted_write_overflow_when_user_attempts_to_write_past_the_buffer_capacity,
    scrump_int_buffer_returns_attempted_write_overflow_when_user_writes_when_cursor_is_at_capacity,
    scrump_int_buffer_returns_attempted_read_overflow_when_user_attempts_to_read_past_the_write_cursor,
    scrump_int_buffer_returns_attempted_read_overflow_when_user_attempts_to_read_an_empty_buffer,
    scrump_int_buffer_can_write_once,
    scrump_int_buffer_can_write_multiple_times,
    scrump_int_buffer_can_read_when_user_reads_within_read_cursor,
    scrump_int_buffer_can_read_when_user_reads_every_written_byte,
    scrump_int_buffer_can_write_and_read_multiple_times,
);
