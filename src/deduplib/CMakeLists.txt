cmake_minimum_required(VERSION 3.28.0)

project(deduplib VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LINK_WHAT_YOU_USE)
set(CXX_INCLUDE_WHAT_YOU_USE)

find_package(Poco REQUIRED COMPONENTS Foundation Util)

add_library(
        ${PROJECT_NAME} SHARED
        include/deduplib/files_map.h
        src/duplicates_finder.cpp
        include/deduplib/duplicates_finder.h
        src/checksummer.cpp
        include/deduplib/checksummer.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_definitions(${PROJECT_NAME} PRIVATE LOGGER_NAME="${PROJECT_NAME}")
target_link_libraries(${PROJECT_NAME} PRIVATE Poco::Foundation Poco::Util)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
    target_link_options(${PROJECT_NAME} PUBLIC --coverage)
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND find ${CMAKE_BINARY_DIR} -type f -name '*.gcda' -exec rm {} +)
endif()

AddClangTidy(${PROJECT_NAME})
AddCppCheck(${PROJECT_NAME})