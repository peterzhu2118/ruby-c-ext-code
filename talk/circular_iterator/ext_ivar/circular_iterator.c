#include <ruby.h>

static VALUE circular_iterator_initialize(VALUE self, VALUE array)
{
    // @array = array
    rb_iv_set(self, "@array", array);
    // @index = 0
    rb_iv_set(self, "@index", LONG2FIX(0));

    return Qnil;
}

static VALUE circular_iterator_next(VALUE self)
{
    // @array
    VALUE array = rb_iv_get(self, "@array");

    // Get pointer to memory region of this array
    VALUE *array_ptr = RARRAY_PTR(array);
    // Convert @index to a C long
    long index = FIX2LONG(rb_iv_get(self, "@index"));
    // element = @array[@index]
    VALUE element = array_ptr[index];

    // @index = (@index + 1) % @array.length
    index = (index + 1) % RARRAY_LEN(array);
    rb_iv_set(self, "@index", LONG2FIX(index));

    return element;
}

void Init_circular_iterator(void)
{
    // class CircularIteratorCIvar < Object
    VALUE cCircularIteratorCIvar = rb_define_class("CircularIteratorCIvar", rb_cObject);

    rb_define_method(cCircularIteratorCIvar, "initialize", circular_iterator_initialize, 1);
    rb_define_method(cCircularIteratorCIvar, "next", circular_iterator_next, 0);
}
