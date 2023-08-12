#include "scrump.h"

//--------------------------------------------------------------------------------------------------
// Scrump buffer core
//--------------------------------------------------------------------------------------------------

ScrumpReturnCodeType scrump_buffer_free(ScrumpBuffer *buffer) {
    if (buffer != NULL) {
        free(buffer->data);
    }
    free(buffer);
    return SCRUMP_SUCCESS;
}

//--------------------------------------------------------------------------------------------------
// Scrump string buffer
//--------------------------------------------------------------------------------------------------

ScrumpBuffer *scrump_string_buffer_create(const size_t capacity) {
    if (capacity == 0) return NULL;

    ScrumpBuffer *buffer = malloc(sizeof(ScrumpBuffer));

    buffer->data = malloc(sizeof(char) * capacity);
    buffer->capacity = capacity;
    buffer->read_cursor = buffer->data;
    buffer->write_cursor = buffer->data;

    return buffer;
}

ScrumpReturnCodeType scrump_string_buffer_write(ScrumpBuffer *buffer, const char *data, const size_t size) {
    const size_t remaining_capacity = buffer->capacity - (buffer->write_cursor - buffer->data);
    if (size > remaining_capacity) return SCRUMP_ATTEMPTED_WRITE_OVERFLOW;

    memcpy(buffer->write_cursor, data, size);
    buffer->write_cursor += size;

    return SCRUMP_SUCCESS;
}

ScrumpReturnCodeType scrump_string_buffer_read(ScrumpBuffer *buffer, char *read_buffer, const size_t size, const size_t read_buffer_size) {
    const size_t read_capacity = buffer->write_cursor - buffer->read_cursor;
    if (size > read_capacity) return SCRUMP_ATTEMPTED_READ_OVERFLOW;

    // Check that the read buffer is big enough to hold the data plus a manually added null terminator.
    if (read_buffer_size < size + 1) return SCRUMP_READ_BUFFER_TOO_SMALL;

    strncpy(read_buffer, buffer->read_cursor, size);
    // manually null terminate the destination string
    read_buffer[size] = '\0';

    buffer->read_cursor += size;

    return SCRUMP_SUCCESS;
}

//--------------------------------------------------------------------------------------------------
// Scrump int buffer
//--------------------------------------------------------------------------------------------------

ScrumpBuffer *scrump_int_buffer_create(const size_t capacity) {
    if (capacity == 0) return NULL;

    ScrumpBuffer *buffer = malloc(sizeof(ScrumpBuffer));

    buffer->data = malloc(sizeof(int) * capacity);
    buffer->capacity = capacity;
    buffer->read_cursor = buffer->data;
    buffer->write_cursor = buffer->data;

    return buffer;
}

ScrumpReturnCodeType scrump_int_buffer_write(ScrumpBuffer *buffer, const int *data, const size_t size) {
    const size_t remaining_capacity = buffer->capacity - (buffer->write_cursor - buffer->data);
    if (size > remaining_capacity) return SCRUMP_ATTEMPTED_WRITE_OVERFLOW;

    memcpy(buffer->write_cursor, data, size);
    buffer->write_cursor += size;

    return SCRUMP_SUCCESS;
}

ScrumpReturnCodeType scrump_int_buffer_read(ScrumpBuffer *buffer, int *read_buffer, const size_t size) {
    const size_t read_capacity = buffer->write_cursor - buffer->read_cursor;
    if (size > read_capacity) return SCRUMP_ATTEMPTED_READ_OVERFLOW;

    memcpy(read_buffer, buffer->read_cursor, size);

    buffer->read_cursor += size;

    return SCRUMP_SUCCESS;
}

//-----------------------------------------------------------------------------------------------------------------
// Scrump buffer utilities
//-----------------------------------------------------------------------------------------------------------------

const char *scrump_return_code_to_string(const int return_code) {
    switch (return_code) {
        case SCRUMP_SUCCESS:
            return "SCRUMP_SUCCESS";
        case SCRUMP_ATTEMPTED_WRITE_OVERFLOW:
            return "SCRUMP_ATTEMPTED_WRITE_OVERFLOW";
        case SCRUMP_ATTEMPTED_READ_OVERFLOW:
            return "SCRUMP_ATTEMPTED_READ_OVERFLOW";
        case SCRUMP_READ_BUFFER_TOO_SMALL:
            return "SCRUMP_READ_BUFFER_TOO_SMALL";
        default:
            return "UNKNOWN";
    }
}
