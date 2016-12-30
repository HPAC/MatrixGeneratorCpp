//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_FACADES_FACADE_HPP
#define LINALG_TESTS_GENERATOR_FACADES_FACADE_HPP

namespace generator { namespace detail {

    // Curiously Recurring Template Pattern
    //
    // Implementation should define two methods:
    // - allocate - create matrix depending on shape type
    // - generate - 
    template<typename GeneratorImpl>
    struct generator_facade
    {
        typedef typename GeneratorImpl::value_t value_t;
        typedef typename GeneratorImpl::matrix_t<Shape>::type matrix_t;
        
        template<typename Shape, typename... Properties>
        matrix_t generate(Shape && shape)
        {
            auto data = dynamic_cast<GeneratorImpl*>(this)->template allocate( std::forward<Shape>(shape) );
            return dynamic_cast<GeneratorImpl*>(this)->template generate<Properties...>(data);
        }
    };

}}

#endif
