cmake_minimum_required(VERSION 3.10)
project(boost)

set(BOOST_ROOT "/home/ubuntu/boost")
set(Boost_NO_SYSTEM_PATHS ON)

#find_package(Boost REQUIRED COMPONENTS date_time)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

if (0) #{{
add_executable(today today.cpp)
target_link_libraries(today ${Boost_LIBRARIES})

add_executable(vector vector.cpp)
target_link_libraries(vector ${Boost_LIBRARIES})

add_executable(poly poly.cpp)
target_link_libraries(poly ${Boost_LIBRARIES})
endif() #}}

add_executable(intersection2 intersection2.cpp)
target_link_libraries(intersection2${Boost_LIBRARIES})


