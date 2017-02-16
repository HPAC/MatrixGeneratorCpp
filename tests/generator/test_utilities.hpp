//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef __LINALG_TESTS_TESTS_GENERATOR_TEST_UTILITIES_HPP__
#define __LINALG_TESTS_TESTS_GENERATOR_TEST_UTILITIES_HPP__

#include <cstdint>
#include <type_traits>

#include <traits/matrix.hpp>

template<typename T>
void verify(T, const generator::property::random &)
{}

template<typename T>
void verify(T val, const generator::property::random &, const generator::property::negative &)
{
	EXPECT_LT(val, 0.0f);
}

template<typename T>
void verify(T val, const generator::property::random &, const generator::property::positive &)
{
	EXPECT_GT(val, 0.0f);
}

template<typename MatType, typename ... Properties>
void verify_general(MatType && mat, uint32_t rows, uint32_t cols, Properties &&... props)
{
    EXPECT_EQ(mat.rows(), rows);
    EXPECT_EQ(mat.columns(), cols);
    for(uint32_t i = 0; i < rows; ++i) {
        for(uint32_t j = 0; j < cols; ++j) {
            verify(mat(i, j), std::forward<Properties>(props)...);
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
        verify(mat(i, i), std::forward<Properties>(props)...);	
        for(uint32_t j = i + 1; j < rows; ++j) {
            EXPECT_NEAR(mat(i, j), mat(j, i), std::numeric_limits<value_t>::epsilon());
            verify(mat(i, j), std::forward<Properties>(props)...);
        }
    }
}

template<typename MatType, typename ... Properties>
void verify_diagonal(MatType && mat, uint32_t rows, Properties &&... props)
{
    EXPECT_EQ(mat.rows(), rows);
    EXPECT_EQ(mat.columns(), rows);

    for(uint32_t i = 0; i < rows; ++i) {
        //diagonal
        verify(mat(i, i), std::forward<Properties>(props)...);
    }
}

#define GENERATE_MATRIX_TEST(shape, verify_func, name_type, name, sizes_obj, ...)   \
TYPED_TEST(random_test, name_type##_test_##name) {   \
    for(auto & sizes : sizes_obj)            \
    {                                         \
        uint32_t rows = std::get<0>(sizes), cols = std::get<1>(sizes);  \
        auto mat = this->gen.generate(shape(rows, cols), __VA_ARGS__);  \
        verify_func(mat, rows, cols, __VA_ARGS__); \
    }   \
}

#define GENERATE_MATRIX_SQUARE_TEST(shape, verify_func, name_type, name, sizes_obj, ...)    \
TYPED_TEST(random_test, name_type##_test_##name) {   \
    for(auto rows : sizes_obj)            \
    {                                         \
        auto mat = this->gen.generate(shape(rows), __VA_ARGS__);   \
        verify_func(mat, rows, __VA_ARGS__); \
    }   \
}

#define GENERATE_GENERAL_TEST(name, sizes_obj, ...)   \
    GENERATE_MATRIX_TEST(generator::shape::general, verify_general, general, name, sizes_obj, __VA_ARGS__)

#define GENERATE_HERMITIAN_TEST(name, sizes_obj, ...)   \
    GENERATE_MATRIX_SQUARE_TEST(generator::shape::self_adjoint, verify_hermitian, hermitian, name, sizes_obj, __VA_ARGS__)

#define GENERATE_DIAGONAL_TEST(name, sizes_obj, ...)   \
    GENERATE_MATRIX_SQUARE_TEST(generator::shape::diagonal, verify_diagonal, diagonal, name, sizes_obj, __VA_ARGS__)


#endif
