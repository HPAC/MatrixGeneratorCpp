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

using std::make_tuple;

template<typename ... T>
struct test_settings;

template<>
struct test_settings<>
{
    typedef testing::Types<
            std::tuple<float, library::blaze>,
            std::tuple<double, library::blaze>
        > types_to_test;

    static constexpr std::array< std::tuple<uint32_t, uint32_t>, 4> small_sizes{
        make_tuple(1,1), make_tuple(2, 1), make_tuple(25, 50), make_tuple(50, 25)
    };
    static constexpr std::array< uint32_t, 4> small_sq_sizes{1, 2, 9, 25};
    static constexpr std::array< std::tuple<uint32_t, uint32_t>, 2> medium_sizes{
        make_tuple(100, 100), make_tuple(199, 173)
    };
    static constexpr std::array< uint32_t, 3> medium_sq_sizes{100, 103, 125};
};

constexpr std::array< std::tuple<uint32_t, uint32_t>, 4> test_settings<>::small_sizes;
constexpr std::array< uint32_t, 4> test_settings<>::small_sq_sizes;
constexpr std::array< std::tuple<uint32_t, uint32_t>, 2> test_settings<>::medium_sizes;
constexpr std::array< uint32_t, 3> test_settings<>::medium_sq_sizes;

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
void verify(T val, const generator::property::zeros &)
{
    EXPECT_NEAR(val, 0.0f, std::numeric_limits<FloatingType>::epsilon());
}

template<typename MatType, typename ... Properties>
void verify_general(MatType && mat, uint32_t rows, uint32_t cols, Properties &&... props)
{
    typedef typename traits::matrix_traits< std::remove_reference_t<MatType> >::value_t value_t;

    EXPECT_EQ(mat.rows(), rows);
    EXPECT_EQ(mat.columns(), cols);
    for(uint32_t i = 0; i < rows; ++i) {
        for(uint32_t j = 0; j < cols; ++j) {
            verify<value_t>(mat(i, j), std::forward<Properties>(props)...);
        } 
    }
}

template<typename MatType, typename ... Properties>
void verify_hermitian(MatType && mat, uint32_t rows, Properties &&... props)
{
    typedef typename traits::matrix_traits< std::remove_reference_t<MatType> >::value_t value_t;

    EXPECT_EQ(mat.rows(), rows);
    EXPECT_EQ(mat.columns(), rows);

    for(uint32_t i = 0; i < rows; ++i) {
    	//diagonal
        verify<value_t>(mat(i, i), std::forward<Properties>(props)...);	
        for(uint32_t j = i + 1; j < rows; ++j) {
            EXPECT_NEAR(mat(i, j), mat(j, i), std::numeric_limits<value_t>::epsilon());
            verify<value_t>(mat(i, j), std::forward<Properties>(props)...);
        }
    }
}

template<typename MatType, typename ... Properties>
void verify_diagonal(MatType && mat, uint32_t rows, Properties &&... props)
{
    typedef typename traits::matrix_traits< std::remove_reference_t<MatType> >::value_t value_t;

    EXPECT_EQ(mat.rows(), rows);
    EXPECT_EQ(mat.columns(), rows);

    for(uint32_t i = 0; i < rows; ++i) {
        //diagonal
        verify<value_t>(mat(i, i), std::forward<Properties>(props)...);
    }
}

#define GENERATE_MATRIX_TEST(test_case_name, shape, verify_func, name_type, name, sizes_obj, ...)   \
TYPED_TEST(test_case_name, name_type##_test_##name) {   \
    for(auto & sizes : sizes_obj)            \
    {                                         \
        uint32_t rows = std::get<0>(sizes), cols = std::get<1>(sizes);  \
        auto mat = this->gen.generate(shape(rows, cols), __VA_ARGS__);  \
        verify_func(mat, rows, cols, __VA_ARGS__); \
    }   \
}

#define GENERATE_MATRIX_SQUARE_TEST(test_case_name, shape, verify_func, name_type, name, sizes_obj, ...)    \
TYPED_TEST(test_case_name, name_type##_test_##name) {   \
    for(auto rows : sizes_obj)            \
    {                                         \
        auto mat = this->gen.generate(shape(rows), __VA_ARGS__);   \
        verify_func(mat, rows, __VA_ARGS__); \
    }   \
}

#define GENERATE_GENERAL_TEST(test_case_name, name, sizes_obj, ...)   \
    GENERATE_MATRIX_TEST(test_case_name, generator::shape::general, verify_general, general, name, sizes_obj, __VA_ARGS__)

#define GENERATE_HERMITIAN_TEST(test_case_name, name, sizes_obj, ...)   \
    GENERATE_MATRIX_SQUARE_TEST(test_case_name, generator::shape::self_adjoint, verify_hermitian, hermitian, name, sizes_obj, __VA_ARGS__)

#define GENERATE_DIAGONAL_TEST(test_case_name, name, sizes_obj, ...)   \
    GENERATE_MATRIX_SQUARE_TEST(test_case_name, generator::shape::diagonal, verify_diagonal, diagonal, name, sizes_obj, __VA_ARGS__)


#endif
