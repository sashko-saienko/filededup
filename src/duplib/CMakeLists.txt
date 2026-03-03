cmake_minimum_required(VERSION 3.28.0)

project(duplib VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Poco REQUIRED COMPONENTS Foundation Util)

add_library(duplib
        include/duplib/files_map.h
        src/duplicates_finder.cpp
        include/duplib/duplicates_finder.h
        src/checksummer.cpp
        include/duplib/checksummer.h)

target_include_directories(duplib PUBLIC include)

target_link_libraries(duplib PRIVATE Poco::Foundation Poco::Util)
