cmake_minimum_required(VERSION 3.21)
project(lib)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE LIB_CPP_FILES *.cpp)
add_library(lib STATIC ${LIB_CPP_FILES})
target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
