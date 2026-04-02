cmake_minimum_required(VERSION 3.28.0)

project(uideduplib VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Poco REQUIRED COMPONENTS Foundation Util)

add_library(
        ${PROJECT_NAME} SHARED
        src/application.cpp
        include/uideduplib/application.h
        src/main_frame.cpp
        include/uideduplib/main_frame.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_definitions(${PROJECT_NAME} PRIVATE LOGGER_NAME="${PROJECT_NAME}")
target_link_libraries(${PROJECT_NAME} PRIVATE Poco::Util)

AddWxWidgets(${PROJECT_NAME})
AddCppCheck(${PROJECT_NAME})

InstrumentForCoverage(${PROJECT_NAME})
CleanCoverage(${PROJECT_NAME})
Format(${PROJECT_NAME} .)

include(BuildInfo)
BuildInfo(${PROJECT_NAME})
