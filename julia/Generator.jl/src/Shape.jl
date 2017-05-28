

module Shape

  export ShapeType, General, SelfAdjoint

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

  immutable Band <: ShapeType
    rows::Int
    cols::Int
    lower_bandwidth::Int
    upper_bandwidth::Int
  end

  function ==(a::Band, b::Band)
      return a.rows == b.rows && a.cols == b.cols &&
        a.lower_bandwidth == b.lower_bandwidth &&
        a.upper_bandwidth == b.upper_bandwidth
  end

end
