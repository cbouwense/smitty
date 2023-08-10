#include "scrump.h"


ScrumpBuffer *scrump_buffer_create_default() {
    return scrump_buffer_create(BUFFER_DEFAULT_CAPACITY);
}

ScrumpBuffer *scrump_buffer_create(size_t capacity) {
    if (capacity == 0) return NULL;

    ScrumpBuffer *buffer = malloc(sizeof(ScrumpBuffer));

    buffer->data = malloc(capacity);
    buffer->capacity = capacity;
    buffer->read_cursor = buffer->data;
    buffer->write_cursor = buffer->data;

    return buffer;
}

// TODO: we probably need to not do this? The differnce between copying pointers and
// non pointers is confusing me...
ScrumpReturnCode scrump_buffer_write(ScrumpBuffer *buffer, void *data, size_t size) {
    const size_t remaining_capacity = buffer->capacity - (buffer->write_cursor - buffer->data);
    if (size > remaining_capacity) return SCRUMP_ATTEMPTED_WRITE_OVERFLOW;

    printf("data:\t\t%p\n", data);
    smitty_test_result (*data_ptr)() = data;

    printf("sizeof(data):\t%zu\n", sizeof(data));
    printf("size:\t\t%zu\n\n", size);
    memcpy(&buffer->read_cursor, &data, size);
    
    printf("buffer->data:\t\t%p\n", (smitty_test_result (**)())buffer->data);
    printf("buffer->write_cursor:\t%p\n", (smitty_test_result (**)())buffer->write_cursor);
    printf("buffer->read_cursor:\t%p\n", (smitty_test_result (**)())buffer->read_cursor);

    buffer->write_cursor += size;

    return SCRUMP_SUCCESS;
}

ScrumpReturnCode scrump_buffer_write_ptr(ScrumpBuffer *buffer, smitty_test_result (*data)()) {
    const size_t remaining_capacity = buffer->capacity - (buffer->write_cursor - buffer->data);
    if (sizeof(data) > remaining_capacity) return SCRUMP_ATTEMPTED_WRITE_OVERFLOW;

    printf("actual function pointer:\t%p\n\n", data);

    printf("read_cursor:\t%p\n", buffer->read_cursor);
    printf("write_cursor:\t%p\n", buffer->write_cursor);
    printf("&read_cursor:\t%p\n", &buffer->read_cursor);
    printf("&write_cursor:\t%p\n\n", &buffer->write_cursor);

    memcpy(buffer->write_cursor, &data, sizeof(data));
    buffer->write_cursor += sizeof(data);

    printf("&read_cursor:\t%p\n", &buffer->read_cursor);
    printf("read_cursor:\t%p\n", buffer->read_cursor);
    printf("*read_cursor:\t%p\n", *((smitty_test_result (*)())buffer->read_cursor));
    printf("**read_cursor:\t%p\n\n", **((smitty_test_result (**)())buffer->read_cursor));

    return SCRUMP_SUCCESS;
}

ScrumpReturnCode scrump_buffer_read(ScrumpBuffer *buffer, smitty_test_result (*read_buffer)(), size_t size) {
    const size_t read_capacity = buffer->write_cursor - buffer->read_cursor;
    if (size > read_capacity) return SCRUMP_ATTEMPTED_READ_OVERFLOW;

    printf("$ read_buffer:\t%p\n", read_buffer);
    printf("read_cursor:\t%p\n", buffer->read_cursor);
    printf("write_cursor:\t%p\n", buffer->write_cursor);
    printf("&read_cursor:\t%p\n", &buffer->read_cursor);
    printf("&write_cursor:\t%p\n\n", &buffer->write_cursor);

    memcpy(&read_buffer, buffer->read_cursor, size);
    buffer->read_cursor += size;

    printf("$ read_buffer:\t%p\n", read_buffer);
    read_buffer();
    printf("&read_cursor:\t%p\n", &buffer->read_cursor);
    printf("read_cursor:\t%p\n", buffer->read_cursor);
    printf("**read_cursor:\t%p\n\n", **((smitty_test_result (**)())buffer->read_cursor));

    return SCRUMP_SUCCESS;
}

const char *scrump_return_code_to_string(ScrumpReturnCode code) {
    switch (code) {
        case SCRUMP_SUCCESS:
            return "SCRUMP_SUCCESS";
        case SCRUMP_ATTEMPTED_WRITE_OVERFLOW:
            return "SCRUMP_ATTEMPTED_WRITE_OVERFLOW";
        case SCRUMP_ATTEMPTED_READ_OVERFLOW:
            return "SCRUMP_ATTEMPTED_READ_OVERFLOW";
        default:
            return "UNKNOWN";
    }
}

void scrump_buffer_debug_print(ScrumpBuffer *buffer) {
    printf("ScrumpBuffer { (%p)\n", buffer);
    printf("    data: %p\n", buffer->data);
    printf("    capacity: %zu\n", buffer->capacity);
    printf("    read_cursor: %p\n", buffer->read_cursor);
    printf("    write_cursor: %p\n", buffer->write_cursor);
    printf("}\n");
}
