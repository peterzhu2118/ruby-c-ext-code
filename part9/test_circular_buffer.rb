require "test/unit"

unless ARGV.length == 2
  puts <<~DOC
    usage: ruby test_circular_buffer.rb implementation_path klass_name
    example: ruby test_circular_buffer.rb circular_buffer_ruby CircularBufferRuby
  DOC
  exit(1)
end

$file = ARGV[0]
$klass = ARGV[1]

require_relative $file

class TestCircularBuffer < Test::Unit::TestCase
  def setup
    @circular_buffer_klass = Object.const_get($klass)
  end

  def test_write_and_read
    circular_buffer = @circular_buffer_klass.new(5)

    # Fill circular_buffer
    (1..5).each do |i|
      assert_equal i, circular_buffer.write(i)
    end

    # Read one elemement
    assert_equal 1, circular_buffer.read

    # Write one element
    assert_equal 6, circular_buffer.write(6)

    # Read remaining elements
    (2..6).each do |i|
      assert_equal i, circular_buffer.read
    end
  end

  def test_zero_capacity
    circular_buffer = @circular_buffer_klass.new(0)

    # Cannot write
    assert_raises_full do
      circular_buffer.write(1)
    end

    # Cannot read
    assert_raises_empty do
      circular_buffer.read
    end
  end

  def test_write_full
    circular_buffer = @circular_buffer_klass.new(5)

    # Fill circular_buffer
    (1..5).each do |i|
      circular_buffer.write(i)
    end

    assert_raises_full do
      circular_buffer.write(6)
    end

    # Check that the write that raised did not succeed
    (1..5).each do |i|
      assert_equal i, circular_buffer.read
    end
  end

  def test_read_empty
    circular_buffer = @circular_buffer_klass.new(5)

    assert_raises_empty do
      circular_buffer.read
    end

    # Fill circular_buffer
    (1..5).each do |i|
      circular_buffer.write(i)
    end

    # Read all elements
    (1..5).each do |i|
      assert_equal i, circular_buffer.read
    end

    assert_raises_empty do
      circular_buffer.read
    end
  end

  # Check that circular buffer works under GC stress (GC stress runs GC
  # at every opportunity)
  def test_gc_stress
    original_gc_stress = GC.stress
    GC.stress = true

    circular_buffer = @circular_buffer_klass.new(5)

    (1..5).each do |i|
      # Must use strings since small numbers are not garbage collectible
      circular_buffer.write("Hello from #{i}")
    end

    (1..5).each do |i|
      assert_equal "Hello from #{i}", circular_buffer.read
    end
  ensure
    GC.stress = original_gc_stress
  end

  # Check that objects are not broken during compaction
  def test_gc_compact
    circular_buffer = @circular_buffer_klass.new(5)

    (1..5).each do |i|
      # Must use strings since small numbers are not garbage collectible
      circular_buffer.write("Hello from #{i}")
    end

    # Use GC.verify_compaction_references to ensure that every object
    # is moved
    GC.verify_compaction_references(toward: :empty, double_heap: true)

    (1..5).each do |i|
      assert_equal "Hello from #{i}", circular_buffer.read
    end
  end

  private

  def assert_raises_full(&blk)
    err = assert_raises do
      yield
    end
    assert_equal("Circular buffer is full", err.message)
  end

  def assert_raises_empty(&blk)
    err = assert_raises do
      yield
    end
    assert_equal("Circular buffer is empty", err.message)
  end
end
