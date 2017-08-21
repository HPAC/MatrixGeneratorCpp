//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <tuple>
#include <type_traits>

#include "gtest/gtest.h"

#include <libraries.hpp>
#include <generator/generator.hpp>
#include <traits/matrix.hpp>
#include "test_utilities.hpp"

/// FIXME: blaze evaluate function hardcoded into tests here

template<typename Types>
struct eye_test : public testing::Test
{
    typedef typename std::tuple_element<0, Types>::type val_t;
    typedef typename std::tuple_element<1, Types>::type lib_t;
    generator::generator<lib_t, val_t> gen;
};

TYPED_TEST_CASE(eye_test, test_settings<>::types_to_test);

template<typename MatType1, typename MatType2>
void verify_left(MatType1 && mat, MatType2 && eye)
{
    // both floating types should be the same
    typedef traits::matrix_traits< std::remove_reference_t<MatType1> > traits_t;
    typedef typename traits_t::value_t val_t;

    // Mat is (s, rows), eye is (rows, cols)
    // Result matrix is of size s x cols
    // If rows > cols then some columns from original matrix is gone
    // If rows < cols then additional columns in new matrix are set to zero
    size_t cols = traits_t::columns(eye);
    auto left_multiplication = traits_t::eval(mat * eye);
    EXPECT_EQ(traits_t::rows(left_multiplication), traits_t::rows(mat));
    EXPECT_EQ(traits_t::columns(left_multiplication), cols);
    size_t rows_to_check = traits_t::rows(mat);
    size_t cols_to_check = std::min(traits_t::rows(eye), cols);
    for(size_t i = 0; i < rows_to_check; ++i) {
        for(size_t j = 0; j < cols_to_check; ++j)
            EXPECT_NEAR(traits_t::get(mat, i, j), traits_t::get(left_multiplication, i, j), std::numeric_limits<val_t>::epsilon());
        for(size_t j = cols_to_check; j < cols; ++j)
            EXPECT_NEAR(0.0, traits_t::get(left_multiplication, i, j), std::numeric_limits<val_t>::epsilon());
    }
}

template<typename MatType1, typename MatType2>
void verify_right(MatType1 && mat, MatType2 && eye)
{
    // both floating types should be the same
    typedef traits::matrix_traits< std::remove_reference_t<MatType1> > traits_t;
    typedef typename traits_t::value_t val_t;

    // Mat is (cols, s), eye is (rows, cols)
    // Result matrix is of size rows x s
    // If rows > cols then additional rows not from original matrix should be zero
    // If rows < cols then some rows from original matrix are gone
    size_t rows = traits_t::rows(eye);
    auto right_multiplication = traits_t::eval(eye * mat);
    EXPECT_EQ(traits_t::rows(right_multiplication), rows);
    EXPECT_EQ(traits_t::columns(right_multiplication), traits_t::columns(mat));
    size_t rows_to_check = std::min(rows, traits_t::columns(eye));
    size_t cols_to_check = traits_t::columns(mat);
    for(size_t i = 0; i < rows_to_check; ++i) {
        for(size_t j = 0; j < cols_to_check; ++j)
            EXPECT_NEAR(traits_t::get(mat, i, j), traits_t::get(right_multiplication, i, j), std::numeric_limits<val_t>::epsilon());
    }
    for(size_t i = rows_to_check; i < rows; ++i) {
        for(size_t j = 0; j < cols_to_check; ++j)
            EXPECT_NEAR(0.0, traits_t::get(right_multiplication, i, j), std::numeric_limits<val_t>::epsilon());
    }
}

#define GENERATE_GENERAL_MATRIX_TEST(name, sizes_obj)   \
TYPED_TEST(eye_test, general_##name)                      \
{                                                       \
    for(auto & sizes : test_settings<>::sizes_obj)      \
    {                                                   \
        uint32_t rows = std::get<0>(sizes), cols = std::get<1>(sizes);  \
        auto mat = this->gen.generate({42, rows}, generator::property::random());  \
        auto eye = this->gen.generate({rows, cols}, generator::property::eye());   \
        verify_left(mat, eye);  \
        eye = this->gen.generate({rows, cols}, generator::property::eye());    \
        mat = this->gen.generate({cols, 42}, generator::property::random());   \
        verify_right(mat, eye); \
    }   \
}

#define GENERATE_HERMITIAN_MATRIX_TEST(name, shape_type, sizes_obj) \
TYPED_TEST(eye_test, shape_type##_##name)                               \
{                                                                   \
    for(auto & sizes : test_settings<>::sizes_obj)                     \
    {                                                                   \
        uint32_t rows = std::get<0>(sizes), cols = std::get<1>(sizes);  \
        auto mat = this->gen.generate({42, cols}, generator::property::random());  \
        auto eye = this->gen.generate({cols, rows}, generator::shape::shape_type{}, generator::property::eye());  \
        verify_left(mat, eye);  \
        eye = this->gen.generate({rows, cols}, generator::shape::shape_type{}, generator::property::eye());   \
        mat = this->gen.generate({cols, 42}, generator::property::random());   \
        verify_right(mat, eye); \
    }   \
}

GENERATE_GENERAL_MATRIX_TEST(small, small_sizes)
GENERATE_GENERAL_MATRIX_TEST(medium, medium_sizes)

GENERATE_HERMITIAN_MATRIX_TEST(small, self_adjoint, small_sq_sizes)
GENERATE_HERMITIAN_MATRIX_TEST(medium, self_adjoint, medium_sq_sizes)

GENERATE_HERMITIAN_MATRIX_TEST(small, upper_triangular, small_sq_sizes)
GENERATE_HERMITIAN_MATRIX_TEST(medium, upper_triangular, medium_sq_sizes)

GENERATE_HERMITIAN_MATRIX_TEST(small, lower_triangular, small_sq_sizes)
GENERATE_HERMITIAN_MATRIX_TEST(medium, lower_triangular, medium_sq_sizes)

GENERATE_HERMITIAN_MATRIX_TEST(small, diagonal, small_sq_sizes)
GENERATE_HERMITIAN_MATRIX_TEST(medium, diagonal, medium_sq_sizes)

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
