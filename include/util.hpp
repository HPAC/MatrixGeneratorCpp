//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_UTIL_HPP
#define LINALG_TESTS_UTIL_HPP

#include <tuple>

template<typename T1, typename T2>
struct tuple_cat_result;

template<typename... T1, typename T2>
struct tuple_cat_result<std::tuple<T1...>, T2>
{
    typedef std::tuple<T1..., T2> type;
};

template<typename... T1, typename... T2>
struct tuple_cat_result<std::tuple<T1...>, std::tuple<T2...>>
{
    typedef std::tuple<T1..., T2...> type;
};

#endif
