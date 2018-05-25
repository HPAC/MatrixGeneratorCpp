[![Build Status](https://travis-ci.org/HPAC/MatrixGeneratorCpp.svg?branch=master)](https://travis-ci.org/mcopik/MatrixGeneratorCpp)

Requirements:
- a C++14 compiler - currently tested against recent versions of GCC and Clang
- BLAS/LAPACK library is required for generation of symmetric positive-definite and orthogonal matrices. currently tested against OpenBLAS and IntelMKL

Supported libraries:
- [Eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page) - tested with 3.3.1
- [Blaze](https://bitbucket.org/blaze-lib/blaze/) - tested with 3.0
- [Armadillo](http://arma.sourceforge.net/) - tested with 7.5

Supported matrix shapes: full, symmetric, upper/lower triangular (non-square as well), diagonal, banded (partial support)

Support matrix properties: random, constant, symmetric positive-definite, orthogonal

An example of configuring project with support for Blaze and Armadillo libraries:

`cmake -DCMAKE_PREFIX_PATH="ARMADILLO_PATH;EIGEN_PATH;BLAZE_PATH" -DWITH_BLAZE=On -DWITH_ARMADILLO=On -DWITH_LAPACK=On`
