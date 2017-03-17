
using .Shape;
using .Properties;

#A Julia "enum"
@enum ValuesType none=0 positive=1 negative=2
#immutable ValuesType
#  type::Int
#end

#const none = ValuesType(0);
#const positive = ValuesType(1);
#const negative = ValuesType(2);

function random{T <: ValuesType}(shape::General, valTypes::T)
  if valTypes == none
    println( "General, none" )
  elseif valTypes == positive
    println( "General, pos")
  else
    println( "General, neg")
  end
end

function random{T <: ValuesType}(shape::SelfAdjoint, valTypes::T)
  if valTypes == none
    println( "SelfAdjoint, none" )
  elseif valTypes == positive
    println( "SelfAdjoint, pos")
  else
    println( "SelfAdjoint, neg")
  end
end
