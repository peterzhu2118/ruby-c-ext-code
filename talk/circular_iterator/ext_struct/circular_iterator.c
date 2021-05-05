#include <ruby.h>

struct circular_iterator {
    VALUE array;
    long index;
};

static void circular_iterator_mark(void *ptr)
{
    struct circular_iterator *circular_iterator = ptr;

    rb_gc_mark(circular_iterator->array);
}

const rb_data_type_t circular_iterator_data_type = {
    "circular_iterator",
    { circular_iterator_mark, RUBY_DEFAULT_FREE, },
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
    circular_iterator->index = 0;

    return Qnil;
}

static VALUE circular_iterator_next(VALUE self)
{
    struct circular_iterator *circular_iterator;
    TypedData_Get_Struct(self, struct circular_iterator, &circular_iterator_data_type, circular_iterator);

    // @array
    VALUE array = circular_iterator->array;

    // Get pointer to memory region of this array
    VALUE *array_ptr = RARRAY_PTR(array);
    // element = @array[@index]
    VALUE element = array_ptr[circular_iterator->index];

    // @index = (@index + 1) % @array.length
    circular_iterator->index = (circular_iterator->index + 1) % RARRAY_LEN(array);

    return element;
}

void Init_circular_iterator(void)
{
    // class CircularIteratorCStruct < Object
    VALUE cCircularIteratorCStruct = rb_define_class("CircularIteratorCStruct", rb_cObject);
    rb_define_alloc_func(cCircularIteratorCStruct, circular_iterator_allocate);

    rb_define_method(cCircularIteratorCStruct, "initialize", circular_iterator_initialize, 1);
    rb_define_method(cCircularIteratorCStruct, "next", circular_iterator_next, 0);
}
