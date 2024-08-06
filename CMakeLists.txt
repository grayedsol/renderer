cmake_minimum_required(VERSION 3.24.2)

project(renderer VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES src/main.cpp
    src/Line.cpp
    src/Triangle.cpp
    src/tgaimage.cpp
    src/Model.cpp
    src/Render.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

file(COPY obj DESTINATION ${CMAKE_BINARY_DIR})
file(COPY material DESTINATION ${CMAKE_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_BINARY_DIR})

target_include_directories(${PROJECT_NAME}
    PRIVATE include
    )

configure_file(rendererconfig.h.in rendererconfig.h)

find_package(glm CONFIG REQUIRED)
target_link_libraries(renderer PRIVATE glm::glm)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})