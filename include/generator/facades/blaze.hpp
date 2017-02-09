//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_FACADES_BLAZE_HPP
#define LINALG_TESTS_GENERATOR_FACADES_BLAZE_HPP

#include <ctime>

#include <libraries.hpp>
#include <generator/generator.hpp>
#include <generator/facades/facade.hpp>

namespace generator {

    namespace detail {

        template<typename T, typename Shape>
        struct blaze_matrix_type;

        template<typename T>
        struct blaze_matrix_type<T, generator::shape::general>
        {
            typedef blaze::DynamicMatrix<T> type;

            static type create(uint32_t rows, uint32_t cols, T * ptr)
            {
                return type(rows, cols, ptr);
            }
        };

        /// As long as we don't use block matrices,
        /// Hermitian and Symmetric in Blaze behaves identically for non-complex types
        /// Hence we can always use Hermitian
        /// \tparam T
        template<typename T>
        struct blaze_matrix_type<T, generator::shape::self_adjoint>
        {
            typedef blaze::HermitianMatrix< blaze::DynamicMatrix<T> > type;

            //FIXME: why the syntax of from-array constructor for DynamicMatrix
            //and HermitianMatrix is different in Blaze?
            static type create(uint32_t rows, uint32_t cols, T * ptr)
            {
                return type(ptr, rows, cols);
            }
        };

    }

    // Forward declaration
    template<typename Library, typename T>
    struct generator;

    template<typename T>
    struct generator<library::blaze, T> : public
        detail::generator_facade<generator<library::blaze, T>, T, detail::blaze_matrix_type>
    {
        typedef detail::generator_facade<
                generator<library::blaze, T>,
                T,
                detail::blaze_matrix_type > base_t;
        typedef T value_t;

        template<typename Shape>
        using intermediate_t = typename base_t::template intermediate_t<Shape>;

        template<typename Shape>
        using matrix_t = typename base_t::template matrix_t<Shape>;

        generator(uint32_t seed = time(0)) :
                base_t(seed)
        {}

        template<typename Shape, typename... Properties>
        matrix_t<Shape> create(Shape && shape, const intermediate_t<Shape> & data)
        {
            //return matrix_t<Shape>(data.get(), shape.rows, shape.cols);
            return detail::blaze_matrix_type<T, Shape>::create(shape.rows, shape.cols, data.get());
        };
    };
}

#endif
