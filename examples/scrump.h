/*
 * "Scrump" is a nickname for my bearded dragon. I think "Scrump Buffer" sounds funny, and therefore
 * I named this buffer module after him. This is a simple buffer implementation that utilizes the 
 * read/write "cursor" pattern.
 */
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    SCRUMP_SUCCESS = 0,
    SCRUMP_ATTEMPTED_WRITE_OVERFLOW = 1,
    SCRUMP_ATTEMPTED_READ_OVERFLOW = 2,
    SCRUMP_READ_BUFFER_TOO_SMALL = 3,
} ScrumpReturnCodeType;

typedef struct scrumb_buffer {
    void *data;
    size_t capacity;
    void *read_cursor;
    void *write_cursor;
} ScrumpBuffer;

//-----------------------------------------------------------------------------------------------------------------
// Scrump buffer core
//-----------------------------------------------------------------------------------------------------------------
ScrumpBuffer         *scrump_string_buffer_create(const size_t capacity);
ScrumpReturnCodeType  scrump_string_buffer_write(ScrumpBuffer *buffer, const char *data, const size_t size);
ScrumpReturnCodeType  scrump_string_buffer_read(ScrumpBuffer *buffer, char *read_buffer, const size_t size, const size_t read_buffer_size);

ScrumpReturnCodeType  scrump_buffer_free(ScrumpBuffer *buffer);

//-----------------------------------------------------------------------------------------------------------------
// Scrump buffer utilities
//-----------------------------------------------------------------------------------------------------------------

const char *scrump_return_code_to_string(const int return_code);
