#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/smitty.h"

#define BUFFER_DEFAULT_CAPACITY 1024

typedef enum {
    SCRUMP_SUCCESS = 0,
    SCRUMP_ATTEMPTED_WRITE_OVERFLOW = 1,
    SCRUMP_ATTEMPTED_READ_OVERFLOW = 2,
} ScrumpReturnCode;

typedef struct scrumb_buffer {
    void *data;
    
    size_t capacity;
    
    void *read_cursor;
    
    void *write_cursor;
} ScrumpBuffer;

ScrumpBuffer *scrump_buffer_create_default();

ScrumpBuffer *scrump_buffer_create(size_t capacity);

ScrumpReturnCode scrump_buffer_write(ScrumpBuffer *buffer, void *data, size_t size);

// ScrumpReturnCode scrump_buffer_write_func_ptr(ScrumpBuffer *buffer, void *data);
ScrumpReturnCode scrump_buffer_write_func_ptr(ScrumpBuffer *buffer, smitty_test_result (*data)());

// ScrumpReturnCode scrump_buffer_read(ScrumpBuffer *buffer, void *read_buffer, size_t size);
smitty_test_result (*scrump_buffer_read_func_ptr(ScrumpBuffer *buffer, size_t size))();

const char *scrump_return_code_to_string(ScrumpReturnCode code);

void scrump_buffer_debug_print(ScrumpBuffer *buffer);
