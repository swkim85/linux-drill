cmake_minimum_required(VERSION 3.10)
project(boost)

set(BOOST_ROOT "/home/ubuntu/boost")
set(Boost_NO_SYSTEM_PATHS ON)

function(print_all_variables)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()


#find_package(Boost REQUIRED COMPONENTS date_time)
find_package(Boost REQUIRED COMPONENTS serialization filesystem)
include_directories(${Boost_INCLUDE_DIRS})
message(STATUS "Boost_INCLUDE_DIRS ==> ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES ==> ${Boost_LIBRARIES}")

#message(STATUS "---------------------------------")
#print_all_variables()

if (0) #{{
add_executable(today today.cpp)
target_link_libraries(today ${Boost_LIBRARIES})

add_executable(vector vector.cpp)
target_link_libraries(vector ${Boost_LIBRARIES})

add_executable(poly poly.cpp)
target_link_libraries(poly ${Boost_LIBRARIES})

add_executable(intersection2 intersection2.cpp)
target_link_libraries(intersection2 ${Boost_LIBRARIES})

add_executable(phone   phone.cpp)
target_link_libraries(phone  ${Boost_LIBRARIES})

add_executable(serial serial.cpp)
target_link_libraries(serial  Boost::serialization)

#add_executable(token1 token1.cpp)
#target_link_libraries(token1  ${Boost_LIBRARIES})
endif() #}}

add_executable(mat1 mat1.cpp)
target_link_libraries(mat1  )



