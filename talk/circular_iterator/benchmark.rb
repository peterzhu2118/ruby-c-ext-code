require "bundler/inline"

gemfile do
  source "https://rubygems.org"

  gem "benchmark-ips"
end

require_relative "circular_iterator_ruby"
require_relative "ext_ivar/circular_iterator"
require_relative "ext_struct/circular_iterator"

NUM_ELEMENTS = 1_000
array = Array.new(NUM_ELEMENTS)
NUM_ELEMENTS.times do |i|
  array[i] = i
end

circular_iterator_ruby = CircularIteratorRuby.new(array)
circular_iterator_c_ivar = CircularIteratorCIvar.new(array)
circular_iterator_c_struct = CircularIteratorCStruct.new(array)

Benchmark.ips do |x|
  x.report("ruby") do
    NUM_ELEMENTS.times { circular_iterator_ruby.next }
  end

  x.report("c ivar") do
    NUM_ELEMENTS.times { circular_iterator_c_ivar.next }
  end

  x.report("c struct") do
    NUM_ELEMENTS.times { circular_iterator_c_struct.next }
  end
end
