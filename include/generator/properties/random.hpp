//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_PROPERTIES_RANDOM_HPP
#define LINALG_TESTS_GENERATOR_PROPERTIES_RANDOM_HPP

#include <memory>
#include <algorithm>

#include <generator/properties/property_id.hpp>
#include <generator/property.hpp>

namespace generator { namespace property {

    namespace detail {

        template<bool symmetric>
        struct Filler;

        template<>
        struct Filler<false>
        {
            template<typename F, typename T>
            static void fill(uint64_t rows, uint64_t cols, std::unique_ptr<T> & ptr, F && f)
            {
                std::generate(ptr.get(), ptr.get() + rows * cols, std::forward<F>(f));
            }
        };

        template<>
        struct Filler<true>
        {
            /// Generate upper triangle, copy data for lower triangle
            /// \tparam F
            /// \tparam T
            /// \param rows
            /// \param cols
            /// \param ptr
            /// \param f
            template<typename F, typename T>
            static void fill(uint64_t rows, uint64_t cols, std::unique_ptr<T> & ptr, F && f)
            {
                for(uint32_t i = 0; i < rows; ++i) {
                    for(uint32_t j = 0; j < i; ++j)
                        ptr[cols*i + j]  = ptr[cols*j + i];
                    // from [i, i] until [i+1, 0] is reached
                    std::generate(ptr + cols*i + i, ptr + cols*(i+1), std::forward<F>(f));
                }
            }
        };

        template<typename T,
                bool positive = false,
                bool negative = false>
        struct random_generator
        {
            typedef typename shape::intermediate<T, shape::general>::type intermediate_t;
        public:
            template<typename RndGen>
            static void fill(const shape::general & shape, intermediate_t & data, RndGen && gen)
            {
                Filler<false>::fill(shape.rows, shape.cols, data, [&]() {
                    T val = gen();
                    return positive ? std::abs(val) : (negative ? -std::abs(val) : val);
                });
            }
        };
    }

    template<typename T, uint64_t type>
    struct property;

    template<typename T>
    struct property<T, hash<generator::property::random>()> :
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