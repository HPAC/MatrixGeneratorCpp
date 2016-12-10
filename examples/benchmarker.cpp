//
// Created by mcopik on 07.12.16.
//

#include <array>
#include <string>
#include <iostream>
#include <random>

#include <benchmarker/benchmarker.hpp>
#include <interfaces/libraries.hpp>

using linalg_tests::benchmarker;

/// Blaze kernel as a function
blaze::DynamicMatrix<double> blaze_kernel(benchmarker & b, int rows, int cols)
{
    int idx = b.add_clock();
    auto begin = std::clock();
    b.start_clock( idx );
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    blaze::DynamicMatrix<double> A(rows, cols);
    blaze::forEach(A, [&](double) { return dis(gen); });
    blaze::DynamicMatrix<double> B(rows, cols);
    blaze::forEach(B, [&](double) { return dis(gen); });
    b.stop_clock(idx);
    double time_spent = (double)(std::clock() - begin) / CLOCKS_PER_SEC;
    std::cout << time_spent << std::endl;

    b.start_clock(b.add_clock());
    auto C = A * blaze::trans(B);
    // TODO: remove when non-lazy evaluation is implemented
    C(0,0);
    // Use the feature of stopping last clock
    b.stop_clock();

    return C;
}

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

int main()
{
    benchmarker benchmark;
    int rows = 200, cols = 100;
    int iters = 100;

    // Run a function with additional args
    //benchmark.run(iters, blaze_kernel, rows, cols);

    // Run a functor without benchmark arg
    eigen_kernel k(benchmark);
    benchmark.run(iters, k, rows, cols);

    // Allocate clocks before run
    // Run a lambda expression without args - capture everything
    //benchmark.reserve_clocks(2);
    /*benchmark.run(10,
                  [=, &benchmark]() {
                      benchmark.start_clock(0);
                      arma::mat A = arma::randu<arma::mat>(rows, cols);
                      arma::mat B = arma::randu<arma::mat>(rows, cols);
                      benchmark.stop_clock(0);

                      benchmark.start_clock(1);
                      A * B.t();
                      benchmark.stop_clock(1);
                  });*/

    std::array<std::string, 3> labels{"Blaze", "Eigen", "Armadillo"};
    std::array<std::string, 2> clock_labels{"Initialization", "Computation"};

    benchmark.print_results(std::cout, labels, clock_labels);
}




