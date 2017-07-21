
function verify(rows, cols, shape::Shape.General, mat, func,
    func_gen = Nullable())

  if cols == 1
    @test isa(mat, Array{Float64, 1})
  elseif rows == 1
    @test isa(mat, RowVector{Float64, Array{Float64, 1}})
  else
    @test isa(mat, Array{Float64, 2})
  end

  @test size(mat, 1) == rows
  @test size(mat, 2) == cols
  if !isnull(func)
    foreach(get(func), mat)
  end
  if !isnull(func_gen)
    get(func_gen)(mat)
  end

end

function verify(rows, cols, shape::Shape.Symmetric, mat, func,
    func_gen = Nullable())

  @test isa(mat, Symmetric)
  @test size(mat, 1) == rows
  @test size(mat, 2) == cols
  if !isnull(func)
    foreach(get(func), mat)
  end
  @test issymmetric(mat)
  if !isnull(func_gen)
    get(func_gen)(mat)
  end

end

function verify(rows, cols, shape::Shape.UpperTriangular, mat, func,
    func_gen = Nullable())

  @test size(mat, 1) == rows
  @test size(mat, 2) == cols

  if rows == cols
    @test isa(mat, UpperTriangular)
    @test istriu(mat)
  else
    for i=2:rows
      for j=1:min(i-1, cols)
        @test mat[i, j] ≈ 0.0
      end
    end
  end

  if !isnull(func)
    func_ = get(func)
    for i=1:rows
      for j=i:min(rows, cols)
        func_( mat[i, j] )
      end
    end
  end
  if !isnull(func_gen)
    get(func_gen)(mat)
  end

end

function verify(rows, cols, shape::Shape.LowerTriangular, mat, func,
    func_gen = Nullable())

  @test size(mat, 1) == rows
  @test size(mat, 2) == cols


  if rows == cols
    @test isa(mat, LowerTriangular)
    @test istril(mat)
  else
    for i=1:rows
      for j=min(i, cols)+1:cols
        @test mat[i, j] ≈ 0.0
      end
    end
  end
  if !isnull(func)
    func_ = get(func)
    for i=1:rows
      for j=1:min(i, cols)
        func_( mat[i, j] )
      end
    end
  end
  if !isnull(func_gen)
    get(func_gen)(mat)
  end

end

function verify(rows, cols, shape::Shape.Diagonal, mat, func,
    func_gen = Nullable())

  @test isa(mat, Diagonal)
  @test size(mat, 1) == rows
  @test size(mat, 2) == cols

  if !isnull(func)
    func_ = get(func)
    for i=1:rows
      func_( mat[i, i] )
    end
    @test isdiag(mat)
  end
  if !isnull(func_gen)
    get(func_gen)(mat)
  end

end

function verify(rows, cols, shape::Shape.Band, mat, func,
    func_gen = Nullable())

  @test size(mat, 1) == rows
  @test size(mat, 2) == cols

  for i=1:rows
      for j=1:min(cols, i-shape.lower_bandwidth-1)
        @test mat[i, j] ≈ 0.0
      end
      for j=max(1, i-shape.lower_bandwidth):min(cols, i+shape.upper_bandwidth)
        if !isnull(func)
          get(func)( mat[i, j] )
        end
      end
      for j=(i+shape.upper_bandwidth+1):cols
        @test mat[i, j] ≈ 0.0
      end
  end
  if !isnull(func_gen)
    get(func_gen)(mat)
  end

end
