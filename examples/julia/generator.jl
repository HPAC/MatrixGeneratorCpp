workspace()
reload("Generator")
using Generator
println(typeof(Generator.GeneratorImpl))
println(typeof(GeneratorImpl))
println(typeof(generator))
generate(generator, 1, [2, 3]);
