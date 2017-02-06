//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_ZEROS_HPP
#define LINALG_TESTS_ZEROS_HPP

#include <generator/properties/property_id.hpp>
#include <generator/property.hpp>
#include <generator/properties/filler.hpp>

namespace generator { namespace property {

    template<typename T, uint64_t type>
    struct property;

    template<typename T>
    struct property<T, hash<generator::property::zeros>()>
    {
        typedef typename shape::intermediate<T, shape::general>::type intermediate_t;
    public:
        template<typename RndGen>
        static void fill(const shape::general & shape, intermediate_t & data, RndGen &&)
        {
            detail::Filler<false>::fill(shape.rows, shape.cols, data, []() { return 0.0; });
        }

    };

}}

#endif //LINALG_TESTS_ZEROS_HPP
