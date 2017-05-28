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
push!(shape_types, ([Shape.General(2, 2)], Shape.Band(2, 2, 1, 1), false))
push!(shape_types, ([Shape.General(3, 2)], Shape.Band(3, 2, 2, 1), false))
# Symmetric
push!(shape_types, ([Shape.Symmetric(3)], Shape.Band(3, 3, 2, 2), true))
push!(shape_types, ([Shape.Symmetric(3), Shape.General(3, 3)], Shape.Band(3, 3, 2, 2), true))
# Triangular
push!(shape_types, ([Shape.Triangular(3, Shape.Upper)], Shape.Band(3, 3, 0, 2), true))
push!(shape_types, ([Shape.Triangular(4, Shape.Lower)], Shape.Band(4, 4, 3, 0), true))
push!(shape_types, ([Shape.Triangular(3, Shape.Upper), Shape.General(3, 3)], Shape.Band(3, 3, 0, 2), true))
push!(shape_types, ([Shape.Triangular(3, Shape.Lower), Shape.Symmetric(3)], Shape.Band(3, 3, 2, 0), true))
# upper & lower -> Diagonal
push!(shape_types, ([Shape.Triangular(4, Shape.Lower), Shape.Triangular(4, Shape.Upper)], Shape.Band(4, 4, 0, 0), true))
# Diagonal
push!(shape_types, ([Shape.Diagonal(3)], Shape.Band(3, 3, 0, 0), true))
push!(shape_types, ([Shape.Diagonal(3), Shape.Triangular(3, Shape.Upper)], Shape.Band(3, 3, 0, 0), true))
push!(shape_types, ([Shape.General(5, 5), Shape.Diagonal(5)], Shape.Band(5, 5, 0, 0), true))
push!(shape_types, ([Shape.Diagonal(3), Shape.Triangular(3, Shape.Upper), Shape.General(3, 3)], Shape.Band(3, 3, 0, 0), true))
push!(shape_types, ([Shape.Diagonal(3), Shape.Triangular(3, Shape.Lower), Shape.Symmetric(3)], Shape.Band(3, 3, 0, 0), true))
# Band
push!(shape_types, ([Shape.General(2, 2), Shape.Band(2, 2, 1, 0)], Shape.Band(2, 2, 1, 0), false))
push!(shape_types, ([Shape.General(5, 4), Shape.Band(5, 4, 4, 3)], Shape.Band(5, 4, 4, 3), false))
push!(shape_types, ([Shape.General(5, 4), Shape.Band(5, 4, 4, 3), Shape.Band(5, 4, 2, 3)], Shape.Band(5, 4, 2, 3), false))
push!(shape_types, ([Shape.Symmetric(5), Shape.Band(5, 5, 4, 3), Shape.Band(5, 5, 2, 3)], Shape.Band(5, 5, 2, 3), true))
push!(shape_types, ([Shape.Triangular(5, Shape.Upper), Shape.Band(5, 5, 4, 3), Shape.Band(5, 5, 2, 3)], Shape.Band(5, 5, 0, 3), true))
push!(shape_types, ([Shape.Triangular(5, Shape.Lower), Shape.Band(5, 5, 4, 3), Shape.Band(5, 5, 2, 3)], Shape.Band(5, 5, 2, 0), true))
push!(shape_types, ([Shape.Diagonal(5), Shape.Band(5, 5 , 4, 3), Shape.Band(5, 5, 2, 3)], Shape.Band(5, 5, 0, 0), true))

# errors
error_types = []
push!(error_types, [Shape.General(2, 2), Shape.General(2, 1)])
push!(error_types, [Shape.General(2, 2), Shape.Symmetric(3)])
push!(error_types, [Shape.General(2, 2), Shape.Symmetric(3), Shape.Triangular(5, Shape.Upper)])

for shape in shape_types

  for properties in other_properties
    ret = Generator.get_shape_type( vcat(shape[1], properties) )
    @test shape[2] == ret[1]
    # if true then symmetry property should be added
    if shape[3]
      @test ret[2][1:length(ret[2])-1] == properties
      @test ret[2][length(ret[2])] == Properties.Symmetric
    else
      @test properties == ret[2]
    end
  end

end

for shape in error_types
  for properties in other_properties
    @test_throws ErrorException Generator.get_shape_type( vcat(shape, properties) )
  end
end
