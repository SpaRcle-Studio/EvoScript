cmake_minimum_required(VERSION 3.16)
project(Example2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_library(Example2 SHARED Example.cpp)
