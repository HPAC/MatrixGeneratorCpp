
using .Shape;

function random(shape::General, positive::Bool, negative::Bool)
  @printf("Random general called with %d %d\n", positive, negative)
end

function random(shape::SelfAdjoint, positive::Bool, negative::Bool)
  @printf("Random SelfAdjoint called with %d %d\n", positive, negative)
end
