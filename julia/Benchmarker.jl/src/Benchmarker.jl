module Benchmarker

	# Benchmarker resources
	#
	# http://docs.julialang.org/en/stable/manual/performance-tips/#measure-performance-with-time-and-pay-attention-to-memory-allocation
	#
	# Wallclock time vs CPU time
	# https://groups.google.com/forum/#!topic/julia-dev/w3NW9JzeV9I
	# https://groups.google.com/forum/#!topic/julia-users/2zB4hajQJuc/discussion
	#
	# Other benchmarkers
	# https://github.com/johnmyleswhite/Benchmarks.jl/blob/master/src/benchmarkable.jl
	# https://github.com/schmrlng/CPUTime.jl/blob/master/src/CPUTime.jl
	function run(iters::Int, f, args...)

		# Compile f and time functions
		tic();
		f(args...);
		toq();

		total_time::Float64 = 0.0;
		for i=1:iters
			tic();
			f(args...);
			total_time = toq();
		end

		println("Average time " * string(total_time / iters));
	end

end
