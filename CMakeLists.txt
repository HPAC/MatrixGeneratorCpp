cmake_minimum_required(VERSION 3.1)

project(linalg_tests)

option(WITH_EXAMPLES "Build examples" Off)

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