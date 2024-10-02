add_library(MathFunctions MathFunctions.cxx)

# should we use our own math functions
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if (USE_MYMATH)
  target_compile_definitions(MathFunctions PRIVATE "USE_MYMATH")

  include(MakeTable.cmake) # generates Table.h

  add_library(SqrtLibrary STATIC
              mysqrt.cxx
              ${CMAKE_CURRENT_BINARY_DIR}/Table.h   # Table.h 를 추가해 줌 
              )

# target_include_directories(SqrtLibrary PRIVATE
#                            ${CMAKE_CURRENT_BINARY_DIR}  # include 경로에도 추가함
#                            )

  # link SqrtLibrary to tutorial_compiler_flags
  target_link_libraries(SqrtLibrary PUBLIC tutorial_compiler_flags)

  # does this system provide the log and exp functions?
  include(CheckCXXSourceCompiles)
  check_cxx_source_compiles("
    #include <cmath>
    int main() {
      std::log(1.0);
      return 0;
    }
  " HAVE_LOG)
  check_cxx_source_compiles("
    #include <cmath>
    int main() {
      std::exp(1.0);
      return 0;
    }
  " HAVE_EXP)

  # add compile definitions
# if(HAVE_LOG AND HAVE_EXP)
#   target_compile_definitions(SqrtLibrary PRIVATE "HAVE_LOG" "HAVE_EXP")
# endif()

  target_link_libraries(MathFunctions PRIVATE SqrtLibrary)
endif()

target_include_directories(MathFunctions
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )

target_link_libraries(MathFunctions PUBLIC tutorial_compiler_flags)

set(installable_libs MathFunctions tutorial_compiler_flags)
if(TARGET SqrtLibrary)
  list(APPEND installable_libs SqrtLibrary)
endif()
install(TARGETS ${installable_libs} DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)
