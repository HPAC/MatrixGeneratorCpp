
module Generator

  export GeneratorImpl
  export generator, generate

  include("Shape.jl")
  include("Properties.jl")

  type GeneratorImpl

    function GeneratorImpl()
      return new()
    end

  end
  const generator = GeneratorImpl()#Dict([1]=>1)
  println(typeof(generator))

  function generate(gen::GeneratorImpl, shape, properties)
  end

end
