cmake_minimum_required(VERSION 3.28.0)

project(find_duplicates VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(find_duplicates)
target_link_libraries(find_duplicates PRIVATE -lPocoFoundation)

target_sources(find_duplicates PRIVATE  src/find_duplicates.cpp
                                        src/chksum.cpp)
