cmake_minimum_required(VERSION 3.15)
project(MyExe)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(myexe main.cxx)
install(TARGETS myexe)
