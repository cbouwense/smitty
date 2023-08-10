#include "buffer.h"

ScrumpBuffer *buffer_create_default() {
    return buffer_create(BUFFER_DEFAULT_CAPACITY);
}

ScrumpBuffer *buffer_create(size_t capacity) {
    if (capacity == 0) return NULL;

    ScrumpBuffer *buffer = malloc(sizeof(ScrumpBuffer));

    buffer->data = malloc(capacity);
    buffer->capacity = capacity;
    buffer->read_cursor = buffer->data;
    buffer->write_cursor = buffer->data;

    return buffer;
}

ReturnCode buffer_write(ScrumpBuffer *buffer, void *data, size_t size) {
    const size_t remaining_capacity = buffer->capacity - (buffer->write_cursor - buffer->data);
    if (size > remaining_capacity) return ATTEMPTED_OVERFLOW;

    buffer->write_cursor = memcpy(buffer->write_cursor, data, size);

    return SUCCESS;
}
