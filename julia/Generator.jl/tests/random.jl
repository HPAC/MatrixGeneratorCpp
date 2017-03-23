using Base.Test
using Generator

function verify(rows, cols, shape, properties, func)
  mat = generate(shape, Set(properties))
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
properties[ [Properties.Random(0, 2.0)] ] = Nullable(x -> @test x >= 0 && x <= 2)
properties[ [Properties.Random(1.3, 5.5)] ] = Nullable(x -> @test x >= 1.3 && x <= 5.5)
properties[ [Properties.Random(-1.5, 3.3)] ] = Nullable(x -> @test x >= -1.5 && x <= 3.3)
properties[ [Properties.Random(-3, 0)] ] = Nullable(x -> @test x >= -3 && x <= 0)
properties[ [Properties.Random(-4.5, -0.99)] ] = Nullable(x -> @test x >= -4.5 && x <= -0.99)
properties[ [Properties.Random, Properties.Positive] ] = Nullable(x -> @test x >= 0)
properties[ [Properties.Positive, Properties.Random(0, 3)] ] = Nullable(x -> @test x >= 0 && x <= 3)
properties[ [Properties.Random(7, 13), Properties.Positive] ] = Nullable(x -> @test x >= 7 && x <= 13)
properties[ [Properties.Random, Properties.Negative] ] = Nullable(x -> @test x <= 0)
properties[ [Properties.Negative, Properties.Random(-1.5, 0)] ] = Nullable(x -> @test x <= 0)
properties[ [Properties.Random(-8.5, -7), Properties.Negative] ] = Nullable(x -> @test x <= 0)

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
    mat = verify(cur_size, cur_size, Shape.SelfAdjoint(cur_size), prop, verificator)
    @test issymmetric(mat)
  end
end

#Incorrect properties
@test_throws ErrorException [Properties.Random(0.3, 0)]
@test_throws ErrorException generate(Shape.General(1, 1),
  Set([Properties.Random(-1.5, 1), Properties.Positive]))
@test_throws ErrorException generate(Shape.General(1, 1),
  Set([Properties.Random(-0.5, 2), Properties.Negative]))
