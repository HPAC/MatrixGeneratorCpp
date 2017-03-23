workspace()
reload("Generator")
using Generator

println(generate(Shape.General(2, 2), Set([Properties.Random(0, 3)])))
println(generate(Shape.General(2, 2), Set([ Properties.Negative, Properties.Random(-2, -1)])))
println(generate(Shape.SelfAdjoint(2), Set([Properties.Random, Properties.Positive])))
