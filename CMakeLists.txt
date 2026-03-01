cmake_minimum_required(VERSION 3.28.0)

project(filededup VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Poco REQUIRED COMPONENTS Foundation Util)

add_executable(filededup
        src/duplicates_finder.cpp
        src/duplicates_deleter.cpp
        src/checksummer.cpp
        src/time_helpers.cpp
        src/filededup.cpp
        src/filededup_application.cpp
        src/filededup_application.h)

target_link_libraries(filededup PRIVATE Poco::Foundation Poco::Util)

configure_file(config/${PROJECT_NAME}.properties ${PROJECT_NAME}.properties COPYONLY)
