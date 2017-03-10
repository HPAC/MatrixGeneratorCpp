

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

function Base.show(io::IO, r::Results)
  @printf(io, "Benchmarking results\n")
  @printf(io, "Function repeated: %d times\n", r.samples)
  @printf(io, "Average: %f\n", r.average_time)
  @printf(io, "Standard deviation: %f\n", r.std_dev)
  @printf(io, "Best: %f %f%%\n", r.min_time,
    100 * (r.min_time - r.average_time) / r.average_time)
  @printf(io, "Worst: %f +%f%%\n", r.max_time,
    100 * (r.max_time - r.average_time) / r.average_time)
end
