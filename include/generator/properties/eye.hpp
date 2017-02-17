//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_PROPERTIES_EYE_HPP
#define LINALG_PROPERTIES_EYE_HPP

#include <generator/shape.hpp>
#include <generator/property.hpp>
#include <generator/properties/filler.hpp>

namespace generator { namespace property {

    template<typename T, uint64_t type>
    struct property;

    template<typename T>
    struct property<T, hash<generator::property::eye>()>
    {

        template<typename ArrayType>
        static void fill(ArrayType && array, size_t rows, size_t cols)
        {
            /// sizeof(double/float) * number of elements in array
            memset(array.get(), 0, sizeof(*array.get())*(rows * cols));
            size_t size = std::min(rows, cols);
            for(size_t i = 0; i < size; ++i) {
                array[i*cols + i] = 1.0;
            }
        }

    public:

        template<typename RndGen>
        static void fill(const shape::general & shape,
                        typename shape::intermediate<T, shape::general>::type & data,
                        RndGen &&)
        {
            fill(data, shape.rows, shape.cols);
        }

        template<typename RndGen>
        static void fill(const shape::self_adjoint & shape,
                        typename shape::intermediate<T, shape::self_adjoint>::type & data,
                        RndGen &&)
        {
            fill(data, shape.rows, shape.rows);
        }

        template<typename RndGen>
        static void fill(const shape::diagonal & shape,
                        typename shape::intermediate<T, shape::diagonal>::type & data,
                        RndGen &&)
        {
            std::fill(data.get(), data.get() + shape.rows, 1.0);
        }


    };

}}

#endif //LINALG_TESTS_EYE_HPP
