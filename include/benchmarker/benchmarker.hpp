
#ifndef LINALG_TESTS_BENCHMARK_HPP
#define LINALG_TESTS_BENCHMARK_HPP

#include <vector>
#include <chrono>
#include <type_traits>
#include <algorithm>
#include <ostream>
#include <tuple>
#include <memory>
#include <random>

namespace linalg_tests {

    template<typename Duration>
    class basic_benchmarker;

    namespace detail {

        struct call_helper
        {
            template<typename Duration, typename F, typename... Args>
            static auto call(basic_benchmarker<Duration> & b, F && f, Args &&... args)
                    -> std::result_of_t<F(basic_benchmarker<Duration>&, Args...)>
            {
                return f(b, std::forward<Args>(args)...);
            }

            template<typename Duration, typename F, typename... Args>
            static auto call(basic_benchmarker<Duration> &, F && f, Args &&... args)
            -> std::result_of_t<F(Args...)>
            {
                return f(std::forward<Args>(args)...);
            }
        };

    }

    struct measurement_data
    {
        //  min, max, avg, stddev
        typedef std::tuple<double, double, double, double> measurement_t;
        measurement_t data;

        measurement_data(double min, double max, double avg, double stddev)
                : data{min, max, avg, stddev}
        {}

        double max() const
        {
            return std::get<0>(data);
        }

        double min() const
        {
            return std::get<1>(data);
        }

        double avg() const
        {
            return std::get<2>(data);
        }

        double std_dev() const
        {
            return std::get<3>(data);
        }

    };

    /**
     * Simple benchmarker capable of running a function and gathering measurements
     * from multiple clocks.
     * Clocks are simply identified by integers. Clocks can not be reused in one rune,
     * but this can be changed.
     *
     * Pointers to member functions are not supported. If necessary, they can be implemented
     * by extending detail::call_helper with C++17 std::invoke
     *
     * @tparam Duration
     */
    template<typename Duration = std::chrono::milliseconds>
    class basic_benchmarker {
        // multiple measurements
        std::vector< std::vector<measurement_data> > measurements;

        // clock type
        typedef std::chrono::high_resolution_clock clock_t2;
        std::vector<double> clock_measurements;
        std::vector<typename clock_t2::time_point> clocks;
        uint32_t clocks_count;
        int clocks_reserved;

        // cache trash
        std::unique_ptr<double> cache_trash;
        uint32_t trash_size;

        const std::string output_separator = "\t";

        const int TOTAL_TIME = -1;

    public:

        basic_benchmarker() :
                clock_measurements(1),
                clocks(1),
                clocks_count(1),
                clocks_reserved(0),
                cache_trash(nullptr)
        {

        }

        /// Pass L3 cache size in bytes
        /// \param l3_size
        void set_cache_size(int l3_size)
        {
            trash_size = l3_size / sizeof(double);
            cache_trash.reset(new double[trash_size]);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0, 1);
            std::for_each(cache_trash.get(), cache_trash.get() + trash_size,
                [&](double & val) {
                   val = dis(gen);
                });
        }

        void reserve_clocks(int count)
        {
            clocks.resize(count + 1);
            clocks_reserved = count;
        }

        int add_clock()
        {
            ++clocks_count;
            if(clocks_count > clocks.size()) {
                clocks.emplace_back();
                clock_measurements.emplace_back();
            }
            // At least one clock is present (system) + one user-defined
            // First id should be zero
            return clocks_count - 2;
        }

        void start_clock(int id)
        {
            clocks[id + 1] = clock_t2::now();
        }

        void stop_clock(int id)
        {
            auto x = clock_t2::now();
            clock_measurements[id + 1] = std::chrono::duration_cast<Duration>(
                    x - clocks[id + 1]
                ).count();
        }

        void stop_clock()
        {
            stop_clock(clocks_count - 2);
        }

