
using .Shape;
using .Properties;

@enum ValuesType none=0 positive=1 negative=2

function define_random(functions)

  functions[ Set([Properties.Random]) ] =
    shape -> random(shape, none);
  functions[ Set([Properties.Random, Properties.Positive])] =
    shape -> random(shape, positive);
  functions[ Set([Properties.Random, Properties.Negative])] =
    shape -> random(shape, negative);

end

function random{T <: ValuesType}(shape::General, valTypes::T)
  if valTypes == none
    return rand(shape.rows, shape.cols)
  elseif valTypes == positive
    return rand(shape.rows, shape.cols)
  else
    return rand(shape.rows, shape.cols) * -1
  end
end

function random{T <: ValuesType}(shape::SelfAdjoint, valTypes::T)
  mat = random(Shape.General(shape.rows, shape.rows), valTypes)
  return Symmetric(mat)
end
