//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <tuple>

#include "gtest/gtest.h"

#include <libraries.hpp>
#include <generator/generator.hpp>


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

TYPED_TEST(random_test, test_small) {
    auto mat = this->gen.generate(generator::shape::general(1, 1), generator::property::random());
    EXPECT_EQ(mat.rows(), 1u);
    EXPECT_EQ(mat.columns(), 1u);

    mat = this->gen.generate(generator::shape::general(2, 1), generator::property::random());
    EXPECT_EQ(mat.rows(), 2u);
    EXPECT_EQ(mat.columns(), 1u);

    mat = this->gen.generate(generator::shape::general(25, 50), generator::property::random());
    EXPECT_EQ(mat.rows(), 25u);
    EXPECT_EQ(mat.columns(), 50u);

    mat = this->gen.generate(generator::shape::general(50, 25), generator::property::random());
    EXPECT_EQ(mat.rows(), 50u);
    EXPECT_EQ(mat.columns(), 25u);
}

TYPED_TEST(random_test, test_medium) {
    auto mat = this->gen.generate(generator::shape::general(100, 100), generator::property::random());
    EXPECT_EQ(mat.rows(), 100u);
    EXPECT_EQ(mat.columns(), 100u);

    mat = this->gen.generate(generator::shape::general(199, 173), generator::property::random());
    EXPECT_EQ(mat.rows(), 199u);
    EXPECT_EQ(mat.columns(), 173u);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
