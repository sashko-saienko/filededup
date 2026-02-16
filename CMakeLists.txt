cmake_minimum_required(VERSION 3.28.0)

project(filededup VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Poco REQUIRED COMPONENTS Foundation Util)

add_executable(filededup)
target_link_libraries(filededup PRIVATE Poco::Foundation Poco::Util)

target_sources(filededup PRIVATE    src/filededup.cpp
                                    src/directory_scanner.cpp
                                    src/chksum.cpp
                                    src/cmdline.cpp)
