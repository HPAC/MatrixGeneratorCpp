//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <tuple>

#include "gtest/gtest.h"

#include <libraries.hpp>
#include <generator/generator.hpp>
#include "test_utilities.hpp"

using std::make_tuple;

template<typename Types>
struct constant_test : public testing::Test
{
    typedef typename std::tuple_element<0, Types>::type val_t;
    typedef typename std::tuple_element<1, Types>::type lib_t;
    generator::generator<lib_t, val_t> gen;
};

TYPED_TEST_CASE(constant_test, test_settings<>::types_to_test);

GENERATE_GENERAL_TEST(constant_test, small, test_settings<>::small_sizes, generator::property::constant(3.14))
GENERATE_GENERAL_TEST(constant_test, medium, test_settings<>::medium_sizes, generator::property::constant(3.14))
GENERATE_HERMITIAN_TEST(constant_test, small, test_settings<>::small_sq_sizes, generator::property::constant(3.14))
GENERATE_HERMITIAN_TEST(constant_test, medium, test_settings<>::medium_sq_sizes, generator::property::constant(3.14))
GENERATE_UPPER_TRIANGULAR_TEST(constant_test, small, test_settings<>::small_sq_sizes, generator::property::constant(3.14))
GENERATE_UPPER_TRIANGULAR_TEST(constant_test, medium, test_settings<>::medium_sq_sizes, generator::property::constant(3.14))
GENERATE_LOWER_TRIANGULAR_TEST(constant_test, small, test_settings<>::small_sq_sizes, generator::property::constant(3.14))
GENERATE_LOWER_TRIANGULAR_TEST(constant_test, medium, test_settings<>::medium_sq_sizes, generator::property::constant(3.14))
GENERATE_DIAGONAL_TEST(constant_test, small, test_settings<>::small_sq_sizes, generator::property::constant(3.14))
GENERATE_DIAGONAL_TEST(constant_test, medium, test_settings<>::medium_sq_sizes, generator::property::constant(3.14))

template<typename Types>
struct zeros_test : public testing::Test
{
    typedef typename std::tuple_element<0, Types>::type val_t;
    typedef typename std::tuple_element<1, Types>::type lib_t;
    generator::generator<lib_t, val_t> gen;
};

TYPED_TEST_CASE(zeros_test, test_settings<>::types_to_test);

GENERATE_GENERAL_TEST(zeros_test, small, test_settings<>::small_sizes, generator::property::zeros())
GENERATE_GENERAL_TEST(zeros_test, medium, test_settings<>::medium_sizes, generator::property::zeros())
GENERATE_HERMITIAN_TEST(zeros_test, small, test_settings<>::small_sq_sizes, generator::property::zeros())
GENERATE_HERMITIAN_TEST(zeros_test, medium, test_settings<>::medium_sq_sizes, generator::property::zeros())
GENERATE_UPPER_TRIANGULAR_TEST(zeros_test, small, test_settings<>::small_sq_sizes, generator::property::zeros())
GENERATE_UPPER_TRIANGULAR_TEST(zeros_test, medium, test_settings<>::medium_sq_sizes, generator::property::zeros())
GENERATE_LOWER_TRIANGULAR_TEST(zeros_test, small, test_settings<>::small_sq_sizes, generator::property::zeros())
GENERATE_LOWER_TRIANGULAR_TEST(zeros_test, medium, test_settings<>::medium_sq_sizes, generator::property::zeros())
GENERATE_DIAGONAL_TEST(zeros_test, small, test_settings<>::small_sq_sizes, generator::property::zeros())
GENERATE_DIAGONAL_TEST(zeros_test, medium, test_settings<>::medium_sq_sizes, generator::property::zeros())

template<typename Types>
struct ones_test : public testing::Test
{
    typedef typename std::tuple_element<0, Types>::type val_t;
    typedef typename std::tuple_element<1, Types>::type lib_t;
    generator::generator<lib_t, val_t> gen;
};

TYPED_TEST_CASE(ones_test, test_settings<>::types_to_test);

GENERATE_GENERAL_TEST(ones_test, small, test_settings<>::small_sizes, generator::property::ones())
GENERATE_GENERAL_TEST(ones_test, medium, test_settings<>::medium_sizes, generator::property::ones())
GENERATE_HERMITIAN_TEST(ones_test, small, test_settings<>::small_sq_sizes, generator::property::ones())
GENERATE_HERMITIAN_TEST(ones_test, medium, test_settings<>::medium_sq_sizes, generator::property::ones())
GENERATE_UPPER_TRIANGULAR_TEST(ones_test, small, test_settings<>::small_sq_sizes, generator::property::ones())
GENERATE_UPPER_TRIANGULAR_TEST(ones_test, medium, test_settings<>::medium_sq_sizes, generator::property::ones())
GENERATE_LOWER_TRIANGULAR_TEST(ones_test, small, test_settings<>::small_sq_sizes, generator::property::ones())
GENERATE_LOWER_TRIANGULAR_TEST(ones_test, medium, test_settings<>::medium_sq_sizes, generator::property::ones())
GENERATE_DIAGONAL_TEST(ones_test, small, test_settings<>::small_sq_sizes, generator::property::ones())
GENERATE_DIAGONAL_TEST(ones_test, medium, test_settings<>::medium_sq_sizes, generator::property::ones())


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
