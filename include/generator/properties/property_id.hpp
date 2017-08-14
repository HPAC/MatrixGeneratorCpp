//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_PROPERTIES_PROPERTY_ID_HPP
#define LINALG_TESTS_GENERATOR_PROPERTIES_PROPERTY_ID_HPP

#include <type_traits>
#include <utility>

namespace generator { namespace property {

    constexpr uint64_t id(int counter) {
        return 2 << counter;
    }

    template<typename V>
    constexpr uint64_t hash()
    {
        return V::value;
    }

    // disable to not create a disambugity for last element.
    // otherwise an empty argument of int and wrap_int may be used
    // to favor other overload due to implicit conversion
    //
    // one function is not sufficient because even with a conditional
    // summation compiler needs to be able to find a match for compute<{}>()
    template<typename Var, typename... Vars>
    constexpr auto hash()
    -> typename std::enable_if<(sizeof...(Vars) > 0), uint64_t>::type
    {
        return Var::value + hash<Vars...>();
    }

    namespace detail {

        template<int pos>
        using position = std::integral_constant<uint64_t, pos>;

        template<typename T, typename Position>
        struct hash_tuple;

        template<typename T>
        struct hash_tuple<T, position<0>>
        {
            typedef typename std::tuple_element<0, T>::type element_type;
            static constexpr uint64_t value = element_type::value;
        };

        template<typename T, std::size_t Position>
        struct hash_tuple<T, position<Position>>
        {
            typedef typename std::tuple_element<Position, T>::type element_type;
            static constexpr uint64_t value =
                element_type::value + hash_tuple<T, position<Position - 1>>::value;
        };
    }

    template<typename T>
    struct hash_tuple
    {
        static constexpr uint64_t value =
            detail::hash_tuple<T, detail::position<std::tuple_size<T>::value - 1>>::value;
    };


    // property id: 2^counter
    template<int counter>
    using property_t = std::integral_constant<uint64_t, id(counter)>;

}}

#endif
