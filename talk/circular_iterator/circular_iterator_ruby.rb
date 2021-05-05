class CircularIteratorRuby
  def initialize(array)
    @array = array
    @index = 0
  end
  
  def next
    element = @array[@index]
    @index = (@index + 1) % @array.length
    element
  end
end
