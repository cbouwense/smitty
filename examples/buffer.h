#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_DEFAULT_CAPACITY 1024

typedef enum {
    SUCCESS = 0,
    FAILURE = 1,
    ATTEMPTED_WRITE_OVERFLOW = 2,
    ATTEMPTED_READ_OVERFLOW = 3,
} ReturnCode;

typedef struct buffer {
    void *data;
    
    size_t capacity;
    
    void *read_cursor;
    
    void *write_cursor;
} ScrumpBuffer;

ScrumpBuffer *buffer_create_default();

ScrumpBuffer *buffer_create(size_t capacity);

ReturnCode buffer_clear(ScrumpBuffer *buffer);

ReturnCode buffer_write(ScrumpBuffer *buffer, void *data, size_t size);

ReturnCode buffer_read(ScrumpBuffer *buffer, void *read_buffer, size_t size);
