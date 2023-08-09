#include "../src/smitty.h"
#include "add.h"

smitty_test_result it_can_add_two_positive_integers() {
    expect_equal(add(2, 3), 5);
}

smitty_test_result it_can_add_two_negative_integers() {
    expect_equal(add(-2, -3), -5);
}

smitty_test_result it_can_add_two_numbers_whose_sum_is_zero() {
    expect_equal(add(-42, 42), 0);
}

smitty_test_result it_can_add_a_positive_and_a_negative_integer() {
    expect_equal(add(-2, 3), 1);
}

//--------------------------------------------------------------------------------------------------
// Smitty boilerplate
//--------------------------------------------------------------------------------------------------

smitty_register_tests(
    smitty_test_as_name_and_callback(it_can_add_two_positive_integers),
    smitty_test_as_name_and_callback(it_can_add_two_negative_integers),
    smitty_test_as_name_and_callback(it_can_add_two_numbers_whose_sum_is_zero),
    smitty_test_as_name_and_callback(it_can_add_a_positive_and_a_negative_integer),
)

smitty_run_test_suite()
