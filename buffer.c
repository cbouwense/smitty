#include "buffer.h"

Buffer *buffer_new_default() {
    Buffer *buffer = malloc(sizeof(Buffer));

    buffer->data = malloc(BUFFER_DEFAULT_CAPACITY);
    buffer->data_size = 1;
    buffer->capacity = BUFFER_DEFAULT_CAPACITY;
    buffer->read_cursor = buffer->data;
    buffer->write_cursor = buffer->data;

    return buffer;
}

