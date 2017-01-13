//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_FACADES_FACADE_HPP
#define LINALG_TESTS_GENERATOR_FACADES_FACADE_HPP

namespace generator { namespace detail {

    namespace detail {

        template<typename T, typename Shape>
        struct blaze_matrix_type;

        template<typename T>
        struct blaze_matrix_type<T, generator::shape::general>
        {
            typedef blaze::DynamicMatrix<T> type;
        };

        template<typename T>
        struct blaze_generator
        {
        };

    }

    // Curiously Recurring Template Pattern
    //
    // Implementation should define two methods:
    // - allocate - create matrix depending on shape type
    // - fill -
    template<typename GeneratorImpl, typename T, template<typename, typename> class MatrixType>
    struct generator_facade
    {
        typedef T value_t;

        // typedefs do not support template parameters
        template<typename Shape>
        using matrix_t = MatrixType<T, Shape>;
    public:
        template<typename Shape, typename... Properties>
        void generate(Shape &&, Properties &&...)
        {
            //auto data = dynamic_cast<GeneratorImpl*>(this)->template allocate( std::forward<Shape>(shape) );
            float * data = nullptr;
            return static_cast<GeneratorImpl*>(this)->template fill<Properties...>(data);
        }
    };

}}

#endif
