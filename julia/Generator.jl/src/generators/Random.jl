
using .Shape;
using .Properties;

# import to extend with a set support
import Base.findfirst

@enum ValuesType none=0 positive=1 negative=2

function define_random(functions)

  functions[ Set([Properties.Random]) ] =
    (shape, props) -> random(shape, props, none);
  functions[ Set([Properties.Random, Properties.Positive])] =
    (shape, props) -> random(shape, props, positive);
  functions[ Set([Properties.Random, Properties.Negative])] =
    (shape, props) -> random(shape, props, negative);

end

function findfirst(f::Function, set::Set)
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

function random{T <: ValuesType}(shape::Shape.General, properties, valTypes::T)
  low, high = get_bounds(properties, valTypes)
  if valTypes == none
    return rand(shape.rows, shape.cols) * (high - low) + low
  elseif valTypes == positive
    if low < 0
      throw(ErrorException(
        @sprintf("Clash between lower bound %f of Random and Positive!", low)
        ))
    end
    return rand(shape.rows, shape.cols) * (high - low) + low
  else
    if high > 0
      throw(ErrorException(
        @sprintf("Clash between upper bound %f of Random and Negative!", high)
        ))
    end
    return rand(shape.rows, shape.cols) * (high - low) + low
  end
end

function random{T <: ValuesType}(shape::Shape.Symmetric, properties, valTypes::T)
  mat = random(Shape.General(shape.rows, shape.rows), properties, valTypes)
  return Symmetric(mat)
end
