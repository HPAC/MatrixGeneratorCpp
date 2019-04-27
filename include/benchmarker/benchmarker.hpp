
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

    class basic_benchmarker;

    namespace detail {

        struct call_helper
        {
            template<typename F, typename... Args>
            static auto call(basic_benchmarker & b, F && f, Args &&... args)
                    -> std::result_of_t<F(basic_benchmarker&, Args...)>
            {
                return f(b, std::forward<Args>(args)...);
            }

            template<typename F, typename... Args>
            static auto call(basic_benchmarker &, F && f, Args &&... args)
            -> std::result_of_t<F(Args...)>
            {
                return f(std::forward<Args>(args)...);
            }
        };

    }

    /**
     * Simple benchmarker capable of running a function and gathering measurements
     *
     * Pointers to member functions are not supported. If necessary, they can be implemented
     * by extending detail::call_helper with C++17 std::invoke
     *
     */
    class basic_benchmarker {

        std::vector<double> measurements;

        double dtime_save;
        typedef std::chrono::high_resolution_clock clock;
        clock::time_point dtime;

        // cache trash
        std::unique_ptr<double> cache_trash;
        uint32_t trash_size;

        const std::string output_separator = "\t";

    public:

        basic_benchmarker() :
                cache_trash(nullptr)
        {}

        void set_cache_size(int l3_size)
        {
            trash_size = l3_size / sizeof(double);
            cache_trash.reset(new double[trash_size]);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0, 1);
            std::for_each(cache_trash.get(), cache_trash.get() + trash_size, [&](double & val) {val = dis(gen);});
        }

        void start_clock()
        {
            dtime = clock::now();
        }

        void stop_clock()
        {
            auto x = clock::now();
            dtime_save = std::chrono::duration_cast<std::chrono::nanoseconds>(x - dtime).count() / 1e9;
        }

        template<typename F, typename ULabels, typename ... Args>
        void run(std::ostream & os, ULabels && user_labels, uint32_t iters, F && f, Args &&... args)
        {
            measurements.resize(iters);

            for (uint32_t i = 0; i < iters; ++i) {
                trash_cache();

                start_clock();
                detail::call_helper::call(*this, std::forward<F>(f), std::forward<Args>(args)...);
                stop_clock();
                measurements[i] = dtime_save;
            }

            print_results(os, user_labels);

        }

        /// size of runs_labels needs to be greater or equal to number of runs
        template<typename ULabels>
        void print_results(std::ostream & os, ULabels && user_labels)
        {
            for(auto & user_label : user_labels)
                os << user_label << output_separator;
            for(auto & measurement : measurements)
                os << measurement << output_separator;
            os << std::endl;
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

    };

    typedef basic_benchmarker benchmarker;
}


#endif //LINALG_TESTS_BENCHMARK_HPP
