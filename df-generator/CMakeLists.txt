cmake_minimum_required(VERSION 2.8)

project(df-generator)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++0x -Wall -Werror")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include_directories(include)
add_subdirectory(include)
add_subdirectory(src)
