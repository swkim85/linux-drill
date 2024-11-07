# step1.ex1
cmake_minimum_required(VERSION 3.10)
project(Tutorial VERSION 1.0)
add_executable(Tutorial tutorial.cxx)
message(STATUS "==> hello cmake step1")

# step1.ex2 - step3
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED True)

# step3
add_library(tutorial_compiler_flags INTERFACE)
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_11)

# step1.ex3
configure_file( TutorialConfig.h.in  TutorialConfig.h )

# step2 - step3.ex2
add_subdirectory(MathFunctions)
#target_link_libraries(Tutorial PUBLIC MathFunctions)
target_link_libraries(Tutorial PUBLIC MathFunctions tutorial_compiler_flags)

# step3.ex1
#delete# list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
message(STATUS "EXTRA_INCLUDES ==> ${EXTRA_INCLUDES}")
message(STATUS "PROJECT_BINARY_DIR ==> ${PROJECT_BINARY_DIR}")

# step1.ex3 - step2 - step3.ex1
target_include_directories(Tutorial PUBLIC
   "${PROJECT_BINARY_DIR}"
#  ${EXTRA_INCLUDES}
   )

