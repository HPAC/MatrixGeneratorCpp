//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_SHAPE_HPP
#define LINALG_TESTS_GENERATOR_SHAPE_HPP

namespace generator { namespace shape {

    namespace detail {

        template<bool _SelfAdjoint>
        struct general
        {
            //typedef _SelfAdjoint SelfAdjoint;
            const uint32_t rows;
            const uint32_t cols;
            
            constexpr general(uint32_t rows, uint32_t cols):
                rows(rows),
                cols(cols)
            {}
        };

    }

    struct general : detail::general<false>
    {
        constexpr general(uint32_t rows, uint32_t cols):
            detail::general<false>(rows, cols) {}
    };

    struct self_adjoint : detail::general<true>
    {
        constexpr self_adjoint(uint32_t rows):
                detail::general<true>(rows, rows) {}
    };

    struct triangular;

    template<int SubDiagonals, int SuperDiagonals, bool Hermitian>
    struct banded
    {
        static constexpr bool is_hermitian = Hermitian;
        static constexpr int subdiagonals = SubDiagonals;
        static constexpr int superdiagonals = SuperDiagonals;
        const uint32_t rows;
        const uint32_t cols;

        constexpr banded(uint32_t rows_, uint32_t cols_):
                rows(rows_),
                cols(cols_)
        {}
    };

    template<bool Hermitian>
    struct tridiagonal : banded<1, 1, Hermitian>
    {
        constexpr tridiagonal(uint32_t rows, uint32_t cols) :
            banded<1, 1, Hermitian>(rows, rows)
        {}
    };

    struct diagonal : banded<0, 0, false>
    {
        constexpr diagonal(uint32_t rows) :
            banded<0, 0, false>(rows, rows)
        {}
    };

}}

#endif
