require_relative "circular_buffer_ruby"
require_relative "circular_buffer_ivar"
require_relative "circular_buffer_typeddata"

require "benchmark/ips"

SIZE = 1000

def benchmark_circular_buffer(klass)
  circular_buffer = klass.new(SIZE)

  SIZE.times do |i|
    circular_buffer.write(i)
  end

  SIZE.times do |i|
    circular_buffer.read
  end
end

Benchmark.ips do |x|
  x.report("circular_buffer_ruby") do
    benchmark_circular_buffer(CircularBufferRuby)
  end

  x.report("circular_buffer_ivar") do
    benchmark_circular_buffer(CircularBufferIvar)
  end

  x.report("circular_buffer_typeddata") do
    benchmark_circular_buffer(CircularBufferTypedData)
  end

  x.compare!
end
