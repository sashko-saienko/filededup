cmake_minimum_required(VERSION 3.28.0)

project(condedup VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Poco REQUIRED COMPONENTS Foundation Util)

add_executable(
        ${PROJECT_NAME}
        src/condedup.cpp
        src/application.cpp
        src/duplicates_deleter.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_compile_definitions(${PROJECT_NAME} PRIVATE LOGGER_NAME="${PROJECT_NAME}")
target_link_libraries(${PROJECT_NAME} PRIVATE deduplib reusablelib Poco::Util)

configure_file(${CMAKE_SOURCE_DIR}/config/logging.properties logging.properties COPYONLY)

AddCppCheck(${PROJECT_NAME})
Format(${PROJECT_NAME} .)

include(BuildInfo)
BuildInfo(${PROJECT_NAME})
