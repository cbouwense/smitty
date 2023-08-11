/*
 * "Scrump" is a nickname for my bearded dragon. I think "Scrump Buffer" sounds funny, and therefore
 * I named this buffer implementation after him. This is a simple buffer implementation that 
 * utilizes the read/write "cursor" pattern.
 */
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    SCRUMP_SUCCESS = 0,
    SCRUMP_ATTEMPTED_WRITE_OVERFLOW = 1,
    SCRUMP_ATTEMPTED_READ_OVERFLOW = 2,
} ScrumpReturnCodeType;

typedef struct scrumb_buffer {
    void *data;
    
    size_t capacity;
    
    void *read_cursor;
    
    void *write_cursor;
} ScrumpBuffer;

ScrumpBuffer *scrump_buffer_create(size_t capacity);

ScrumpReturnCodeType scrump_buffer_write(ScrumpBuffer *buffer, void *data, size_t size);

ScrumpReturnCodeType scrump_buffer_read(ScrumpBuffer *buffer, void *read_buffer, size_t size);

ScrumpReturnCodeType scrump_buffer_free(ScrumpBuffer *buffer);
