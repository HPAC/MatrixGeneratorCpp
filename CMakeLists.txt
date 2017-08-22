cmake_minimum_required(VERSION 3.1)
include(ExternalProject)

project(linalg_tests)

option(WITH_EXAMPLES "Build examples" Off)
option(WITH_TESTS "Build tests" Off)
option(TESTS_FORCE_BUILD "Build Gtest" Off)
option(WITH_BLAS "Build with BLAS" On)
option(WITH_LAPACK "Build with LAPACK" On)

option(WITH_BLAZE "Use Blaze library")
option(WITH_EIGEN "Use Eigen3 library")
option(WITH_ARMADILLO "Use Blaze library")
option(WITH_MTL4 "Use Blaze library")

set(CMAKE_CXX_STANDARD 14)
set(LINK_LIBRARIES "")

add_library(libtests INTERFACE)

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
else()
    message("Building without LAPACK - orthogonal generator won't be available")
endif()

#Armadillo
if(${WITH_ARMADILLO})
    find_package(Armadillo CONFIG REQUIRED)
    target_include_directories(libtests INTERFACE ${ARMADILLO_INCLUDE_DIRS})
    target_link_libraries(libtests INTERFACE ${ARMADILLO_LIBRARIES})
    add_definitions(-DHAVE_ARMADILLO)
endif()

#Eigen
if(${WITH_EIGEN})
    find_package(Eigen3 CONFIG REQUIRED)
    target_include_directories(libtests INTERFACE ${EIGEN3_INCLUDE_DIRS})
    add_definitions(-DHAVE_EIGEN)
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
    add_definitions(-DHAVE_BLAZE)
    target_include_directories(libtests INTERFACE $<TARGET_PROPERTY:blaze::blaze,INTERFACE_INCLUDE_DIRECTORIES>)
endif()

#Warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    #Warnins for Clang/AppleClang
    #undef warnings called for gtest or blaze
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Werror -Wno-shorten-64-to-32\
        -Wno-documentation-unknown-command -Wno-undef -Wno-c++98-compat-pedantic \
        -Wno-reserved-id-macro -Wno-documentation -Wno-used-but-marked-unused\
        -Wno-deprecated -Wno-sign-conversion -Wno-double-promotion -Wno-float-equal\
        -Wno-missing-noreturn -Wno-padded -Wno-shift-sign-overflow -Wno-weak-vtables\
        -Wno-missing-variable-declarations -Wno-global-constructors -Wno-float-conversion\
        -Wno-old-style-cast -Wno-extra-semi")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    #Warnings for GCC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

# Our headers
target_include_directories(libtests INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

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
