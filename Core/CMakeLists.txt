cmake_minimum_required(VERSION 3.16)
project(EvoScriptCore)

set(CMAKE_CXX_STANDARD 20)

add_library(EvoScriptCore STATIC
        src/EvoScript/Compilation/Casting.cpp
        src/EvoScript/Compilation/Compiler.cpp
        src/EvoScript/Compilation/AddressTableGen.cpp
        src/EvoScript/Compilation/Method.cpp

        src/EvoScript/Script.cpp
        src/EvoScript/IState.cpp

        inc/EvoScript/Win32State.hpp
        )

target_include_directories(EvoScriptCore PUBLIC inc)
