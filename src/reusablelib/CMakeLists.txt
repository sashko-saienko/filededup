cmake_minimum_required(VERSION 3.28.0)

project(reusablelib VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Poco REQUIRED COMPONENTS Util)

add_library(
        reusablelib SHARED
        include/reusablelib/logging.h
        src/logging.cpp
)

target_include_directories(reusablelib PUBLIC include)
target_link_libraries(reusablelib PRIVATE Poco::Util)

AddClangTidy(reusablelib)