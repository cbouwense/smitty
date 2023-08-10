#include "buffer.h"

Buffer *buffer_new_default() {
    return buffer_new(BUFFER_DEFAULT_CAPACITY);
}

Buffer *buffer_new(size_t capacity) {
    if (capacity == 0) return NULL;

    Buffer *buffer = malloc(sizeof(Buffer));

    // buffer->data = malloc(capacity);
    buffer->data = NULL;
    buffer->capacity = capacity;
    buffer->read_cursor = buffer->data;
    buffer->write_cursor = buffer->data;

    return buffer;
}

ReturnCode buffer_write(Buffer *buffer, char *data) {
    const size_t data_size = strlen(data);
    const size_t remaining_capacity = buffer->capacity - (buffer->write_cursor - buffer->data);
    if (data_size > remaining_capacity) return ATTEMPTED_OVERFLOW;

    buffer->write_cursor = memcpy(buffer->write_cursor, data, data_size);

    return SUCCESS;
}