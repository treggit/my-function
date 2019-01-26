cmake_minimum_required(VERSION 3.13)
project(my_function)

set(CMAKE_CXX_STANDARD 14)

add_executable(my_function gtest/gtest_main.cc
    gtest/gtest-all.cc
    gtest/gtest.h test.cpp function.h)