//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_PROPERTIES_ZEROS_HPP
#define LINALG_PROPERTIES_ZEROS_HPP

#include <type_traits>

#include <generator/shape.hpp>
#include <generator/property.hpp>
#include <generator/properties/filler.hpp>

namespace generator { namespace property {

    // look for the first object
    namespace detail {

        template<typename T>
        struct is_const_property : std::false_type {};
        template<>
        struct is_const_property<constant> : std::true_type {};
        template<>
        struct is_const_property<zeros> : std::true_type {};
        template<>
        struct is_const_property<ones> : std::true_type {};

        template<typename... Properties>
        double call(const constant & x, Properties &&...)
        {
            return x.fill_value;
        }

        // Avoid taking a reference via x.fill_value to static constexpr (would require a definition)
        template<typename Property, typename... Properties>
        auto call(Property &&, Properties &&...)
        -> typename std::enable_if<
            !std::is_same<constant, Property>::value && is_const_property<Property>::value,
            double
        >::type
        {
            return Property::fill_value;
        }

        template<typename Property, typename... Properties>
        auto call(Property &&, Properties &&... props)
        -> typename std::enable_if< !is_const_property<Property>::value, double>::type
        {
            return call(std::forward<Properties>(props)...);
        }

        template<typename T>
        struct constant_generator
        {
            template<typename Shape, typename RndGen, typename... Properties>
            static void fill(Shape && shape, RndGen &&, Properties &&... props)
            {
                T val = static_cast<T>(call(std::forward<Properties>(props)...));
                detail::fill(std::forward<Shape>(shape),
                        [&]() { return val; }
                    );
            }
        };
    }

    template<typename T, uint64_t type>
    struct property;

    template<typename T>
    struct property<T, hash<constant>()> : detail::constant_generator<T>
    {};

    template<typename T>
    struct property<T, hash<ones>()> : detail::constant_generator<T>
    {};

    template<typename T>
    struct property<T, hash<zeros>()> : detail::constant_generator<T>
    {};

}}

#endif //LINALG_TESTS_ZEROS_HPP
