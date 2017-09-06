//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_PROPERTIES_CBLAS_WRAPPER_HPP
#define LINALG_PROPERTIES_CBLAS_WRAPPER_HPP

#ifdef HAVE_BLAS

#include <cstring>

#ifdef HAVE_MKL
#include <mkl_cblas.h>
#else
#include <cblas.h>
#endif

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


	template<typename T>
	struct GEMM;

	template<>
	struct GEMM<float>
	{
		// output = input1 * input2'
		static void call(const shape::matrix_size & size,
						const std::unique_ptr<float[]> & input1,
						const std::unique_ptr<float[]> & input2,
						const std::unique_ptr<float[]> & output)
		{
			cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, size.rows, size.rows,
				size.cols, 1.0f, input1.get(), size.cols, input2.get(), size.rows, 0.0f,
				output.get(), size.cols);
		}
	};

	template<>
	struct GEMM<double>
	{
		// output = input1 * input2'
		static void call(const shape::matrix_size & size,
						const std::unique_ptr<double[]> & input1,
						const std::unique_ptr<double[]> & input2,
						const std::unique_ptr<double[]> & output)
		{
			cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, size.rows, size.rows,
				size.cols, 1.0, input1.get(), size.cols, input2.get(), size.cols, 0.0,
				output.get(), size.rows);
		}
	};

	template<typename T>
	struct SYRK;

	template<>
	struct SYRK<float>
	{
		static void call(const shape::matrix_size & size,
						const std::unique_ptr<float[]> & input,
						const std::unique_ptr<float[]> & output)
		{
			cblas_ssyrk(CblasRowMajor, CblasUpper, CblasNoTrans, size.rows, size.rows,
						1.0f, input.get(), size.rows, 0.0f, output.get(), size.rows);
		}
	};

	template<>
	struct SYRK<double>
	{
		// Performs multiplication matrix * matrix', writing result to output array
		static void call(const shape::matrix_size & size,
						const std::unique_ptr<double[]> & input,
						const std::unique_ptr<double[]> & output)
		{
			cblas_dsyrk(CblasRowMajor, CblasUpper, CblasNoTrans, size.rows, size.rows,
						1.0, input.get(), size.rows, 0.0, output.get(), size.rows);
		}
	};
}}

#endif
#endif
