cmake_minimum_required(VERSION 3.1)
include(ExternalProject)

project(linalg_tests)

option(WITH_EXAMPLES "Build examples" Off)
option(WITH_TESTS "Build tests" Off)
option(TESTS_FORCE_BUILD "Build Gtest" Off)

set(CMAKE_CXX_STANDARD 14)
set(LINK_LIBRARIES "")

#Armadillo
find_package(Armadillo CONFIG REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})
set(LINK_LIBRARIES ${ARMADILLO_LIBRARIES})

#Eigen
find_package(Eigen3 CONFIG REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

#MTL4
find_package(MTL CONFIG REQUIRED)
include_directories(${MTL_INCLUDE_DIRS})

#Blaze
#Currently no CMake support
if(NOT Blaze_DIR)
   message(FATAL_ERROR "Path to Blaze Blaze_DIR has to be defined")
endif()
#Simple correctness check
if(EXISTS "${Blaze_DIR}/blaze/Blaze.h")
   include_directories(${Blaze_DIR})
else(EXISTS "${Blaze_DIR}/blaze/Blaze.h")
   message(FATAL_ERROR "Could not locate blaze/Blaze.h in ${Blaze_DIR}")
endif()

#Warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    #Warnins for Clang/AppleClang
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    #Warnings for GCC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

# Our headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

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
            GIT_REPOSITORY git@github.com:google/googletest.git
            CMAKE_ARGS -DBUILD_GTEST=On
            PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
            UPDATE_DISCONNECTED 1
            INSTALL_COMMAND "")
        ExternalProject_Get_Property(googletest source_dir binary_dir)
        include_directories(${source_dir}/googletest/include/)
        set(GTEST_LINK_FLAGS ${binary_dir}/googlemock/gtest/libgtest.a ${binary_dir}/googlemock/gtest/libgtest_main.a -lpthread CACHE STRING "GTest link flags" FORCE)
        # for local build one has to make tests dependent on the external project
        set(GTEST_DEPENDENCY googletest)
    endif(GTEST_FOUND)

    add_subdirectory(tests)
endif(${WITH_TESTS} AND ${TESTS_FORCE_BUILD})
