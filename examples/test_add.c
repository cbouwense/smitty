#include "../src/smitty.h"
#include "add.h"
#include "scrump.h"

int failed_expect_count = 0;

smitty_test(it_can_add_two_positive_integers, {
    expect_int_equal(add(2, 2), 5);
});

smitty_test(it_can_add_two_negative_integers, {
    expect_int_equal(add(-2, -2), -5);
});

int main() {
    ScrumpBuffer *test_case_ptrs = scrump_buffer_create_default();

    scrump_buffer_write_func_ptr(test_case_ptrs, it_can_add_two_positive_integers);
    scrump_buffer_write_func_ptr(test_case_ptrs, it_can_add_two_negative_integers);

    const smitty_test_result (*test_case_1)() = scrump_buffer_read_func_ptr(test_case_ptrs, 8);
    const smitty_test_result (*test_case_2)() = scrump_buffer_read_func_ptr(test_case_ptrs, 8);

    test_case_1();
    test_case_2();

    printf("failed_expect_count: %d\n", failed_expect_count);

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
