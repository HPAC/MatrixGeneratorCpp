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

template<typename Mat>
void verify_matrix(const Eigen::MatrixBase<Mat> & mat, const generator::property::orthogonal &)
{
    typedef typename traits::matrix_traits< std::remove_reference_t<Mat> >::value_t value_t;

    uint32_t rows = mat.rows(), cols = mat.cols();
    // multiply matrix * matrix'
    auto multiplication = (mat * mat.transpose() - Mat::Identity(rows, cols)).eval();
    // now all elements should be quite close to zero
    EXPECT_NEAR(multiplication.norm(), static_cast<value_t>(0.0), generator::lapack::QR<value_t>::epsilon());
}

#endif
