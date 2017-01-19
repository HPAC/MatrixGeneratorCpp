//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_PROPERTY_HPP
#define LINALG_TESTS_GENERATOR_PROPERTY_HPP

#include <generator/properties/property_id.hpp>

namespace generator { namespace property {

    struct random : property_t<0> {};
    struct positive : property_t<1> {};
    struct negative : property_t<2> {};

    template<typename T, uint64_t type>
    struct property;

}}



#endif
