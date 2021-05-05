# Ruby implementation
# require_relative "circular_iterator_ruby"
# CircularIterator = CircularIteratorRuby

# C implementation with ivars
# require_relative "ext_ivar/circular_iterator"
# CircularIterator = CircularIteratorCIvar

# C implementation with structs
require_relative "ext_struct/circular_iterator"
CircularIterator = CircularIteratorCStruct

array = ["one", "two", "three"]
circular_iterator = CircularIterator.new(array)

puts circular_iterator.next #=> "one"
puts circular_iterator.next #=> "two"
puts circular_iterator.next #=> "three"
puts circular_iterator.next #=> "one"
puts circular_iterator.next #=> "two"
puts circular_iterator.next #=> "three"
