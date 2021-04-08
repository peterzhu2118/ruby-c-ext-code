require_relative "ext/methods"

puts "----- Testing Object#my_fixed_args_method -----"

"Hi from self".my_fixed_args_method("Hi from argument 1", "Hi from argument 2")

puts
puts "----- Testing Object#my_var_args_c_array_method -----"

"Hi from self".my_var_args_c_array_method("Hi from argument 1", "Hi from argument 2", "Hi from argument 3")

puts
puts "----- Testing Object#my_var_args_rb_array_method -----"

"Hi from self".my_var_args_rb_array_method("Hi from argument 1", "Hi from argument 2", "Hi from argument 3")

puts
puts "----- Testing Object#my_method_with_required_block -----"

my_method_with_required_block do
  "foo"
end
