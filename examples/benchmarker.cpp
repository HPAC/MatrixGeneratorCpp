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

#ifdef HAVE_BLAZE
    /// Blaze kernel as a function
    /// Random initialization takes more time due to:
    /// a) lack of randomly initialized matrices
    /// b) unnecessary copy caused by forEach being an expression
    ///   evaluated on assignment
    /// \param b
    /// \param rows
    /// \param cols
    /// \return
    static void blaze_kernel(benchmarker & b, int rows, int cols)
    {
        int idx = b.add_clock();
        b.start_clock( idx );
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1);
        blaze::DynamicMatrix<double> A(rows, cols);
        A = blaze::forEach(A, [&](double) { return dis(gen); });
        blaze::DynamicMatrix<double> B(rows, cols);
        B = blaze::forEach(B, [&](double) { return dis(gen); });
        b.stop_clock(idx);

        b.start_clock(b.add_clock());
        // TODO: remove when better non-lazy evaluation is implemented
        blaze::DynamicMatrix<double> C = blaze::eval(A * blaze::trans(B));
        // Use the feature of stopping last clock
        b.stop_clock();
    }
#endif

#ifdef HAVE_EIGEN
    /// Eigen kernel as a function object
    struct eigen_kernel
    {
        benchmarker & b;

        eigen_kernel(benchmarker & b_) :
                b(b_) {}

        void operator()(int rows, int cols)
        {
            // Note that we can rely on increasing indices, remembering is not necessary
            b.start_clock( b.add_clock() );
            Eigen::MatrixXd A = Eigen::MatrixXd::Random(rows, cols);
            Eigen::MatrixXd B = Eigen::MatrixXd::Random(rows, cols);
            // Use the feature of stopping last clock
            b.stop_clock();

            b.start_clock(b.add_clock());
            auto C = (A * B.transpose()).eval();
            b.stop_clock();
        }

    };
#endif

#ifdef HAVE_ARMADILLO
    static void arma_kernel(benchmarker & b, int rows, int cols)
    {
        b.start_clock(0);
        arma::mat A = arma::randu<arma::mat>(rows, cols);
        arma::mat B = arma::randu<arma::mat>(rows, cols);
        b.stop_clock(0);

        b.start_clock(1);
        auto C = (A * B.t() ).eval();
        b.stop_clock(1);
    }
#endif

int main()
{
    benchmarker benchmark;
    benchmark.set_cache_size(8 * 1024 * 1024);
    int rows = 200, cols = 100;
    int iters = 100;

    // Run a function with additional args
#ifdef HAVE_BLAZE
    benchmark.run(iters, blaze_kernel, rows, cols);
#endif

    // Run a functor without benchmark arg
#ifdef HAVE_EIGEN
    eigen_kernel k(benchmark);
    benchmark.run(iters, k, rows, cols);
#endif

    // Allocate clocks before run
    // Run a lambda expression without args - capture everything
    benchmark.reserve_clocks(2);
#ifdef HAVE_ARMADILLO
    benchmark.run(iters,
                  [=, &benchmark]() {
                      benchmark.start_clock(0);
                      arma::mat A = arma::randu<arma::mat>(rows, cols);
                      arma::mat B = arma::randu<arma::mat>(rows, cols);
                      benchmark.stop_clock(0);

                      benchmark.start_clock(1);
                      auto C = (A * B.t() ).eval();
                      benchmark.stop_clock(1);
                  });
#endif

#ifdef HAVE_MTL4
    benchmark.run(iters,
                  [=, &benchmark]() {
                      benchmark.start_clock(0);
                      std::random_device rd;
                      std::mt19937 gen(rd());
                      std::uniform_real_distribution<> dis(0, 1);
                      mtl::mat::dense2D<double> A(rows, cols);
                      std::for_each(A.address_data(), A.address_data() + rows*cols,
                                    [&](double & val) {
                                        val = dis(gen);
                                    });
                      mtl::mat::dense2D<double> B(rows, cols);
                      std::for_each(B.address_data(), B.address_data() + rows*cols,
                                    [&](double & val) {
                                        val = dis(gen);
                                    });
                      benchmark.stop_clock(0);

                      benchmark.start_clock(1);
                      auto C = mtl::traits::evaluate(A * mtl::mat::trans(B) );
                      benchmark.stop_clock(1);
                  });
#endif

    std::array<std::string, 4> labels{{"Blaze", "Eigen", "Armadillo", "MTL4"}};
    std::array<std::string, 2> clock_labels{{"Initialization", "Computation"}};

    benchmark.print_results(std::cout, labels, clock_labels);
}




