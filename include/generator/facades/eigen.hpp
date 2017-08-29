//  Copyright (c) 2017 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_FACADES_EIGEN_HPP
#define LINALG_TESTS_GENERATOR_FACADES_EIGEN_HPP

#include <ctime>

#include <Eigen/Dense>

#include <libraries.hpp>
#include <generator/facades/facade.hpp>

namespace generator {

    namespace detail {

        template<typename T, typename Shape>
        struct eigen_matrix_type
        {
            typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> type;

            static type create(uint32_t rows, uint32_t cols, T * ptr)
            {
                return type{Eigen::Map<type>(ptr, cols, rows)}.transpose();
            }
        };

        template<typename T>
        struct eigen_matrix_type<T, ::generator::intermediate::self_adjoint<T>>
        {
            typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> type;

            static type create(uint32_t rows, uint32_t cols, T * ptr)
            {
                return Eigen::Map<type>(ptr, rows, cols);
            }
        };

        template<typename T>
        struct eigen_matrix_type<T, ::generator::intermediate::diagonal<T>>
        {
            typedef Eigen::DiagonalMatrix<T, Eigen::Dynamic, Eigen::Dynamic> type;

            static type create(uint32_t rows, uint32_t, T * ptr)
            {
                //Eigen::Map does not work very well with DiagonalMatrix
                type matrix{rows};
                auto & diagonal = matrix.diagonal();
                for(uint32_t i = 0; i < rows; ++i) {
                    diagonal.coeffRef(i) = ptr[i];
                }
                return matrix;
            }
        };

    }

    // Forward declaration
    template<typename Library, typename T>
    struct generator;

    template<typename T>
    struct generator<library::eigen, T> : public
        detail::generator_facade<generator<library::eigen, T>, T, detail::eigen_matrix_type>
    {
        typedef detail::generator_facade<
                generator<library::eigen, T>,
                T,
                detail::eigen_matrix_type
            > base_t;
        typedef T value_t;


        template<typename Intermediate>
        using matrix_t = detail::eigen_matrix_type<T, std::decay_t<Intermediate>>;

        generator(uint32_t seed = time(0)) :
                base_t(seed)
        {}

        template<typename Intermediate>
        typename matrix_t<Intermediate>::type create(Intermediate && intermediate)
        {
            return matrix_t<Intermediate>::create(
                intermediate.size.rows,
                intermediate.size.cols,
                intermediate.data.get()
                );
        }
    };
}

#endif
