//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_FACADES_BLAZE_HPP
#define LINALG_TESTS_GENERATOR_FACADES_BLAZE_HPP

#include <generator/facades/facade.hpp>

namespace generator {

    namespace detail {

        template<typename T>
        struct blaze_generator
        {
            typedef typename T value_t;
        };

    }

    template<typename T>
    struct generator<library::blaze, T> : public
        detail::generator_facade<detail::blaze_generator, T>
    {}
}

#endif
