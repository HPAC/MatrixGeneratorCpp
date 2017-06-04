
import Base.==

module Shape

  export ShapeType, General

  abstract ShapeType

  immutable General <: ShapeType
    rows::Int
    cols::Int
  end

  immutable Symmetric <: ShapeType
    rows::Int
  end

  @enum Triangularity Upper=1 Lower=2

  immutable Triangular <: ShapeType
    rows::Int
    data_placement::Triangularity
  end

  immutable Diagonal <: ShapeType
    rows::Int
  end

  type Band <: ShapeType
    rows::Int
    cols::Int
    lower_bandwidth::Int
    upper_bandwidth::Int
    symmetric::Bool
  end

end

function ==(a::Shape.Band, b::Shape.Band)
  return a.rows == b.rows && a.cols == b.cols &&
    a.lower_bandwidth == b.lower_bandwidth &&
    a.upper_bandwidth == b.upper_bandwidth &&
    a.symmetric == b.symmetric
end
