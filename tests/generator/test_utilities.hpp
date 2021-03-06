//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef __LINALG_TESTS_TESTS_GENERATOR_TEST_UTILITIES_HPP__
#define __LINALG_TESTS_TESTS_GENERATOR_TEST_UTILITIES_HPP__

#include <tuple>
#include <cstdint>
#include <type_traits>

#include <traits/matrix.hpp>
#include <util.hpp>

#ifdef HAVE_BLAZE
#include "test_utilities_blaze.hpp"
#endif

#ifdef HAVE_EIGEN
#include "test_utilities_eigen.hpp"
#endif

#ifdef HAVE_ARMADILLO
#include "test_utilities_armadillo.hpp"
#endif

using std::make_tuple;

template<typename ... T>
struct test_settings;

template<>
struct test_settings<>
{
    typedef testing::Types<
        #ifdef HAVE_BLAZE
            std::tuple<float, library::blaze>,
            std::tuple<double, library::blaze>
        #endif
        #ifdef HAVE_EIGEN
            #ifdef HAVE_BLAZE
                ,
            #endif
            std::tuple<float, library::eigen>,
            std::tuple<double, library::eigen>
        #endif
        #ifdef HAVE_ARMADILLO
            #if defined(HAVE_BLAZE) || defined(HAVE_EIGEN)
                ,
            #endif
            std::tuple<float, library::arma>,
            std::tuple<double, library::arma>
        #endif
    > types_to_test;

    static constexpr std::array< std::tuple<uint32_t, uint32_t>, 4> small_sizes{
        {make_tuple(1, 1), make_tuple(2, 1), make_tuple(25, 50), make_tuple(50, 25)}
    };
    static constexpr std::array< std::tuple<uint32_t, uint32_t>, 4> small_sq_sizes{
        {make_tuple(1, 1), make_tuple(2, 2), make_tuple(9, 9), make_tuple(25, 25)}
    };
    static constexpr std::array< std::tuple<uint32_t, uint32_t>, 2> medium_sizes{
        {make_tuple(100, 100), make_tuple(199, 173)}
    };
    static constexpr std::array< std::tuple<uint32_t, uint32_t>, 3> medium_sq_sizes{
        {make_tuple(100, 100), make_tuple(103, 103), std::make_tuple(125, 125)}
    };
    static constexpr std::array< std::tuple<uint32_t, uint32_t>, 4> row_vector_sizes{
        {make_tuple(1, 1), make_tuple(1, 2), make_tuple(1, 53), make_tuple(1, 200)}
    };
    static constexpr std::array< std::tuple<uint32_t, uint32_t>, 4> col_vector_sizes{
        {make_tuple(1, 1), make_tuple(2, 1), make_tuple(53, 1), make_tuple(200, 1)}
    };
};

constexpr std::array< std::tuple<uint32_t, uint32_t>, 4> test_settings<>::small_sizes;
constexpr std::array< std::tuple<uint32_t, uint32_t>, 4> test_settings<>::small_sq_sizes;
constexpr std::array< std::tuple<uint32_t, uint32_t>, 2> test_settings<>::medium_sizes;
constexpr std::array< std::tuple<uint32_t, uint32_t>, 3> test_settings<>::medium_sq_sizes;
constexpr std::array< std::tuple<uint32_t, uint32_t>, 4> test_settings<>::row_vector_sizes;
constexpr std::array< std::tuple<uint32_t, uint32_t>, 4> test_settings<>::col_vector_sizes;

/// The additional template parameter is used because matrix access operator() may return
/// an object with type other than a usual floating-point type. Example is DiagonalProxy in
/// Blaze DiagonalMatrix.
template<typename FloatingType, typename T>
void verify(T, const generator::property::random &)
{}

template<typename FloatingType, typename T>
void verify(T val, const generator::property::random &, const generator::property::negative &)
{
	EXPECT_LT(val, 0.0f);
}

template<typename FloatingType, typename T>
void verify(T val, const generator::property::random &, const generator::property::positive &)
{
	EXPECT_GT(val, 0.0f);
}

template<typename FloatingType, typename T>
void verify(T val, const generator::property::constant & prop)
{
    EXPECT_NEAR(val, prop.fill_value, std::numeric_limits<FloatingType>::epsilon());
}

template<typename FloatingType, typename T>
void verify(T val, const generator::property::zeros &)
{
    EXPECT_NEAR(val, 0.0f, std::numeric_limits<FloatingType>::epsilon());
}

template<typename FloatingType, typename T>
void verify(T val, const generator::property::ones &)
{
    EXPECT_NEAR(val, 1.0f, std::numeric_limits<FloatingType>::epsilon());
}

template<typename FloatingType, typename T>
void verify(T, const generator::property::spd &)
{}

template<typename FloatingType, typename T>
void verify(T val, const generator::property::spd &, const generator::property::positive &)
{
    EXPECT_GT(val, 0.0f);
}

