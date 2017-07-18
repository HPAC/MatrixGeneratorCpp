
@enum ValuesType none=0 positive=1 negative=2

function isa_obj_type(obj, obj_type)
  return obj == obj_type || isa(obj, obj_type)
end

# returns (f, g, h)
# f - true if it is a shape type
# g - true if's symmetric
# h - type
function cast_type(rows, cols, property)
  if isa_obj_type(property, Shape.General)
    # matrix (m, n) with bands m -1, n-1
    return (true, false, Shape.Band(rows - 1, cols - 1))
  elseif isa_obj_type(property, Shape.Symmetric)
    # matrix (m, n) with bands m -1, n-1
    return (true, true, Shape.Band(rows - 1, cols - 1))
  elseif isa_obj_type(property, Shape.LowerTriangular)
    # triangular matrix (m, n) with bands m -1, 0
    return (true, false, Shape.Band(rows - 1, 0))
  elseif isa_obj_type(property, Shape.UpperTriangular)
    # triangular matrix (m, n) with bands 0, n -1
    return (true, false, Shape.Band(0, cols - 1))
  elseif isa_obj_type(property, Shape.Diagonal)
    # matrix (m, n) with bands m -1, n-1
    return (true, false, Shape.Band(0, 0))
  elseif isa_obj_type(property, Shape.Band)
    return (true, false, property)
  else
    return (false, false, property)
  end
end

function merge_shapes(shape, new_shape)

  if isa(shape, Nullable) && isnull(shape)
    return new_shape
  end

  # For two band matrices
  # (m, n, k1, k2)
  # (p, q, l1, l2)
  # the result matrix will be the one with a smaller bandwidth
  lower_band = min(shape.lower_bandwidth, new_shape.lower_bandwidth)
  upper_band = min(shape.upper_bandwidth, new_shape.upper_bandwidth)
  return Shape.Band(lower_band, upper_band)
end

function get_shape_type(mat_size, properties)

  shape = Nullable{Shape.Band}()
  other_properties = []
  symmetric = false
  rows, cols = mat_size

  for p in properties
    res = cast_type(rows, cols, p)
    if res[1]
      shape = merge_shapes(shape, res[3])
      symmetric |= res[2]
    else
      if isa(p, Properties.Symmetric) || p == Properties.Symmetric
        symmetric = true
      end
      push!(other_properties, p)
    end
  end
  return (shape, symmetric, other_properties)
end

function extract_basic_properties(properties)

  valTypes = none
  major_property = Nullable()
  major_properties = [Properties.Constant, Properties.Random,
    Properties.Orthogonal, Properties.SPD]

  for p in properties
    if p == Properties.Positive || isa(p, Properties.Positive)
      valTypes = positive
    elseif p == Properties.Negative || isa(p, Properties.Negative)
      valTypes = negative
    else
      for prop in major_properties
        if p == prop || isa(p, prop)
          if isa(major_property, Nullable) && isnull(major_property)
            major_property = p
          else
            throw(ErrorException("Clash between major properties: %s %s",
              major_property, p
            ))
          end
        end
      end
    end
  end

  if isa(major_property, Nullable) && isnull(major_property)
    throw(ErrorException("No major property!"))
  end

  return (valTypes, major_property)
end

function cast_band(mat_size, symmetric::Bool, shape::Shape.Band)

  rows, cols = mat_size

  if shape.lower_bandwidth == 0 && shape.upper_bandwidth == 0
    return Shape.Diagonal()
  elseif shape.lower_bandwidth == 0 && rows == cols && shape.upper_bandwidth + 1 == cols
    return Shape.UpperTriangular()
  elseif shape.upper_bandwidth == 0 && rows == cols && shape.lower_bandwidth + 1 == rows
    return Shape.LowerTriangular()
  else
    if symmetric && rows == cols
      return Shape.Symmetric()
    else
      return Shape.General()
    end
  end

end

function apply_band(shape::Shape.General, original_shape, rows, cols, matrix)

  if original_shape.lower_bandwidth + 1 == rows &&
      original_shape.upper_bandwidth + 1 == cols
    return matrix
  end

  for i=1:rows
    for j=1:min(cols, i - original_shape.lower_bandwidth - 1)
      matrix[i, j] = 0.0
    end
    for j=(i + original_shape.upper_bandwidth + 1):cols
      matrix[i, j] = 0.0
    end
  end
  return matrix
end

function apply_band(shape::Shape.Symmetric, original_shape, rows, cols, matrix)
  nonsymm = matrix.data
  for i=1:rows
    for j=1:min(rows, i - original_shape.lower_bandwidth - 1)
      nonsymm[i, j] = 0.0
    end
  end
  return Symmetric(nonsymm, :L)
end

# Triangular, Diagonal - don't do anything
function apply_band(shape, original_shape, rows, cols, matrix)
  return matrix
end
