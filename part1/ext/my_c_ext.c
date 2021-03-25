#include <ruby.h>

void Init_my_c_ext(void)
{
    ID id_puts = rb_intern("puts");

    VALUE hello_world_str = rb_str_new_cstr("Hello world!");
    rb_funcall(rb_mKernel, id_puts, 1, hello_world_str);
}

