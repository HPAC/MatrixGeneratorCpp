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

TYPED_TEST(random_test, general_test_small)
{
    for(auto & sizes : small_sizes)
    {
        uint32_t rows = std::get<0>(sizes), cols = std::get<1>(sizes);
        auto mat = this->gen.generate(generator::shape::general(rows, cols), generator::property::random());
        EXPECT_EQ(mat.rows(), rows);
        EXPECT_EQ(mat.columns(), cols);
    }
}

TYPED_TEST(random_test, general_test_medium)
{
    for(auto & sizes : medium_sizes)
    {
        uint32_t rows = std::get<0>(sizes), cols = std::get<1>(sizes);
        auto mat = this->gen.generate(generator::shape::general(rows, cols), generator::property::random());
        EXPECT_EQ(mat.rows(), rows);
        EXPECT_EQ(mat.columns(), cols);
    }
}

TYPED_TEST(random_test, symmetric_test_small)
{
    for(auto rows : small_sq_sizes)
    {
        auto mat = this->gen.generate(generator::shape::self_adjoint(rows), generator::property::random());
        verify_hermitian(mat, rows);
    }
}

TYPED_TEST(random_test, symmetric_test_medium)
{
    for(auto rows : medium_sq_sizes)
    {
        auto mat = this->gen.generate(generator::shape::self_adjoint(rows), generator::property::random());
        verify_hermitian(mat, rows);
    }
}

GENERATE_TESTS(positive_small, generator::property::positive(), small_sizes)
GENERATE_TESTS(positive_medium, generator::property::positive(), medium_sizes)
GENERATE_TESTS(negative_small, generator::property::negative(), small_sizes)
GENERATE_TESTS(negative_medium, generator::property::negative(), medium_sizes)

GENERATE_HERMITIAN_TESTS(positive_small, generator::property::positive(), small_sq_sizes)
GENERATE_HERMITIAN_TESTS(positive_medium, generator::property::positive(), medium_sq_sizes)
GENERATE_HERMITIAN_TESTS(negative_small, generator::property::negative(), small_sq_sizes)
GENERATE_HERMITIAN_TESTS(negative_medium, generator::property::negative(), medium_sq_sizes)


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
