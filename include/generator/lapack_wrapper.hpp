//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_PROPERTIES_LAPACK_WRAPPER_HPP
#define LINALG_PROPERTIES_LAPACK_WRAPPER_HPP

#ifdef HAVE_LAPACK

#include <cassert>

#include <generator/shape.hpp>

#if !defined(HAVE_MKL)
    /** Cholesky **/
    extern "C" void spotrf_(char *, int*, float*, int*, int*);
    extern "C" void dpotrf_(char *, int*, double*, int*, int*);
    /** Blocked QR factorization **/
    extern "C" void sgeqrf_(int *, int*, float*, int*, float*, float*, int*, int*);
    extern "C" void dgeqrf_(int *, int*, double*, int*, double*, double*, int*, int*);
    /** Construct Q matrix **/
    extern "C" void sorgqr_(int *, int*, int *, float*, int*, float*, float*, int*, int*);
    extern "C" void dorgqr_(int *, int*, int *, double*, int*, double*, double*, int*, int*);
#else
    #include <mkl.h>
#endif

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

	// Since the input matrix is randomized, we don't care about transposition
	// of the input matrix.
	template<typename T>
	struct QR;

	template<>
	struct QR<float>
	{
		static void q_matrix(const shape::matrix_size & size,
						std::unique_ptr<float[]> & input,
						std::unique_ptr<float[]> & output,
						bool force_copy = false)
		{
			// Factorize input into reflectors
			int rows = size.rows, cols = size.cols;
			int rank = std::min(rows, cols);
			int lwork = -1, info;
			float lwork_size;
			std::unique_ptr<float[]> tau(new float[rank]);
			// query optimal work size - returns in lwork_size
			sgeqrf_(&rows, &cols, input.get(), &cols, tau.get(), &lwork_size, &lwork, &info);
			assert(!info);
			lwork = static_cast<int>(lwork_size);
			std::unique_ptr<float[]> work(new float[lwork]);
			sgeqrf_(&rows, &cols, input.get(), &cols, tau.get(), work.get(), &lwork, &info);
			assert(!info);

			// Create Q matrix
			// query optimal work size - returns in lwork_size
			lwork = -1;
			sorgqr_(&rows, &cols, &rank, input.get(), &cols, tau.get(), &lwork_size, &lwork, &info);
			assert(!info);
			lwork = static_cast<int>(lwork_size);
			work.reset(new float[static_cast<int>(lwork)]);
			sorgqr_(&rows, &cols, &rank, input.get(), &cols, tau.get(), work.get(), &lwork, &info);
			assert(!info);

			// Avoid copying
			if(force_copy) {
				memcpy(output.get(), input.get(), sizeof(float) * size.rows * size.cols);
			} else {
				output = std::move(input);
			}
		}

		static float epsilon()
		{
			return 1e-5f;
		}
	};

	template<>
	struct QR<double>
	{
		static void q_matrix(const shape::matrix_size & size,
						std::unique_ptr<double[]> & input,
						std::unique_ptr<double[]> & output,
						bool force_copy = false)
		{
			// Factorize input into reflectors
			int rows = size.rows, cols = size.cols;
			int rank = std::min(rows, cols);
			int lwork = -1, info;
			double lwork_size;
			std::unique_ptr<double[]> tau(new double[rank]);
			// query optimal work size - returns in lwork_size
			dgeqrf_(&rows, &cols, input.get(), &cols, tau.get(), &lwork_size, &lwork, &info);
			assert(!info);
			lwork = static_cast<int>(lwork_size);
			std::unique_ptr<double[]> work(new double[lwork]);
			dgeqrf_(&rows, &cols, input.get(), &cols, tau.get(), work.get(), &lwork, &info);
			assert(!info);

			// Create Q matrix
			// query optimal work size - returns in lwork_size
			lwork = -1;
			dorgqr_(&rows, &cols, &rank, input.get(), &cols, tau.get(), &lwork_size, &lwork, &info);
			assert(!info);
			lwork = static_cast<int>(lwork_size);
			work.reset(new double[static_cast<int>(lwork)]);
			dorgqr_(&rows, &cols, &rank, input.get(), &cols, tau.get(), work.get(), &lwork, &info);
			assert(!info);

			// Avoid copying
			if(force_copy) {
				memcpy(output.get(), input.get(), sizeof(double) * size.rows * size.cols);
			} else {
				output = std::move(input);
			}
		}

		static double epsilon()
		{
			return 1e-13;
		}
	};

}}

#endif
#endif
