

tests = ["constants", "random", "spd"]

for f in tests
  include("$(f).jl")
end
