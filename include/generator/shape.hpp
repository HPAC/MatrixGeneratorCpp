//  Copyright (c) 2016-2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_SHAPE_HPP
#define LINALG_TESTS_GENERATOR_SHAPE_HPP

#include <tuple>
#include <stdexcept>
#include <limits>

#include <util.hpp>

namespace generator { namespace shape {

    struct matrix_size
    {
        uint32_t rows, cols;
    };

    enum class vector_type
    {
        row = 0,
        col = 1,
        none = 2
    };

    template<uint32_t LowerBandwidth, uint32_t UpperBandwidth,
            bool Symmetry = false, bool MustBeSquare = true,
            vector_type Vector = vector_type::none>
    struct band
    {
        static constexpr uint32_t lower_bandwidth = LowerBandwidth;
        static constexpr uint32_t upper_bandwidth = UpperBandwidth;
        static constexpr bool symmetry = Symmetry;
        static constexpr bool must_be_square = MustBeSquare;
        static constexpr vector_type vector = Vector;
        typedef band<LowerBandwidth, UpperBandwidth, Symmetry, MustBeSquare, Vector> band_type;
    };

    struct self_adjoint
    {
        typedef band<
            std::numeric_limits<uint32_t>::max(),
            std::numeric_limits<uint32_t>::max(),
            true,
            true,
            vector_type::none
        > band_type;
    };

    struct upper_triangular
    {
        typedef band<
            0,
            std::numeric_limits<uint32_t>::max(),
            false,
            true,
            vector_type::none
        > band_type;
    };

    struct lower_triangular
    {
        typedef band<
            std::numeric_limits<uint32_t>::max(),
            0,
            false,
            true,
            vector_type::none
        > band_type;
    };

    struct tridiagonal
    {
        typedef band<
            1,
            1,
            true,
            true,
            vector_type::none
        > band_type;
    };

    struct diagonal
    {
        typedef band<
            0,
            0,
            true,
            true,
            vector_type::none
        > band_type;
    };

    struct row_vector
    {
        typedef band<
            0,
            0,
            true,
            true,
            vector_type::row
        > band_type;
    };

    struct col_vector
    {
        typedef band<
            0,
            0,
            true,
            true,
            vector_type::col
        > band_type;
    };
    
    struct not_square
    {
        typedef band<
            std::numeric_limits<uint32_t>::max(),
            std::numeric_limits<uint32_t>::max(),
            false,
            false,
            vector_type::none
        > band_type;
    };

    template<typename Matrix1, typename Matrix2>
    struct merge_band
    {
        typedef typename Matrix1::band_type band_1;
        typedef typename Matrix2::band_type band_2;

        // Avoid clash between row and col vector
        // Disallowed: different vector settings and both of them are not none
        static_assert(band_1::vector == band_2::vector ||
                      band_1::vector == vector_type::none ||
                      band_2::vector == vector_type::none,
                      "Clash between row_vector and col_vector");
        typedef band<
            std::min(band_1::lower_bandwidth, band_2::lower_bandwidth),
            std::min(band_1::upper_bandwidth, band_2::upper_bandwidth),
            band_1::symmetry | band_2::symmetry,
            band_1::must_be_square & band_2::must_be_square, 
            band_1::vector != vector_type::none ? band_1::vector : band_2::vector
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
    template<>
    struct is_shape_type<row_vector> : std::true_type {};
    template<>
    struct is_shape_type<col_vector> : std::true_type {};
    template<>
    struct is_shape_type<not_square> : std::true_type {};

    namespace detail {

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
