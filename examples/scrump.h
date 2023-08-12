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

typedef struct scrump_char_buffer {
    char   *data;
    size_t  capacity;
    char   *read_cursor;
    char   *write_cursor;
} ScrumpCharBuffer;

typedef struct scrump_int_buffer {
    int    *data;
    size_t  capacity;
    int    *read_cursor;
    int    *write_cursor;
} ScrumpIntBuffer;

//-----------------------------------------------------------------------------------------------------------------
// Scrump string buffer
//-----------------------------------------------------------------------------------------------------------------

ScrumpCharBuffer     *scrump_char_buffer_create(const size_t capacity);
ScrumpReturnCodeType  scrump_char_buffer_free(ScrumpCharBuffer *buffer);
ScrumpReturnCodeType  scrump_char_buffer_write(ScrumpCharBuffer *buffer, const char data[], const size_t size);
ScrumpReturnCodeType  scrump_char_buffer_read(ScrumpCharBuffer *buffer, char *read_buffer, const size_t size, const size_t read_buffer_size);
void                  scrump_char_buffer_debug(const ScrumpCharBuffer *buffer, const char *buffer_name);

//-----------------------------------------------------------------------------------------------------------------
// Scrump int buffer
//-----------------------------------------------------------------------------------------------------------------

ScrumpIntBuffer      *scrump_int_buffer_create(const size_t capacity);
ScrumpReturnCodeType  scrump_int_buffer_free(ScrumpIntBuffer *buffer);
ScrumpReturnCodeType  scrump_int_buffer_write(ScrumpIntBuffer *buffer, const int data[], const size_t size);
ScrumpReturnCodeType  scrump_int_buffer_read(ScrumpIntBuffer *buffer, int *read_buffer, const size_t size);
void                  scrump_int_buffer_debug(const ScrumpIntBuffer *buffer, const char *buffer_name);

//-----------------------------------------------------------------------------------------------------------------
// Scrump buffer utilities
//-----------------------------------------------------------------------------------------------------------------

const char *scrump_return_code_to_string(const int return_code);
