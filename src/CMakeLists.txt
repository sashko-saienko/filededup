cmake_minimum_required(VERSION 3.28.0)

project(deduplicators VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Poco REQUIRED COMPONENTS Foundation Util)

add_subdirectory(condedup)
add_subdirectory(uidedup)
add_subdirectory(deduplib)
add_subdirectory(reusablelib)
