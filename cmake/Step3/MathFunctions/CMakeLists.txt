message(STATUS ">> start of MathFunctions")
# step2.ex1
# add_library(MathFunctions MathFunctions.cxx mysqrt.cxx)
# step2.ex2
add_library(MathFunctions MathFunctions.cxx)

# step3.ex1
message(STATUS "CMAKE_CURRENT_SOURCE_DIR ==> ${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(MathFunctions
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
    )

# step2.ex2
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if (USE_MYMATH)
  target_compile_definitions(MathFunctions PRIVATE "USE_MYMATH")
  add_library(SqrtLibrary STATIC
              mysqrt.cxx
              )

  # step2.ex2
  #target_link_libraries(MathFunctions PRIVATE SqrtLibrary)

  # step3.ex2
  target_link_libraries(SqrtLibrary PUBLIC tutorial_compiler_flags)
  target_link_libraries(MathFunctions PRIVATE SqrtLibrary)
endif()

# step3.ex2
target_link_libraries(MathFunctions PUBLIC tutorial_compiler_flags)
message(STATUS "tutorial_compiler_flags ==> ${tutorial_compiler_flags}")

message(STATUS "<< end of MathFunctions")
