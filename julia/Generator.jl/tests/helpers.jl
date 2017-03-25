
function verify(rows, cols, shape::Shape.General, properties, func)
  mat = generate(shape, Set(properties))
  @test size(mat, 1) == rows
  @test size(mat, 2) == cols
  if !isnull(func)
    foreach(get(func), mat)
  end
end

function verify(rows, cols, shape::Shape.Symmetric, properties, func)
  mat = generate(shape, Set(properties))
  @test size(mat, 1) == rows
  @test size(mat, 2) == cols
  if !isnull(func)
    foreach(get(func), mat)
  end
  @test issymmetric(mat)
end

function verify(rows, cols, shape::Shape.Triangular, properties, func)
  mat = generate(shape, Set(properties))
  @test size(mat, 1) == rows
  @test size(mat, 2) == cols
    
  if !isnull(func)
    func_ = get(func)
    if shape.data_placement == Shape.Upper
      for i=1:rows
        for j=i:rows
          func_( mat[i, j] )
        end
      end
      @test istriu(mat)
    else
      for i=1:rows
        for j=1:i
          func_( mat[i, j] )
        end
      end
      @test istril(mat)
    end
  end
end

function verify(rows, cols, shape::Shape.Diagonal, properties, func)
  mat = generate(shape, Set(properties))
  @test size(mat, 1) == rows
  @test size(mat, 2) == cols
    
  if !isnull(func)
    func_ = get(func)
    for i=1:rows
      func_( mat[i, i] )
    end
    @test isdiag(mat)
  end
end

