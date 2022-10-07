#include <ruby.h>

struct circular_buffer {
    long capacity;
    long length;
    long read_cursor;
    long write_cursor;
    VALUE *buffer;
};

static long next_cursor_position(struct circular_buffer *circular_buffer, long cursor)
{
    if (cursor == circular_buffer->capacity - 1) {
        return 0;
    } else {
        return cursor + 1;
    }
}

static void circular_buffer_mark(void *ptr)
{
    struct circular_buffer *circular_buffer = ptr;

    long cursor = circular_buffer->read_cursor;
    // Mark all the objects from the read_cursor up to the write_cursor
    for (long i = 0; i < circular_buffer->length; i++) {
        rb_gc_mark_movable(circular_buffer->buffer[cursor]);

        cursor = next_cursor_position(circular_buffer, cursor);
    }
}

static void circular_buffer_free(void *ptr)
{
    struct circular_buffer *circular_buffer = ptr;
    // Free the buffer
    free(circular_buffer->buffer);
    // Don't forget to free the struct itself using xfree (free function in Ruby)
    xfree(circular_buffer);
}

static size_t circular_buffer_memsize(const void *ptr)
{
    const struct circular_buffer *circular_buffer = ptr;

    return sizeof(struct circular_buffer) + (sizeof(VALUE) * circular_buffer->capacity);
}

static void circular_buffer_compact(void *ptr)
{
    struct circular_buffer *circular_buffer = ptr;

    long cursor = circular_buffer->read_cursor;
    // Mark all the objects from the read_cursor up to the write_cursor
    for (long i = 0; i < circular_buffer->length; i++) {
        circular_buffer->buffer[cursor] = rb_gc_location(circular_buffer->buffer[cursor]);

        cursor = next_cursor_position(circular_buffer, cursor);
    }
}

const rb_data_type_t circular_buffer_data_type = {
    .wrap_struct_name = "circular_buffer",
    .function = {
        .dmark = circular_buffer_mark,
        .dfree = circular_buffer_free,
        .dsize = circular_buffer_memsize,
        .dcompact = circular_buffer_compact,
    },
};

static VALUE circular_buffer_allocate(VALUE klass)
{
    struct circular_buffer *circular_buffer;
    VALUE obj = TypedData_Make_Struct(klass, struct circular_buffer,
                                      &circular_buffer_data_type, circular_buffer);
    return obj;
}

static VALUE circular_buffer_initialize(VALUE self, VALUE capacity_value)
{
    struct circular_buffer *circular_buffer;
    TypedData_Get_Struct(self, struct circular_buffer,
                         &circular_buffer_data_type, circular_buffer);

    // Convert capacity_value from fixnum to C long
    long capacity = FIX2LONG(capacity_value);

    circular_buffer->capacity = capacity;
    // Allocate memory for the buffer
    circular_buffer->buffer = malloc(sizeof(VALUE) * capacity);

    return Qnil;
}

static VALUE circular_buffer_write(VALUE self, VALUE obj)
{
    struct circular_buffer *circular_buffer;
    TypedData_Get_Struct(self, struct circular_buffer,
                         &circular_buffer_data_type, circular_buffer);

    // Check if buffer is full
    if (circular_buffer->length == circular_buffer->capacity) {
        rb_raise(rb_eRuntimeError, "Circular buffer is full");
    }

    circular_buffer->buffer[circular_buffer->write_cursor] = obj;

    // Advance write_cursor
    circular_buffer->write_cursor = next_cursor_position(circular_buffer, circular_buffer->write_cursor);
    circular_buffer->length++;

    return obj;
}

static VALUE circular_buffer_read(VALUE self)
{
    struct circular_buffer *circular_buffer;
    TypedData_Get_Struct(self, struct circular_buffer,
                         &circular_buffer_data_type, circular_buffer);

    // Check if buffer is empty
    if (circular_buffer->length == 0) {
        rb_raise(rb_eRuntimeError, "Circular buffer is empty");
    }

    VALUE obj = circular_buffer->buffer[circular_buffer->read_cursor];

    // Advance read_cursor
    circular_buffer->read_cursor = next_cursor_position(circular_buffer, circular_buffer->read_cursor);
    circular_buffer->length--;

    return obj;
}

void Init_circular_buffer_typeddata(void)
{
    // class CircularBufferTypedData < Object
    VALUE cCircularBufferTypedData = rb_define_class("CircularBufferTypedData", rb_cObject);
    rb_define_alloc_func(cCircularBufferTypedData, circular_buffer_allocate);

    rb_define_method(cCircularBufferTypedData, "initialize", circular_buffer_initialize, 1);
    rb_define_method(cCircularBufferTypedData, "write", circular_buffer_write, 1);
    rb_define_method(cCircularBufferTypedData, "read", circular_buffer_read, 0);
}
