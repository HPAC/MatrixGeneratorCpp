using Base.Test
using Generator

other_properties = Set()
push!(other_properties, [])
push!(other_properties, [Properties.Constant])
push!(other_properties, [Properties.Constant(3.)])
push!(other_properties, [Properties.Random])
push!(other_properties, [Properties.Constant(3.), Properties.Random])
push!(other_properties, [Properties.Random(1, 2), Properties.SPD])
push!(other_properties, [Properties.Orthogonal, Properties.Random(2, 3)])

# General
shape_types = Set()
push!(shape_types, ([2, 2], [Shape.General], [false, Shape.Band(1, 1)]))
push!(shape_types, ([3, 2],[Shape.General], [false, Shape.Band(2, 1)]))
# Symmetric
push!(shape_types, ([3, 3], [Shape.Symmetric], [true, Shape.Band(2, 2)]))
push!(shape_types, ([3, 3], [Shape.Symmetric, Shape.General], [true, Shape.Band(2, 2)]))
# Triangular
push!(shape_types, ([3, 3], [Shape.UpperTriangular()], [false, Shape.Band(0, 2)]))
push!(shape_types, ([4, 3], [Shape.LowerTriangular()], [false, Shape.Band(3, 0)]))
push!(shape_types, ([3, 4], [Shape.UpperTriangular(), Shape.General], [false, Shape.Band(0, 3)]))
# upper & lower -> Diagonal
push!(shape_types, ([4, 4], [Shape.LowerTriangular(), Shape.UpperTriangular()], [false, Shape.Band(0, 0)]))
# Diagonal
push!(shape_types, ([3, 3], [Shape.Diagonal], [false, Shape.Band(0, 0)]))
push!(shape_types, ([3, 3], [Shape.Diagonal, Shape.UpperTriangular()], [false, Shape.Band(0, 0)]))
push!(shape_types, ([5, 5], [Shape.General, Shape.Diagonal], [false, Shape.Band(0, 0)]))
push!(shape_types, ([3, 3], [Shape.Diagonal, Shape.UpperTriangular(), Shape.General], [false, Shape.Band(0, 0)]))
push!(shape_types, ([3, 3], [Shape.Diagonal, Shape.LowerTriangular(), Shape.Symmetric], [true, Shape.Band(0, 0)]))
# Band
push!(shape_types, ([2, 2], [Shape.General, Shape.Band(1, 0)], [false, Shape.Band(1, 0)]))
push!(shape_types, ([5, 4], [Shape.General, Shape.Band(4, 3)], [false, Shape.Band(4, 3)]))
push!(shape_types, ([5, 4], [Shape.General, Shape.Band(4, 3), Shape.Band(2, 3)], [false, Shape.Band(2, 3)]))
push!(shape_types, ([5, 5], [Shape.Symmetric, Shape.Band(4, 3), Shape.Band(2, 2)], [true, Shape.Band(2, 2)]))
push!(shape_types, ([5, 4], [Shape.UpperTriangular(), Shape.Band(4, 3), Shape.Band(2, 3)], [false, Shape.Band(0, 3)]))
push!(shape_types, ([4, 5], [Shape.LowerTriangular(), Shape.Band(4, 3), Shape.Band(2, 3)], [false, Shape.Band(2, 0)]))
push!(shape_types, ([5, 5], [Shape.Diagonal, Shape.Band(4, 3), Shape.Band(2, 3)], [false, Shape.Band(0, 0)]))

# errors
#error_types = []
#push!(error_types, [Shape.General(2, 2), Shape.General(2, 1)])
#push!(error_types, [Shape.General(2, 2), Shape.Symmetric(3)])
#push!(error_types, [Shape.General(2, 2), Shape.Symmetric(3), Shape.Triangular(5, Shape.Upper)])
# clash between symmetry and unsymmetric size
#push!(error_types, [Shape.General(2, 1), Properties.Symmetric])
#push!(error_types, [Shape.Symmetric(3), Shape.General(5, 4)])
#push!(error_types, ([Shape.Triangular(3, Shape.Lower), Shape.Symmetric(3)]))
#push!(error_types, ([Shape.Triangular(3, Shape.Lower), Shape.Symmetric(3), Shape.Band(3, 3, 2, 0, false)]))
#push!(error_types, ([Shape.Triangular(3, Shape.Lower), Shape.Symmetric(3), Shape.Band(3, 3, 2, 0, true)]))

for shape in shape_types

  for properties in other_properties
    ret = Generator.get_shape_type( shape[1], vcat(shape[2], properties) )
    # TEST: correct returned shape
    @test shape[3][2] == ret[1]
    # TEST: symmetry'
    @test shape[3][1] == ret[2]
    # TEST: correct properties
    @test ret[3] == properties
  end

end

#for shape in error_types
#  for properties in other_properties
    # TEST: incorrect matrix types & properties
#    @test_throws ErrorException Generator.get_shape_type( vcat(shape, properties) )
#  end
#end
