
using .Shape;
using .Properties;

include("Random.jl")

function define_spd(functions)

  functions[ Set([Properties.SPD]) ] =
    (shape, props) -> spd(shape, props, false);

  functions[ Set([Properties.SPD, Properties.Positive]) ] =
    (shape, props) -> spd(shape, props, true);

end


function spd(shape::Shape.General, properties, positive::Bool)

  if shape.rows != shape.cols
    throw(ErrorException("A not square matrix cannot be symmetric positive definite!"))
  else
    return spd(Shape.Symmetric(shape.rows), properties, positive).data
  end

end

"""
  Generate an SPD matrix by using the property that for each invertible matrix A,
  A' * A is SPD. A randomized matrix may be singular, although this case is
  extremely unlikely. To avoid additional check for determinant, we use the
  Cholesky factorization and create a triangular matrix.
  Triangular matrices are non-singular iff all diagonal entries are non-zero.
  The proof is trivial - express matrix determinant with Laplace expansion, starting
  from a first row.
"""
function spd(shape::Shape.Symmetric, properties, _positive::Bool)

  if _positive
    mat = random(Shape.Triangular(shape.rows, Shape.Upper),
      Set([Properties.Random]), positive)
  else
    mat = random(Shape.Triangular(shape.rows, Shape.Upper),
      Set([Properties.Random(-1, 1)]), none)
  end
  # Avoid very low determinant
  mat = mat + eye(shape.rows)*3
  return Symmetric(mat' * mat)

end

function spd(shape::Shape.Triangular, properties, positive::Bool)
  throw(ErrorException("Triangular matrix cannot be symmetric positive definite!"))
end

"""
  A diagonal matrix is SPD iff all entries are positive.
"""
function spd(shape::Shape.Diagonal, properties, positive::Bool)
  return Diagonal( vec(rand(shape.rows, 1)) )
end
