#pragma once

#include <stdlib.h>
#include <string.h>

#define BUFFER_DEFAULT_CAPACITY 1024

typedef enum {
    SUCCESS = 0,
    FAILURE = 1,
    ATTEMPTED_OVERFLOW = 2
} ReturnCode;

// TODO: should these pointers reall be a char? or a void? or something else?
typedef struct buffer {
    char *data;
    
    size_t capacity;
    
    char *read_cursor;
    
    char *write_cursor;
} Buffer;

Buffer *buffer_new_default();

Buffer *buffer_new(size_t capacity);

ReturnCode buffer_clear(Buffer *buffer);

ReturnCode buffer_write(Buffer *buffer, char *data);

ReturnCode buffer_read(Buffer *buffer, size_t size);
