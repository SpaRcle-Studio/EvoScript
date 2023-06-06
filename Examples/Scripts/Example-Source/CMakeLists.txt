cmake_minimum_required(VERSION 3.16)
project(Example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_library(Example SHARED Example.cpp)
