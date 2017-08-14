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

    template<uint32_t LowerBandwidth, uint32_t UpperBandwidth, bool Symmetry = false>
    struct band
    {
        static constexpr uint32_t lower_bandwidth = LowerBandwidth;
        static constexpr uint32_t upper_bandwidth = UpperBandwidth;
        static constexpr bool symmetry = Symmetry;
        typedef band<LowerBandwidth, UpperBandwidth, Symmetry> band_type;
    };

    struct self_adjoint
    {
        typedef band<
            std::numeric_limits<uint32_t>::max(),
            std::numeric_limits<uint32_t>::max(),
            true
        > band_type;
    };

    struct upper_triangular
    {
        typedef band<
            0,
            std::numeric_limits<uint32_t>::max(),
            false
        > band_type;
    };

    struct lower_triangular
    {
        typedef band<
            std::numeric_limits<uint32_t>::max(),
            0,
            false
        > band_type;
    };

    struct tridiagonal
    {
        typedef band<
            1,
            1,
            true
        > band_type;
    };

    struct diagonal
    {
        typedef band<
            0,
            0,
            true
        > band_type;
    };

    template<typename Matrix1, typename Matrix2>
    struct merge_band
    {
        typedef typename Matrix1::band_type band_1;
        typedef typename Matrix2::band_type band_2;

        typedef band<
            std::min(band_1::lower_bandwidth, band_2::lower_bandwidth),
            std::min(band_1::upper_bandwidth, band_2::upper_bandwidth),
            band_1::symmetry | band_2::symmetry
        > type;
    };

    template<typename T>
    struct is_shape_type : std::false_type {};

    template<uint32_t L, uint32_t U, bool S>
    struct is_shape_type<band<L, U, S>> : std::true_type {};
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

        template<typename Band, typename OldTuple, typename Enable, typename... Properties>
        struct from_properties;

        template<typename Band, typename OldTuple, typename Property>
        struct from_properties<
            Band, OldTuple,
            typename std::enable_if<is_shape_type<Property>::value, void>::type,
            Property
        >
        {
            typedef typename merge_band<Band, Property>::type band_type;
            typedef OldTuple properties_type;
        };


        template<typename Band, typename OldTuple, typename Property>
        struct from_properties<
            Band, OldTuple,
            typename std::enable_if<!is_shape_type<Property>::value, void>::type,
            Property
        >
        {
            typedef Band band_type;
            typedef typename tuple_cat_result<OldTuple, Property>::type properties_type;
        };

        template<
            typename Band,
            typename OldTuple,
            typename Property,
            typename... Properties
        >
        struct from_properties<
            Band, OldTuple,
            typename std::enable_if<is_shape_type<Property>::value, void>::type,
            Property, Properties...
        >
        {
            typedef from_properties<
                typename merge_band<Band, Property>::type,
                OldTuple,
                void,
                Properties...
            > properties_t;
            typedef typename properties_t::band_type band_type;
            typedef typename properties_t::properties_type properties_type;
        };

        template<
            typename Band,
            typename OldTuple,
            typename Property,
            typename... Properties
        >
        struct from_properties<
            Band, OldTuple,
            typename std::enable_if<!is_shape_type<Property>::value, void>::type,
            Property, Properties...
        >
        {
            typedef from_properties<
                Band,
                typename tuple_cat_result<OldTuple, Property>::type,
                void,
                Properties...
            > properties_t;
            typedef typename properties_t::band_type band_type;
            typedef typename properties_t::properties_type properties_type;
        };
    }

    template<typename... Properties>
    struct from_properties
    {
        typedef detail::from_properties<
            band<std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max()>,
            std::tuple<>,
            void,
            Properties...
        > properties_t;
        typedef typename properties_t::band_type band_type;
        typedef typename properties_t::properties_type properties_type;
    };

}}

#endif
