//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_PROPERTIES_ORTHOGONAL_HPP
#define LINALG_PROPERTIES_ORTHOGONAL_HPP

#include <algorithm>

#include <generator/intermediate.hpp>
#include <generator/properties/random.hpp>
#include <generator/cblas_wrapper.hpp>
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

            // Each symmetric orthogonal matrix can be represented as Q * D * Q'
            // where Q is an orthogonal matrix and D is a diagonal matrix of eigenvalues
            // Orthogonal matrix has eigenvalues: 1 and -1
            // Here we use the
            template<typename RndGen, typename... Properties>
            static void fill(intermediate::self_adjoint<T> & shape,
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
                >::fill(shape, gen, std::forward<Properties>(props)...);
                // Now create in temp an orthogonal matrix, don't destroy shape
                lapack::QR<T>::q_matrix(shape.size, shape.data, temp.data, true);

                // Generate diagonal matrix of eigenvalues
                intermediate::diagonal<T> diagonal{shape.size};
                fill(diagonal, std::forward<RndGen>(gen));

                // result = Q * D - each row is multiplied by the diagonal
                // Save in temporary to use as an input for GEMM
                intermediate::general<T> result{shape.size};
                for(uint64_t i = 0; i < shape.size.rows; ++i) {
                    uint64_t offset = shape.size.cols * i;
                    std::transform(
                        temp.data.get() + offset, temp.data.get() + offset + shape.size.cols,
                        diagonal.data.get(), result.data.get() + offset,
                        [](T & val1, T & val2) {
                            return val1 * val2;
                        }
                    );
                }
                // shape = result * temp'
                cblas::GEMM<T>::call(shape.size, result.data, temp.data, shape.data);
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
                std::for_each(shape.data.get(), shape.data.get() + shape.size.rows,
                    [](T & val) {
                        val = val >= static_cast<T>(0.5) ? 1 : -1;
                    }
                );
            }

            // Not implemented for: upper_triangular, lower_triangular
            template<typename Shape, typename RndGen, typename... Properties>
            static void fill(const Shape &,
                            RndGen &&, Properties &&...)
            {
                static_assert(
                    !(std::is_same<Shape, intermediate::upper_triangular<T>>::value
                        || std::is_same<Shape, intermediate::lower_triangular<T>>::value),
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
