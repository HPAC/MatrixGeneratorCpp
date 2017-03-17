

module Shape

  export General, SelfAdjoint

  immutable General
    rows::Int
    cols::Int
  end

  immutable SelfAdjoint
  end

  immutable Diagonal
  end
end
