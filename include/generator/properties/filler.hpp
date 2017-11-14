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

        template<typename T, typename F>
        void fill(const intermediate::general<T> & shape, F && f)
        {
            std::generate(shape.data.get(),
                shape.data.get() + shape.size.rows * shape.size.cols,
                std::forward<F>(f)
                );
        }

        /// Generate upper triangle, copy data for lower triangle
        template<typename T, typename F>
        void fill(const intermediate::self_adjoint<T> & shape, F && f)
        {
            uint32_t rows = shape.size.rows, cols = shape.size.cols;
            auto ptr = shape.data.get();
            for (uint32_t i = 0; i < rows; ++i) {
                for (uint32_t j = 0; j < i; ++j)
                    ptr[cols * i + j] = ptr[cols * j + i];
                // from [i, i] until [i+1, 0] is reached
                std::generate(ptr + cols * i + i,
                    ptr + cols * (i + 1),
                    std::forward<F>(f)
                    );
            }
        }

        template<typename T, typename F>
        void fill(const intermediate::upper_triangular<T> & shape, F && f)
        {
            uint32_t rows = shape.size.rows, cols = shape.size.cols;
            auto ptr = shape.data.get();
            for (uint32_t i = 0; i < rows; ++i) {
                uint32_t central_point = std::min(i, cols);
                for (uint32_t j = 0; j < central_point; ++j)
                    ptr[cols * i + j] = static_cast<T>(0.0);
                // from [i, i] until [i+1, 0] is reached
                std::generate(ptr + cols * i + central_point,
                    ptr + cols * (i + 1),
                    std::forward<F>(f)
                    );
            }
        }

        template<typename T, typename F>
        void fill(const intermediate::lower_triangular<T> & shape, F && f)
        {
            uint32_t rows = shape.size.rows, cols = shape.size.cols;
            auto ptr = shape.data.get();
            for (uint32_t i = 0; i < rows; ++i) {
                // from [i, 0] until [i, i] is reached
                std::generate(ptr + cols * i,
                    ptr + cols * i + i + 1,
                    std::forward<F>(f)
                    );
                for (uint32_t j = i + 1; j < cols; ++j)
                    ptr[cols * i + j] = static_cast<T>(0.0);
            }
        }

        template<typename T, typename F>
        void fill(const intermediate::diagonal<T> & shape, F && f)
        {
            std::generate(shape.data.get(),
                shape.data.get() + std::min(shape.size.rows, shape.size.cols),
                std::forward<F>(f)
                );
        }

        template<typename T, typename F>
        void fill(const intermediate::row_vector<T> & shape, F && f)
        {
            std::generate(shape.data.get(),
                shape.data.get() + shape.size.cols,
                std::forward<F>(f)
                );
        }

        template<typename T, typename F>
        void fill(const intermediate::col_vector<T> & shape, F && f)
        {
            std::generate(shape.data.get(),
                shape.data.get() + shape.size.rows,
                std::forward<F>(f)
                );
        }

    }
}}

#endif //LINALG_TESTS_FILLER_HPP_HPP
