cmake_minimum_required(VERSION 3.28.0)

project(condeduplib VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Poco REQUIRED COMPONENTS Foundation Util)

add_library(
        ${PROJECT_NAME} SHARED
        include/condeduplib/application.h
        src/application.cpp
        include/condeduplib/deleter_keep_policy.h
        include/condeduplib/duplicates_deleter.h
        src/duplicates_deleter.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_definitions(${PROJECT_NAME} PRIVATE LOGGER_NAME="${PROJECT_NAME}")
target_link_libraries(${PROJECT_NAME} PRIVATE deduplib reusablelib Poco::Foundation Poco::Util)

InstrumentForCoverage(${PROJECT_NAME})
CleanCoverage(${PROJECT_NAME})

AddCppCheck(${PROJECT_NAME})
