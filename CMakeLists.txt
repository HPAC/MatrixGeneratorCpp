cmake_minimum_required(VERSION 3.1)
include(ExternalProject)

project(linalg_tests)

#FIXME: Install target should NOT contain include paths
#This may lead to an error that we're including paths appearing in prefix paths,
#passed to locate dependent libraries
#Question is: should we have two targets, one to install and one for examples/tests
cmake_policy(SET CMP0041 OLD)

option(WITH_EXAMPLES "Build examples" Off)
option(WITH_TESTS "Build tests" Off)
option(TESTS_FORCE_BUILD "Build Gtest" Off)
option(WITH_BLAS "Build with BLAS" Off)
option(WITH_LAPACK "Build with LAPACK" Off)

option(WITH_BLAZE "Use Blaze library")
option(WITH_EIGEN "Use Eigen3 library")
option(WITH_ARMADILLO "Use Blaze library")
option(WITH_MTL4 "Use Blaze library")

option(WITH_MKL "Use Intel MKL" Off)
option(WITH_MULTITHREADING "Enable parallelization inside linked libraries" Off)

option(GENERATE_RELOCATABLE_TARGETS "" Off)

set(LINK_LIBRARIES "")

add_library(libtests INTERFACE)

# Use cxx compile features for CMake >= 3.8
if(${CMAKE_VERSION} VERSION_LESS "3.8.0")
    if(${WITH_BLAZE})
        set(CMAKE_CXX_STANDARD 14)
    else()
        set(CMAKE_CXX_STANDARD 11)
    endif()
else()
    if(${WITH_BLAZE})
        target_compile_features(libtests INTERFACE cxx_std_14)
    else()
        target_compile_features(libtests INTERFACE cxx_std_11)
    endif()
endif()

# Setup BLAS and LAPACK finders to find MKL
# Works with MKL versions newer than 10.3 and on 64 bit systems
if(${WITH_MKL})
    set(WITH_BLAS On)
    set(WITH_LAPACK On)
    if(${WITH_MULTITHREADING})
        set(BLA_VENDOR "Intel10_64lp")
    else()
        set(BLA_VENDOR "Intel10_64lp_seq")
    endif()
    target_compile_definitions(libtests INTERFACE HAVE_MKL)
endif()

#Look for BLAS
if(${WITH_BLAS})
    find_package(BLAS REQUIRED)
    add_definitions(-DHAVE_BLAS)
    target_compile_options(libtests INTERFACE ${BLAS_LINKER_FLAGS})
    target_link_libraries(libtests INTERFACE ${BLAS_LIBRARIES})
else()
    message("Building without BLAS - SPD generator won't be available")
endif()

#Look for LAPACK
if(${WITH_LAPACK})
    find_package(LAPACK REQUIRED)
    add_definitions(-DHAVE_LAPACK)
    target_compile_options(libtests INTERFACE ${LAPACK_LINKER_FLAGS})
    target_link_libraries(libtests INTERFACE ${LAPACK_LIBRARIES})

    # Neither BLAS nor LAPACK find modules export include directories
    # Eigen, on the other hand, uses mkl.h and we use mkl_cblas.h
    # This code assumes that MKL has a main directory with include/
    # and MKL libriares are located somewhere deep in in lib/
    if(${WITH_MKL})
        foreach(lib ${LAPACK_LIBRARIES})
            string(REGEX REPLACE "lib/.*libmkl_core.so" "" find_res ${lib})
            if(NOT ${lib} STREQUAL ${find_res})
                target_include_directories(libtests INTERFACE ${find_res}/include)
            endif()
        endforeach()
    endif()
else()
    message("Building without LAPACK - orthogonal generator won't be available")
endif()

#Armadillo
if(${WITH_ARMADILLO})
    find_package(Armadillo REQUIRED)
    target_include_directories(libtests INTERFACE ${ARMADILLO_INCLUDE_DIRS})
    target_link_libraries(libtests INTERFACE ${ARMADILLO_LIBRARIES})
    
    target_compile_definitions(libtests INTERFACE HAVE_ARMADILLO)
    target_compile_definitions(libtests INTERFACE $<$<CONFIG:RELEASE>:ARMA_NO_DEBUG>)
endif()

