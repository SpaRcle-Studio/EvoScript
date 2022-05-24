cmake_minimum_required(VERSION 3.16)
project(EvoScript)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Core)

add_executable(EvoScript UnitTests/main4.cpp)
target_link_libraries(EvoScript EvoScriptCore)