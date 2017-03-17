
module Generator

  export GeneratorImpl
  export generator, generate


  include("Shape.jl")
  include("Properties.jl")
  #import shape types
  using .Shape
  #export General, ShapeType, SelfAdjoint
  export Shape

  using .Properties
  export Properties

  include("generators/Random.jl")

  #export Shape::General


  type GeneratorImpl

    generators::Dict

    function GeneratorImpl()
      a = Dict{Set{DataType}, Any}();
      a[ Set([Properties.Random]) ] = shape -> random(shape, none);
      a[ Set([Properties.Random, Properties.Positive])] = shape -> random(shape, positive);
      a[ Set([Properties.Random, Properties.Negative])] = shape -> random(shape, negative);
      return new(a)
    end

  end
  const generator = GeneratorImpl()#Dict([1]=>1)

  function generate{T <: ShapeType}(gen::GeneratorImpl, shape::T, properties)
    #println(gen.generators[[1]])
    gen.generators[ Set(properties) ](shape)
  end

end
