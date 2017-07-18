
using .Shape;
using .Properties;

function define_orthogonal(functions, generic_functions)

  functions[ Set([Properties.Orthogonal]) ] =
    (size, shape, props) -> orthogonal(size..., shape, props, none);

  functions[ Set([Properties.Orthogonal, Properties.Positive]) ] =
    (size, shape, props) -> orthogonal(size..., shape, props, positive);

  functions[ Set([Properties.Orthogonal, Properties.Negative]) ] =
    (size, shape, props) -> orthogonal(size..., shape, props, negative);

  generic_functions[Properties.Orthogonal] =
    (shape, val_types, props) -> orthogonal(shape, val_types, props)
end

function orthogonal{T}(packed_shape::Tuple{T, Shape.Band, Bool, Int, Int}, properties, valTypes)

  special_shape, shape, symmetric, rows, cols = packed_shape
  # verify if we can use one of easy generators
  if (isa(special_shape, Shape.General) || isa(special_shape, Shape.Symmetric)) &&
    (shape.upper_bandwidth + 1 != cols || shape.lower_bandwidth + 1 != rows)
    throw(ErrorException("Banded orthogonal not supported!"))
  end
  mat = orthogonal(rows, cols, special_shape, properties, valTypes)
  # apply band to remove unnecessary elems
  return apply_band(special_shape, shape, rows, cols, mat)
end

"""
  Generate an orthogonal matrix by a QR factorization.
  Only diagonal orthogonal matrices can be positive/negative,
  a general positive/negative matrix would not have orthogonal columns.
"""
function orthogonal(rows, cols, shape::Shape.General, properties, type_::ValuesType)

  if type_ != none
    throw(ErrorException("Only diagonal orthogonal matrix can be positive/negative!"))
  end

  if rows != cols
    throw(ErrorException("A non-square matrix cannot be orthogonal!"))
  else
    q, = qr( rand(rows, rows) )
    return q
  end

end

"""
  Some orthogonal matrices are symmetric - all diagonal matrices,
"""
function orthogonal(rows, cols, shape::Shape.Symmetric, properties, type_::ValuesType)
  throw(ErrorException("Symmetric orthogonal matrices are not supported!"))
end

function orthogonal(rows, cols, shape::Shape.UpperTriangular, properties, type_::ValuesType)
  throw(ErrorException("Triangular matrix cannot be orthogonal!"))
end

function orthogonal(rows, cols, shape::Shape.LowerTriangular, properties, type_::ValuesType)
  throw(ErrorException("Triangular matrix cannot be orthogonal!"))
end

"""
  A diagonal matrix is orthogonal iff columns are basis vectors of Euclidean space,
  with possible negative values.
  FIXME: move the implementation to permutation matrix
"""
function orthogonal(rows, cols, shape::Shape.Diagonal, properties, type_::ValuesType)

  # mix negative and positive values
  if type_ == none
    vals = [rand() >= 0.5 ? 1.0 : -1.0 for n in 1:rows]
  elseif type_ == negative
    vals = ones(rows) * -1
  else
    vals = ones(rows)
  end

  return Diagonal(vals)
end
