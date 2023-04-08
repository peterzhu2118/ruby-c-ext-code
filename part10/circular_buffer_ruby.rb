# Ruby implementation of circular buffer backed by an array. A circular buffer
# is a buffer of a finite size with a read and write cursor. When the
# read or write cursor reaches the end of the buffer, it wraps around to
# the beginning again.
# See: https://en.wikipedia.org/wiki/Circular_buffer

class CircularBufferRuby
  # Initializes the circular buffer.
  #
  # @param capacity [Integer] the capacity of the circular buffer
  def initialize(capacity)
    @buffer = Array.new(capacity)
    @length = 0
    @read_cursor = 0
    @write_cursor = 0
  end

  # Writes an object to the circular buffer at the current write cursor and
  # moves the write cursor forward. Raises if the buffer is full.
  #
  # @param obj [Object] the object to write to the circular buffer
  # @return the object written
  def write(obj)
    raise "Circular buffer is full" if full?

    @buffer[@write_cursor] = obj

    # Advance write_cursor
    @write_cursor = next_cursor_position(@write_cursor)
    @length += 1

    obj
  end

  # Reads the object in the circular buffer at the current read cursor and
  # moves the read cursor forward. Raises if the buffer is empty.
  #
  # @return the object read
  def read
    raise "Circular buffer is empty" if empty?

    obj = @buffer[@read_cursor]

    # Advance read_cursor
    @read_cursor = next_cursor_position(@read_cursor)
    @length -= 1

    obj
  end

  private

  def next_cursor_position(cursor)
    (cursor + 1) % @buffer.length
  end

  def full?
    @length == @buffer.length
  end

  def empty?
    @length == 0
  end
end
