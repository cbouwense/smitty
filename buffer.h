#pragma once

#include <stdlib.h>

#define BUFFER_DEFAULT_CAPACITY 1024
// TODO: this should probably be a size_t?
#define Bytes int

// TODO: should these pointers be a char? or a void? or something else?
typedef struct buffer {
    char *data;

    /**
     * The size of each data element, in bytes. For example, a buffer of
     * characters would have a data_size of 1, while a buffer of integers would
     * have a data_size of 4.
     */
    Bytes data_size;
    
    Bytes capacity;
    
    char *read_cursor;
    
    char *write_cursor;
} Buffer;

Buffer *buffer_new_default();

Buffer *buffer_new(int capacity);

void buffer_free(Buffer *buffer);

void buffer_clear(Buffer *buffer);

void buffer_write(Buffer *buffer, char *data, int size);

void buffer_read(Buffer *buffer, int size);
