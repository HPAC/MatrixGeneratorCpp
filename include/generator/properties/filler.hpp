//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef LINALG_TESTS_FILLER_HPP
#define LINALG_TESTS_FILLER_HPP

#include <memory>
#include <algorithm>

namespace generator { namespace property {

    namespace detail {

        template<bool symmetric>
        struct Filler;

        template<>
        struct Filler<false> {
            template<typename F, typename T>
            static void fill(uint64_t rows, uint64_t cols, std::unique_ptr <T> &ptr, F &&f) {
                std::generate(ptr.get(), ptr.get() + rows * cols, std::forward<F>(f));
            }
        };

        template<>
        struct Filler<true> {
            /// Generate upper triangle, copy data for lower triangle
            /// \tparam F
            /// \tparam T
            /// \param rows
            /// \param cols
            /// \param ptr
            /// \param f
            template<typename F, typename T>
            static void fill(uint64_t rows, uint64_t cols, std::unique_ptr <T> &ptr, F &&f) {
                for (uint32_t i = 0; i < rows; ++i) {
                    for (uint32_t j = 0; j < i; ++j)
                        ptr[cols * i + j] = ptr[cols * j + i];
                    // from [i, i] until [i+1, 0] is reached
                    std::generate(ptr + cols * i + i, ptr + cols * (i + 1), std::forward<F>(f));
                }
            }
        };

    }
}}

#endif //LINALG_TESTS_FILLER_HPP_HPP
