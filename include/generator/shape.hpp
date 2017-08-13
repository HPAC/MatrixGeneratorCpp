//  Copyright (c) 2016-2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_SHAPE_HPP
#define LINALG_TESTS_GENERATOR_SHAPE_HPP

#include <tuple>
#include <stdexcept>
#include <limits>

namespace generator { namespace shape {

    struct matrix_size
    {
        uint32_t rows, cols;
    };

    struct band
    {
        uint32_t lower_bandwidth;
        uint32_t upper_bandwidth;
        bool symmetry;

        constexpr band():
                lower_bandwidth(std::numeric_limits<uint32_t>::max()),
                upper_bandwidth(std::numeric_limits<uint32_t>::max()),
                symmetry(false)
        {}

        constexpr band(uint32_t lower_, uint32_t upper_, bool symmetry_ = false):
                lower_bandwidth(lower_),
                upper_bandwidth(upper_),
                symmetry(symmetry_)
        {}

        /*constexpr band(const matrix_size & size):
                lower_bandwidth(size.rows - 1),
                upper_bandwidth(size.cols - 1)
        {}*/

        constexpr band to_band() const
        {
            return *this;
        }
    };

    /*struct general
    {
        constexpr band to_band(const matrix_size & size)
        {
            return band(size.rows - 1, size.cols - 1);
        }
    };*/

    struct self_adjoint
    {
        constexpr band to_band() const
        {
            return band(std::numeric_limits<uint32_t>::max(),
                        std::numeric_limits<uint32_t>::max(),
                        true);
        }
    };

    struct upper_triangular
    {
        static constexpr bool symmetric = false;

        constexpr band to_band() const
        {
            return band(0, std::numeric_limits<uint32_t>::max());
        }
    };

    struct lower_triangular
    {
        static constexpr bool symmetric = false;

        constexpr band to_band() const
        {
            return band(std::numeric_limits<uint32_t>::max(), 0);
        }
    };

    struct tridiagonal : public band
    {
        static constexpr bool symmetric = false;

        constexpr tridiagonal(): band(1, 1)
        {}

        constexpr band to_band() const
        {
            return band(1, 1, true);
        }
    };

    struct diagonal : public band
    {
        static constexpr bool symmetric = true;

        constexpr diagonal(): band(0, 0)
        {}

        constexpr band to_band() const
        {
            return band(0, 0, true);
        }
    };

    constexpr band merge_band(const band first, const band second)
    {
        return band(
            std::min(first.lower_bandwidth, second.lower_bandwidth),
            std::min(first.upper_bandwidth, second.upper_bandwidth),
            first.symmetry | second.symmetry
        );
    }

    template<typename T>
    struct is_shape_type : std::false_type {};

    template<>
    struct is_shape_type<band> : std::true_type {};
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

        template<typename T1, typename T2>
        struct tuple_cat_result;

        template<typename... T1, typename T2>
        struct tuple_cat_result<std::tuple<T1...>, T2>
        {
            typedef std::tuple<T1..., T2> type;
        };

        // Forward declarations
        template<typename OldTuple, typename Property,
            typename std::enable_if<!is_shape_type<Property>::value, int>::type = 0
        >
        constexpr auto from_properties(const band & band_type, OldTuple && tuple,
            Property && property)
            -> std::tuple<band, typename tuple_cat_result<OldTuple, Property>::type>;

        template<typename OldTuple, typename Property, typename... Properties,
            typename std::enable_if<!is_shape_type<Property>::value, int>::type = 0
        >
        constexpr decltype(auto) from_properties(const band & band_type, OldTuple && tuple,
            Property && property, Properties &&... props);

        // Update band type with a shape type, tuple unchanged
        template<typename OldTuple, typename Property,
            typename std::enable_if<is_shape_type<Property>::value, int>::type = 0
        >
        constexpr std::tuple<band, OldTuple> from_properties(const band & band_type,
            OldTuple && tuple, Property && property)
        {
            return std::make_tuple(merge_band(band_type, property.to_band()),
                tuple
                );
        }

        template<typename OldTuple, typename Property, typename... Properties,
            typename std::enable_if<is_shape_type<Property>::value, int>::type = 0
        >
        constexpr decltype(auto) from_properties(const band & band_type,
            OldTuple && tuple, Property && property, Properties &&... props)
        {
            return detail::from_properties(merge_band(band_type, property.to_band()),
                std::forward<OldTuple>(tuple),
                std::forward<Properties>(props)...
                );
        }

        // Add non-shape type to a non-empty tuple
        template<typename OldTuple, typename Property,
            typename std::enable_if<!is_shape_type<Property>::value, int>::type = 0
        >
        constexpr auto from_properties(const band & band_type, OldTuple && tuple,
            Property && property)
            -> std::tuple<band, typename tuple_cat_result<OldTuple, Property>::type>
        {
            return std::make_tuple(band_type, std::tuple_cat(tuple, std::make_tuple(std::forward<Property>(property))));
        }

        template<typename OldTuple, typename Property, typename... Properties,
            typename std::enable_if<!is_shape_type<Property>::value, int>::type = 0
        >
        constexpr decltype(auto) from_properties(const band & band_type, OldTuple && tuple,
            Property && property, Properties &&... props)
        {
            return detail::from_properties(band_type,
                std::tuple_cat(tuple, std::make_tuple(std::forward<Property>(property))),
                std::forward<Properties>(props)...
                );
        }
    }

    template<typename... Properties>
    constexpr decltype(auto) from_properties(Properties &&... props)
    {
        return detail::from_properties(band(),
            std::tuple<>{},
            std::forward<Properties>(props)...
            );
    }

}}

#endif
