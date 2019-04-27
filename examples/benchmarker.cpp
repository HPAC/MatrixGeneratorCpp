//
// Created by mcopik on 07.12.16.
//

#include <array>
#include <string>
#include <iostream>
#include <random>

#include <benchmarker/benchmarker.hpp>
#include <libraries.hpp>

using linalg_tests::benchmarker;

#ifdef HAVE_EIGEN
    /// Eigen kernel as a function object
    struct eigen_kernel
    {
        benchmarker & b;

        eigen_kernel(benchmarker & b_) :
                b(b_) {}

        void operator()(int rows, int cols)
        {
            Eigen::MatrixXd A = Eigen::MatrixXd::Random(rows, cols);
            Eigen::MatrixXd B = Eigen::MatrixXd::Random(rows, cols);

            auto C = (A * B.transpose()).eval();
        }

    };
#endif

int main()
{
    benchmarker benchmark;
    benchmark.set_cache_size(8 * 1024 * 1024);
    int rows = 200, cols = 100;
    int iters = 5;

    // Run a functor without benchmark arg

#ifdef HAVE_ARMADILLO
    std::array<std::string, 2> labels_arma{{"Armadillo", "1"}};
    benchmark.run(std::cout, labels_arma, iters,
                  [=, &benchmark]() {
                      arma::mat A = arma::randu<arma::mat>(rows, cols);
                      arma::mat B = arma::randu<arma::mat>(rows, cols);

                      auto C = (A * B.t() ).eval();
                  });
#endif

#ifdef HAVE_EIGEN
    std::array<std::string, 2> labels_eigen{{"Eigen", "1"}};
    eigen_kernel k(benchmark);
    benchmark.run(std::cout, labels_eigen, iters, k, rows, cols);
#endif

}




