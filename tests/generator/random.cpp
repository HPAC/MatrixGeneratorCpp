//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <tuple>

#include "gtest/gtest.h"

#include <libraries.hpp>
#include <generator/generator.hpp>
#include "test_utilities.hpp"

template<typename Types>
struct random_test : public testing::Test
{
    typedef typename std::tuple_element<0, Types>::type val_t;
    typedef typename std::tuple_element<1, Types>::type lib_t;
    generator::generator<lib_t, val_t> gen;
};

TYPED_TEST_CASE(random_test, test_settings<>::types_to_test);

GENERATE_GENERAL_TEST(random_test, small, test_settings<>::small_sizes, generator::property::random())
GENERATE_GENERAL_TEST(random_test, medium, test_settings<>::medium_sizes, generator::property::random())
GENERATE_GENERAL_TEST(random_test, positive_small, test_settings<>::small_sizes, generator::property::random(), generator::property::positive())
GENERATE_GENERAL_TEST(random_test, positive_medium, test_settings<>::medium_sizes, generator::property::random(), generator::property::positive())
GENERATE_GENERAL_TEST(random_test, negative_small, test_settings<>::small_sizes, generator::property::random(), generator::property::negative())
GENERATE_GENERAL_TEST(random_test, negative_medium, test_settings<>::medium_sizes, generator::property::random(), generator::property::negative())

GENERATE_HERMITIAN_TEST(random_test, small, test_settings<>::small_sq_sizes, generator::property::random())
GENERATE_HERMITIAN_TEST(random_test, medium, test_settings<>::medium_sq_sizes, generator::property::random())
GENERATE_HERMITIAN_TEST(random_test, positive_small, test_settings<>::small_sq_sizes, generator::property::random(), generator::property::positive())
GENERATE_HERMITIAN_TEST(random_test, positive_medium, test_settings<>::medium_sq_sizes, generator::property::random(), generator::property::positive())
GENERATE_HERMITIAN_TEST(random_test, negative_small, test_settings<>::small_sq_sizes, generator::property::random(), generator::property::negative())
GENERATE_HERMITIAN_TEST(random_test, negative_medium, test_settings<>::medium_sq_sizes, generator::property::random(), generator::property::negative())

// FIXME: for Blaze, test only for square - what about else?
GENERATE_UPPER_TRIANGULAR_TEST(random_test, small, test_settings<>::small_sq_sizes, generator::property::random())
GENERATE_UPPER_TRIANGULAR_TEST(random_test, medium, test_settings<>::medium_sq_sizes, generator::property::random())
GENERATE_UPPER_TRIANGULAR_TEST(random_test, positive_small, test_settings<>::small_sq_sizes, generator::property::random(), generator::property::positive())
GENERATE_UPPER_TRIANGULAR_TEST(random_test, positive_medium, test_settings<>::medium_sq_sizes, generator::property::random(), generator::property::positive())
GENERATE_UPPER_TRIANGULAR_TEST(random_test, negative_small, test_settings<>::small_sq_sizes, generator::property::random(), generator::property::negative())
GENERATE_UPPER_TRIANGULAR_TEST(random_test, negative_medium, test_settings<>::medium_sq_sizes, generator::property::random(), generator::property::negative())

// FIXME: for Blaze, test only for square - what about else?
GENERATE_LOWER_TRIANGULAR_TEST(random_test, small, test_settings<>::small_sq_sizes, generator::property::random())
GENERATE_LOWER_TRIANGULAR_TEST(random_test, medium, test_settings<>::medium_sq_sizes, generator::property::random())
GENERATE_LOWER_TRIANGULAR_TEST(random_test, positive_small, test_settings<>::small_sq_sizes, generator::property::random(), generator::property::positive())
GENERATE_LOWER_TRIANGULAR_TEST(random_test, positive_medium, test_settings<>::medium_sq_sizes, generator::property::random(), generator::property::positive())
GENERATE_LOWER_TRIANGULAR_TEST(random_test, negative_small, test_settings<>::small_sq_sizes, generator::property::random(), generator::property::negative())
GENERATE_LOWER_TRIANGULAR_TEST(random_test, negative_medium, test_settings<>::medium_sq_sizes, generator::property::random(), generator::property::negative())

GENERATE_DIAGONAL_TEST(random_test, small, test_settings<>::small_sq_sizes, generator::property::random())
GENERATE_DIAGONAL_TEST(random_test, medium, test_settings<>::medium_sq_sizes, generator::property::random())
GENERATE_DIAGONAL_TEST(random_test, positive_small, test_settings<>::small_sq_sizes, generator::property::random(), generator::property::positive())
GENERATE_DIAGONAL_TEST(random_test, positive_medium, test_settings<>::medium_sq_sizes, generator::property::random(), generator::property::positive())
GENERATE_DIAGONAL_TEST(random_test, negative_small, test_settings<>::small_sq_sizes, generator::property::random(), generator::property::negative())
GENERATE_DIAGONAL_TEST(random_test, negative_medium, test_settings<>::medium_sq_sizes, generator::property::random(), generator::property::negative())

GENERATE_ROW_VECTOR_TEST(random_test, small, test_settings<>::row_vector_sizes, generator::property::random())
GENERATE_ROW_VECTOR_TEST(random_test, positive_small, test_settings<>::row_vector_sizes, generator::property::random(), generator::property::positive())
GENERATE_ROW_VECTOR_TEST(random_test, negative_small, test_settings<>::row_vector_sizes, generator::property::random(), generator::property::negative())

GENERATE_COL_VECTOR_TEST(random_test, small, test_settings<>::col_vector_sizes, generator::property::random())
GENERATE_COL_VECTOR_TEST(random_test, positive_small, test_settings<>::col_vector_sizes, generator::property::random(), generator::property::positive())
GENERATE_COL_VECTOR_TEST(random_test, negative_small, test_settings<>::col_vector_sizes, generator::property::random(), generator::property::negative())


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
