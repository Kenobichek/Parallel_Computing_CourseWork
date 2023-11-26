cmake_minimum_required(VERSION 3.12)

project(InvertedIndex)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES "src/*.cc")

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
