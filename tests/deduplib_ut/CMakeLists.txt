cmake_minimum_required(VERSION 3.28.0)

project(deduplib_ut VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Poco REQUIRED COMPONENTS Foundation)

add_executable(
        ${PROJECT_NAME}
        src/duplicates_finder_test.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_NAME="${PROJECT_NAME}")
target_link_libraries(${PROJECT_NAME} PRIVATE deduplib testsuplib Catch2::Catch2WithMain Poco::Foundation)
catch_discover_tests(${PROJECT_NAME})

Format(${PROJECT_NAME} .)

AddCoverage(${PROJECT_NAME})
