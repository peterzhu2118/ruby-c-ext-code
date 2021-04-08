#include <ruby.h>

static ID id_puts;

static void kernel_puts(VALUE val)
{
    rb_funcall(rb_mKernel, id_puts, 1, val);
}

static VALUE my_fixed_args_method(VALUE self, VALUE arg1, VALUE arg2)
{
    kernel_puts(self);
    kernel_puts(arg1);
    kernel_puts(arg2);

    return Qnil;
}

static VALUE my_var_args_c_array_method(int argc, VALUE* argv, VALUE self)
{
    kernel_puts(self);

    for (int i = 0; i < argc; i++) {
        kernel_puts(argv[i]);
    }

    return Qnil;
}

static VALUE my_var_args_rb_array_method(VALUE self, VALUE args)
{
    kernel_puts(self);
    kernel_puts(args);

    return Qnil;
}

static VALUE my_method_with_required_block(VALUE self)
{
    VALUE block_ret = rb_yield_values(0);
    kernel_puts(block_ret);

    return Qnil;
}

void Init_methods(void)
{
    id_puts = rb_intern("puts");

    rb_define_method(rb_cObject, "my_fixed_args_method", my_fixed_args_method, 2);
    rb_define_method(rb_cObject, "my_var_args_c_array_method", my_var_args_c_array_method, -1);
    rb_define_method(rb_cObject, "my_var_args_rb_array_method", my_var_args_rb_array_method, -2);

    rb_define_method(rb_cObject, "my_method_with_required_block", my_method_with_required_block, 0);
}

