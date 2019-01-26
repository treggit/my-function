cmake_minimum_required(VERSION 3.13)
project(my_function)

set(CMAKE_CXX_STANDARD 14)

add_executable(my_function main.cpp function.cpp function.h)