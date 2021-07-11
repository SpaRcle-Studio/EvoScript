cmake_minimum_required(VERSION 3.16)
project(Example2)

set(CMAKE_CXX_STANDARD 20)

add_library(Example2 SHARED Example.cpp)
