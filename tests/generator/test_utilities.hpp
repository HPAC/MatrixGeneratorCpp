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
void verify(T)
{}

template<typename T>
void verify(T val, const generator::property::negative &)
{
	EXPECT_LT(val, 0.0f);
}

template<typename T>
void verify(T val, const generator::property::positive &)
{
	EXPECT_GT(val, 0.0f);
}

template<typename MatType, typename ... Properties>
void verify_hermitian(MatType && mat, uint32_t rows, uint32_t cols, Properties &&... props)
{
    typedef typename traits::matrix_traits< std::remove_reference_t<MatType> >::value_t value_t;

    EXPECT_EQ(mat.rows(), rows);
    EXPECT_EQ(mat.columns(), cols);

    for(uint32_t i = 0; i < rows; ++i) {
    	//diagonal
        verify(mat(i, i), std::forward<Properties>(props)...);	
        for(uint32_t j = i + 1; j < cols; ++j) {
            EXPECT_NEAR(mat(i, j), mat(j, i), std::numeric_limits<value_t>::epsilon());
            verify(mat(i, j), std::forward<Properties>(props)...);
        }
    }
}

#define GENERATE_HERMITIAN_TESTS(name, prop, sizes_obj)	\
TYPED_TEST(random_test, symmetric_test_##name) {   \
    for(auto & sizes : sizes_obj)            \
    {                                         \
        uint32_t rows = std::get<0>(sizes), cols = std::get<1>(sizes);	\
        auto mat = this->gen.generate(generator::shape::self_adjoint(rows, cols), generator::property::random(), prop);	\
        verify_hermitian(mat, rows, cols, prop);	\
    }   \
}

#endif
