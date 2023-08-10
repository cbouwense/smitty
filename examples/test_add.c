#include "../src/smitty.h"
#include "add.h"
#include "scrump.h"

smitty_test_result it_can_add_two_positive_integers() {
    printf("it_can_add_two_positive_integers()\n");
}

int main() {
    ScrumpBuffer *test_function_pointer_buffer = scrump_buffer_create_default();

    scrump_buffer_debug_print(test_function_pointer_buffer);

    const ScrumpReturnCode write_result = scrump_buffer_write_ptr(test_function_pointer_buffer, it_can_add_two_positive_integers);
    printf("write result:\t%s\n", scrump_return_code_to_string(write_result));

    const smitty_test_result (*read_buffer)() = malloc(8);
    const ScrumpReturnCode read_result = scrump_buffer_read(test_function_pointer_buffer, read_buffer, 8);
    smitty_test_result (*naive_test_ptr)() = (smitty_test_result (*)())it_can_add_two_positive_integers;
    printf("read result:\t%s\n\n", scrump_return_code_to_string(read_result));

    printf("&read_buffer:\t%p\n", &read_buffer);
    printf("read_buffer:\t%p\n", read_buffer);
    printf("*read_buffer:\t%p\n", *read_buffer);

    return 0;
}

// smitty_test(it_can_add_two_positive_integers, {
//     expect_int_equal(add(2, 2), 4);
// });

// smitty_test(it_can_add_two_negative_integers, {
//     expect_int_equal(add(-2, -3), -5);
// });

// smitty_test(it_can_add_two_numbers_whose_sum_is_zero, {
//     expect_int_equal(add(-42, 42), 0);
// });

// smitty_test(it_can_add_a_positive_and_a_negative_integer, {
//     expect_int_equal(add(-2, 3), 1);
// });

//--------------------------------------------------------------------------------------------------
// Smitty boilerplate
//--------------------------------------------------------------------------------------------------

// smitty_register_tests(
//     smitty_test_as_name_and_callback(it_can_add_two_positive_integers),
//     smitty_test_as_name_and_callback(it_can_add_two_negative_integers),
//     smitty_test_as_name_and_callback(it_can_add_two_numbers_whose_sum_is_zero),
//     smitty_test_as_name_and_callback(it_can_add_a_positive_and_a_negative_integer),
// )

// smitty_run_test_suite()
