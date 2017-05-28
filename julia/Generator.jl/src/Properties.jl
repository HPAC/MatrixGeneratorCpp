

module Properties

  export Random, Positive, Negative

  immutable Random
    lower_bound::Float64
    upper_bound::Float64

    function Random(lower_bound, upper_bound)
      if lower_bound > upper_bound
        throw(ErrorException( @sprintf("Incorrect random boundaries: lower bound %f
            greater than upper bound %f", lower_bound,upper_bound) ))
      end
      return new(lower_bound, upper_bound)
    end
  end

  immutable Positive
  end

  immutable Negative
  end

  immutable Symmetric
  end

  immutable Constant
    value::Float64

    function Constant(value::Float64 = 0.0)
      return new(value)
    end
  end

  immutable SPD
  end

  immutable Orthogonal
  end

end
