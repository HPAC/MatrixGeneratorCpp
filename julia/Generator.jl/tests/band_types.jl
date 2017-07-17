
function generate_band_types(withSymmetric::Bool=true,withTriangular::Bool=true, withBanded::Bool=true)

  shape_types = Set()
  push!(shape_types, ([2, 2], [Shape.General], Shape.General()))
  push!(shape_types, ([3, 2], [Shape.General], Shape.General()))
  # Symmetric
  if withSymmetric
    push!(shape_types, ([3, 3], [Shape.Symmetric], Shape.Symmetric()))
    push!(shape_types, ([3, 3], [Shape.Symmetric, Shape.General], Shape.Symmetric()))
  end
  # Triangular
  if withTriangular
    push!(shape_types, ([3, 3], [Shape.Triangular(Shape.Upper), Shape.General], Shape.Triangular(Shape.Upper)))
    push!(shape_types, ([4, 4], [Shape.Triangular(Shape.Lower), Shape.General], Shape.Triangular(Shape.Lower)))
  end
  push!(shape_types, ([4, 4], [Shape.Triangular(Shape.Lower), Shape.Triangular(Shape.Upper)], Shape.Diagonal()))
  if withBanded
    # Band non-symmetric
    push!(shape_types, ([1, 1], [Shape.General], Shape.Band(0, 0)))
    push!(shape_types, ([2, 2], [Shape.General, Shape.Band(1, 0)], Shape.Band(1, 0)))
    push!(shape_types, ([3, 2], [Shape.General, Shape.Band(1, 1)], Shape.Band(1, 1)))
    push!(shape_types, ([3, 2], [Shape.General, Shape.Band(0, 1)], Shape.Band(0, 1)))
    push!(shape_types, ([5, 4], [Shape.General, Shape.Band(4, 3)], Shape.Band(4, 3)))
    # band sizes with very non-square matrices
    push!(shape_types, ([1, 3], [Shape.General, Shape.Band(0, 1)], Shape.Band(0, 1)))
    push!(shape_types, ([3, 1], [Shape.General, Shape.Band(1, 0)], Shape.Band(1, 0)))
    push!(shape_types, ([5, 2], [Shape.General, Shape.Band(0, 1)], Shape.Band( 0, 1)))
    push!(shape_types, ([2 ,5], [Shape.General, Shape.Band(1, 0)], Shape.Band( 1, 0)))
    push!(shape_types, ([5, 4], [Shape.General, Shape.Band(4, 3), Shape.Band(2, 3)], Shape.Band(2, 3)))
    push!(shape_types, ([5, 5], [Shape.Triangular(Shape.Upper), Shape.Band(4, 3), Shape.Band(2, 3)], Shape.Band(0, 3)))
    push!(shape_types, ([5 ,5], [Shape.Triangular(Shape.Lower), Shape.Band(4, 3), Shape.Band(2, 3)], Shape.Band(2, 0)))
    # Band symmetric
    push!(shape_types, ([1, 1], [Shape.Symmetric], Shape.Band(0, 0)))
    push!(shape_types, ([5, 5], [Shape.Symmetric, Shape.Band(4, 4)], Shape.Band(4, 4)))
    push!(shape_types, ([5, 5], [Shape.Symmetric, Shape.Band(4, 3), Shape.Band(2, 2)], Shape.Band(2, 2)))
    push!(shape_types, ([5, 5], [Shape.Diagonal, Shape.Band(4, 3), Shape.Band(2, 3)], Shape.Band(0, 0)))
  end

  return shape_types
end
