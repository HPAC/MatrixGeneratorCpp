//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "gtest/gtest.h"

#include <libraries.hpp>
#include <generator/generator.hpp>

TEST(random, test_eq) {
    generator::generator<library::blaze, float> gen;
    gen.generate(generator::shape::general(100, 100), generator::property::random());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
