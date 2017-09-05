//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_TRAITS_MATRIX_TRAITS_ARMADILLO_HPP
#define LINALG_TESTS_TRAITS_MATRIX_TRAITS_ARMADILLO_HPP

#include <type_traits>

#include <armadillo>

namespace traits {

    template<typename MatType, typename Enable>
    struct matrix_traits;

    template<typename T>
    struct matrix_traits<
        arma::Mat<T>,
        void
    >
    {
        typedef T value_t;

        static uint64_t rows(const arma::Mat<T> & mat)
        {
            return mat.n_rows;
        }

        static uint64_t columns(const arma::Mat<T> & mat)
        {
            return mat.n_cols;
        }

        template<typename ExprType>
        static decltype(auto) eval(ExprType && expr)
        {
            return expr.eval();
        }

        static decltype(auto) get(const arma::Mat<T> & mat, uint64_t i, uint64_t j)
        {
            return mat(i, j);
        }
    };

    namespace detail {

        template<typename T>
        struct vec_traits
        {
            typedef T value_t;

            template<typename ExprType>
            static decltype(auto) eval(ExprType && expr)
            {
                return expr.eval();
            }

            template<typename Vec>
            static decltype(auto) get(const Vec & mat, uint64_t i)
            {
                return mat(i);
            }

        };
    }

    template<typename T>
    struct matrix_traits<
        arma::Row<T>,
        void
    > : detail::vec_traits<T>
    {
        static uint64_t rows(const arma::Row<T> &)
        {
            return 1;
        }

        static uint64_t columns(const arma::Row<T> & mat)
        {
            return mat.n_cols;
        }
    };

    template<typename T>
    struct matrix_traits<
        arma::Col<T>,
        void
    > : detail::vec_traits<T>
    {
        static uint64_t rows(const arma::Col<T> & mat)
        {
            return mat.n_rows;
        }

        static uint64_t columns(const arma::Col<T> &)
        {
            return 1;
        }
    };

}

#endif
