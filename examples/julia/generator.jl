workspace()
reload("Generator")
using Generator
println(typeof(Generator.GeneratorImpl))
println(typeof(GeneratorImpl))
println(typeof(generator))
println(typeof(General))
println(General <: ShapeType)
println(supertype(General))
#typeof(Shape::General)
generate(generator, General(10, 10), [2, 3]);

generate(generator, SelfAdjoint(10, 10), [2, 3]);
