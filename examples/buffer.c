#include "buffer.h"

Buffer *buffer_new_default() {
    return buffer_new(BUFFER_DEFAULT_CAPACITY);
}

Buffer *buffer_new(size_t capacity) {
    Buffer *buffer = malloc(sizeof(Buffer));

    buffer->data = malloc(capacity);
    buffer->data_element_size = 1;
    buffer->capacity = capacity;
    buffer->read_cursor = buffer->data;
    buffer->write_cursor = buffer->data;

    return buffer;
}