cmake_minimum_required(VERSION 3.28.0)

project(condedup VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Poco REQUIRED COMPONENTS Util)

add_executable(
        ${PROJECT_NAME}
        src/condedup.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE condeduplib Poco::Util)

configure_file(${CMAKE_SOURCE_DIR}/config/logging.properties logging.properties COPYONLY)

AddCppCheck(${PROJECT_NAME})
Format(${PROJECT_NAME} .)
