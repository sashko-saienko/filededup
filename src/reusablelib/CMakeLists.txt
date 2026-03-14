cmake_minimum_required(VERSION 3.28.0)

project(reusablelib VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Poco REQUIRED COMPONENTS Foundation Util)

add_library(
        ${PROJECT_NAME} SHARED
        include/reusablelib/logging.h
        src/logging.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE Poco::Foundation Poco::Util)

AddClangTidy(${PROJECT_NAME})
AddCppCheck(${PROJECT_NAME})