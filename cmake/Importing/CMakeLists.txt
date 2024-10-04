cmake_minimum_required(VERSION 3.15)
project(Importing)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
add_executable(myexe IMPORTED)
set_property(TARGET myexe PROPERTY IMPORTED_LOCATION "$ENV{myexeloc}/bin/myexe")

add_custom_command(OUTPUT main.cc COMMAND myexe)

add_executable(mynewexe main.cc)
