

module Shape

  export ShapeType, General, SelfAdjoint

  abstract ShapeType

  immutable General <: ShapeType
    rows::Int
    cols::Int
  end

  immutable SelfAdjoint <: ShapeType
    rows::Int
  end

  immutable Diagonal
  end
end
