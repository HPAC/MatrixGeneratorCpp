using Base.Test
using Generator

function verify(rows, cols, shape, properties, func)
  mat = generate(shape, Set(properties))
  @test size(mat, 1) == rows
  @test size(mat, 2) == cols
  foreach(get(func), mat)
  return mat
end

matrix_sizes = [ [1, 1], [2, 2], [25, 50], [50, 25] ]
matrix_sq_sizes = [ 1, 2, 33, 49 ]
properties = Dict()
properties[ [Properties.Constant] ] = Nullable(x -> @test x ≈ 0.0)
properties[ [Properties.Constant(0.0)] ] = Nullable(x -> @test x ≈ 0.0)
properties[ [Properties.Constant(1.0)] ] = Nullable(x -> @test x ≈ 1.0)
properties[ [Properties.Constant(-3.0)] ] = Nullable(x -> @test x ≈ -3.0)
properties[ [Properties.Constant(Float64(pi))] ] = Nullable(x -> @test x ≈ pi)


#General matrix
for (prop, verificator) in properties
  for cur_size in matrix_sizes
    verify(cur_size[1], cur_size[2], Shape.General(cur_size[1], cur_size[2]),
      prop, verificator)
  end
end

#Symmetric matrix
for (prop, verificator) in properties
  for cur_size in matrix_sq_sizes
    mat = verify(cur_size, cur_size, Shape.Symmetric(cur_size), prop, verificator)
    @test issymmetric(mat)
  end
end
