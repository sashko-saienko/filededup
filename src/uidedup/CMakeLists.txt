cmake_minimum_required(VERSION 3.28.0)

project(uidedup VERSION 0.0.1 LANGUAGES CXX)

include(wxWidgets)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
        ${PROJECT_NAME}
        src/uidedup.cpp
        src/application.cpp
        src/main_frame.cpp
        include/application.h
        include/main_frame.h
)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_compile_definitions(${PROJECT_NAME} PRIVATE LOGGER_NAME="${PROJECT_NAME}")
target_link_libraries(${PROJECT_NAME} PRIVATE deduplib reusablelib Poco::Util)

configure_file(${CMAKE_SOURCE_DIR}/config/logging.properties logging.properties COPYONLY)

AddWxWidgets(${PROJECT_NAME})
AddCppCheck(${PROJECT_NAME})
Format(${PROJECT_NAME} .)
