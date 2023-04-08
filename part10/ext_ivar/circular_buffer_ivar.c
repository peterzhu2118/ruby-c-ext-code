#include <ruby.h>

static ID ivar_buffer, ivar_length, ivar_read_cursor, ivar_write_cursor;

static VALUE circular_buffer_initialize(VALUE self, VALUE capacity_value)
{
    // Convert capacity_value from fixnum to C long
    long capacity = FIX2LONG(capacity_value);

    VALUE buffer = rb_ary_new_capa(capacity);
    rb_ary_resize(buffer, capacity);

    rb_ivar_set(self, ivar_buffer, buffer);
    rb_ivar_set(self, ivar_length, LONG2FIX(0));
    rb_ivar_set(self, ivar_read_cursor, LONG2FIX(0));
    rb_ivar_set(self, ivar_write_cursor, LONG2FIX(0));

    return Qnil;
}

static long next_cursor_position(long capacity, long cursor)
{
    return (cursor + 1) % capacity;
}

static VALUE circular_buffer_write(VALUE self, VALUE obj)
{
    VALUE buffer = rb_ivar_get(self, ivar_buffer);
    long capacity = RARRAY_LEN(buffer);
    long length = FIX2LONG(rb_ivar_get(self, ivar_length));
    long write_cursor = FIX2LONG(rb_ivar_get(self, ivar_write_cursor));

    // Check if buffer is full
    if (length == capacity) {
        rb_raise(rb_eRuntimeError, "Circular buffer is full");
    }

    // Write obj to buffer
    RARRAY_ASET(buffer, write_cursor, obj);

    long next_write_cursor = next_cursor_position(capacity, write_cursor);
    // Set @write_cursor
    rb_ivar_set(self, ivar_write_cursor, LONG2FIX(next_write_cursor));

    // Increment @length
    rb_ivar_set(self, ivar_length, LONG2FIX(length + 1));

    return obj;
}

static VALUE circular_buffer_read(VALUE self)
{
    VALUE buffer = rb_ivar_get(self, ivar_buffer);
    long capacity = RARRAY_LEN(buffer);
    long length = FIX2LONG(rb_ivar_get(self, ivar_length));
    long read_cursor = FIX2LONG(rb_ivar_get(self, ivar_read_cursor));

    // Check if buffer is empty
    if (length == 0) {
        rb_raise(rb_eRuntimeError, "Circular buffer is empty");
    }

    VALUE obj = RARRAY_AREF(buffer, read_cursor);

    long next_read_cursor = next_cursor_position(capacity, read_cursor);
    // Set @read_cursor
    rb_ivar_set(self, ivar_read_cursor, LONG2FIX(next_read_cursor));

    // Decrement @length
    rb_ivar_set(self, ivar_length, LONG2FIX(length - 1));

    return obj;
}

void Init_circular_buffer_ivar(void)
{
    ivar_buffer = rb_intern("@buffer");
    ivar_length = rb_intern("@length");
    ivar_read_cursor = rb_intern("@read_cursor");
    ivar_write_cursor = rb_intern("@write_cursor");

    // class CircularBufferIvar < Object
    VALUE cCircularBufferIvar = rb_define_class("CircularBufferIvar", rb_cObject);

    rb_define_method(cCircularBufferIvar, "initialize", circular_buffer_initialize, 1);
    rb_define_method(cCircularBufferIvar, "write", circular_buffer_write, 1);
    rb_define_method(cCircularBufferIvar, "read", circular_buffer_read, 0);
}
