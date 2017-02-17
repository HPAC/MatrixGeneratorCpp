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
            template<typename RndGen>
            static void fill(const shape::general & shape,
                            typename shape::intermediate<T, shape::general>::type & data,
                            RndGen && gen)
            {
                Filler<false>::fill(shape.rows, shape.cols, data,
                                    [&]() {
                                        T val = gen();
                                        return positive ? std::abs(val) : (negative ? -std::abs(val) : val);
                                    });
            }

            template<typename RndGen>
            static void fill(const shape::self_adjoint & shape,
                            typename shape::intermediate<T, shape::self_adjoint>::type & data,
                            RndGen && gen)
            {
                Filler<true>::fill(shape.rows, shape.cols, data,
                                    [&]() {
                                        T val = gen();
                                        return positive ? std::abs(val) : (negative ? -std::abs(val) : val);
                                    });
            }

            template<typename RndGen>
            static void fill(const shape::diagonal & shape,
                            typename shape::intermediate<T, shape::diagonal>::type & data,
                            RndGen && gen)
            {
                // Fill one row of a matrix according to our needs.
                // Symmetry does not matter here.
                Filler<false>::fill(shape.rows, 1, data,
                                    [&]() {
                                        T val = gen();
                                        return positive ? std::abs(val) : (negative ? -std::abs(val) : val);
                                    });
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
