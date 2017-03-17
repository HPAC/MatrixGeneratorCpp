workspace()
reload("Generator")
using Generator
println(typeof(Generator.GeneratorImpl))
println(typeof(GeneratorImpl))
println(typeof(generator))
println(typeof(Shape.General))
println(Shape.General <: Shape.ShapeType)
println(supertype(Shape.General))
#typeof(Shape::General)

generate(generator, Shape.General(10, 10), [Properties.Random]);

generate(generator, Shape.SelfAdjoint(10, 10), [Properties.Random, Properties.Positive]);
generate(generator, Shape.SelfAdjoint(10, 10), [Properties.Random, Properties.Negative]);