#Eigen
if(${WITH_EIGEN})
    find_package(Eigen3 CONFIG REQUIRED)
    target_include_directories(libtests INTERFACE ${EIGEN3_INCLUDE_DIRS})
    target_compile_definitions(libtests INTERFACE HAVE_EIGEN)
    if(${WITH_MKL})
        target_compile_definitions(libtests INTERFACE EIGEN_USE_MKL_ALL)
        #Eigen includes mkl.h but neither BLAS or LAPACK find modules allow
    endif()
endif()

#MTL4
if(${WITH_MTL4})
    find_package(MTL CONFIG REQUIRED)
    include_directories(${MTL_INCLUDE_DIRS})
    add_definitions(-DHAVE_MTL4)
endif()

#Blaze
if(${WITH_BLAZE})
    find_package(blaze CONFIG REQUIRED)
    target_compile_definitions(libtests INTERFACE HAVE_BLAZE)
    # For future dependencies on imported targets
    #target_include_directories(libtests INTERFACE $<TARGET_PROPERTY:blaze::blaze,INTERFACE_INCLUDE_DIRECTORIES>)
    if(${GENERATE_RELOCATABLE_TARGETS})
        target_link_libraries(libtests INTERFACE blaze::blaze)
    else()
        # one can't use generator expressions for exported targets - use has to find blaze target
        get_target_property(BLAZE_INCLUDE blaze::blaze INTERFACE_INCLUDE_DIRECTORIES)
        target_include_directories(libtests INTERFACE ${BLAZE_INCLUDE})
    endif()

    if(${WITH_MKL})
        # Otherwise Blaze would not compiler
        target_compile_definitions(libtests INTERFACE BLAZE_BLAS_INCLUDE_FILE=<mkl_cblas.h>)
    endif()
endif()

#Warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    #Warnins for Clang/AppleClang
    #undef warnings called for gtest or blaze
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-shorten-64-to-32\
        -Wno-documentation-unknown-command -Wno-undef -Wno-c++98-compat-pedantic \
        -Wno-reserved-id-macro -Wno-documentation -Wno-used-but-marked-unused\
        -Wno-deprecated -Wno-sign-conversion -Wno-double-promotion -Wno-float-equal\
        -Wno-missing-noreturn -Wno-padded -Wno-shift-sign-overflow -Wno-weak-vtables\
        -Wno-missing-variable-declarations -Wno-global-constructors -Wno-float-conversion\
        -Wno-old-style-cast -Wno-extra-semi -Wno-unused-parameter")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    #Warnings for GCC
    #Werror is not possible because we can't disable warning on enumerator comparison in
    #conditional expression which is shown for Eigen code
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

# Our headers
target_include_directories(libtests INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

# Installation procedure
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})
install(TARGETS libtests EXPORT LibTestsConfig)
export(EXPORT LibTestsConfig FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/LibTestsConfig.cmake")
export(PACKAGE libtests)
install(EXPORT LibTestsConfig
    FILE LibTestsConfig.cmake
    DESTINATION share/libtests/cmake
)

if(${WITH_EXAMPLES})
    add_subdirectory(examples)
endif(${WITH_EXAMPLES})

################################
# GTest
################################
if(${WITH_TESTS})

    enable_testing()
    find_package(GTest)
    if(GTEST_FOUND AND ${TESTS_FORCE_BUILD})
        include_directories(${GTEST_INCLUDE_DIRS})
        set(GTEST_LINK_FLAGS ${GTEST_BOTH_LIBRARIES} -lpthread)
    else()
        # don't update automatically
        ExternalProject_Add(googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            CMAKE_ARGS -DBUILD_GTEST=On
            PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
            UPDATE_DISCONNECTED 1
            INSTALL_COMMAND "")
        ExternalProject_Get_Property(googletest source_dir binary_dir)
        include_directories(${source_dir}/googletest/include/)
        set(GTEST_LINK_FLAGS ${binary_dir}/googlemock/gtest/libgtest.a ${binary_dir}/googlemock/gtest/libgtest_main.a -lpthread CACHE STRING "GTest link flags" FORCE)
        # for local build one has to make tests dependent on the external project
        set(GTEST_DEPENDENCY googletest)
    endif(GTEST_FOUND AND ${TESTS_FORCE_BUILD})

    add_subdirectory(tests)
endif(${WITH_TESTS})
