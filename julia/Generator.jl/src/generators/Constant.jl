
using .Shape;
using .Properties;

function define_constant(functions, generic_functions)

  functions[ Set([Properties.Constant]) ] =
    (size, shape, props) -> constant(size..., shape, props);

  generic_functions[Properties.Constant] =
    (shape, val_types, props) -> constant(shape, val_types, props)
end

function constant{T}(packed_shape::Tuple{T, Shape.Band, Bool, Int, Int}, properties, valTypes)

  # verify if we can use one of easy generators
  special_shape, shape, symmetric, rows, cols = packed_shape
  mat = constant(rows, cols, special_shape, properties)
  # apply band to remove unnecessary elems
  return apply_band(special_shape, shape, rows, cols, mat)
end

function constant(rows, cols, shape::Shape.General, properties)

  const_prop = first(properties)
  if isa(const_prop, DataType)
    return fill(0.0, (rows, cols))
  else
    return fill(const_prop.value, (rows, cols))
  end

end

function constant(rows, cols, shape::Shape.Symmetric, properties)
  if rows != cols
    throw(ErrorException("Non-square matrix passed to a symmetric generator!"))
  end
  mat = constant(rows, cols, Shape.General(), properties)
  return Symmetric(mat)
end

function constant(rows, cols, shape::Shape.LowerTriangular, properties)
  mat = constant(rows, cols, Shape.General(), properties)
  return LowerTriangular(mat)
end

function constant(rows, cols, shape::Shape.UpperTriangular, properties)
  mat = constant(rows, cols, Shape.General(), properties)
  return UpperTriangular(mat)
end

function constant(rows, cols, shape::Shape.Diagonal, properties)
  mat = constant(1, rows, Shape.General(), properties)
  return Diagonal( vec(mat) )
end
