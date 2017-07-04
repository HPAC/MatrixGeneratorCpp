
function generate_band_types(withSymmetric::Bool=true,withTriangular::Bool=true, withBanded::Bool=true)

  shape_types = Set()
  push!(shape_types, ([Shape.General(2, 2)], Shape.General(2, 2)))
  push!(shape_types, ([Shape.General(3, 2)], Shape.General(3, 2)))
  # Symmetric
  if withSymmetric
    push!(shape_types, ([Shape.Symmetric(3)], Shape.Symmetric(3)))
    push!(shape_types, ([Shape.Symmetric(3), Shape.General(3, 3)], Shape.Symmetric(3)))
  end
  # Triangular
  if withTriangular
    push!(shape_types, ([Shape.Triangular(3, Shape.Upper), Shape.General(3, 3)], Shape.Triangular(3, Shape.Upper)))
    push!(shape_types, ([Shape.Triangular(4, Shape.Lower), Shape.General(4, 4)], Shape.Triangular(4, Shape.Lower)))
  end
  push!(shape_types, ([Shape.Triangular(4, Shape.Lower), Shape.Triangular(4, Shape.Upper)], Shape.Diagonal(4)))
  if withBanded
    # Band non-symmetric
    push!(shape_types, ([Shape.General(1, 1)], Shape.Band(1, 1, 0, 0, false)))
    push!(shape_types, ([Shape.General(2, 2), Shape.Band(2, 2, 1, 0, false)], Shape.Band(2, 2, 1, 0, false)))
    push!(shape_types, ([Shape.General(3, 2), Shape.Band(3, 2, 1, 1, false)], Shape.Band(3, 2, 1, 1, false)))
    push!(shape_types, ([Shape.General(3, 2), Shape.Band(3, 2, 0, 1, false)], Shape.Band(3, 2, 0, 1, false)))
    push!(shape_types, ([Shape.General(5, 4), Shape.Band(5, 4, 4, 3, false)], Shape.Band(5, 4, 4, 3, false)))
    # band sizes with very non-square matrices
    push!(shape_types, ([Shape.General(1, 3), Shape.Band(1, 3, 0, 1, false)], Shape.Band(1, 3, 0, 1, false)))
    push!(shape_types, ([Shape.General(3, 1), Shape.Band(3, 1, 1, 0, false)], Shape.Band(3, 1, 1, 0, false)))
    push!(shape_types, ([Shape.General(5, 2), Shape.Band(5, 2, 0, 1, false)], Shape.Band(5, 2, 0, 1, false)))
    push!(shape_types, ([Shape.General(2, 5), Shape.Band(2, 5, 1, 0, false)], Shape.Band(2, 5, 1, 0, false)))
    push!(shape_types, ([Shape.General(5, 4), Shape.Band(5, 4, 4, 3, false), Shape.Band(5, 4, 2, 3, false)], Shape.Band(5, 4, 2, 3, false)))
    push!(shape_types, ([Shape.Triangular(5, Shape.Upper), Shape.Band(5, 5, 4, 3, false), Shape.Band(5, 5, 2, 3, false)], Shape.Band(5, 5, 0, 3, false)))
    push!(shape_types, ([Shape.Triangular(5, Shape.Lower), Shape.Band(5, 5, 4, 3, false), Shape.Band(5, 5, 2, 3, false)], Shape.Band(5, 5, 2, 0, false)))
    # Band symmetric
    push!(shape_types, ([Shape.Symmetric(1)], Shape.Band(1, 1, 0, 0, true)))
    push!(shape_types, ([Shape.Symmetric(5), Shape.Band(5, 5, 4, 4, false)], Shape.Band(5, 5, 4, 4, true)))
    push!(shape_types, ([Shape.Symmetric(5), Shape.Band(5, 5, 4, 3, false), Shape.Band(5, 5, 2, 2, false)], Shape.Band(5, 5, 2, 2, true)))
    push!(shape_types, ([Shape.Diagonal(5), Shape.Band(5, 5 , 4, 3, false), Shape.Band(5, 5, 2, 3, false)], Shape.Band(5, 5, 0, 0, true)))
  end

  return shape_types
end
