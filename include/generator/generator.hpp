//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_GENERATOR_GENERATOR_HPP
#define LINALG_TESTS_GENERATOR_GENERATOR_HPP

#include <generator/shape.hpp>
#include <generator/property.hpp>
#include <generator/properties/properties.hpp>
//FIXME: create header for all facades
#ifdef HAVE_BLAZE
#include <generator/facades/blaze.hpp>
#endif
#ifdef HAVE_EIGEN
#include <generator/facades/eigen.hpp>
#endif
#ifdef HAVE_ARMADILLO
#include <generator/facades/armadillo.hpp>
#endif

#endif
