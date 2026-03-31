cmake_minimum_required(VERSION 3.28.0)

project(uidedup VERSION 0.0.1 LANGUAGES CXX)

include(wxWidgets)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
        ${PROJECT_NAME}
        src/uidedup.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE uideduplib)

configure_file(${CMAKE_SOURCE_DIR}/config/logging.properties logging.properties COPYONLY)

AddWxWidgets(${PROJECT_NAME})
AddCppCheck(${PROJECT_NAME})
