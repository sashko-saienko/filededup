cmake_minimum_required(VERSION 3.28.0)

project(reusablelib VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(
        ${PROJECT_NAME} SHARED
        src/time_helpers.cpp
        include/${PROJECT_NAME}/time_helpers.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

AddClangTidy(${PROJECT_NAME})
AddCppCheck(${PROJECT_NAME})