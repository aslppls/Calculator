project(Google_tests)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run main.cpp tests.cpp ../Calculator.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main)

