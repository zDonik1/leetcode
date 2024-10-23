cmake_minimum_required(VERSION 3.29)

project(leetcode_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

set(SOURCE_FILES doctest.h main.cpp number_of_atoms.cpp
                 parsing_boolean_expression.cpp cousins_in_binary_tree_ii.cpp)

add_executable(leetcode_cpp ${SOURCE_FILES})
