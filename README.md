Requirements:
- a C++14 compiler - currently tested only with GCC 5.4
- [Eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page) - tested with 3.3.1
- [Blaze](https://bitbucket.org/blaze-lib/blaze/) - tested with 3.0
- [Armadillo](http://arma.sourceforge.net/) - tested with 7.5
- [MTL4 Open-Source Edition](http://www.simunova.com/node/135) - tested with 4.0.9555

We do not provide CMake find package modules but rely on on configuration files shipped by library managers. Currently Blaze does not support CMake scripts hence no package module is required and directory is specified manually. MTL4 ships CMake config file in a directory where CMake does not look by default, hence an additional path specification. It is not required if you have installed MTL4 in a system directory (/usr).

One way to configure the project:

`cmake -DCMAKE_PREFIX_PATH="ARMADILLO_PATH;EIGEN_PATH;MTL4_PATH/usr/share/mtl" -DBlaze_DIR=BLAZE_DIR $PROJECT_DIR`
