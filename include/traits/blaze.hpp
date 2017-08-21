//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_TRAITS_MATRIX_TRAITS_BLAZE_HPP
#define LINALG_TESTS_TRAITS_MATRIX_TRAITS_BLAZE_HPP

#include <blaze/Blaze.h>

namespace traits {

    template<typename MatType, typename Enable>
    struct matrix_traits;

    namespace detail {
        /// FIXME: why naming of those types is different?
        /// We have Type for Dense, ElementType for Hermitian
        template<typename MatType>
        struct blaze_matrix
        {
            typedef typename MatType::ElementType value_t;

            static uint64_t rows(const MatType & mat)
            {
                return mat.rows();
            }

            static uint64_t columns(const MatType & mat)
            {
                return mat.columns();
            }

            static decltype(auto) get(const MatType & mat, uint64_t i, uint64_t j)
            {
                return mat(i, j);
            }

            template<typename ExprType>
            static decltype(auto) eval(ExprType && expr)
            {
                return blaze::evaluate(expr);
            }
        };
    }

    template<typename T>
    struct matrix_traits< blaze::DynamicMatrix<T>, void> :
        detail::blaze_matrix< blaze::DynamicMatrix<T> >
    {};

    template<typename T>
    struct matrix_traits< blaze::HermitianMatrix<blaze::DynamicMatrix<T>>, void> :
        detail::blaze_matrix< blaze::HermitianMatrix<blaze::DynamicMatrix<T>> >
    {};

    template<typename T>
    struct matrix_traits< blaze::UpperMatrix<blaze::DynamicMatrix<T>>, void> :
        detail::blaze_matrix< blaze::UpperMatrix<blaze::DynamicMatrix<T>> >
    {};

    template<typename T>
    struct matrix_traits< blaze::LowerMatrix<blaze::DynamicMatrix<T>>, void> :
        detail::blaze_matrix< blaze::LowerMatrix<blaze::DynamicMatrix<T>> >
    {};

    template<typename T>
    struct matrix_traits< blaze::DiagonalMatrix<blaze::DynamicMatrix<T>>, void> :
        detail::blaze_matrix< blaze::DiagonalMatrix<blaze::DynamicMatrix<T>> >
    {};
}

#endif
