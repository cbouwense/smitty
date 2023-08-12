#include "scrump.h"

//--------------------------------------------------------------------------------------------------
// Scrump string buffer
//--------------------------------------------------------------------------------------------------

ScrumpCharBuffer *scrump_char_buffer_create(const size_t capacity) {
    if (capacity == 0) return NULL;

    ScrumpCharBuffer *buffer = malloc(sizeof(ScrumpCharBuffer));

    buffer->data = malloc(sizeof(char) * capacity);
    buffer->capacity = capacity;
    buffer->read_cursor = buffer->data;
    buffer->write_cursor = buffer->data;

    return buffer;
}

ScrumpReturnCodeType scrump_char_buffer_free(ScrumpCharBuffer *buffer) {
    if (buffer != NULL) {
        free(buffer->data);
    }
    free(buffer);
    return SCRUMP_SUCCESS;
}

/* TODO: I think my implementation of the ScrumpCharBuffer isn't quite right. Consider:
 *
 * ScrumpCharBuffer looks like this:
 * [
 *  p1, --> ['H', 'e', 'l', 'l', 'o']
 *  p2, --> ['w', 'o', 'r', 'l', 'd']
 *  p3, --> ['!']
 * ]
 * 
 * So a read of size 1 returns ['H'], and a read of size 3 returns ['H', 'e', 'l'].
 * Whereas, a ScrumpIntBuffer looks like this:
 * [1, 7, 3, 8]
 *
 * Really, I would imagine that a ScrumpCharBuffer should look like this:
 * ['H', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd', '!']
 * 
 * So I think I want to refactor my string_buffer to basically just be a string_view. And then later
 * I can also make a pointer buffer.
 */

ScrumpReturnCodeType scrump_char_buffer_write(ScrumpCharBuffer *buffer, const char data[], const size_t size) {
    const size_t remaining_capacity = buffer->capacity - (buffer->write_cursor - buffer->data);
    if (size > remaining_capacity) return SCRUMP_ATTEMPTED_WRITE_OVERFLOW;
    
    // Intentionally not setting a null terminator. This is a buffer, not a string.
    // We null terminate on read for convenience.
    for (size_t i = 0; i < size; i++) {
        *buffer->write_cursor = data[i];
        buffer->write_cursor++;
    }

    return SCRUMP_SUCCESS;
}

ScrumpReturnCodeType scrump_char_buffer_read(ScrumpCharBuffer *buffer, char *read_buffer, const size_t size, const size_t read_buffer_size) {
    const size_t read_capacity = buffer->write_cursor - buffer->read_cursor;
    if (size > read_capacity) return SCRUMP_ATTEMPTED_READ_OVERFLOW;

    // Check that the read buffer is big enough to hold the data plus a manually added null terminator.
    if (read_buffer_size < size + 1) return SCRUMP_READ_BUFFER_TOO_SMALL;

    strncpy(read_buffer, buffer->read_cursor, size);
    // Manually null terminate the destination string
    read_buffer[size] = '\0';

    buffer->read_cursor += size;

    return SCRUMP_SUCCESS;
}

// TODO: the read and write cursor is pretty half baked, but it's pretty good. Maybe consider making
// this better, or more robust.
void scrump_char_buffer_debug(const ScrumpCharBuffer *buffer, const char *buffer_name) {
    const size_t size = buffer->write_cursor - buffer->data;
    const char *data = buffer->data;
    const size_t read_cursor_offset = buffer->read_cursor - buffer->data;

    printf("========== %s ==========\n", buffer_name);
    printf("| size: %zu | capacity: %zu\n", size, buffer->capacity);
    printf("--------------------------------\n");
    for(int i = 0; i < read_cursor_offset + 1; i++) printf(" ");
    printf("read |\n");
    for(int i = 0; i < read_cursor_offset + 1; i++) printf(" ");
    printf("     v\n");
    printf("data=[");
    for (size_t i = 0; i < size; i++) {
        printf("'%c'", data[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
    
    const size_t write_cursor_offset = buffer->write_cursor - buffer->data;

    
    // For example, if the first character is 'e', it will look like `'e', `, thus 5 spaces.
    const int spaces_per_character = 5;
    const int spaces_between_read_and_write = ((write_cursor_offset - read_cursor_offset) * spaces_per_character) - 2;
    for(int i = 0; i < spaces_between_read_and_write; i++) printf(" ");
    printf("      ^\n");
    for(int i = 0; i < spaces_between_read_and_write; i++) printf(" ");
    printf("write |\n\n");
}

//--------------------------------------------------------------------------------------------------
// Scrump int buffer
//--------------------------------------------------------------------------------------------------

ScrumpIntBuffer *scrump_int_buffer_create(const size_t capacity) {
    if (capacity == 0) return NULL;

    ScrumpIntBuffer *buffer = malloc(sizeof(ScrumpIntBuffer));

    buffer->data = malloc(sizeof(int) * capacity);
    buffer->capacity = capacity;
    buffer->read_cursor = buffer->data;
    buffer->write_cursor = buffer->data;

    return buffer;
}

ScrumpReturnCodeType scrump_int_buffer_free(ScrumpIntBuffer *buffer) {
    if (buffer != NULL) {
        free(buffer->data);
    }
    free(buffer);
    return SCRUMP_SUCCESS;
}

ScrumpReturnCodeType scrump_int_buffer_write(ScrumpIntBuffer *buffer, const int data[], const size_t size) {
    const size_t remaining_capacity = buffer->capacity - (buffer->write_cursor - buffer->data);
    if (size > remaining_capacity) return SCRUMP_ATTEMPTED_WRITE_OVERFLOW;

    for (size_t i = 0; i < size; i++) {
        *buffer->write_cursor = data[i];
        buffer->write_cursor++;
    }

    return SCRUMP_SUCCESS;
}

ScrumpReturnCodeType scrump_int_buffer_read(ScrumpIntBuffer *buffer, int *read_buffer, const size_t size) {
    const size_t read_capacity = buffer->write_cursor - buffer->read_cursor;
    if (size > read_capacity) return SCRUMP_ATTEMPTED_READ_OVERFLOW;

    for (size_t i = 0; i < size; i++) {
        read_buffer[i] = *buffer->read_cursor;
        buffer->read_cursor++;
    }

    return SCRUMP_SUCCESS;
}

void scrump_int_buffer_debug(const ScrumpIntBuffer *buffer, const char *buffer_name) {
    const size_t size = buffer->write_cursor - buffer->data;
    const int *data = buffer->data;

    printf("%s: size=%zu, capacity=%zu, data=[", buffer_name, size, buffer->capacity);
    for (size_t i = 0; i < size; i++) {
        printf("%d", data[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
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
