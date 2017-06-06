using Base.Test
using Generator

include("helpers.jl")

matrix_sizes = [ [1, 1], [2, 2], [25, 50], [50, 25] ]
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
@test_throws ErrorException generate(Shape.General(1, 1),
  Set([Properties.Random(-1.5, 1), Properties.Positive]))
@test_throws ErrorException generate(Shape.General(1, 1),
  Set([Properties.Random(-0.5, 2), Properties.Negative]))

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

band_shapes = Set()
# General
shape_types = Set()
push!(band_shapes, ([Shape.General(2, 2)], Shape.General(2, 2)))
push!(shape_types, ([Shape.General(3, 2)], Shape.General(3, 2)))
# Symmetric
push!(shape_types, ([Shape.Symmetric(3)], Shape.Symmetric(3)))
push!(shape_types, ([Shape.Symmetric(3), Shape.General(3, 3)], Shape.Symmetric(3)))
# Triangular
push!(shape_types, ([Shape.Triangular(3, Shape.Upper), Shape.General(3, 3)], Shape.Triangular(3, Shape.Upper)))
push!(shape_types, ([Shape.Triangular(4, Shape.Lower), Shape.General(4, 4)], Shape.Triangular(4, Shape.Lower)))
push!(shape_types, ([Shape.Triangular(4, Shape.Lower), Shape.Triangular(4, Shape.Upper)], Shape.Diagonal(4)))
# Band non-symmetric
push!(shape_types, ([Shape.General(1, 1)], Shape.Band(1, 1, 0, 0, false)))
push!(shape_types, ([Shape.General(2, 2), Shape.Band(2, 2, 1, 0, false)], Shape.Band(2, 2, 1, 0, false)))
push!(shape_types, ([Shape.General(3, 2), Shape.Band(3, 2, 1, 1, false)], Shape.Band(3, 2, 1, 1, false)))
push!(shape_types, ([Shape.General(3, 2), Shape.Band(3, 2, 0, 1, false)], Shape.Band(3, 2, 0, 1, false)))
push!(shape_types, ([Shape.General(5, 4), Shape.Band(5, 4, 4, 3, false)], Shape.Band(5, 4, 4, 3, false)))
# band sizes with very non-square matrices
push!(shape_types, ([Shape.General(5, 2), Shape.Band(5, 2, 0, 1, false)], Shape.Band(5, 2, 0, 1, false)))
push!(shape_types, ([Shape.General(2, 5), Shape.Band(2, 5, 1, 0, false)], Shape.Band(2, 5, 1, 0, false)))
push!(shape_types, ([Shape.General(5, 4), Shape.Band(5, 4, 4, 3, false), Shape.Band(5, 4, 2, 3, false)], Shape.Band(5, 4, 2, 3, false)))
push!(shape_types, ([Shape.Triangular(5, Shape.Upper), Shape.Band(5, 5, 4, 3, false), Shape.Band(5, 5, 2, 3, false)], Shape.Band(5, 5, 0, 3, false)))
push!(shape_types, ([Shape.Triangular(5, Shape.Lower), Shape.Band(5, 5, 4, 3, false), Shape.Band(5, 5, 2, 3, false)], Shape.Band(5, 5, 2, 0, false)))
# Band symmetric
#push!(shape_types, ([Shape.Symmetric(1, 1)], Shape.Band(1, 1, 0, 0, true)))
#push!(shape_types, ([Shape.Symmetric(5), Shape.Band(5, 5, 4, 3, false), Shape.Band(5, 5, 2, 2, false)], Shape.Band(5, 5, 2, 2, true)))
#push!(shape_types, ([Shape.Diagonal(5), Shape.Band(5, 5 , 4, 3, false), Shape.Band(5, 5, 2, 3, false)], Shape.Band(5, 5, 0, 0, true)))

for (shape, shape_dst) in shape_types
  for (prop, verificator) in properties
    mat = generate( vcat(shape, prop) )
    println(shape)
    println(shape_dst)
    println(mat)
    cols = isa(shape_dst, Shape.General) || isa(shape_dst, Shape.Band) ? shape_dst.cols : shape_dst.rows
    verify(shape_dst.rows, cols, shape_dst, mat, verificator)
  end
end
