#include <ruby.h>

// Store a string globally
static VALUE my_string;

static VALUE get_my_string(VALUE _)
{
    return my_string;
}

void Init_gv_bug(void)
{
    // Create a new string "Hello world!"
    my_string = rb_str_new_cstr("Hello world!");

    // Define a getter method for my_string
    rb_define_method(rb_cObject, "my_string", get_my_string, 0);
}
