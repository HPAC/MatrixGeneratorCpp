//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_PROPERTIES_RANDOM_HPP
#define LINALG_TESTS_GENERATOR_PROPERTIES_RANDOM_HPP

#include <generator/shape.hpp>
#include <generator/intermediate.hpp>
#include <generator/property.hpp>
#include <generator/properties/filler.hpp>

namespace generator { namespace property {

    namespace detail {
        template<typename T,
                bool positive = false,
                bool negative = false>
        struct random_generator
        {
        public:
            template<typename Shape, typename RndGen>
            static void fill(Shape && shape, RndGen && gen)
            {
                detail::fill(std::forward<Shape>(shape),
                        [&]() {
                            T val = gen();
                            return positive ? std::abs(val) : (negative ? -std::abs(val) : val);
                        }
                    );
            }
        };
    }

    template<typename T, uint64_t type>
    struct property;

    template<typename T>
    struct property<T, generator::property::hash<generator::property::random>()> :
            detail::random_generator<T, false, false>
    {};

    template<typename T>
    struct property<T,
            hash<random, positive>()
            > : detail::random_generator<T, true, false>
    {};

    template<typename T>
    struct property<T,
            hash<random, negative>()
        > : detail::random_generator<T, false, true>
    {};

}}

#endif
