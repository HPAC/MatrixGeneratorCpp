//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <generator/facades/blaze.hpp>

namespace matrix {

    namespace detail {

        // Curiously Recurring Template Pattern
        //
        // Implementation should define two methods:
        // - allocate - create matrix depending on shape type
        // - generate - 
        template<typename GeneratorImpl>
        struct generator
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

    }

    template<typename T>
    struct generator;
}
