//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_FACADES_FACADE_HPP
#define LINALG_TESTS_GENERATOR_FACADES_FACADE_HPP

#include <generator/shape.hpp>
#include <generator/intermediate.hpp>

namespace generator { namespace detail {

    // Curiously Recurring Template Pattern
    //
    // Implementation should define two methods:
    // - allocate - create matrix depending on shape type
    // - fill -
    template<typename GeneratorImpl, typename T, template<typename, typename> class MatrixType>
    struct generator_facade
    {
        typedef T value_t;

        template<typename Shape>
        using intermediate_t = typename shape::intermediate<T, Shape>::type;

        template<typename Shape>
        using matrix_t = typename MatrixType<T, Shape>::type;
    public:
        template<typename Shape, typename... Properties>
        matrix_t<Shape> generate(Shape && shape, Properties &&...)
        {
            //auto data = dynamic_cast<GeneratorImpl*>(this)->template allocate( std::forward<Shape>(shape) );
            intermediate_t<Shape> data = shape::intermediate<T, Shape>::create(std::forward<Shape>(shape));

            return static_cast<GeneratorImpl*>(this)->template create<Shape, Properties...>(std::forward<Shape>(shape), std::move(data));
        }
    };

}}

#endif
