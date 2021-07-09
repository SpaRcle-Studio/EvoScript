cmake_minimum_required(VERSION 3.16)
project(Example)

set(CMAKE_CXX_STANDARD 20)

add_library(Example SHARED Example.cpp)
