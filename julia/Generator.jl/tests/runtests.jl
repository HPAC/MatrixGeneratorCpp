

tests = ["constants", "random", "spd", "orthogonal", "typematcher"]

for f in tests
  include("$(f).jl")
end
