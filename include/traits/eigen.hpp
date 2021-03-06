//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_TRAITS_MATRIX_TRAITS_EIGEN_HPP
#define LINALG_TESTS_TRAITS_MATRIX_TRAITS_EIGEN_HPP

#include <type_traits>

#include <Eigen/Core>

namespace traits {

    template<typename MatType, typename Enable>
    struct matrix_traits;

    namespace detail {

        template<typename MatType>
        struct eigen_traits
        {
            static uint64_t rows(const MatType & mat)
            {
                return mat.rows();
            }

            static uint64_t columns(const MatType & mat)
            {
                return mat.cols();
            }

            template<typename ExprType>
            static decltype(auto) eval(ExprType && expr)
            {
                return expr.eval();
            }
        };

        template<typename MatType>
        struct eigen_vec_traits
        {
            typedef typename MatType::value_type value_t;

            static uint64_t rows(const MatType & mat)
            {
                return mat.rows();
            }

            static uint64_t columns(const MatType & mat)
            {
                return mat.cols();
            }

            template<typename ExprType>
            static decltype(auto) eval(ExprType && expr)
            {
                return expr.eval();
            }

            static decltype(auto) get(const MatType & mat, uint64_t i)
            {
                return mat(i);
            }
        };

    }

    template<typename T>
    struct matrix_traits<
        /*MatType,
        typename std::enable_if<
            std::is_base_of<Eigen::EigenBase<MatType>, MatType>::value &&
            !std::is_same<MatType, Eigen::DiagonalMatrix<typename MatType::value_type, Eigen::Dynamic, Eigen::Dynamic>>::value
        >::type*/
        Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>,
        void
    > : detail::eigen_traits< Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> >
    {
        typedef T value_t;
        typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> mat_t;

        static decltype(auto) get(const mat_t & mat, uint64_t i, uint64_t j)
        {
            return mat(i, j);
        }
    };  

#if defined(HAVE_EIGEN_DIAGONAL_MATRIX)
    template<typename T>
    struct matrix_traits<
        Eigen::DiagonalMatrix<T, Eigen::Dynamic, Eigen::Dynamic>,
        void
    > : detail::eigen_traits<
            Eigen::DiagonalMatrix<T, Eigen::Dynamic, Eigen::Dynamic>
        >
    {
        typedef Eigen::DiagonalMatrix<T, Eigen::Dynamic, Eigen::Dynamic> mat_t;
        typedef typename mat_t::Scalar value_t;

        // Eigen::DiagonalMatrix does not implement a call operator
        static T get(const mat_t & mat, uint64_t i, uint64_t j)
        {
            if(i == j) {
                return mat.diagonal().coeffRef(i);
            } else {
                return static_cast<T>(0.0);
            }
        }

    };
#endif

    template<typename T>
    struct matrix_traits<
        Eigen::Matrix<T, 1, Eigen::Dynamic>,
        void
    > : detail::eigen_vec_traits< Eigen::Matrix<T, 1, Eigen::Dynamic> >
    {};

    template<typename T>
    struct matrix_traits<
        Eigen::Matrix<T, Eigen::Dynamic, 1>,
        void
    > : detail::eigen_vec_traits< Eigen::Matrix<T, Eigen::Dynamic, 1> >
    {};  
}

#endif
