cmake_minimum_required(VERSION 3.28.0)

project(duplib_ut VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
        duplib_ut
        duplicates_finder_test.cpp
)

target_link_libraries(duplib_ut PRIVATE duplib testsuplib Catch2::Catch2WithMain)

catch_discover_tests(duplib_ut)