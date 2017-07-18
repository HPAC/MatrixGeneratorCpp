using Base.Test
using Generator

matrix_sq_sizes = [ [1, 1], [2, 2], [33, 33], [49, 49], [50, 50] ]
properties = Dict()
properties[ [Properties.Orthogonal] ] = Nullable()
properties[ [Properties.Orthogonal, Properties.Positive] ] = Nullable(x -> @test x >= 0)
properties[ [Properties.Orthogonal, Properties.Negative] ] = Nullable(x -> @test x < 0)

#Incorrect shapes
@test_throws ErrorException generate([1, 2], Shape.General(), Set([Properties.Orthogonal]))
@test_throws ErrorException generate([2, 1], Shape.General(), Set([Properties.Orthogonal]))
@test_throws ErrorException generate([4, 4], Shape.Symmetric(), Set([Properties.Orthogonal]))
@test_throws ErrorException generate([2, 2], Shape.Symmetric(), Set([Properties.Orthogonal]))
@test_throws ErrorException generate([3, 3], Shape.Triangular(Shape.Upper),
  Set([Properties.Orthogonal]))
@test_throws ErrorException generate([3, 3], Shape.Triangular(Shape.Lower),
  Set([Properties.Orthogonal]))

#Incorrect combination
@test_throws ErrorException generate([1, 2], Shape.General(),
    Set([Properties.Orthogonal, Properties.Positive]))
@test_throws ErrorException generate([2, 1], Shape.General(),
    Set([Properties.Orthogonal, Properties.Negative]))

# For orthogonal matrices A^-1 = A'
# Thus A * A' = I
verification_function = (x) -> @test isapprox(x*x', eye(size(x, 1)))

# Only diagonal has supports full set of properties
types = [ (Shape.General(), Dict([Properties.Orthogonal] => Nullable()), matrix_sq_sizes),
          (Shape.Diagonal(), properties, matrix_sq_sizes)
        ]

for (datatype, props, matrix_sizes) in types
  for (prop, verificator) in props
    for cur_size in matrix_sizes
      mat = generate([cur_size[1], cur_size[2]], datatype, Set(prop))
      verify(cur_size[1], cur_size[2], datatype,
        mat, verificator, Nullable(verification_function))
    end
  end
end

shape_types = Set()
push!(shape_types, ([2, 2], [Shape.General], Shape.General()))
push!(shape_types, ([2, 2], [Shape.General, Shape.Diagonal], Shape.Diagonal()))
push!(shape_types, ([4, 4], [Shape.General, Shape.Band(3, 3)], Shape.Band(3, 3)))

for (size, shape, shape_dst) in shape_types
  mat = generate( size, vcat(shape, [Properties.Orthogonal]) )
  verify(size..., shape_dst, mat, Nullable(), Nullable(verification_function))
end
