//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_INTERMEDIATE_HPP
#define LINALG_TESTS_GENERATOR_INTERMEDIATE_HPP

#include <generator/shape.hpp>

namespace generator { namespace shape {

    template<typename T, typename Shape>
    struct intermediate;

    template<typename T>
    struct intermediate<T, generator::shape::general>
    {
        typedef std::unique_ptr<T[]> type;

        static type create(const generator::shape::general & gen)
        {
            return create(gen.rows, gen.cols);
        }
    protected:
        static type create(uint32_t rows, uint32_t cols)
        {
            return type(new T[rows * cols]);
        }
    };

    /// Self-adjoint matrix is stored as a general matrix
    template<typename T>
    struct intermediate<T, generator::shape::self_adjoint> : 
        intermediate<T,generator::shape::general>
    {
        typedef std::unique_ptr<T[]> type;
        typedef intermediate<T,generator::shape::general> base_t;

        static type create(const generator::shape::self_adjoint & gen)
        {
            return base_t::create(gen.rows, gen.cols);
        }
    };

}}

#endif
