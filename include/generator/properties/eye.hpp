//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_PROPERTIES_EYE_HPP
#define LINALG_PROPERTIES_EYE_HPP

#include <cstring>

#include <generator/shape.hpp>
#include <generator/intermediate.hpp>
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

        template<typename RndGen, typename... Properties>
        static void fill(const intermediate::general<T> & shape,
                        RndGen &&, Properties &&...)
        {
            fill(shape.data, shape.size.rows, shape.size.cols);
        }

        template<typename RndGen, typename... Properties>
        static void fill(const intermediate::upper_triangular<T> & shape,
                        RndGen &&, Properties &&...)
        {
            fill(shape.data, shape.size.rows, shape.size.cols);
        }

        template<typename RndGen, typename... Properties>
        static void fill(const intermediate::lower_triangular<T> & shape,
                        RndGen &&, Properties &&...)
        {
            fill(shape.data, shape.size.rows, shape.size.cols);
        }

        template<typename RndGen, typename... Properties>
        static void fill(const intermediate::self_adjoint<T> & shape,
                        RndGen &&, Properties &&...)
        {
            fill(shape.data, shape.rows, shape.rows);
        }

        template<typename RndGen, typename... Properties>
        static void fill(const intermediate::diagonal<T> & shape,
                        RndGen &&, Properties &&...)
        {
            std::fill(shape.data.get(), shape.data.get() + shape.size.rows, 1.0);
        }

    };

}}

#endif //LINALG_TESTS_EYE_HPP
