

tests = ["constants", "random", "spd", "orthogonal"]

for f in tests
  include("$(f).jl")
end
