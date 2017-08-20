//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_PROPERTIES_CBLAS_WRAPPER_HPP
#define LINALG_PROPERTIES_CBLAS_WRAPPER_HPP

#ifdef HAVE_BLAS

#include <cstring>

#include <cblas.h>

#include <generator/shape.hpp>

namespace generator { namespace cblas {

	template<typename T>
	struct TRMM;

	template<>
	struct TRMM<float>
	{
		static void call(const shape::matrix_size & size,
						const std::unique_ptr<float[]> & input,
						const std::unique_ptr<float[]> & output)
		{
			memcpy(output.get(), input.get(), sizeof(float) * size.rows * size.rows);
			cblas_strmm(CblasRowMajor, CblasRight, CblasUpper, CblasTrans,
						CblasNonUnit, size.rows, size.rows, 1.0f, input.get(),
						size.rows, output.get(), size.rows);
		}
	};

	template<>
	struct TRMM<double>
	{
		// Performs multiplication matrix * matrix', writing result to output array
		static void call(const shape::matrix_size & size,
						const std::unique_ptr<double[]> & input,
						const std::unique_ptr<double[]> & output)
		{
			memcpy(output.get(), input.get(), sizeof(double) * size.rows * size.rows);
			cblas_dtrmm(CblasRowMajor, CblasRight, CblasUpper, CblasTrans,
						CblasNonUnit, size.rows, size.rows, 1.0, input.get(),
						size.rows, output.get(), size.rows);
		}
	};

}}

#endif
#endif