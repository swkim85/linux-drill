# step1.ex1
# cmake_minimum_required(VERSION 3.10)
cmake_minimum_required(VERSION 3.15) # step5.ex1
project(Tutorial VERSION 1.0)
add_executable(Tutorial tutorial.cxx)
message(STATUS "==> hello cmake step1")

# step1.ex2 - step3
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED True)

# step12
set(CMAKE_DEBUG_POSTFIX dbg)
set_target_properties(Tutorial PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
message(STATUS "CMAKE_DEBUG_POSTFIX ==> ${CMAKE_DEBUG_POSTFIX}")

# step3
add_library(tutorial_compiler_flags INTERFACE)
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_11)

# step4
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
message(STATUS "gcc_like_cxx ==> ${gcc_like_cxx}")

# step4
#target_compile_options(tutorial_compiler_flags INTERFACE
#  "$<${gcc_like_cxx}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>"
#  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
#)
target_compile_options(tutorial_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)
message(STATUS "tutorial_compiler_flags ==> ${tutorial_compiler_flags}")

# step1.ex3
configure_file( TutorialConfig.h.in  TutorialConfig.h )

# step10
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

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

# step5.ex1
install(TARGETS Tutorial DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  DESTINATION include
  )

# step5.ex2
if (1) #{{
enable_testing() # test
add_test(NAME Runs COMMAND Tutorial 25)

add_test(NAME Usage COMMAND Tutorial)
set_tests_properties(Usage
  PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number"
  )

add_test(NAME StandardUse COMMAND Tutorial 4)
set_tests_properties(StandardUse
  PROPERTIES PASS_REGULAR_EXPRESSION "4 is 2"
  )

function(do_test target arg result)
  add_test(NAME Comp${arg} COMMAND ${target} ${arg})
  set_tests_properties(Comp${arg}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result}
    )
endfunction()

# do a bunch of result based tests
do_test(Tutorial 4 "4 is 2")
do_test(Tutorial 9 "9 is 3")
do_test(Tutorial 5 "5 is 2.236")
do_test(Tutorial 7 "7 is 2.645")
do_test(Tutorial 25 "25 is 5")
do_test(Tutorial -25 "-25 is (-nan|nan|0)")
do_test(Tutorial 0.0001 "0.0001 is 0.01")

endif() #}}

# step6
include(CTest)

# step9
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${Tutorial_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Tutorial_VERSION_MINOR}")
set(CPACK_GENERATOR "TGZ")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

# step11
install(EXPORT MathFunctionsTargets
  FILE MathFunctionsTargets.cmake
  DESTINATION lib/cmake/MathFunctions
)

# step11
include(CMakePackageConfigHelpers)

message(STATUS "<< end of CMakeLists.txt")



