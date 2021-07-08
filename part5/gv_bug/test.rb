require_relative "ext/gv_bug"

puts "my_string is `#{my_string}`"

# Here we manually trigger the GC to collect objects that have no references
GC.start

puts "my_string is `#{my_string}`"
