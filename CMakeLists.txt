cmake_minimum_required(VERSION 3.1)

project(linalg_tests)

#Armadillo
find_package(Armadillo CONFIG REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})
set(LINK_LIBRARIES ${ARMADILLO_LIBRARIES})

#Eigen
find_package(Eigen3 CONFIG REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

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

add_executable(test_arm test.cpp)
target_link_libraries(test_arm ${LINK_LIBRARIES})
