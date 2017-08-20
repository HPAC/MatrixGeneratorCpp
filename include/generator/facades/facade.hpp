//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_FACADES_FACADE_HPP
#define LINALG_TESTS_GENERATOR_FACADES_FACADE_HPP

#include <random>

#include <generator/shape.hpp>
#include <generator/intermediate.hpp>
#include <generator/property.hpp>
//#include <generator/properties/properties.hpp>

namespace generator { namespace detail {

    // Curiously Recurring Template Pattern
    //
    // Implementation should define two methods:
    // - allocate - create matrix depending on shape type
    // - fill -
    template<typename GeneratorImpl, typename T, template<typename, typename> class MatrixType>
    struct generator_facade
    {
        typedef T value_t;

        template<typename Shape>
        using matrix_t = typename MatrixType<T, Shape>::type;

        typedef std::uniform_real_distribution<> distr_t;

        //Mersenne Twister RNG
        std::mt19937 rng;
        distr_t distribution;
    public:

        generator_facade(uint32_t seed) :
                rng(seed),
                distribution(-1, 1)
        {

        }

        template<typename... Properties>
        decltype(auto) generate(const shape::matrix_size & size, Properties &&... properties)
        {
            //auto data = dynamic_cast<GeneratorImpl*>(this)->template allocate( std::forward<Shape>(shape) );
            //constexpr auto band_types = shape::from_properties(std::forward<Properties>(properties)...);

            //constexpr auto band_types = shape::from_properties(std::forward<Properties>(properties)...);
            typedef shape::from_properties<Properties...> filtered_properties;
            typedef typename filtered_properties::band_type band_type;
            typedef typename filtered_properties::properties_type properties_type;
            typedef property::property<T, property::hash_tuple<properties_type>::value > property_t;
            // band_types is not usable in a constexpr since other properties are not constexpr
            // FIXME: better hash implementation, without this thing
            //typedef property::property<T, property::hash_computer< std::tuple_element<1, decltype(band_types)>>::hash() > property_t;
            typedef typename intermediate::intermediate_selector<
                T,
                band_type::lower_bandwidth,
                band_type::upper_bandwidth,
                band_type::symmetry
                >::type intermediate_t;
            intermediate_t matrix{size};
            property_t::fill(matrix,
                    [&]() {
                        return distribution(rng);
                    },
                    std::forward<Properties>(properties)...
                );
            return static_cast<GeneratorImpl*>(this)->template create(matrix);
            //std::unique_ptr<intermediate<Property, Generator>> intermediate = intermediate::from_band(size, std::get<0>(band_types));
            //intermediate->fill()
            //typedef  
            //std::unique_ptr<intermediate<
            //intermediate_t<Shape> data = shape::intermediate<T, Shape>::create(size, std::forward<Shape>(shape));
            /*property::property<T, property::hash<Properties...>()>::fill(
                    std::forward<Shape>(shape), data,
                    [&]() {
                        return distribution(rng);
                    }
                );*/
            //return static_cast<GeneratorImpl*>(this)->template create<Shape, Properties...>(std::forward<Shape>(shape), std::move(data));
        }
    };

}}

#endif
