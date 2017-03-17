workspace()
reload("Generator")
using Generator

println(generate(generator, Shape.General(2, 2), [Properties.Random]))
println(generate(generator, Shape.General(2, 2), [Properties.Random, Properties.Negative]))
println(generate(generator, Shape.SelfAdjoint(2), [Properties.Random, Properties.Positive]))
