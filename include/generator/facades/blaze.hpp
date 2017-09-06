//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_FACADES_BLAZE_HPP
#define LINALG_TESTS_GENERATOR_FACADES_BLAZE_HPP

#include <ctime>

#include <libraries.hpp>
#include <generator/generator.hpp>
#include <generator/facades/facade.hpp>

namespace generator {

    namespace detail {

        template<typename T, typename Shape>
        struct blaze_matrix_type;

        template<typename T>
        struct blaze_matrix_type<T, ::generator::intermediate::general<T>>
        {
            typedef blaze::DynamicMatrix<T> type;

            static type create(uint32_t rows, uint32_t cols, T * ptr)
            {
                return type(rows, cols, ptr);
            }
        };

        /// As long as we don't use block matrices,
        /// Hermitian and Symmetric in Blaze behaves identically for non-complex types
        /// Hence we can always use Hermitian
        /// \tparam T
        template<typename T>
        struct blaze_matrix_type<T, ::generator::intermediate::self_adjoint<T>>
        {
            typedef blaze::HermitianMatrix< blaze::DynamicMatrix<T> > type;

            static type create(uint32_t rows, uint32_t, T * ptr)
            {
                return type(rows, ptr);
            }
        };

        template<typename T>
        struct blaze_matrix_type<T, ::generator::intermediate::upper_triangular<T>>
        {
            typedef blaze::UpperMatrix< blaze::DynamicMatrix<T> > type;

            static type create(uint32_t rows, uint32_t, T * ptr)
            {
                return type(rows, ptr);
            }
        };

        template<typename T>
        struct blaze_matrix_type<T, ::generator::intermediate::lower_triangular<T>>
        {
            typedef blaze::LowerMatrix< blaze::DynamicMatrix<T> > type;

            static type create(uint32_t rows, uint32_t, T * ptr)
            {
                return type(rows, ptr);
            }
        };

        /// As long as we don't use block matrices,
        /// Hermitian and Symmetric in Blaze behaves identically for non-complex types
        /// Hence we can always use Hermitian
        /// \tparam T
        template<typename T>
        struct blaze_matrix_type<T, ::generator::intermediate::diagonal<T>>
        {
            // FIXME: should we use here CompressedMatrix?
            typedef blaze::DiagonalMatrix< blaze::DynamicMatrix<T> > type;

            /// Blaze constructors expect an n*n matrix even for diagonal matrix
            /// It's easier to just write values
            static type create(uint32_t rows, uint32_t, T * ptr)
            {
                type mat(rows, 0);
                for(std::size_t i = 0; i < rows; ++i)
                    mat(i, i) = ptr[i];
                return mat;
            }
        };

        template<typename T>
        struct blaze_matrix_type<T, ::generator::intermediate::row_vector<T>>
        {
            typedef blaze::DynamicVector<T, blaze::rowVector> type;

            static type create(uint32_t, uint32_t cols, T * ptr)
            {
                return type(cols, ptr);
            }
        };

        template<typename T>
        struct blaze_matrix_type<T, ::generator::intermediate::col_vector<T>>
        {
            typedef blaze::DynamicVector<T, blaze::columnVector> type;

            static type create(uint32_t rows, uint32_t, T * ptr)
            {
                return type(rows, ptr);
            }
        };

    }

    // Forward declaration
    template<typename Library, typename T>
    struct generator;

    template<typename T>
    struct generator<library::blaze, T> : public
        detail::generator_facade<generator<library::blaze, T>, T, detail::blaze_matrix_type>
    {
        typedef detail::generator_facade<
                generator<library::blaze, T>,
                T,
                detail::blaze_matrix_type
            > base_t;
        typedef T value_t;


        template<typename Intermediate>
        using matrix_t = detail::blaze_matrix_type<T, std::decay_t<Intermediate>>;

        generator(uint32_t seed = static_cast<uint32_t>(time(0))) :
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
