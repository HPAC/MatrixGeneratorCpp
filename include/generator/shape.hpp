//  Copyright (c) 2016-2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_SHAPE_HPP
#define LINALG_TESTS_GENERATOR_SHAPE_HPP

#include <stdexcept>

namespace generator { namespace shape {

    struct band;

    struct band
    {
        static constexpr bool symmetric = false;
        const uint32_t lower_bandwidth;
        const uint32_t upper_bandwidth;

        banded(uint32_t lower_, uint32_t upper_):
                lower_bandwidth(lower_),
                upper_bandwidth(upper_)
        {}

        const band & to_band(uint32_t, uint32_t) const
        {
            return *this;
        }
    };

    struct general
    {
        static constexpr bool symmetric = false;

        band to_band(uint32_t rows, uint32_t cols) const
        {
            return band(rows - 1, cols - 1);
        }
    };

    struct self_adjoint
    {
        static constexpr bool symmetric = true;

        band to_band(uint32_t rows, uint32_t cols) const
        {
            if(rows != cols) {
                throw std::runtime_error("Non-square matrix sizes passed to aself-adjoint matrix!");
            }
            return band(rows - 1, cols - 1);
        }
    };

    struct upper_triangular
    {
        static constexpr bool symmetric = false;

        band to_band(uint32_t, uint32_t cols) const
        {
            return band(0, cols - 1);
        }
    };

    struct lower_triangular
    {
        static constexpr bool symmetric = false;

        band to_band(uint32_t rows, uint32_t) const
        {
            return band(rows - 1, 0);
        }
    };

    struct tridiagonal : public band
    {
        static constexpr bool symmetric = false;

        tridiagonal(): band(1, 1)
        {}

        band to_band(uint32_t, uint32_t) const
        {
            return band(1, 1);
        }
    };

    struct diagonal : public band
    {
        static constexpr bool symmetric = true;

        diagonal(): band(0, 0)
        {}

        band to_band(uint32_t, uint32_t) const
        {
            return band(0, 0);
        }
    };

}}

#endif
