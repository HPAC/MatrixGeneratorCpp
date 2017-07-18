using Generator


matrix_sizes = [ [1, 1], [2, 2], [25, 50], [50, 25] ]
matrix_sq_sizes = [ [1, 1], [2, 2], [33, 33], [49, 49] ]
properties = Dict()
properties[ [Properties.Constant] ] = Nullable(x -> @test x ≈ 0.0)
properties[ [Properties.Constant(0.0)] ] = Nullable(x -> @test x ≈ 0.0)
properties[ [Properties.Constant(1.0)] ] = Nullable(x -> @test x ≈ 1.0)
properties[ [Properties.Constant(-3.0)] ] = Nullable(x -> @test x ≈ -3.0)
properties[ [Properties.Constant(Float64(pi))] ] = Nullable(x -> @test x ≈ pi)


#General matrix
types = [ (Shape.General(), matrix_sizes)
          (Shape.Symmetric(), matrix_sq_sizes)
          (Shape.Triangular(Shape.Upper), matrix_sq_sizes)
          (Shape.Triangular(Shape.Lower), matrix_sq_sizes)
          (Shape.Diagonal(), matrix_sq_sizes)
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
