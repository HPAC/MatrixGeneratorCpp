//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "gtest/gtest.h"

#include <libraries.hpp>
#include <generator/generator.hpp>
#include <generator/lapack_wrapper.hpp>
#include "test_utilities.hpp"

template<typename Types>
struct spd_test : public testing::Test
{
    typedef typename std::tuple_element<0, Types>::type val_t;
    typedef typename std::tuple_element<1, Types>::type lib_t;
    generator::generator<lib_t, val_t> gen;
};

TYPED_TEST_CASE(spd_test, test_settings<>::types_to_test);

GENERATE_GENERAL_TEST(spd_test, small, test_settings<>::small_sq_sizes, generator::property::spd())
GENERATE_GENERAL_TEST(spd_test, medium, test_settings<>::medium_sq_sizes, generator::property::spd())
GENERATE_GENERAL_TEST(spd_test, positive_small, test_settings<>::small_sq_sizes, generator::property::spd(), generator::property::positive())
GENERATE_GENERAL_TEST(spd_test, positive_medium, test_settings<>::medium_sq_sizes, generator::property::spd(), generator::property::positive())

GENERATE_HERMITIAN_TEST(spd_test, small, test_settings<>::small_sq_sizes, generator::property::spd())
GENERATE_HERMITIAN_TEST(spd_test, medium, test_settings<>::medium_sq_sizes, generator::property::spd())
GENERATE_HERMITIAN_TEST(spd_test, positive_small, test_settings<>::small_sq_sizes, generator::property::spd(), generator::property::positive())
GENERATE_HERMITIAN_TEST(spd_test, positive_medium, test_settings<>::medium_sq_sizes, generator::property::spd(), generator::property::positive())

GENERATE_DIAGONAL_TEST(spd_test, small, test_settings<>::small_sq_sizes, generator::property::spd())
GENERATE_DIAGONAL_TEST(spd_test, medium, test_settings<>::medium_sq_sizes, generator::property::spd())
GENERATE_DIAGONAL_TEST(spd_test, positive_small, test_settings<>::small_sq_sizes, generator::property::spd(), generator::property::positive())
GENERATE_DIAGONAL_TEST(spd_test, positive_medium, test_settings<>::medium_sq_sizes, generator::property::spd(), generator::property::positive())


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
