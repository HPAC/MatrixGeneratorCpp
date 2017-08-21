//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_PROPERTIES_ORTHOGONAL_HPP
#define LINALG_PROPERTIES_ORTHOGONAL_HPP

#include <generator/intermediate.hpp>
#include <generator/properties/random.hpp>
#include <generator/lapack_wrapper.hpp>

namespace generator { namespace property {

    namespace detail {

        template<typename T, bool Positive, bool Negative>
        struct orthogonal_generator
        {
            template<typename RndGen, typename... Properties>
            static void fill(intermediate::general<T> & shape,
                            RndGen && gen, Properties &&... props)
            {
                if(shape.size.rows != shape.size.cols) {
                    throw std::runtime_error("An orthogonal matrix has to be square!");
                }
                // Create a regular, random matrix
                intermediate::general<T> temp{shape.size};
                property<
                    T,
                    hash<generator::property::random>()
                >::fill(temp, gen, std::forward<Properties>(props)...);
                // Now create Q matrix from QR factorization as the return matrix
                lapack::QR<T>::q_matrix(shape.size, temp.data, shape.data);
            }

            // A diagonal matrix is orthogonal iff all columns are Euclidean basis vector
            // Thus, randomize -1 and 1 on diagonal
            template<typename RndGen, typename... Properties>
            static void fill(const intermediate::diagonal<T> & shape,
                            RndGen && gen, Properties &&... props)
            {
                if(shape.size.rows != shape.size.cols) {
                    throw std::runtime_error("An orthogonal matrix has to be square!");
                }
                property<
                    T,
                    hash<generator::property::random, generator::property::positive>()
                >::fill(shape, gen, std::forward<Properties>(props)...);
            }

            // Not implemented for: upper_triangular, lower_triangular
            template<typename Shape, typename RndGen, typename... Properties>
            static void fill(const Shape &,
                            RndGen &&, Properties &&...)
            {
                static_assert(
                    !(std::is_same<Shape, intermediate::upper_triangular<T>>::value
                        || std::is_same<Shape, intermediate::lower_triangular<T>>::value
                        || std::is_same<Shape, intermediate::self_adjoint<T>>::value),
                    "Orthogonal generation is not possible for symmetric and triangular matrices"
                    );
            }
        };
    }

    template<typename T, uint64_t type>
    struct property;

    template<typename T>
    struct property<T, hash<generator::property::orthogonal>()>
        : detail::orthogonal_generator<T, false, false>
    {};

    template<typename T>
    struct property<T, hash<generator::property::orthogonal, generator::property::positive>()>
        : detail::orthogonal_generator<T, true, false>
    {};

    template<typename T>
    struct property<T, hash<generator::property::orthogonal, generator::property::negative>()>
        : detail::orthogonal_generator<T, false, true>
    {};

}}

#endif //LINALG_TESTS_EYE_HPP
