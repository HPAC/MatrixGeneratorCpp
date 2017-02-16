//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_PROPERTIES_ZEROS_HPP
#define LINALG_PROPERTIES_ZEROS_HPP

#include <generator/shape.hpp>
#include <generator/property.hpp>
#include <generator/properties/filler.hpp>

namespace generator { namespace property {

    template<typename T, uint64_t type>
    struct property;

    template<typename T>
    struct property<T, hash<generator::property::zeros>()>
    {
        template<typename FwdIt>
        static void fill(FwdIt begin, FwdIt end)
        {
            std::fill(begin, end, 0.0);
        }

    public:

        template<typename RndGen>
        static void fill(const shape::general & shape,
                        typename shape::intermediate<T, shape::general>::type & data,
                        RndGen &&)
        {
            fill(data.get(), data.get() + shape.rows * shape.cols);
        }

        template<typename RndGen>
        static void fill(const shape::self_adjoint & shape,
                        typename shape::intermediate<T, shape::self_adjoint>::type & data,
                        RndGen &&)
        {
            fill(data.get(), data.get() + shape.rows * shape.rows);
        }

        template<typename RndGen>
        static void fill(const shape::diagonal & shape,
                        typename shape::intermediate<T, shape::diagonal>::type & data,
                        RndGen &&)
        {
            fill(data.get(), data.get() + shape.rows);
        }

    };

}}

#endif //LINALG_TESTS_ZEROS_HPP
