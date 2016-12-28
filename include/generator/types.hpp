//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>

namespace detail {
    
    constexpr uint64_t id(int counter)
    {
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
        return Var::value + compute<Vars...>();
    }

    // property id: 2^counter
    template<int counter>
    using property_t = std::integral_constant<int, id(counter)>;

}

namespace matrix {

    namespace property {
    
        struct random : property_t<0> {};
        struct positive : property_t<1> {};
        struct negative : property_t<2> {};
    
    }
}
