//  Copyright (c) 2016 Marcin Copik
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
struct random_test : public testing::Test
{
    typedef typename std::tuple_element<0, Types>::type val_t;
    typedef typename std::tuple_element<1, Types>::type lib_t;
    generator::generator<lib_t, val_t> gen;
};

typedef testing::Types<
            std::tuple<float, library::blaze>,
            std::tuple<double, library::blaze>
        > types_to_test;

TYPED_TEST_CASE(random_test, types_to_test);

std::array< std::tuple<uint32_t, uint32_t>, 4> small_sizes{ make_tuple(1,1), make_tuple(2, 1), make_tuple(25, 50), make_tuple(50, 25)};
std::array< uint32_t, 4> small_sq_sizes{1, 2, 9, 25};
std::array< std::tuple<uint32_t, uint32_t>, 2> medium_sizes{make_tuple(100, 100), make_tuple(199, 173)};
std::array< uint32_t, 3> medium_sq_sizes{100, 103, 125};

GENERATE_GENERAL_TEST(small, small_sizes, generator::property::random())
GENERATE_GENERAL_TEST(medium, medium_sizes, generator::property::random())
GENERATE_GENERAL_TEST(positive_small, small_sizes, generator::property::random(), generator::property::positive())
GENERATE_GENERAL_TEST(positive_medium, medium_sizes, generator::property::random(), generator::property::positive())
GENERATE_GENERAL_TEST(negative_small, small_sizes, generator::property::random(), generator::property::negative())
GENERATE_GENERAL_TEST(negative_medium, medium_sizes, generator::property::random(), generator::property::negative())

GENERATE_HERMITIAN_TEST(small, small_sq_sizes, generator::property::random())
GENERATE_HERMITIAN_TEST(medium, medium_sq_sizes, generator::property::random())
GENERATE_HERMITIAN_TEST(positive_small, small_sq_sizes, generator::property::random(), generator::property::positive())
GENERATE_HERMITIAN_TEST(positive_medium, medium_sq_sizes, generator::property::random(), generator::property::positive())
GENERATE_HERMITIAN_TEST(negative_small, small_sq_sizes, generator::property::random(), generator::property::negative())
GENERATE_HERMITIAN_TEST(negative_medium, medium_sq_sizes, generator::property::random(), generator::property::negative())

GENERATE_DIAGONAL_TEST(small, small_sq_sizes, generator::property::random())
GENERATE_DIAGONAL_TEST(medium, medium_sq_sizes, generator::property::random())
GENERATE_DIAGONAL_TEST(positive_small, small_sq_sizes, generator::property::random(), generator::property::positive())
GENERATE_DIAGONAL_TEST(positive_medium, medium_sq_sizes, generator::property::random(), generator::property::positive())
GENERATE_DIAGONAL_TEST(negative_small, small_sq_sizes, generator::property::random(), generator::property::negative())
GENERATE_DIAGONAL_TEST(negative_medium, medium_sq_sizes, generator::property::random(), generator::property::negative())


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
