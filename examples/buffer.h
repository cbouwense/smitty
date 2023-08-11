#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_DEFAULT_CAPACITY 1024

typedef enum {
    SCRUMP_SUCCESS = 0,
    SCRUMP_ATTEMPTED_WRITE_OVERFLOW = 1,
    SCRUMP_ATTEMPTED_READ_OVERFLOW = 2,
} ReturnCode;

typedef struct scrumb_buffer {
    void *data;
    
    size_t capacity;
    
    void *read_cursor;
    
    void *write_cursor;
} ScrumpBuffer;

ScrumpBuffer *scrump_buffer_create_default();

ScrumpBuffer *scrump_buffer_create(size_t capacity);

ReturnCode scrump_buffer_write(ScrumpBuffer *buffer, void *data, size_t size);

ReturnCode scrump_buffer_read(ScrumpBuffer *buffer, void *read_buffer, size_t size);
