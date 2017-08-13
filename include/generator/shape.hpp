//  Copyright (c) 2016-2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_SHAPE_HPP
#define LINALG_TESTS_GENERATOR_SHAPE_HPP

#include <tuple>
#include <stdexcept>

namespace generator { namespace shape {

    struct band;

    struct band
    {
        static constexpr bool symmetric = false;
        const uint32_t lower_bandwidth;
        const uint32_t upper_bandwidth;

        band(uint32_t lower_, uint32_t upper_):
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

    band merge_band(const band & first, const band & second)
    {
        return band(
            std::min(first.lower_bandwidth, second.lower_bandwidth),
            std::min(first.upper_bandwidth, second.upper_bandwidth)
        );
    }

    template<typename T>
    struct is_shape_type : std::false_type {};

    template<>
    struct is_shape_type<band> : std::true_type {};
    template<>
    struct is_shape_type<general> : std::true_type {};
    template<>
    struct is_shape_type<self_adjoint> : std::true_type {};
    template<>
    struct is_shape_type<upper_triangular> : std::true_type {};
    template<>
    struct is_shape_type<lower_triangular> : std::true_type {};
    template<>
    struct is_shape_type<tridiagonal> : std::true_type {};
    template<>
    struct is_shape_type<diagonal> : std::true_type {};

    namespace detail {

        typedef std::tuple<uint32_t, uint32_t> matrix_size_t;
        struct empty_tuple {};

        template<typename T1, typename T2>
        struct tuple_cat_result;

        template<typename... T1, typename T2>
        struct tuple_cat_result<std::tuple<T1...>, T2>
        {
            typedef std::tuple<T1..., T2> type;
        };

        // Update band type with a shape type, tuple unchanged
        template<typename OldTuple, typename Property,
            typename = typename std::enable_if<is_shape_type<Property>::type>
        >
        std::tuple<band, OldTuple> from_properties(const matrix_size_t & size, const band & band_type,
            OldTuple && tuple, Property && property)
        {
            return std::make_tuple(
                merge_band(band_type, property.to_band(std::get<0>(size), std::get<1>(size))),
                tuple
                );
        }

        template<typename OldTuple, typename Property, typename... Properties,
            typename = typename std::enable_if<is_shape_type<Property>::type>
        >
        std::tuple<band, OldTuple> from_properties(const matrix_size_t & size, const band & band_type,
            OldTuple && tuple, Property && property, Properties &&... props)
        {
            return from_properties(size,
                merge_band(band_type, property.to_band(std::get<0>(size), std::get<1>(size))),
                std::forward<OldTuple>(tuple),
                std::forward<Properties>(props)...
                );
        }

        // Add non-shape type to an empty tuple
        template<typename Property>
        std::tuple<band, std::tuple<Property>> from_properties(const matrix_size_t &,
            const band & band_type, const empty_tuple & tuple, Property && property)
        {
            return std::make_tuple(band_type, std::make_tuple(property));
        }

        template<typename Property, typename... Properties>
        std::tuple<band, std::tuple<Property>> from_properties(const matrix_size_t & size,
            const band & band_type, const empty_tuple & tuple, Property && property, Properties &&... props)
        {
            return from_properties(size,
                band_type,
                std::make_tuple(std::forward<Property>(property)),
                std::forward<Properties>(props)...
                );
        }

        // Add non-shape type to a non-empty tuple
        template<typename OldTuple, typename Property>
        auto from_properties(const matrix_size_t &, const band & band_type, OldTuple && tuple,
            Property && property)
            -> std::tuple<band, typename tuple_cat_result<OldTuple, Property>::type>
        {
            return std::make_tuple(band_type, std::tuple_cat(tuple, std::make_tuple(std::forward<Property>(property))));
        }

        template<typename OldTuple, typename Property, typename... Properties>
        auto from_properties(const matrix_size_t & size, const band & band_type, OldTuple && tuple,
            Property && property, Properties &&... props)
            -> std::tuple<band, typename tuple_cat_result<OldTuple, Property>::type>
        {
            return from_properties(size,
                band_type,
                std::tuple_cat(tuple, std::make_tuple(std::forward<Property>(property))),
                std::forward<Properties>(props)...
                );
        }
    }

    template<typename... Properties>
    decltype(auto) from_properties(const std::tuple<uint32_t, uint32_t> & size, Properties &&... props)
    {
        return detail::from_properties(size,
            band(std::get<0>(size), std::get<1>(size)),
            detail::empty_tuple(),
            std::forward<Properties>(props)...
            );
    }

}}

#endif
