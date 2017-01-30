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

#define GENERATE_TESTS(name, prop, gtest_operand)           \
template<typename Matrix>                                       \
void verify_##name(Matrix && mat, uint32_t rows, uint32_t cols)\
{                                                                                       \
    EXPECT_EQ(mat.rows(), rows);            \
    EXPECT_EQ(mat.columns(), cols);         \
    for(uint32_t i = 0; i < rows; ++i) {    \
        for(uint32_t j = 0; j < cols; ++j) {    \
            gtest_operand(mat(i, j), 0.0f); \
        }   \
    }   \
}   \
    \
TYPED_TEST(random_test, test_small_##name) {   \
    auto mat = this->gen.generate(generator::shape::general(1, 1), generator::property::random(),   \
                                  prop);    \
    verify_##name(mat, 1u, 1u);   \
    \
    mat = this->gen.generate(generator::shape::general(2, 1), generator::property::random(),    \
                             prop);  \
    verify_##name(mat, 2u, 1u);   \
    \
    mat = this->gen.generate(generator::shape::general(25, 50), generator::property::random(),  \
                             prop);  \
    verify_##name(mat, 25u, 50u); \
    \
    mat = this->gen.generate(generator::shape::general(50, 25), generator::property::random(),  \
                             prop);  \
    verify_##name(mat, 50u, 25u); \
}  \
    \
TYPED_TEST(random_test, test_medium_##name) {  \
    auto mat = this->gen.generate(generator::shape::general(100, 100), generator::property::random(),   \
                                  prop); \
    verify_##name(mat, 100u, 100u);   \
    \
    mat = this->gen.generate(generator::shape::general(199, 173), generator::property::random(),    \
                             prop);  \
    verify_##name(mat, 199u, 173u);   \
}

GENERATE_TESTS(positive, generator::property::positive(), EXPECT_GT)
GENERATE_TESTS(negative, generator::property::negative(), EXPECT_LT)


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