template<typename FloatingType, typename T>
void verify(T, const generator::property::orthogonal &)
{}

template<typename MatType, typename ... Properties>
void verify_general(MatType && mat, uint32_t rows, uint32_t cols, Properties &&... props)
{
    typedef traits::matrix_traits< std::remove_reference_t<MatType> > traits_t;
    typedef typename traits_t::value_t value_t;

    EXPECT_EQ(traits_t::rows(mat), rows);
    EXPECT_EQ(traits_t::columns(mat), cols);
    verify_matrix(std::forward<MatType>(mat), std::forward<Properties>(props)...);

    for(uint32_t i = 0; i < rows; ++i) {
        for(uint32_t j = 0; j < cols; ++j) {
            verify<value_t>(traits_t::get(mat, i, j), std::forward<Properties>(props)...);
        } 
    }
}

template<typename MatType, typename ... Properties>
void verify_hermitian(MatType && mat, uint32_t rows, uint32_t, Properties &&... props)
{
    typedef traits::matrix_traits< std::remove_reference_t<MatType> > traits_t;
    typedef typename traits_t::value_t value_t;

    EXPECT_EQ(traits_t::rows(mat), rows);
    EXPECT_EQ(traits_t::columns(mat), rows);
    verify_matrix(std::forward<MatType>(mat), std::forward<Properties>(props)...);

    for(uint32_t i = 0; i < rows; ++i) {
    	//diagonal
        verify<value_t>(mat(i, i), std::forward<Properties>(props)...);	
        for(uint32_t j = i + 1; j < rows; ++j) {
            EXPECT_NEAR(traits_t::get(mat, i, j), traits_t::get(mat, j, i), std::numeric_limits<value_t>::epsilon());
            verify<value_t>(traits_t::get(mat, i, j), std::forward<Properties>(props)...);
        }
    }
}

template<typename MatType, typename ... Properties>
void verify_upper_triangular(MatType && mat, uint32_t rows, uint32_t, Properties &&... props)
{
    typedef traits::matrix_traits< std::remove_reference_t<MatType> > traits_t;
    typedef typename traits_t::value_t value_t;

    EXPECT_EQ(traits_t::rows(mat), rows);
    EXPECT_EQ(traits_t::columns(mat), rows);
    verify_matrix(std::forward<MatType>(mat), std::forward<Properties>(props)...);

    // FIXME: add non-square tests
    uint32_t cols = rows;
    for(uint32_t i = 0; i < rows; ++i) {
        // in a non-square matrix the upper bound on index might lower than i
        uint32_t max_cols = std::min(i, cols);
        //empty
        for(uint32_t j = 0; j < max_cols; ++j) {
            EXPECT_NEAR(traits_t::get(mat, i, j), static_cast<value_t>(0.0), std::numeric_limits<value_t>::epsilon());
        }
        for(uint32_t j = i; j < cols; ++j) {
            verify<value_t>(traits_t::get(mat, i, j), std::forward<Properties>(props)...); 
        }
    }
}

template<typename MatType, typename ... Properties>
void verify_lower_triangular(MatType && mat, uint32_t rows, uint32_t, Properties &&... props)
{
    typedef traits::matrix_traits< std::remove_reference_t<MatType> > traits_t;
    typedef typename traits_t::value_t value_t;

    EXPECT_EQ(traits_t::rows(mat), rows);
    EXPECT_EQ(traits_t::columns(mat), rows);
    verify_matrix(std::forward<MatType>(mat), std::forward<Properties>(props)...);

    // FIXME: add non-square tests
    uint32_t cols = rows;
    for(uint32_t i = 0; i < rows; ++i) {
        // in a non-square matrix the upper bound on index might lower than i
        uint32_t max_cols = std::min(i + 1, cols);
        for(uint32_t j = 0; j < max_cols; ++j) {
            verify<value_t>(traits_t::get(mat, i, j), std::forward<Properties>(props)...); 
        }
        //empty
        for(uint32_t j = i + 1; j < cols; ++j) {
            EXPECT_NEAR(traits_t::get(mat, i, j), static_cast<value_t>(0.0), std::numeric_limits<value_t>::epsilon());
        }
    }
}

template<typename MatType, typename ... Properties>
void verify_diagonal(MatType && mat, uint32_t rows, uint32_t, Properties &&... props)
{
    typedef traits::matrix_traits< std::remove_reference_t<MatType> > traits_t;
    typedef typename traits_t::value_t value_t;

    EXPECT_EQ(traits_t::rows(mat), rows);
    EXPECT_EQ(traits_t::columns(mat), rows);
    verify_matrix(std::forward<MatType>(mat), std::forward<Properties>(props)...);

    for(uint32_t i = 0; i < rows; ++i) {
        //empty
        for(uint32_t j = 0; j < i; ++j) {
            EXPECT_NEAR(traits_t::get(mat, i, j), static_cast<value_t>(0.0), std::numeric_limits<value_t>::epsilon());
        }
        //diagonal
        verify<value_t>(traits_t::get(mat, i, i), std::forward<Properties>(props)...);
        //empty
        for(uint32_t j = i + 1; j < rows; ++j) {
            EXPECT_NEAR(traits_t::get(mat, i, j), static_cast<value_t>(0.0), std::numeric_limits<value_t>::epsilon());
        }
    }
}

