//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_PROPERTIES_SPD_HPP
#define LINALG_PROPERTIES_SPD_HPP

#include <generator/intermediate.hpp>
#include <generator/properties/random.hpp>
#include <generator/cblas_wrapper.hpp>

namespace generator { namespace property {

    template<typename T, uint64_t type>
    struct property;

    template<typename T>
    struct property<T, hash<generator::property::spd>()>
    {
        template<typename ArrayType, typename RndGen, typename... Properties>
        static void create_spd(ArrayType && array, const shape::matrix_size & size,
            RndGen && gen, Properties &&... props)
        {
            // Create an upper triangular matrix
            intermediate::upper_triangular<T> temp{size};
            property<
                T, 
                hash<generator::property::random>()
            >::fill(temp, gen, std::forward<Properties>(props)...);
            // To ensure that matrix is non-singular, add a value to diagonal
            T * ptr = temp.data.get();
            for(uint32_t i = 0; i < size.rows; ++i) {
                ptr[i*size.rows + i] += static_cast<T>(3.0);
            }
            // U * U' creates an SPD matrix
            cblas::TRMM<T>::call(size, temp.data, array);
        }

        template<typename RndGen, typename... Properties>
        static void fill(intermediate::general<T> & shape,
                        RndGen && gen, Properties &&... props)
        {
            if(shape.size.rows != shape.size.cols) {
                throw std::runtime_error("An SPD matrix has to be square!");
            }
            create_spd(shape.data, shape.size, gen, std::forward<Properties>(props)...);
        }

        template<typename RndGen, typename... Properties>
        static void fill(intermediate::self_adjoint<T> & shape,
                        RndGen && gen, Properties &&... props)
        {
            create_spd(shape.data, shape.size, gen, std::forward<Properties>(props)...);
        }

        template<typename RndGen, typename... Properties>
        static void fill(const intermediate::upper_triangular<T> & shape,
                        RndGen &&, Properties &&...)
        {
            //static_assert(false, "An upper triangular matrix cannot be symmetric positive-definite!");
        }

        template<typename RndGen, typename... Properties>
        static void fill(const intermediate::lower_triangular<T> & shape,
                        RndGen &&, Properties &&...)
        {
            //static_assert(false, "A lower triangular matrix cannot be symmetric positive-definite!");
        }

        // A diagonal matrix is SPD when all entries are positive
        template<typename RndGen, typename... Properties>
        static void fill(const intermediate::diagonal<T> & shape,
                        RndGen && gen, Properties &&... props)
        {
            if(shape.size.rows != shape.size.cols) {
                throw std::runtime_error("An SPD matrix has to be square!");
            }
            property<
                T, 
                hash<generator::property::random, generator::property::positive>()
            >::fill(shape, gen, std::forward<Properties>(props)...);
        }

    };

}}

#endif //LINALG_TESTS_EYE_HPP
