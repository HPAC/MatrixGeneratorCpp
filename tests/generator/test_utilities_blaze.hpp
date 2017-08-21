//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef __LINALG_TESTS_TESTS_GENERATOR_TEST_UTILITIES_BLAZE_HPP__
#define __LINALG_TESTS_TESTS_GENERATOR_TEST_UTILITIES_BLAZE_HPP__

#include <blaze/Blaze.h>
#include <blaze/math/DenseMatrix.h>
#include <blaze/math/dense/DynamicMatrix.h>
#include <generator/lapack_wrapper.hpp>

template<typename Mat, bool SO, typename... Properties>
void verify_matrix(const blaze::DenseMatrix<Mat, SO> &, const Properties &...)
{}

// Blaze llh should throw if Cholesky factorization fails on a not positive definite matrix
template<typename Mat, bool SO>
void verify_matrix(const blaze::DenseMatrix<Mat, SO> & mat, const generator::property::spd &)
{
    typedef typename traits::matrix_traits<Mat>::value_t value_t;

	blaze::DynamicMatrix<value_t, blaze::rowMajor> result;
	ASSERT_NO_THROW( blaze::llh(mat, result); );
}

template<typename Mat, bool SO>
void verify_matrix(const blaze::DenseMatrix<Mat, SO> & mat, const generator::property::spd & spd_prop, const generator::property::positive &)
{
    verify_matrix(mat, spd_prop);
}

// Blaze llh should throw if Cholesky factorization fails on a not positive definite matrix
template<typename Mat, bool SO>
void verify_matrix(const blaze::DenseMatrix<Mat, SO> & mat_, const generator::property::orthogonal &)
{
    typedef typename traits::matrix_traits<Mat>::value_t value_t;

    auto mat = ~mat_;
    uint64_t rows = mat.rows(), cols = mat.columns();
    // multiply matrix * matrix'
    auto multiplication = blaze::eval(mat * blaze::trans(mat) - blaze::IdentityMatrix<double, blaze::rowMajor>(rows));
    // now all elements should be quite close to zero
    //auto matrix_norm = blaze::length(multiplication);
    value_t sum = 0.0;
    for(uint64_t i = 0; i < rows; ++i) {
        for(uint64_t j = 0; j < cols; ++j) {
            sum += std::pow(multiplication(i, j), 2);
        }
    }
    EXPECT_NEAR(std::sqrt(sum), static_cast<value_t>(0.0), generator::lapack::QR<value_t>::epsilon());
}

#endif
