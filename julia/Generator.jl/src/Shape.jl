
import Base.==

module Shape

  export ShapeType, General

  abstract ShapeType

  immutable General <: ShapeType
  end

  immutable Symmetric <: ShapeType
  end

  immutable LowerTriangular <: ShapeType
  end

  immutable UpperTriangular <: ShapeType
  end

  immutable Diagonal <: ShapeType
  end

  type Band <: ShapeType
    lower_bandwidth::Int
    upper_bandwidth::Int
  end

end

function ==(a::Shape.Band, b::Shape.Band)
  return a.lower_bandwidth == b.lower_bandwidth &&
    a.upper_bandwidth == b.upper_bandwidth
end
