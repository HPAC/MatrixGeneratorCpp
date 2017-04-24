using Base.Test
using Generator

include("helpers.jl")

matrix_sizes = [ [1, 1], [2, 2], [25, 50], [50, 25] ]
matrix_sq_sizes = [ [1, 1], [2, 2], [33, 33], [49, 49], [50, 50] ]
properties = Dict()
properties[ [Properties.SPD] ] = Nullable()
properties[ [Properties.SPD, Properties.Positive] ] = Nullable(x -> @test x >= 0)

#Incorrect shapes
@test_throws ErrorException generate(Shape.General(1, 2), Set([Properties.SPD]))
@test_throws ErrorException generate(Shape.General(2, 1), Set([Properties.SPD]))
@test_throws ErrorException generate(Shape.Triangular(3, Shape.Upper),
  Set([Properties.SPD]))
@test_throws ErrorException generate(Shape.Triangular(3, Shape.Lower),
  Set([Properties.SPD]))

#For some reason isposdef is not overloaded for SymmetricMatrix
#We have to treat it differently.
# Also for general matrix: for a vector case 1x1 ensure that only element is > 0
types = [ (Shape.General, (x, y) -> Shape.General(x, y), matrix_sq_sizes,
            mat -> @test size(mat, 1) > 1 ? issymmetric(mat) && isposdef(mat) : mat[1] > 0)
          (Shape.Symmetric, (x, y) -> Shape.Symmetric(x), matrix_sq_sizes,
            mat -> @test isposdef(mat.data))
          (Shape.Diagonal, (x, y) -> Shape.Diagonal(x), matrix_sq_sizes,
            mat -> @test isposdef(mat))
        ]

for (datatype, creator, matrix_sizes, gen_func) in types
  for (prop, verificator) in properties
    for cur_size in matrix_sizes
      verify(cur_size[1], cur_size[2], creator(cur_size[1], cur_size[2]),
        prop, verificator, Nullable(gen_func))
    end
  end
end
