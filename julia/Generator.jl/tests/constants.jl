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
types = [ (Shape.General, (x, y) -> Shape.General(x, y), matrix_sizes)
          (Shape.Symmetric, (x, y) -> Shape.Symmetric(x), matrix_sq_sizes)
          (Shape.Triangular, (x, y) -> Shape.Triangular(x, Shape.Upper), matrix_sq_sizes)
          (Shape.Triangular, (x, y) -> Shape.Triangular(x, Shape.Lower), matrix_sq_sizes)
          (Shape.Diagonal, (x, y) -> Shape.Diagonal(x), matrix_sq_sizes)
        ]

for (datatype, creator, matrix_sizes) in types
  for (prop, verificator) in properties
    for cur_size in matrix_sizes
      mat = generate(creator(cur_size[1], cur_size[2]), Set(prop))
      verify(cur_size[1], cur_size[2], creator(cur_size[1], cur_size[2]),
        mat, verificator)
    end
  end
end

band_shapes = generate_band_types()

for (shape, shape_dst) in band_shapes
  for (prop, verificator) in properties
    mat = generate( vcat(shape, prop) )
    cols = isa(shape_dst, Shape.General) || isa(shape_dst, Shape.Band) ? shape_dst.cols : shape_dst.rows
    verify(shape_dst.rows, cols, shape_dst, mat, verificator)
  end
end
