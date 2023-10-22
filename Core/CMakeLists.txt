cmake_minimum_required(VERSION 3.16)
project(EvoScriptCore)

set(CMAKE_CXX_STANDARD 20)

if (EVO_SCRIPT_STATIC_LIBRARY)
    add_library(EvoScriptCore STATIC EvoScript.cxx)
else()
    add_library(EvoScriptCore SHARED EvoScript.cxx)
    add_library(EvoScriptCore::lib ALIAS EvoScriptCore)
endif()

target_include_directories(EvoScriptCore PUBLIC inc)
