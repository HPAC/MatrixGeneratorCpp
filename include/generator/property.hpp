//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_PROPERTY_HPP
#define LINALG_TESTS_GENERATOR_PROPERTY_HPP

#include <generator/properties/property_id.hpp>

namespace generator { namespace property {

    struct random   : property_t<0> {};
    struct positive : property_t<1> {};
    struct negative : property_t<2> {};

    struct constant : property_t<3>
    {
    	double fill_value;
    	constexpr constant(double value_):
    		fill_value(value_)
    	{}
    };
    
    struct zeros    : property_t<4>
    {
        constexpr static double fill_value = 0.0;
    };

    struct ones    	: property_t<5>
    {
        constexpr static double fill_value = 1.0;
    };

    struct eye      : property_t<6> {};

#ifdef HAVE_BLAS
    struct spd		: property_t<7> {};
#endif

#ifdef HAVE_LAPACK
    struct orthogonal: property_t<8> {};
#endif

    template<typename T, uint64_t type>
    struct property;

}}



#endif
