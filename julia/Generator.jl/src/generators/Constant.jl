
using .Shape;
using .Properties;

# import to extend with a set support
import Base.findfirst

function define_constant(functions)

  functions[ Set([Properties.Constant]) ] =
    (shape, props) -> constant(shape, props);

end

function constant(shape::Shape.General, properties)

  const_prop = first(properties)
  if isa(const_prop, DataType)
    return fill(0, (shape.rows, shape.cols))
  else
    return fill(const_prop.value, (shape.rows, shape.cols))
  end

end

function constant(shape::Shape.Symmetric, properties)
  mat = constant(Shape.General(shape.rows, shape.rows), properties)
  return Symmetric(mat)
end
