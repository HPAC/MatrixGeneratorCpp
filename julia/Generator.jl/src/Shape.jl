

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
end
