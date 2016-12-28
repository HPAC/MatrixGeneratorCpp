//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_SHAPE_HPP
#define LINALG_TESTS_GENERATOR_SHAPE_HPP

namespace matrix { namespace shape {

    namespace detail {

        template<bool SelfAdjoint>
        struct general
        {
            typedef SelfAdjoint SelfAdjoint;
        protected:
            constexpr int rows_;
            constexpr int cols_;
            
            constexpr general(int rows, int cols):
                rows_(rows),
                cols_(cols)
            {}
        };

    }

    struct general : detail::general<false>
    {
        constexpr general(int rows, int cols):
            detail::general<false>(rows, cols) {}
    };

    struct self_adjoint : detail::general<true> {};

    struct triangular;

    struct tridiagonal;

}}

#endif
