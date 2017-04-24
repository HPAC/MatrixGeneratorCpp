using Base.Test
using Generator

include("helpers.jl")

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
      verify(cur_size[1], cur_size[2], creator(cur_size[1], cur_size[2]),
        prop, verificator, Nullable(verification_function))
    end
  end
end
