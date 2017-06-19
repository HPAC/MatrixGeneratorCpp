using Base.Test
using Generator

include("helpers.jl")
include("band_types.jl")

matrix_sq_sizes = [ [1, 1], [2, 2], [33, 33], [49, 49], [50, 50] ]
properties = Dict()
properties[ [Properties.Orthogonal] ] = Nullable()
properties[ [Properties.Orthogonal, Properties.Positive] ] = Nullable(x -> @test x >= 0)
properties[ [Properties.Orthogonal, Properties.Negative] ] = Nullable(x -> @test x < 0)

#Incorrect shapes
@test_throws ErrorException generate(Shape.General(1, 2), Set([Properties.Orthogonal]))
@test_throws ErrorException generate(Shape.General(2, 1), Set([Properties.Orthogonal]))
@test_throws ErrorException generate(Shape.Symmetric(4), Set([Properties.Orthogonal]))
@test_throws ErrorException generate(Shape.Symmetric(2), Set([Properties.Orthogonal]))
@test_throws ErrorException generate(Shape.Triangular(3, Shape.Upper),
  Set([Properties.Orthogonal]))
@test_throws ErrorException generate(Shape.Triangular(3, Shape.Lower),
  Set([Properties.Orthogonal]))

#Incorrect combination
@test_throws ErrorException generate(Shape.General(1, 2),
    Set([Properties.Orthogonal, Properties.Positive]))
@test_throws ErrorException generate(Shape.General(2, 1),
    Set([Properties.Orthogonal, Properties.Negative]))

# For orthogonal matrices A^-1 = A'
# Thus A * A' = I
verification_function = (x) -> @test isapprox(x*x', eye(size(x, 1)))

# Only diagonal has supports full set of properties
types = [ (Shape.General, (x, y) -> Shape.General(x, y), Dict([Properties.Orthogonal] => Nullable()), matrix_sq_sizes),
          (Shape.Diagonal, (x, y) -> Shape.Diagonal(x), properties, matrix_sq_sizes)
        ]

for (datatype, creator, props, matrix_sizes) in types
  for (prop, verificator) in props
    for cur_size in matrix_sizes
      mat = generate(creator(cur_size[1], cur_size[2]), Set(prop))
      verify(cur_size[1], cur_size[2], creator(cur_size[1], cur_size[2]),
        mat, verificator, Nullable(verification_function))
    end
  end
end

shape_types = Set()
push!(shape_types, ([Shape.General(2, 2)], Shape.General(2, 2)))
push!(shape_types, ([Shape.General(2, 2), Shape.Diagonal(2)], Shape.Diagonal(2)))
push!(shape_types, ([Shape.General(4, 4), Shape.Band(4, 4, 3, 3, false)], Shape.Band(4, 4, 3, 3, false)))

for (shape, shape_dst) in shape_types
  mat = generate( vcat(shape, [Properties.Orthogonal]) )
  cols = isa(shape_dst, Shape.General) || isa(shape_dst, Shape.Band) ? shape_dst.cols : shape_dst.rows
  verify(shape_dst.rows, cols, shape_dst, mat, Nullable(), Nullable(verification_function))
end
