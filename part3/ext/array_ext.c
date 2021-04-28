#include <ruby.h>

static ID id_each, id_puts;

static VALUE array_puts_every_other_i(VALUE yielded_arg, VALUE data, int argc, const VALUE *argv, VALUE blockarg)
{
    // Cast the data back to a pointer
    int *puts_curr_ptr = (int *)data;
    int puts_curr = *puts_curr_ptr;

    // If we should put the current element
    if (puts_curr) {
        rb_funcall(rb_mKernel, id_puts, 1, yielded_arg);
    }

    // Flip the bit for puts_curr
    *puts_curr_ptr = !puts_curr;

    return Qnil;
}

static VALUE array_puts_every_other(VALUE self)
{
    // The first element shout be outputted
    int puts_curr = 1;

    // Call Array#each with block array_puts_every_other_i
    rb_block_call(self, id_each, 0, NULL, array_puts_every_other_i, (VALUE)&puts_curr);

    return Qnil;
}

void Init_array_ext(void)
{
    id_each = rb_intern("each");
    id_puts = rb_intern("puts");

    // Define method Array#puts_every_other
    rb_define_method(rb_cArray, "puts_every_other", array_puts_every_other, 0);
}
