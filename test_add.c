#include "add.h"
#include "smitty.h"

before_each({
    printf("before_each\n");
})

test_case(it_can_add_two_positive_integers, {
    expect(add(2, 3) == 5);
})

test_case(it_can_add_two_negative_integers, {
    expect(add(-2, -3) == -5);
})

test_case(it_can_add_two_numbers_whose_sum_is_zero, {
    expect(add(-42, 42) == 0);
})

test_case(it_can_add_a_positive_and_a_negative_integer, {
    expect(add(-2, 3) == 1);
})

//--------------------------------------------------------------------------------------------------
// Smitty boilerplate
//--------------------------------------------------------------------------------------------------

test_case_info tests[] = {
    register_test(it_can_add_two_positive_integers),
    register_test(it_can_add_two_negative_integers),
    register_test(it_can_add_two_numbers_whose_sum_is_zero),
    register_test(it_can_add_a_positive_and_a_negative_integer),
    {NULL, NULL}
};

run_smitty_suite()
