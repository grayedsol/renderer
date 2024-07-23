cmake_minimum_required(VERSION 3.24.2)

project(renderer VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES src/main.cpp
    src/DrawLine.cpp
    src/tgaimage.cpp
    src/model.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE include
    )

configure_file(rendererconfig.h.in rendererconfig.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})