template<typename MatType, typename ... Properties>
void verify_row_vec(MatType && mat, uint32_t, uint32_t cols, Properties &&... props)
{
    typedef traits::matrix_traits< std::remove_reference_t<MatType> > traits_t;
    typedef typename traits_t::value_t value_t;

    EXPECT_EQ(traits_t::rows(mat), 1u);
    EXPECT_EQ(traits_t::columns(mat), cols);

    for(uint32_t i = 0; i < cols; ++i) {
        // the existence of traits method taking only one index verifies that MatType is indeed a vector
        verify<value_t>(traits_t::get(mat, i), std::forward<Properties>(props)...);
    }
}

template<typename MatType, typename ... Properties>
void verify_col_vec(MatType && mat, uint32_t rows, uint32_t, Properties &&... props)
{
    typedef traits::matrix_traits< std::remove_reference_t<MatType> > traits_t;
    typedef typename traits_t::value_t value_t;

    EXPECT_EQ(traits_t::rows(mat), rows);
    EXPECT_EQ(traits_t::columns(mat), 1u);

    for(uint32_t i = 0; i < rows; ++i) {
        // the existence of traits method taking only one index verifies that MatType is indeed a vector
        verify<value_t>(traits_t::get(mat, i), std::forward<Properties>(props)...);
    }
}

#define GENERATE_MATRIX_TEST(test_case_name, shape, verify_func, name_type, name, sizes_obj, ...)   \
TYPED_TEST(test_case_name, name_type##_test_##name) {   \
    for(auto & sizes : sizes_obj)            \
    {                                         \
        uint32_t rows = std::get<0>(sizes), cols = std::get<1>(sizes);  \
        auto mat = this->gen.generate({rows, cols}, shape{}, __VA_ARGS__);  \
        verify_func(mat, rows, cols, __VA_ARGS__); \
    }   \
}

#define GENERATE_MATRIX_SQUARE_TEST(test_case_name, shape, verify_func, name_type, name, sizes_obj, ...)    \
TYPED_TEST(test_case_name, name_type##_test_##name) {   \
    for(auto rows : sizes_obj)            \
    {                                         \
        auto mat = this->gen.generate({rows, rows}, shape{}, __VA_ARGS__);   \
        verify_func(mat, rows, __VA_ARGS__); \
    }   \
}

#define COMMA ,

#define GENERATE_GENERAL_TEST(test_case_name, name, sizes_obj, ...)   \
    GENERATE_MATRIX_TEST(test_case_name, generator::shape::band<1000 COMMA 1000>, verify_general, general, name, sizes_obj, __VA_ARGS__)

#define GENERATE_HERMITIAN_TEST(test_case_name, name, sizes_obj, ...)   \
    GENERATE_MATRIX_TEST(test_case_name, generator::shape::self_adjoint, verify_hermitian, hermitian, name, sizes_obj, __VA_ARGS__)

#define GENERATE_UPPER_TRIANGULAR_TEST(test_case_name, name, sizes_obj, ...)   \
    GENERATE_MATRIX_TEST(test_case_name, generator::shape::upper_triangular, verify_upper_triangular, upper_triangular, name, sizes_obj, __VA_ARGS__)

#define GENERATE_LOWER_TRIANGULAR_TEST(test_case_name, name, sizes_obj, ...)   \
    GENERATE_MATRIX_TEST(test_case_name, generator::shape::lower_triangular, verify_lower_triangular, lower_triangular, name, sizes_obj, __VA_ARGS__)

#define GENERATE_DIAGONAL_TEST(test_case_name, name, sizes_obj, ...)   \
    GENERATE_MATRIX_TEST(test_case_name, generator::shape::diagonal, verify_diagonal, diagonal, name, sizes_obj, __VA_ARGS__)

#define GENERATE_ROW_VECTOR_TEST(test_case_name, name, sizes_obj, ...)   \
    GENERATE_MATRIX_TEST(test_case_name, generator::shape::row_vector, verify_row_vec, row_vec, name, sizes_obj, __VA_ARGS__)

#define GENERATE_COL_VECTOR_TEST(test_case_name, name, sizes_obj, ...)   \
    GENERATE_MATRIX_TEST(test_case_name, generator::shape::col_vector, verify_col_vec, col_vec, name, sizes_obj, __VA_ARGS__)


#endif
