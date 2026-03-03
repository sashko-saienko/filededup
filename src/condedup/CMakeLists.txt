cmake_minimum_required(VERSION 3.28.0)

project(condedup VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Poco REQUIRED COMPONENTS Foundation Util)

add_executable(condedup
        src/condedup.cpp
        src/deleter_keep_policy.h
        src/duplicates_deleter.cpp
        src/duplicates_deleter.h
        src/time_helpers.cpp
        src/time_helpers.h
        src/condedup_application.cpp
        src/condedup_application.h
        src/logging.cpp
        src/logging.h)

target_link_libraries(condedup PRIVATE duplib)
target_link_libraries(condedup PRIVATE Poco::Foundation Poco::Util)

configure_file(config/${PROJECT_NAME}.properties ${PROJECT_NAME}.properties COPYONLY)