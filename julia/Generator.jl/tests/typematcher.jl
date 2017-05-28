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
push!(shape_types, Pair([Shape.General(2, 2)], Shape.Band(2, 2, 1, 1)))


for shape in shape_types

  for properties in other_properties
    ret = Generator.get_shape_type( vcat(shape[1], properties) )
    @test shape[2] == ret[1]
    @test properties == ret[2]
  end

end
