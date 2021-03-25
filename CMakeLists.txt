cmake_minimum_required(VERSION 3.17)
project(calc)

set(CMAKE_CXX_STANDARD 17)

include_directories(GoogleTests/lib/googletest/include)

add_executable(calc main.cpp Calculator.cpp Calculator.h)
target_link_libraries(calc gtest gtest_main)
target_link_libraries(calc gmock gmock_main)

add_subdirectory(lib)
