

immutable Results

  samples::Int
  average_time::Float64
  std_dev::Float64
  min_time::Float64
  max_time::Float64

  function Results(n::Int, timings::Array{Float64, 1})

    samples = n;
    average_time = mean(timings);
    std_dev = stdm(timings, average_time);
    min_time = minimum(timings);
    max_time = maximum(timings);

    new(samples, average_time, std_dev, min_time, max_time)
  end

end
