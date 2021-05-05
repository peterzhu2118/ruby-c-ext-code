#include <ruby.h>

struct circular_iterator {
    VALUE array;
    VALUE *array_ptr;
    long array_len;
    long index;
};

static void circular_iterator_mark(void *ptr)
{
    struct circular_iterator *circular_iterator = ptr;

    rb_gc_mark(circular_iterator->array);
}

static void circular_iterator_free(void *ptr)
{
    xfree(ptr);
}

const rb_data_type_t circular_iterator_data_type = {
    "circular_iterator",
    { circular_iterator_mark, circular_iterator_free, },
    NULL, NULL, RUBY_TYPED_FREE_IMMEDIATELY
};

static VALUE circular_iterator_allocate(VALUE klass)
{
    struct circular_iterator *circular_iterator;
    VALUE obj = TypedData_Make_Struct(klass, struct circular_iterator, &circular_iterator_data_type, circular_iterator);
    return obj;
}

static VALUE circular_iterator_initialize(VALUE self, VALUE array)
{
    struct circular_iterator *circular_iterator;
    TypedData_Get_Struct(self, struct circular_iterator, &circular_iterator_data_type, circular_iterator);

    circular_iterator->array = array;
    circular_iterator->array_ptr = RARRAY_PTR(array);
    circular_iterator->array_len = RARRAY_LEN(array);
    circular_iterator->index = 0;

    return Qnil;
}

static VALUE circular_iterator_next(VALUE self)
{
    struct circular_iterator *circular_iterator;
    TypedData_Get_Struct(self, struct circular_iterator, &circular_iterator_data_type, circular_iterator);

    // Get pointer to memory region of this array
    VALUE *array = circular_iterator->array_ptr;
    // element = @array[@index]
    VALUE element = array[circular_iterator->index];

    // @index = (@index + 1) % @array.length
    circular_iterator->index = (circular_iterator->index + 1) % circular_iterator->array_len;

    return element;
}

void Init_circular_iterator(void)
{
    // class CircularIteratorCOpt < Object
    VALUE cCircularIteratorCOpt = rb_define_class("CircularIteratorCOpt", rb_cObject);
    rb_define_alloc_func(cCircularIteratorCOpt, circular_iterator_allocate);

    rb_define_method(cCircularIteratorCOpt, "initialize", circular_iterator_initialize, 1);
    rb_define_method(cCircularIteratorCOpt, "next", circular_iterator_next, 0);
}
