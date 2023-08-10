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
    if (size > remaining_capacity) return ATTEMPTED_WRITE_OVERFLOW;

    memcpy(buffer->write_cursor, data, size);
    buffer->write_cursor += size;

    return SUCCESS;
}

ReturnCode buffer_read(ScrumpBuffer *buffer, void *read_buffer, size_t size) {
    const size_t read_capacity = buffer->write_cursor - buffer->read_cursor;
    if (size > read_capacity) return ATTEMPTED_READ_OVERFLOW;

    memcpy(read_buffer, buffer->read_cursor, size);
    buffer->read_cursor += size;

    return SUCCESS;
}
