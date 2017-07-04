using Base.Test
using Generator


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
func_general = mat -> @test size(mat, 1) > 1 ? issymmetric(mat) && isposdef(mat) : mat[1] > 0
func_symmetric = mat -> @test isposdef(mat.data)
func_diagonal = mat -> @test isposdef(mat)
types = [ (Shape.General, (x, y) -> Shape.General(x, y), matrix_sq_sizes,
            func_general)
          (Shape.Symmetric, (x, y) -> Shape.Symmetric(x), matrix_sq_sizes,
            func_symmetric)
          (Shape.Diagonal, (x, y) -> Shape.Diagonal(x), matrix_sq_sizes,
            func_diagonal)
        ]

for (datatype, creator, matrix_sizes, gen_func) in types
  for (prop, verificator) in properties
    for cur_size in matrix_sizes
      mat = generate(creator(cur_size[1], cur_size[2]), Set(prop))
      verify(cur_size[1], cur_size[2], creator(cur_size[1], cur_size[2]),
        mat, verificator, Nullable(gen_func))
    end
  end
end

shape_types = Set()
push!(shape_types, ([Shape.General(2, 2)], Shape.General(2, 2), func_general))
push!(shape_types, ([Shape.Symmetric(2)], Shape.Symmetric(2), func_symmetric))
push!(shape_types, ([Shape.General(2, 2), Shape.Diagonal(2)], Shape.Diagonal(2), func_diagonal))
push!(shape_types, ([Shape.General(4, 4), Shape.Band(4, 4, 3, 3, false)], Shape.Band(4, 4, 3, 3, false), func_general))
push!(shape_types, ([Shape.General(4, 4), Shape.Band(4, 4, 3, 3, true)], Shape.Band(4, 4, 3, 3, true), func_symmetric))

for (shape, shape_dst, verif_func) in shape_types
  mat = generate( vcat(shape, [Properties.SPD]) )
  cols = isa(shape_dst, Shape.General) || isa(shape_dst, Shape.Band) ? shape_dst.cols : shape_dst.rows
  verify(shape_dst.rows, cols, shape_dst, mat, Nullable(), Nullable(verif_func))
end
