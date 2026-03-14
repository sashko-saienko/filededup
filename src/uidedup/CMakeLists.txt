cmake_minimum_required(VERSION 3.28.0)

project(uidedup VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Poco REQUIRED COMPONENTS Util)

add_executable(
        ${PROJECT_NAME}
        src/uidedup.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE uideduplib Poco::Util)

configure_file(config/${PROJECT_NAME}.properties ${PROJECT_NAME}.properties COPYONLY)

AddClangTidy(${PROJECT_NAME})
AddCppCheck(${PROJECT_NAME})