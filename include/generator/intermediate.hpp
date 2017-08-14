//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_INTERMEDIATE_HPP
#define LINALG_TESTS_GENERATOR_INTERMEDIATE_HPP

#include <memory>
#include <functional>

#include <generator/shape.hpp>

namespace generator { namespace intermediate {

    template<typename T> 
    struct general
    {
        //using intermediate<Property, Generator>::rnd_gen_type;
        //typedef typename intermediate<Property, Generator>::value_type value_type;

        general(const shape::matrix_size & size_) :
            size(size_),
            data(new T[size_.rows * size_.cols])
        {}

        /*void fill(const rnd_gen_type & gen_func) override
        {
            Property::fill(*this, gen_func);
        }

        auto create() override
        {
            return Generator::create(*this);
        }*/

        shape::matrix_size size;
        std::unique_ptr<T[]> data;
    };

    /// Self-adjoint matrix is stored as a general matrix
    /*template<typename T>
    struct intermediate<T, generator::shape::self_adjoint> : 
        intermediate<T, generator::shape::general>
    {
        typedef intermediate<T, generator::shape::general> base_t;
        typedef typename base_t::type type;

        static type create(const generator::shape::self_adjoint & gen)
        {
            return base_t::create(gen.rows, gen.rows);
        }
    };

    /// Diagonal matrix is stored as one row - 1D array
    template<typename T>
    struct intermediate<T, generator::shape::diagonal>
    {
        typedef std::unique_ptr<T[]> type;

        static type create(const generator::shape::diagonal & gen)
        {
            return type(new T[gen.rows]);
        }
    };*/

    template<typename T> 
    struct diagonal
    {
        //using intermediate<Property, Generator>::rnd_gen_type;
        //typedef typename intermediate<Property, Generator>::value_type value_type;

        diagonal(const shape::matrix_size & size_) :
            size(size_),
            data(new T[std::min(size_.rows, size_.cols)])
        {}

        /*void fill(const rnd_gen_type & gen_func) override
        {
            Property::fill(*this, gen_func);
        }

        auto create() override
        {
            return Generator::create(*this);
        }*/

        shape::matrix_size size;
        std::unique_ptr<T[]> data;
    };


    template<typename T, uint32_t LowerBandwidth, uint32_t UpperBandwidth, typename = void>
    struct intermediate_traits;

    template<typename T, uint32_t LowerBandwidth, uint32_t UpperBandwidth>
    struct intermediate_traits<
        T,
        LowerBandwidth,
        UpperBandwidth,
        typename std::enable_if<(LowerBandwidth > 0 && UpperBandwidth > 0)>::type
    >
    {
        typedef general<T> type;
    };

    template<typename T, uint32_t LowerBandwidth, uint32_t UpperBandwidth>
    struct intermediate_traits<
        T,
        LowerBandwidth,
        UpperBandwidth,
        typename std::enable_if<(LowerBandwidth == 0 && UpperBandwidth == 0)>::type
    >
    {
        typedef diagonal<T> type;
    };
}}


#endif
