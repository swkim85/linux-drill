# CMakeLists.txt
cmake_minimum_required(VERSION 3.24)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL "file://${PROJECT_SOURCE_DIR}/03597a01ee50ed33e9dfd640b249b4be3799d395.zip"
  # URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(CTest)
enable_testing()

include(GoogleTest)

add_executable( sort_test sort_test.cc sort.cc )
target_link_libraries( sort_test  GTest::gtest_main )
gtest_discover_tests(sort_test DISCOVERY_MODE POST_BUILD)

#add_executable( hello_test hello_test.cpp )
#target_link_libraries( hello_test GTest::gtest_main )
#gtest_discover_tests(hello_test)


