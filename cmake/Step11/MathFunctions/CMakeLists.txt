message(STATUS ">> start of MathFunctions")
# step2.ex1
# add_library(MathFunctions MathFunctions.cxx mysqrt.cxx)
# step2.ex2
add_library(MathFunctions MathFunctions.cxx)

# step3.ex1
message(STATUS "CMAKE_CURRENT_SOURCE_DIR ==> ${CMAKE_CURRENT_SOURCE_DIR}")
#target_include_directories(MathFunctions
#    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
#    )
# step11
target_include_directories(MathFunctions INTERFACE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:include>
      )


# step2.ex2
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if (USE_MYMATH)
  target_compile_definitions(MathFunctions PRIVATE "USE_MYMATH")

  # step8.ex2
  include(MakeTable.cmake) # generates Table.h
  #add_library(SqrtLibrary STATIC
  #  mysqrt.cxx
  #  )
  add_library(SqrtLibrary STATIC
     mysqrt.cxx
     ${CMAKE_CURRENT_BINARY_DIR}/Table.h   # step8.ex2
     )
  # step8.ex2
  target_include_directories(SqrtLibrary PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}  # include 경로에도 추가함
        )

  # step10
  set_target_properties(SqrtLibrary PROPERTIES
        POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
        )

  # step2.ex2
  #target_link_libraries(MathFunctions PRIVATE SqrtLibrary)

  # step3.ex2
  target_link_libraries(SqrtLibrary PUBLIC tutorial_compiler_flags)
  target_link_libraries(MathFunctions PRIVATE SqrtLibrary)

  # step7 - step8
  if (0) #{{
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
  if (HAVE_LOG AND HAVE_EXP)
    target_compile_definitions(SqrtLibrary
       PRIVATE "HAVE_LOG" "HAVE_EXP"
       )
  endif()
  endif() #}}

endif()

# step3.ex2
target_link_libraries(MathFunctions PUBLIC tutorial_compiler_flags)
message(STATUS "tutorial_compiler_flags ==> ${tutorial_compiler_flags}")


# step5
set(installable_libs MathFunctions tutorial_compiler_flags)
if(TARGET SqrtLibrary)
  list(APPEND installable_libs SqrtLibrary)
endif()
install(TARGETS ${installable_libs}
   EXPORT MathFunctionsTargets   # step11
   DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)
message(STATUS "installable_libs ==> ${installable_libs}")

message(STATUS "<< end of MathFunctions")



