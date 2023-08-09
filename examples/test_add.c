#include "../src/smitty.h"
#include "add.h"

void before_each() {
    // printf("This runs before each test asdfa\n");
}

void after_each() {
    // printf("This runs after each test\n");
}

smitty_test_result it_can_run_expect_equal() {
    expect_int_equal(1, 2);
}

void internal_smitty_test(
    char                *test_description,
    smitty_test_result (*test_body)(),
    char                *file,
    int                  line
) {
    smitty_test_result result = test_body();
    printf("%s: %s\n", test_description, smitty_test_result_to_string(result));
}

int main() {
    smitty_test(it_can_run_expect_equal);

    return 0;
}

// smitty_test(it_can_add_two_positive_integers, {
//     expect(add(2, 3) == 5);
// })

// smitty_test(it_can_add_two_negative_integers, {
//     expect(add(-2, -3) == -5);
// })

// smitty_test(it_can_add_two_numbers_whose_sum_is_zero, {
//     expect(add(-42, 42) == 0);
// })

// smitty_test(it_can_add_a_positive_and_a_negative_integer, {
//     expect(add(-2, 3) == 1);
// })

//--------------------------------------------------------------------------------------------------
// Smitty boilerplate
//--------------------------------------------------------------------------------------------------

// smitty_register_tests(
//     smitty_test_as_name_and_callback(it_can_run_expect_equal),
//     // smitty_test_as_name_and_callback(it_can_add_two_positive_integers),
//     // smitty_test_as_name_and_callback(it_can_add_two_negative_integers),
//     // smitty_test_as_name_and_callback(it_can_add_two_numbers_whose_sum_is_zero),
//     // smitty_test_as_name_and_callback(it_can_add_a_positive_and_a_negative_integer),
// )

// // TODO: It might be nice to have some sort of options struct.
// smitty_run_test_suite_with_before_and_after_each(before_each, after_each)
