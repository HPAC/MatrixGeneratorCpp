//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_FACADES_ARMADILLO_HPP
#define LINALG_TESTS_GENERATOR_FACADES_ARMADILLO_HPP

#include <ctime>

#include <armadillo>

#include <libraries.hpp>
#include <generator/facades/facade.hpp>

namespace generator {

    namespace detail {

        template<typename T, typename Shape>
        struct arma_matrix_type
        {
            typedef arma::Mat<T> type;

            // Armadillo stores matrices in a column-major fashion
            // Instead of manually transposing data, let the library handle it
            // When generating matrix (m, n) from transposed data:
            // 1) Create a non-copy temporary of size (n, m)
            // 2) Return transpose, enforcing a call to copy constructor
            static type create(uint32_t rows, uint32_t cols, T * ptr)
            {
                return type{ptr, cols, rows, false}.t();
            }
        };

        template<typename T>
        struct arma_matrix_type<T, ::generator::intermediate::row_vector<T>>
        {
            typedef arma::Row<T> type;

            static type create(uint32_t, uint32_t cols, T * ptr)
            {
                return type{ptr, cols};
            }
        };

        template<typename T>
        struct arma_matrix_type<T, ::generator::intermediate::col_vector<T>>
        {
            typedef arma::Col<T> type;

            static type create(uint32_t rows, uint32_t, T * ptr)
            {
                return type{ptr, rows};
            }
        };

        template<typename T>
        struct arma_matrix_type<T, ::generator::intermediate::diagonal<T>>
        {
            typedef arma::Mat<T> type;

            static type create(uint32_t rows, uint32_t, T * ptr)
            {
                // diagmat copies elements of a vector
                // vector DOES not copy - this is safe since it's only a temporary
                return arma::diagmat( arma::Col<T>{ptr, rows, false, true} );
            }
        };

    }

    // Forward declaration
    template<typename Library, typename T>
    struct generator;

    template<typename T>
    struct generator<library::arma, T> : public
        detail::generator_facade<generator<library::arma, T>, T, detail::arma_matrix_type>
    {
        typedef detail::generator_facade<
                generator<library::arma, T>,
                T,
                detail::arma_matrix_type
            > base_t;
        typedef T value_t;


        template<typename Intermediate>
        using matrix_t = detail::arma_matrix_type<T, std::decay_t<Intermediate>>;

        generator(uint32_t seed = time(0)) :
                base_t(seed)
        {}

        template<typename Intermediate>
        typename matrix_t<Intermediate>::type create(Intermediate && intermediate)
        {
            return matrix_t<Intermediate>::create(
                intermediate.size.rows,
                intermediate.size.cols,
                intermediate.data.get()
                );
        }
    };
}

#endif
