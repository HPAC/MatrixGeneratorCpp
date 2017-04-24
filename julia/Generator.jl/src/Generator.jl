
module Generator

  export GeneratorImpl
  export generate


  include("Shape.jl")
  include("Properties.jl")
  #import shape types
  using .Shape
  #export General, ShapeType, SelfAdjoint
  export Shape

  using .Properties
  export Properties

  include("generators/Random.jl")
  include("generators/Constant.jl")
  include("generators/SPD.jl")
  include("generators/Orthogonal.jl")

  type GeneratorImpl

    generators::Dict

    function GeneratorImpl()
      a = Dict{Set{DataType}, Any}();
      define_random(a);
      define_constant(a);
      define_spd(a);
      define_orthogonal(a)
      return new(a)
    end

  end
  const generator = GeneratorImpl()

  function extract_type(obj)
    return isa(obj, DataType) ? obj : typeof(obj)
  end

  function generate{T <: ShapeType}(shape::T, properties)
    mat = generator.generators[map(extract_type, properties)](shape, properties)
    if isa(shape, Shape.General) && shape.rows == 1
      return vec(mat)
    else
      return mat
    end
  end

end
