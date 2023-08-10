#include "../src/smitty.h"
#include "add.h"
#include "scrump.h"

smitty_test(it_can_add_two_positive_integers, {
    expect_int_equal(add(2, 2), 4);
});

smitty_test(it_can_add_two_negative_integers, {
    expect_int_equal(add(-2, -3), -5);
});

smitty_test(it_can_add_two_numbers_whose_sum_is_zero, {
    expect_int_equal(add(-42, 42), 0);
});

smitty_test(it_can_add_a_positive_and_a_negative_integer, {
    expect_int_equal(add(-2, 3), 1);
});

int main() {
    ScrumpBuffer *test_case_func_ptrs_buffer = scrump_buffer_create_default();

    smitty_register_test(it_can_add_two_positive_integers);
    smitty_register_test(it_can_add_two_negative_integers);

    smitty_run_test_suite(test_case_func_ptrs_buffer);

    return 0;
}

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
