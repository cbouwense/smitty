#pragma once

#include <stdlib.h>

#define BUFFER_DEFAULT_CAPACITY 1024

typedef enum {
    SUCCESS = 0,
    FAILURE = 1
} ReturnCode;

// TODO: should these pointers reall be a char? or a void? or something else?
typedef struct buffer {
    char *data;

    /**
     * The size of each data element, in bytes. For example, a buffer of
     * characters would have a data_element_size of 1, while a buffer of integers would
     * have a data_element_size of 4.
     */
    size_t data_element_size;
    
    size_t capacity;
    
    char *read_cursor;
    
    char *write_cursor;
} Buffer;

Buffer *buffer_new_default();

Buffer *buffer_new(size_t capacity);

ReturnCode buffer_clear(Buffer *buffer);

ReturnCode buffer_write(Buffer *buffer, char *data, size_t size);

ReturnCode buffer_read(Buffer *buffer, size_t size);
