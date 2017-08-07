using Base.Test
using Generator


matrix_sizes = [ [1, 1], [2, 2], [3, 1], [1, 3], [25, 50], [50, 25] ]
matrix_sq_sizes = [ [1, 1], [2, 2], [33, 33], [49, 49] ]
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

#Incorrect properties
@test_throws ErrorException [Properties.Random(0.3, 0)]
@test_throws ErrorException generate((1, 1), Shape.General(),
  Set([Properties.Random(-1.5, 1), Properties.Positive]))
@test_throws ErrorException generate((1, 1), Shape.General(),
  Set([Properties.Random(-0.5, 2), Properties.Negative]))

types = [ (Shape.General(), matrix_sizes)
          (Shape.Symmetric(), matrix_sq_sizes)
          (Shape.UpperTriangular(), matrix_sizes)
          (Shape.LowerTriangular(), matrix_sizes)
          (Shape.Diagonal(), matrix_sizes)
        ]

for (datatype, matrix_sizes) in types
  for (prop, verificator) in properties
    for cur_size in matrix_sizes
      mat = generate([cur_size[1], cur_size[2]], datatype, Set(prop))
      verify(cur_size[1], cur_size[2], datatype, mat, verificator)
    end
  end
end

band_shapes = generate_band_types()

for (size, shape, shape_dst) in band_shapes
  for (prop, verificator) in properties
    mat = generate( size, vcat(shape, prop) )
    verify(size..., shape_dst, mat, verificator, Nullable())
  end
end
