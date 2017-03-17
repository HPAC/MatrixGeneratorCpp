
module Generator

  export GeneratorImpl
  export generator, generate


  include("Shape.jl")
  include("Properties.jl")
  #import shape types
  using .Shape
  export General, ShapeType, SelfAdjoint

  include("generators/Random.jl")

  #export Shape::General


  type GeneratorImpl

    generators::Dict

    function GeneratorImpl()
      a = Dict([1]=>1);
      return new(a)
    end

  end
  const generator = GeneratorImpl()#Dict([1]=>1)

  function generate{T <: Shape.ShapeType}(gen::GeneratorImpl, shape::T, properties)
    #println(gen.generators[[1]])
    random(shape, false, false)
  end

end
