//  Copyright (c) 2016 Marcin Copik
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINALG_TESTS_LIBRARIES_HPP
#define LINALG_TESTS_LIBRARIES_HPP

#include <armadillo>
#include <Eigen/Dense>
#include <blaze/Blaze.h>
#include <boost/numeric/mtl/mtl.hpp>
// Required for mtl::traits::eval
#include <boost/numeric/mtl/utility/eval.hpp>

namespace library {

    struct eigen {};
    
    struct blaze {};

    struct mtl4 {};

    struct arma {};

}

#endif
