//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef __LINALG_TESTS_TESTS_GENERATOR_TEST_UTILITIES_EIGEN_HPP__
#define __LINALG_TESTS_TESTS_GENERATOR_TEST_UTILITIES_EIGEN_HPP__

#include <Eigen/Dense>

// Eigen::DiagonalMatrix does not inherit from Eigen::DenseBase

template<typename Mat, typename... Properties>
void verify_matrix(const Eigen::EigenBase<Mat> &, const Properties &...)
{}

// An info parameter is set if Cholesky does not convergence
template<typename Mat>
void verify_matrix(const Eigen::EigenBase<Mat> & mat, const generator::property::spd &)
{
    Eigen::LLT<Mat> cholesky(mat);
	EXPECT_EQ(cholesky.info(), Eigen::ComputationInfo::Success);
}

template<typename Mat>
void verify_matrix(const Eigen::EigenBase<Mat> & mat, const generator::property::spd & spd_prop, const generator::property::positive &)
{
    verify_matrix(mat, spd_prop);
}

// Cholesky does not work for Diagonal
// Use the knowledge that diagonal matrix is SPD iff all entries are positive
template<typename T>
void verify_matrix(const Eigen::DiagonalMatrix<T, Eigen::Dynamic, Eigen::Dynamic> & mat, const generator::property::spd &)
{
    uint64_t cols = mat.cols();
    for(uint64_t i = 0; i < cols; ++i) {
        EXPECT_GT(mat.diagonal().coeff(i), static_cast<T>(0.0));
    }
}

template<typename T>
void verify_matrix(const Eigen::DiagonalMatrix<T, Eigen::Dynamic, Eigen::Dynamic> & mat, const generator::property::spd &spd_prop,
    const generator::property::positive &)
{
    verify_matrix(mat, spd_prop);
}

// Blaze llh should throw if Cholesky factorization fails on a not positive definite matrix
template<typename Mat>
void verify_matrix(const Eigen::EigenBase<Mat> &, const generator::property::orthogonal &)
{
    //uint32_t rows = mat.rows(), cols = mat.columns();
    // multiply matrix * matrix'
    //auto multiplication = blaze::eval(mat * blaze::trans(mat) - blaze::IdentityMatrix<double, blaze::rowMajor>(rows));
    // now all elements should be quite close to zero
    //auto matrix_norm = blaze::length(multiplication);
    /*value_t sum;
    for(uint32_t i = 0; i < rows; ++i) {
        for(uint32_t j = 0; j < cols; ++j) {
        	//FIXME: proper epsilon for QR
            //EXPECT_NEAR(multiplication(i, j), static_cast<value_t>(0.0), 5*std::numeric_limits<value_t>::epsilon());
            sum += std::pow(multiplication(i, j), 2);
        }
    }
    //std::cout << mat << std::endl;
    EXPECT_NEAR(std::sqrt(sum), static_cast<value_t>(0.0), generator::lapack::QR<value_t>::epsilon());*/
}

#endif
