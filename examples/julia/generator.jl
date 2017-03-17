workspace()
reload("Generator")
using Generator

println(generate(generator, Shape.General(10, 10), [Properties.Random]))
println(generate(generator, Shape.General(10, 10), [Properties.Random, Properties.Negative]))

#generate(generator, Shape.SelfAdjoint(10, 10), [Properties.Random, Properties.Positive]);
#generate(generator, Shape.SelfAdjoint(10, 10), [Properties.Random, Properties.Negative]);
