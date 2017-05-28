
# returns (f, g, h)
# f - true if it is a shape type
# g - true if's symmetric
# h - type
function cast_type(property)
  if isa(property, Shape.General)
    # matrix (m, n) with bands m -1, n-1
    m = property.rows
    n = property.cols
    return (true, false, Shape.Band(m, n, m - 1, n - 1))
  elseif isa(property, Shape.Symmetric)
    # matrix (m, n) with bands m -1, n-1
    m = property.rows
    return (true, true, Shape.Band(m, m, m - 1, m - 1))
  elseif isa(property, Shape.Triangular)
    # matrix (m, n) with bands m -1, n-1
    m = property.rows
    if property.data_placement == Shape.Upper
      return (true, true, Shape.Band(m, m, 0, m - 1))
    else
      return (true, true, Shape.Band(m, m, m - 1, 0))
    end
  elseif isa(property, Shape.Diagonal)
    # matrix (m, n) with bands m -1, n-1
    m = property.rows
    return (true, true, Shape.Band(m, m, 0, 0))
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
  return Shape.Band(shape.rows, shape.cols, lower_band, upper_band)
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
      push!(other_properties, p)
    end
  end
  if symmetric
    push!(other_properties, Properties.Symmetric)
  end
  return Pair(shape, other_properties)
end
