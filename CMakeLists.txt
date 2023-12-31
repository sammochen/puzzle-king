cmake_minimum_required(VERSION 3.14)

# set the project name
project(puzzle_king)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(all_tests all_tests.cpp)
target_link_libraries(all_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(all_tests)
