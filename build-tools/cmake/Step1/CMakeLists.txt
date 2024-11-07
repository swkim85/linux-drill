
# step1.ex1
cmake_minimum_required(VERSION 3.10)
project(Tutorial VERSION 1.0)
add_executable(Tutorial tutorial.cxx)

if(VERBOSE)
  message(STATUS "==> hello cmake step1")
endif()

# step1.ex2
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# step1.ex3
configure_file( TutorialConfig.h.in  TutorialConfig.h )
target_include_directories(Tutorial PUBLIC "${PROJECT_BINARY_DIR}")
if(VERBOSE)
  message(STATUS "PROJECT_BINARY_DIR ==> ${PROJECT_BINARY_DIR}")
endif()

