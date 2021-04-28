require_relative "ext/array_ext"

["one", "two", "three", "four", "five"].puts_every_other #=> one three five
["one", "two", "three", "four"].puts_every_other #=> one three
[].puts_every_other #=> empty output
