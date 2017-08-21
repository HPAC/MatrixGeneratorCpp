//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_PROPERTIES_LAPACK_WRAPPER_HPP
#define LINALG_PROPERTIES_LAPACK_WRAPPER_HPP

#ifdef HAVE_LAPACK

#include <generator/shape.hpp>

extern "C" void spotrf_(char *, int*, float*, int*, int*);
extern "C" void dpotrf_(char *, int*, double*, int*, int*);

namespace generator { namespace lapack {

	// Fortran functions in LAPACK store matrices column-wise
	// In the Cholesky factorization, the input matrix has to be symmetric
	// Thus, no transposition is necessary
	template<typename T>
	struct cholesky;

	template<>
	struct cholesky<float>
	{
		static bool test(const shape::matrix_size & size,
						const std::unique_ptr<float[]> & input)
		{
			// Arguments: Upper/Lower, Order, Matrix*, Leading Dim, Info
			int info, rows = size.rows, lda = size.rows;
			char triangular = 'U';
			spotrf_(&triangular, &rows, input.get(), &lda, &info);
			return !info;
		}
	};

	template<>
	struct cholesky<double>
	{
		// Returns true if Cholesky factorization was succesfull
		static bool test(const shape::matrix_size & size,
						const std::unique_ptr<double[]> & input)
		{
			// Arguments: Upper/Lower, Order, Matrix*, Leading Dim, Info
			int info, rows = size.rows, lda = size.rows;
			char triangular = 'U';
			dpotrf_(&triangular, &rows, input.get(), &lda, &info);
			return !info;
		}
	};

}}

#endif
#endif