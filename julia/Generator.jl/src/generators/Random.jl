
using .Shape;
using .Properties;

# import to extend with a set support
#import Base.findfirst

function define_random(functions, generic_functions)

  functions[ Set([Properties.Random]) ] =
    (size, shape, props) -> random(size..., shape, props, none);
  functions[ Set([Properties.Random, Properties.Positive])] =
    (size, shape, props) -> random(size..., shape, props, positive);
  functions[ Set([Properties.Random, Properties.Negative])] =
    (size, shape, props) -> random(size..., shape, props, negative);

  generic_functions[Properties.Random] =
    (shape, val_types, props) -> random(shape, val_types, props)

end

function findfirst(f::Function, set::Set)
  for element in set
    if f(element)
      return element
    end
  end
  return nothing
end

function findfirst(f::Function, set::Array)
  for element in set
    if f(element)
      return element
    end
  end
  return nothing
end

function get_bounds(properties, valTypes)
  rand_prop = findfirst(x -> x == Properties.Random || isa(x, Properties.Random), properties)
  if rand_prop == Properties.Random
    return valTypes == negative ? (-1, 0) : (0, 1)
  else
    return rand_prop.lower_bound, rand_prop.upper_bound
  end
end

function random{T, U <: ValuesType}(packed_shape::Tuple{T, Shape.Band, Bool, Int, Int}, properties, valTypes::U)
  special_shape, shape, symmetric, rows, cols = packed_shape
  mat = random(rows, cols, special_shape, properties, valTypes)
  # apply band to remove unnecessary elems
  return apply_band(special_shape, shape, rows, cols, mat)
end

function random{T <: ValuesType}(rows, cols, shape::Shape.General, properties, valTypes::T)
  low, high = get_bounds(properties, valTypes)
  if valTypes == none
    return rand(rows, cols) * (high - low) + low
  elseif valTypes == positive
    if low < 0
      throw(ErrorException(
        @sprintf("Clash between lower bound %f of Random and Positive!", low)
        ))
    end
    return rand(rows, cols) * (high - low) + low
  else
    if high > 0
      throw(ErrorException(
        @sprintf("Clash between upper bound %f of Random and Negative!", high)
        ))
    end
    return rand(rows, cols) * (high - low) + low
  end
end

function random{T <: ValuesType}(rows, cols, shape::Shape.Symmetric, properties, valTypes::T)
  if rows != cols
    throw(ErrorException("Non-square matrix passed to a symmetric generator!"))
  end
  mat = random(rows, rows, Shape.General(), properties, valTypes)
  return Symmetric(mat)
end

function random{T <: ValuesType}(rows, cols, shape::Shape.Triangular, properties, valTypes::T)
  # fill whole matrix, one part will be ignored
  mat = random(rows, cols, Shape.General(), properties, valTypes)
  return  shape.data_placement == Shape.Upper ?
            UpperTriangular(mat) :
            LowerTriangular(mat)
end

function random{T <: ValuesType}(rows, cols, shape::Shape.Diagonal, properties, valTypes::T)
  # fill one row
  mat = random(1, rows, Shape.General(), properties, valTypes)
  return Diagonal( vec(mat) )
end
