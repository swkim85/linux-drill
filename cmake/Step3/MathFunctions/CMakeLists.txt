add_library(MathFunctions MathFunctions.cxx)

# TODO 1: State that anybody linking to MathFunctions needs to include the
# current source directory, while MathFunctions itself doesn't.
# Hint: Use target_include_directories with the INTERFACE keyword
message(STATUS "CMAKE_CURRENT_SOURCE_DIR -->: ${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(MathFunctions
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )

# should we use our own math functions
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if (USE_MYMATH)
  target_compile_definitions(MathFunctions PRIVATE "USE_MYMATH")

  # library that just does sqrt
  add_library(SqrtLibrary STATIC
              mysqrt.cxx
              )

  # target_link_libraries(MathFunctions PRIVATE SqrtLibrary)
  target_link_libraries(SqrtLibrary PUBLIC tutorial_compiler_flags)
  target_link_libraries(MathFunctions PRIVATE SqrtLibrary)
endif()

target_link_libraries(MathFunctions PUBLIC tutorial_compiler_flags)

