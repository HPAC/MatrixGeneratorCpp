
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
      define_random(a, generic_gen);
      define_constant(a, generic_gen);
      define_spd(a, generic_gen);
      define_orthogonal(a, generic_gen);
      return new(a, generic_gen)
    end

  end
  const generator = GeneratorImpl()

  function extract_type(obj)
    return isa(obj, DataType) ? obj : typeof(obj)
  end

  function generate{T <: ShapeType}(size, shape::T, properties)
    mat = generator.generators[map(extract_type, properties)](size, shape, properties)
    if isa(shape, Shape.General)
      if size[2] == 1
        return vec(mat)
      elseif size[1] == 1
        return vec(mat)'
      else
        return mat
      end
    else
      return mat
    end
  end

  function generate(size, properties)
    shape, symmetric, other_properties = get_shape_type(size, properties)
    special_shape = cast_band(size, symmetric, shape)
    val_types, major_prop = extract_basic_properties(properties)
    mat = generator.generic_generators[extract_type(major_prop)](
        (special_shape, shape, symmetric, size...), other_properties, val_types
      )
    if size[2] == 1
      return vec(mat)
    elseif size[1] == 1
      return vec(mat)'
    else
      return mat
    end
  end

end
