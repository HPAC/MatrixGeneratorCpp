using Base.Test
using Generator

function verify(rows, cols, shape, properties, func)
  mat = generate(generator, shape, properties)
  @test size(mat, 1) == rows
  @test size(mat, 2) == cols
  if !isnull(func)
    foreach(get(func), mat)
  end
  return mat
end

matrix_sizes = [ [1, 1], [2, 2], [25, 50], [50, 25] ]
matrix_sq_sizes = [ 1, 2, 33, 49 ]
properties = Dict()
properties[ [Properties.Random] ] = Nullable()
properties[ [Properties.Random, Properties.Positive] ] = Nullable(x -> @test x >= 0)
properties[ [Properties.Random, Properties.Negative] ] = Nullable(x -> @test x <= 0)

for (prop, verificator) in properties
  for cur_size in matrix_sizes
    verify(cur_size[1], cur_size[2], Shape.General(cur_size[1], cur_size[2]),
      prop, verificator)
  end
end

for (prop, verificator) in properties
  for cur_size in matrix_sq_sizes
    mat = verify(cur_size, cur_size, Shape.SelfAdjoint(cur_size), prop, verificator)
    @test issymmetric(mat)
  end
end
