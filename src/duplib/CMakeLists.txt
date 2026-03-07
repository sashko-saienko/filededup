cmake_minimum_required(VERSION 3.28.0)

project(duplib VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Poco REQUIRED COMPONENTS Foundation Util)

add_library(
        duplib SHARED
        include/duplib/files_map.h
        src/duplicates_finder.cpp
        include/duplib/duplicates_finder.h
        src/checksummer.cpp
        include/duplib/checksummer.h
)

target_include_directories(duplib PUBLIC include)
target_compile_definitions(duplib PRIVATE LOGGER_NAME="${PROJECT_NAME}")
target_link_libraries(duplib PRIVATE reusablelib Poco::Foundation Poco::Util)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_options(duplib PRIVATE --coverage)
    target_link_options(duplib PUBLIC --coverage)
    add_custom_command(TARGET duplib PRE_BUILD COMMAND find ${CMAKE_BINARY_DIR} -type f -name '*.gcda' -exec rm {} +)
endif()

AddClangTidy(duplib)