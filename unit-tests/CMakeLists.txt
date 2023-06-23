cmake_minimum_required(VERSION 3.21)
project(unit-tests)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

file(GLOB TEST_CPP_FILES *.cpp)
add_executable(unit-tests ${TEST_CPP_FILES})

target_link_libraries(unit-tests GTest::gtest GTest::gtest_main lib)
