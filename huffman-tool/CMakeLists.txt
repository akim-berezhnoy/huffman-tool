cmake_minimum_required(VERSION 3.25)
project(huffman-tool)

set(CMAKE_CXX_STANDARD 20)

file(GLOB TOOL_SRC *.cpp)

find_package(Boost REQUIRED COMPONENTS program_options)

add_executable(huffman-tool ${TOOL_SRC})
target_link_libraries(huffman-tool PRIVATE ${Boost_LIBRARIES} lib)
target_include_directories(huffman-tool PRIVATE ${Boost_INCLUDE_DIRS})
