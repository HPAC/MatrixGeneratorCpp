using Base.Test
using Generator

matrix_sq_sizes = [ [1, 1], [2, 2], [33, 33], [49, 49], [50, 50] ]
properties = Dict()
properties[ [Properties.SPD] ] = Nullable()
properties[ [Properties.SPD, Properties.Positive] ] = Nullable(x -> @test x >= 0)

#Incorrect shapes
@test_throws ErrorException generate([1, 2], Shape.General(), Set([Properties.SPD]))
@test_throws ErrorException generate([2, 1], Shape.General(), Set([Properties.SPD]))
@test_throws ErrorException generate([3, 3], Shape.UpperTriangular(),
  Set([Properties.SPD]))
@test_throws ErrorException generate([3, 3], Shape.LowerTriangular(),
  Set([Properties.SPD]))

#For some reason isposdef is not overloaded for SymmetricMatrix
#We have to treat it differently.
# Also for general matrix: for a vector case 1x1 ensure that only element is > 0
func_general = mat -> @test size(mat, 1) > 1 ? issymmetric(mat) && isposdef(mat) : mat[1] > 0
func_symmetric = mat -> @test isposdef(mat.data)
func_diagonal = mat -> @test isposdef(mat)
types = [ (Shape.General, matrix_sq_sizes,
            func_general)
          (Shape.Symmetric, matrix_sq_sizes,
            func_symmetric)
          (Shape.Diagonal, matrix_sq_sizes,
            func_diagonal)
        ]

for (datatype, matrix_sizes, gen_func) in types
  for (prop, verificator) in properties
    for cur_size in matrix_sizes
      mat = generate([cur_size[1], cur_size[2]], datatype(), Set(prop))
      verify(cur_size[1], cur_size[2], datatype(), mat, verificator, Nullable(gen_func))
    end
  end
end

shape_types = Set()
push!(shape_types, ([2, 2], [Shape.General], Shape.General(), func_general))
push!(shape_types, ([2, 2], [Shape.Symmetric], Shape.Symmetric(), func_symmetric))
push!(shape_types, ([2, 2], [Shape.General, Shape.Diagonal], Shape.Diagonal(), func_diagonal))
push!(shape_types, ([4, 4], [Shape.General, Shape.Band(3, 3)], Shape.Band(3, 3), func_general))
push!(shape_types, ([4, 4], [Shape.General, Shape.Symmetric, Shape.Band(3, 3)], Shape.Band(3, 3), func_symmetric))

for (size, shape, shape_dst, verif_func) in shape_types
  mat = generate(size, vcat(shape, [Properties.SPD]) )
  verify(size[1], size[2], shape_dst, mat, Nullable(), Nullable(verif_func))
end
