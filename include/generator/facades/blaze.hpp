//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_FACADES_BLAZE_HPP
#define LINALG_TESTS_GENERATOR_FACADES_BLAZE_HPP

#include <libraries.hpp>
#include <generator/generator.hpp>
#include <generator/facades/facade.hpp>
#include <generator/shape.hpp>

namespace generator {

    namespace detail {

        template<typename T, typename Shape>
        struct blaze_matrix_type;

        template<typename T>
        struct blaze_matrix_type<T, generator::shape::general>
        {
            typedef blaze::DynamicMatrix<T> type;
        };

    }

    // Forward declaration
    template<typename Library, typename T>
    struct generator;

    template<typename T>
    struct generator<library::blaze, T> : public
        detail::generator_facade<generator<library::blaze, T>, T, detail::blaze_matrix_type>
    {
        typedef T value_t;

        //explicit specialization inside class is not allowed
        template<typename Shape>
        using matrix_t = typename detail::blaze_matrix_type<T, Shape>::type;

        template<typename... Properties>
        void fill(float *) {}
    };
}

#endif
