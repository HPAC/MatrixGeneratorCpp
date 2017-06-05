
@enum ValuesType none=0 positive=1 negative=2

# returns (f, g, h)
# f - true if it is a shape type
# g - true if's symmetric
# h - type
function cast_type(property)
  if isa(property, Shape.General)
    # matrix (m, n) with bands m -1, n-1
    m = property.rows
    n = property.cols
    return (true, false, Shape.Band(m, n, m - 1, n - 1, false))
  elseif isa(property, Shape.Symmetric)
    # matrix (m, n) with bands m -1, n-1
    m = property.rows
    return (true, true, Shape.Band(m, m, m - 1, m - 1, true))
  elseif isa(property, Shape.Triangular)
    # matrix (m, n) with bands m -1, n-1
    m = property.rows
    if property.data_placement == Shape.Upper
      return (true, false, Shape.Band(m, m, 0, m - 1, false))
    else
      return (true, false, Shape.Band(m, m, m - 1, 0, false))
    end
  elseif isa(property, Shape.Diagonal)
    # matrix (m, n) with bands m -1, n-1
    m = property.rows
    return (true, true, Shape.Band(m, m, 0, 0, true))
  elseif isa(property, Shape.Band)
    return (true, false, property)
  else
    return (false, false, property)
  end
end

function merge_shapes(shape, new_shape)

  if isnull(shape)
    return new_shape
  end

  # For two band matrices
  # (m, n, k1, k2)
  # (p, q, l1, l2)
  # (m, n) == (p, q) must hold
  # and the result matrix will be the one with a smaller bandwidth
  if shape.rows != new_shape.rows || shape.cols != new_shape.cols
    throw(ErrorException(
        @sprintf("Clash between sizes matrix sizes: (%d, %d) versus (%d, %d)!",
          shape.rows, shape.cols, new_shape.rows, new_shape.cols)
        ))
  end
  lower_band = min(shape.lower_bandwidth, new_shape.lower_bandwidth)
  upper_band = min(shape.upper_bandwidth, new_shape.upper_bandwidth)
  symmetric = shape.symmetric || new_shape.symmetric
  return Shape.Band(shape.rows, shape.cols, lower_band, upper_band, symmetric)
end

function get_shape_type(properties)

  shape = Nullable{Shape.Band}()
  other_properties = []
  symmetric = false

  for p in properties
    res = cast_type(p)
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
  if symmetric
    # verify if banded matrix can be symmetric
    if shape.rows != shape.cols || shape.lower_bandwidth != shape.upper_bandwidth
      throw(ErrorException(
          @sprintf("Symmetric types clashing with an unsymmetric band matrix of
            size (%f, %f) and band (%f, %f)!",
            shape.rows, shape.cols, shape.lower_bandwidth, shape.upper_bandwidth)
          ))
    end
    shape.symmetric = true
  end
  return Pair(shape, other_properties)
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
          if isnull(major_property)
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

  if isnull(major_property)
    throw(ErrorException("No major property!"))
  end

  return (valTypes, major_property)
end

function cast_band(shape::Shape.Band)

  if shape.lower_bandwidth == 0 && shape.upper_bandwidth == 0
    return Shape.Diagonal(shape.rows)
  elseif shape.lower_bandwidth == 0
    return Shape.Triangular(shape.rows, Shape.Upper)
  elseif shape.upper_bandwidth == 0
    return Shape.Triangular(shape.rows, Shape.Lower)
  else
    if shape.symmetric
      return Shape.Symmetric(shape.rows)
    else
      return Shape.General(shape.rows, shape.cols)
    end
  end

end

function apply_band(shape::Shape.General, original_shape, matrix)
  for i=1:shape.rows
    for j=1:(i-original_shape.lower_bandwidth-1)
      matrix[i, j] = 0.0
    end
    for j=(i+original_shape.upper_bandwidth+1):shape.cols
      matrix[i, j] = 0.0
    end
  end
end

function apply_band(shape::Shape.Symmetric, original_shape, matrix)
end

# Triangular, Diagonal - don't do anything
function apply_band(shape, original_shape, matrix)
end
