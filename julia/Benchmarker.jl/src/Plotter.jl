
module Plotter

  using ..Results;

  type Plot{T}
    file::IOStream;
    delimiter::Char;

    function Plot(name::String, labels::Array{String, 1} = Array{String, 1}(),
        delimiter::Char = '\t')
      f = open(name, "w");
      full_labels = ["Time" "StdDev" "Min" "Max"];
      if !isempty(labels)
        full_labels = hcat(labels', full_labels);
      end
      writedlm(f, full_labels, delimiter);
      return new{T}(f, delimiter);
    end

  end

  function add_data{T}(p::Plot{T}, timings::Results)
    t = [timings.average_time timings.std_dev timings.min_time timings.max_time];
    writedlm(p.file, t, p.delimiter);
  end

  function add_data{T, P}(p::Plot{T}, data::Array{P, 1}, timings::Results)
    t = [timings.average_time timings.std_dev timings.min_time timings.max_time];
    data_to_write = [data t];
    writedlm(p.file, data_to_write, p.delimiter);
  end

  function finish{T}(p::Plot{T})
    close(p.file);
  end

end
