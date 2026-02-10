cmake_minimum_required(VERSION 3.28.0)

project(rm_duplicates VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(rm_duplicates)
target_link_libraries(rm_duplicates PRIVATE -lPocoFoundation)

target_sources(rm_duplicates PRIVATE    src/rm_duplicates.cpp
                                        src/chksum.cpp
                                        src/cmdline.cpp)
