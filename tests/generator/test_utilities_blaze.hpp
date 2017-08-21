//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef __LINALG_TESTS_TESTS_GENERATOR_TEST_UTILITIES_BLAZE_HPP__
#define __LINALG_TESTS_TESTS_GENERATOR_TEST_UTILITIES_BLAZE_HPP__

#ifdef HAVE_BLAZE

#include <blaze/Blaze.h>

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
#endif
#endif