        template<typename F, typename ... Args>
        void run(uint32_t iters, F && f, Args &&... args)
        {
            clocks_count = clocks_reserved + 1;
            std::fill(clock_measurements.begin(), clock_measurements.end(), 0.0);

            // First run
            detail::call_helper::call(*this, std::forward<F>(f), std::forward<Args>(args)...);

            std::vector<double> data(iters * clocks_count);

            for (uint32_t i = 0; i < iters; ++i) {
                clocks_count = clocks_reserved + 1;
                trash_cache();

                start_clock(TOTAL_TIME);
                //auto begin = std::clock();
                detail::call_helper::call(*this, std::forward<F>(f), std::forward<Args>(args)...);
                //double time_spent = (double)(std::clock() - begin) / CLOCKS_PER_SEC;
                //stop_clock();
                stop_clock(TOTAL_TIME);

                //std::cout << time_spent << std::endl;

                for (size_t j = 0; j < clocks_count; ++j)//{
                    data[j * iters + i] = clock_measurements[j];
                //std::cout << clocks_count << " " << data[j*iters + i] << " " << clock_measurements[j] << std::endl;}
            }

            // compute statistics
            auto begin = data.begin(), end = begin + iters;
            std::vector<measurement_data> results;
            for (size_t j = 0; j < clocks_count; ++j) {
                double max = *std::max_element(begin, end);
                double min = *std::min_element(begin, end);
                double mean = std::accumulate(begin, end, 0.0) / iters;
                // shift by mean
                std::for_each(begin, end, [=](double &v) { v -= mean; });
                // sqrt(sum of squares / size)
                double std_dev = std::sqrt(std::inner_product(begin, end, begin, 0.0) / (iters-1) );

                results.emplace_back(max, min, mean, std_dev);

                begin += iters;
                end += iters;
            }
            measurements.push_back(std::move(results));
        }

        measurement_data get(int run, int clock)
        {
            return measurements[run][clock];
        }

        /// size of runs_labels needs to be greater or equal to number of runs
        template<typename CLabels, typename RLabels>
        void print_results(std::ostream & os, RLabels && runs_labels, CLabels && clock_labels)
        {
            auto label = std::begin(runs_labels);
            for(auto & run : measurements)
            {
                auto clock_label = std::begin(clock_labels);
                os << "Run" << output_separator << *label++ << '\n';
                auto data_begin = std::begin(run), data_end = std::end(run);
                print(os, "Total", *data_begin++);
                for(; data_begin != data_end; ++data_begin)
                {
                    print(os, *clock_label++, *data_begin);
                }
                os << '\n';
            }
        }

        /// size of runs_labels needs to be greater or equal to number of runs
        template<typename RLabels>
        void output_results(std::ostream & os, RLabels && runs_labels)
        {
            auto label = std::begin(runs_labels);
            os << "algorithm\tTime\tStdDev\tMin\tMax\n";
            for(auto & run : measurements)
            {
                os << *label++ << '\t';
                measurement_data & data = run[0];
                os << data.avg() << '\t' << data.std_dev() << '\t';
                os << data.min() << '\t' << data.max() << '\n';
            }
        }

    private:

        void trash_cache()
        {
            if(cache_trash) {
                std::for_each(cache_trash.get(), cache_trash.get() + trash_size,
                    [](double & val) {
                      val += 0.01;
                    });
            }
        }

        void print(std::ostream & os, const std::string & label, const measurement_data & data)
        {
            os << "Clock: " << output_separator << label << '\n';
            os << "Average: " << output_separator << data.avg() << '\n';
            os << "Standard deviation: " << data.std_dev() << output_separator
               << 100 * data.std_dev() / data.avg() << "% \n";
            os << "Best: " << output_separator << data.min() << output_separator
               << 100*(data.min() - data.avg()) / data.avg() << "% \n";
            os << "Worst: " << output_separator << data.max() << output_separator
               << "+" << 100*(data.max() - data.avg()) / data.avg() << "% \n";
        }

    };

    typedef basic_benchmarker<std::chrono::microseconds> benchmarker;
}


#endif //LINALG_TESTS_BENCHMARK_HPP
