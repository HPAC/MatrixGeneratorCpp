//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_LIBRARIES_HPP
#define LINALG_TESTS_LIBRARIES_HPP

#ifdef HAVE_ARMADILLO
#include <armadillo>
#endif

#ifdef HAVE_EIGEN
#include <Eigen/Dense>
#endif

#ifdef HAVE_BLAZE
#include <blaze/Blaze.h>
#endif

#ifdef HAVE_MTL4
#include <boost/numeric/mtl/mtl.hpp>
// Required for mtl::traits::eval
#include <boost/numeric/mtl/utility/eval.hpp>
#endif

namespace library {

#ifdef HAVE_EIGEN
    struct eigen {};
#endif
    
#ifdef HAVE_BLAZE
    struct blaze {};
#endif

#ifdef HAVE_MTL4
    struct mtl4 {};
#endif

#ifdef HAVE_ARMADILLO
    struct arma {};
#endif

}

#endif
