#include "add.h"
#include "smitty.h"

smitty_test(it_can_add_two_positive_integers, {
    expect(add(2, 3) == 5);
})

smitty_test(it_can_add_two_negative_integers, {
    expect(add(-2, -3) == -5);
})

smitty_test(it_can_add_two_numbers_whose_sum_is_zero, {
    expect(add(-42, 42) == 0);
})

smitty_test(it_can_add_a_positive_and_a_negative_integer, {
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

void before_each() {
    printf("This runs before each test\n");
}

run_smitty_suite_with_before_each(before_each)
