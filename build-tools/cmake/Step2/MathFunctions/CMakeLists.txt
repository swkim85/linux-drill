# step2.ex1
# add_library(MathFunctions MathFunctions.cxx mysqrt.cxx)
# step2.ex2
add_library(MathFunctions MathFunctions.cxx)

# step2.ex2
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if (USE_MYMATH)
  target_compile_definitions(MathFunctions PRIVATE "USE_MYMATH")
  add_library(SqrtLibrary STATIC
              mysqrt.cxx
              )
  target_link_libraries(MathFunctions PRIVATE SqrtLibrary)
endif()

