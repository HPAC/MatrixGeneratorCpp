
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
      define_random(a);
      return new(a)
    end

  end
  const generator = GeneratorImpl()

  function generate{T <: ShapeType}(gen::GeneratorImpl, shape::T, properties)
    return gen.generators[ Set(properties) ](shape)
  end

end
