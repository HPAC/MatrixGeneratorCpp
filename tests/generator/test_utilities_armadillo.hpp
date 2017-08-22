//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef __LINALG_TESTS_TESTS_GENERATOR_TEST_UTILITIES_ARMADILLO_HPP__
#define __LINALG_TESTS_TESTS_GENERATOR_TEST_UTILITIES_ARMADILLO_HPP__

#include <armadillo>


template<typename T, typename... Properties>
void verify_matrix(const arma::Mat<T> &, const Properties &...)
{}

// An info parameter is set if Cholesky does not convergence
template<typename T>
void verify_matrix(const arma::Mat<T> & mat, const generator::property::spd &)
{
	ASSERT_NO_THROW( auto s = arma::chol(mat); );
}

template<typename T>
void verify_matrix(const arma::Mat<T> & mat, const generator::property::spd & spd_prop, const generator::property::positive &)
{
    verify_matrix(mat, spd_prop);
}

template<typename T>
void verify_matrix(const arma::Mat<T> & mat, const generator::property::orthogonal &)
{
    uint32_t rows = mat.n_rows, cols = mat.n_cols;
    // multiply matrix * matrix'
    auto multiplication = (mat * mat.t() - arma::eye(rows, cols)).eval();
    // now all elements should be quite close to zero
    EXPECT_NEAR(norm(multiplication), static_cast<T>(0.0), generator::lapack::QR<T>::epsilon());
}

#endif
