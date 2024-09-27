cmake_minimum_required(VERSION 3.10)

project(Tutorial VERSION 1.0)

# TODO 4: Replace the following code by:
# * Creating an interface library called tutorial_compiler_flags
#   Hint: use add_library() with the INTERFACE signature
# * Add compiler feature cxx_std_11 to tutorial_compiler_flags
#   Hint: Use target_compile_features()

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED True)
add_library(tutorial_compiler_flags INTERFACE)
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_11)

configure_file(TutorialConfig.h.in TutorialConfig.h)

add_subdirectory(MathFunctions)
#delete# list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")

add_executable(Tutorial tutorial.cxx)

# target_link_libraries(Tutorial PUBLIC MathFunctions)
target_link_libraries(Tutorial PUBLIC MathFunctions tutorial_compiler_flags)

target_include_directories(Tutorial PUBLIC
    "${PROJECT_BINARY_DIR}"
# ${EXTRA_INCLUDES}
    )

