
cmake_minimum_required(VERSION 3.15)

project(Tutorial VERSION 1.0)

add_library(tutorial_compiler_flags INTERFACE)
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_11)

# TODO 2: Create helper variables to determine which compiler we are using:
# * Create a new variable gcc_like_cxx that is true if we are using CXX and
#   any of the following compilers: ARMClang, AppleClang, Clang, GNU, LCC
# * Create a new variable msvc_cxx that is true if we are using CXX and MSVC
# Hint: Use set() and COMPILE_LANG_AND_ID
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")


# TODO 3: Add warning flag compile options to the interface library
# tutorial_compiler_flags.
# * For gcc_like_cxx, add flags -Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused
# * For msvc_cxx, add flags -W3
# Hint: Use target_compile_options()
target_compile_options(tutorial_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>"
  "$<${msvc_cxx}:-W3>"
)

# TODO 4: With nested generator expressions, only use the flags for the
# build-tree
# Hint: Use BUILD_INTERFACE
target_compile_options(tutorial_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

configure_file(TutorialConfig.h.in TutorialConfig.h)

add_subdirectory(MathFunctions)

add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC MathFunctions tutorial_compiler_flags)

target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

message(STATUS "gcc_like_cxx -->: ${gcc_like_cxx}")

