#pragma once

#include <stdio.h>

#define guppy_print_int_array(array) guppy_print_int_array_internal(array, sizeof(array))

void guppy_print_int_array_internal(int array[], size_t length);
