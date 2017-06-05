
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

function verify(rows, cols, shape::Shape.Triangular, mat, func,
    func_gen = Nullable())

  @test size(mat, 1) == rows
  @test size(mat, 2) == cols

  if !isnull(func)
    func_ = get(func)
    if shape.data_placement == Shape.Upper
      @test isa(mat, UpperTriangular)
      for i=1:rows
        for j=i:rows
          func_( mat[i, j] )
        end
      end
      @test istriu(mat)
    else
      @test isa(mat, LowerTriangular)
      for i=1:rows
        for j=1:i
          func_( mat[i, j] )
        end
      end
      @test istril(mat)
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
