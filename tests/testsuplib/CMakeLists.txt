cmake_minimum_required(VERSION 3.28.0)

project(testsuplib VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(
        testsuplib SHARED
        src/time_helpers.cpp
        include/testsuplib/time_helpers.h
        src/file_helpers.cpp
        include/testsuplib/file_helpers.h
)

target_include_directories(testsuplib PUBLIC include)

AddCppCheck(${PROJECT_NAME})
