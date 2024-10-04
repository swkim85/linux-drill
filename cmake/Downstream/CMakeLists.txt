cmake_minimum_required(VERSION 3.15)
project(Downstream)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

function(print_all_variables)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()

#message(STATUS "---------------------------------")
#print_all_variables()

# find MathFunctions
find_package(MathFunctions 3.4.1 EXACT)
message(STATUS "MathFunctions_FOUND ==> ${MathFunctions_FOUND}")
message(STATUS "MathFunctions_DIR ==> ${MathFunctions_DIR}")
message(STATUS "MathFunctions_VERSION ==> ${MathFunctions_VERSION}")
message(STATUS "MathFunctions_CONFIG ==> ${MathFunctions_CONFIG}")
message(STATUS "MathFunctions_CONSIDERED_CONFIG ==> ${MathFunctions_CONSIDERED_CONFIG}")

#message(STATUS "---------------------------------")
#print_all_variables()


# create executable
add_executable(myexe main.cc)

# use MathFunctions library
target_link_libraries(myexe PRIVATE MathFunctions::MathFunctions)
