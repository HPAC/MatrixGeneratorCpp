
using .Shape;
using .Properties;

function define_constant(functions, generic_functions)

  functions[ Set([Properties.Constant]) ] =
    (shape, props) -> constant(shape, props);

  generic_functions[Properties.Constant] =
      (shape, val_types, props) -> constant(shape, val_types, props)
end

function constant(shape::Shape.Band, properties, valTypes)

  # verify if we can use one of easy generators
  special_shape = cast_band(shape)
  mat = constant(special_shape, properties)
  # apply band to remove unnecessary elems
  return apply_band(special_shape, shape, mat)
end

function constant(shape::Shape.General, properties)

  const_prop = first(properties)
  if isa(const_prop, DataType)
    return fill(0.0, (shape.rows, shape.cols))
  else
    return fill(const_prop.value, (shape.rows, shape.cols))
  end

end

function constant(shape::Shape.Symmetric, properties)
  mat = constant(Shape.General(shape.rows, shape.rows), properties)
  return Symmetric(mat)
end

function constant(shape::Shape.Triangular, properties)
  mat = constant(Shape.General(shape.rows, shape.rows), properties)
  return shape.data_placement == Shape.Upper ?
            UpperTriangular(mat) :
            LowerTriangular(mat)
end

function constant(shape::Shape.Diagonal, properties)
  mat = constant(Shape.General(1, shape.rows), properties)
  return Diagonal( vec(mat) )
end
