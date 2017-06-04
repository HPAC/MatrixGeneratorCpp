
module Generator

  export GeneratorImpl
  export generate

  include("Shape.jl")
  include("Properties.jl")
  include("TypeMatcher.jl")
  #import shape types
  using .Shape
  #export General, ShapeType
  export Shape

  using .Properties
  export Properties

  @enum ValuesType none=0 positive=1 negative=2

  include("generators/Random.jl")
  include("generators/Constant.jl")
  include("generators/SPD.jl")
  include("generators/Orthogonal.jl")

  type GeneratorImpl

    generators::Dict
    generic_generators::Dict

    function GeneratorImpl()
      a = Dict{Set{DataType}, Any}();
      generic_gen = Dict{DataType, Any}();
      define_random(a);
      define_constant(a);
      define_spd(a);
      define_orthogonal(a)
      return new(a, generic_gen)
    end

  end
  const generator = GeneratorImpl()

  function extract_type(obj)
    return isa(obj, DataType) ? obj : typeof(obj)
  end

  function generate{T <: ShapeType}(shape::T, properties)
    mat = generator.generators[map(extract_type, properties)](shape, properties)
    if isa(shape, Shape.General)
      if shape.cols == 1
        return vec(mat)
      elseif shape.rows == 1
        return vec(mat)'
      else
        return mat
      end
    else
      return mat
    end
  end

  function generate(properties)
    shape, other_properties = get_get_shape_type(properties)
    #mat = generator.generators[map(extract_type, properties)](shape, properties)
  end

end
