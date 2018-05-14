//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_INTERMEDIATE_HPP
#define LINALG_TESTS_GENERATOR_INTERMEDIATE_HPP

#include <memory>
#include <functional>
#include <exception>

#include <generator/shape.hpp>

namespace generator { namespace intermediate {

    template<typename T> 
    struct general
    {
        general(const shape::matrix_size & size_) :
            size(size_),
            data(new T[size_.rows * size_.cols])
        {}

        shape::matrix_size size;
        std::unique_ptr<T[]> data;
    };

    template<typename T>
    struct self_adjoint : general<T>
    {
        self_adjoint(const shape::matrix_size & size_) :
            general<T>(size_)
        {
            if(size_.rows != size_.cols) {
                throw std::runtime_error("Non-square matrix passed to self_adjoint!");
            }
        }
    };

    template<typename T, bool MustBeSquare = true>
    struct upper_triangular : general<T>
    {
        upper_triangular(const shape::matrix_size & size_) :
            general<T>(size_)
        {}
    };

    template<typename T, bool MustBeSquare = true>
    struct lower_triangular : general<T>
    {
        lower_triangular(const shape::matrix_size & size_) :
            general<T>(size_)
        {}
    };

    template<typename T> 
    struct diagonal
    {
        diagonal(const shape::matrix_size & size_) :
            size(size_),
            data(new T[std::min(size_.rows, size_.cols)])
        {}

        shape::matrix_size size;
        std::unique_ptr<T[]> data;
    };

    template<typename T>
    struct row_vector
    {
        row_vector(const shape::matrix_size & size_) :
            size(size_),
            data(new T[size_.cols])
        {
            if(size_.rows != 1) {
                throw std::runtime_error("Number of rows different from one for row_vector");
            }
        }

        shape::matrix_size size;
        std::unique_ptr<T[]> data;
    };

    template<typename T>
    struct col_vector
    {
        col_vector(const shape::matrix_size & size_) :
            size(size_),
            data(new T[size_.rows])
        {
            if(size_.cols != 1) {
                throw std::runtime_error("Number of cols different from one for col_vector");
            }
        }

        shape::matrix_size size;
        std::unique_ptr<T[]> data;
    };


    template<typename T, uint32_t LowerBandwidth, uint32_t UpperBandwidth, bool Symmetry, bool MustBeSquare, shape::vector_type Vector, typename = void>
    struct intermediate_selector;

    template<typename T, uint32_t LowerBandwidth, uint32_t UpperBandwidth, bool Symmetry, bool MustBeSquare, shape::vector_type Vector>
    struct intermediate_selector<
        T,
        LowerBandwidth,
        UpperBandwidth,
        Symmetry,
        MustBeSquare,
        Vector,
        typename std::enable_if<(LowerBandwidth > 0 && UpperBandwidth > 0 && Vector == shape::vector_type::none)>::type
    >
    {
        typedef typename std::conditional<Symmetry, self_adjoint<T>, general<T>>::type type;
    };

    template<typename T, uint32_t LowerBandwidth, uint32_t UpperBandwidth, bool Symmetry, bool MustBeSquare, shape::vector_type Vector>
    struct intermediate_selector<
        T,
        LowerBandwidth,
        UpperBandwidth,
        Symmetry,
        MustBeSquare,
        Vector,
        typename std::enable_if<(LowerBandwidth == 0 && UpperBandwidth == 0 && Vector == shape::vector_type::none)>::type
    >
    {
        typedef diagonal<T> type;
    };

    template<typename T, uint32_t LowerBandwidth, uint32_t UpperBandwidth, bool Symmetry, bool MustBeSquare, shape::vector_type Vector>
    struct intermediate_selector<
        T,
        LowerBandwidth,
        UpperBandwidth,
        Symmetry,
        MustBeSquare,
        Vector,
        typename std::enable_if<(LowerBandwidth == 0 && UpperBandwidth > 0 && Vector == shape::vector_type::none)>::type
    >
    {
        typedef upper_triangular<T, MustBeSquare> type;
    };

    template<typename T, uint32_t LowerBandwidth, uint32_t UpperBandwidth, bool Symmetry, bool MustBeSquare, shape::vector_type Vector>
    struct intermediate_selector<
        T,
        LowerBandwidth,
        UpperBandwidth,
        Symmetry,
        MustBeSquare,
        Vector,
        typename std::enable_if<(LowerBandwidth > 0 && UpperBandwidth == 0 && Vector == shape::vector_type::none)>::type
    >
    {
        typedef lower_triangular<T, MustBeSquare> type;
    };

    template<typename T, uint32_t LowerBandwidth, uint32_t UpperBandwidth, bool Symmetry, bool MustBeSquare, shape::vector_type Vector>
    struct intermediate_selector<
        T,
        LowerBandwidth,
        UpperBandwidth,
        Symmetry,
        MustBeSquare,
        Vector,
        typename std::enable_if<(Vector == shape::vector_type::row)>::type
    >
    {
        typedef row_vector<T> type;
    };

    template<typename T, uint32_t LowerBandwidth, uint32_t UpperBandwidth, bool Symmetry, bool MustBeSquare, shape::vector_type Vector>
    struct intermediate_selector<
        T,
        LowerBandwidth,
        UpperBandwidth,
        Symmetry,
        MustBeSquare,
        Vector,
        typename std::enable_if<(Vector == shape::vector_type::col)>::type
    >
    {
        typedef col_vector<T> type;
    };
}}


#endif
